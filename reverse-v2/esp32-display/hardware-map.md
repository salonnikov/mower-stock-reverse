# What the stock firmware tells us about the display board's hardware

Extracted from the decompile (see [README](README.md)). The point is not the vendor's logic — it is
the wiring and the peripheral inventory, so our own firmware can drive this board.

## The vendor's own software map

The firmware keeps `__FILE__` paths, which name the modules it is built from:

| Source file | What it is |
|---|---|
| `../main/src/app/rw_display.c` | display application |
| `../main/src/driver/driver_mboard_port.c` | the link to the mainboard |
| `../main/src/driver/driver_rf.c` | sub-GHz radio driver |
| `../components/spi_tube_driver/src/spi_tube_drive.c` | **the segment display, driven over SPI** |
| `../components/cmt2300a/src/cmt2300a_hal.c` | **CMT2300A** sub-GHz transceiver HAL |
| `../components/Iot_Xulian/src/LSBle.c` | BLE stack glue |
| `../components/wifi_driver/src/wifi_driver_{api,port}.c` | Wi-Fi |

Two of these change the picture of what this board is.

## The rain sensor is the pair of springs, J3 and J4

The two conical compression springs at the top-left of the component side, each landing on a single
pad, are the **rain-sensor electrodes** (confirmed against the hardware). They press against the
contact plate in the housing, and the firmware reads the resistance between them through the ADC —
which is why the logs say `rain adc value:%d` rather than reporting a switch.

## The sub-GHz radio is in the firmware but NOT on this board

`ft-rf-1` in the factory-test set is neither Wi-Fi nor BLE. It is a **HopeRF CMT2300A** transceiver
on the SPI bus, and the firmware switches it between bands: `RF mode:NO`, `RF mode:868`,
`RF mode:915`.

What it is for shows up in the log strings: `RF open Wireless Charge` / `RF close Wireless Charge`,
and in NVS the driver keeps a pair of pairing seeds — `STA_seed` and `MOW_seed`
(`nvs_set_u16(seed_nvs_handler, "STA_seed", *s_seed)`). So in the models that have it, this board is
the node that talks to the charging station over the air and holds the mower↔station pairing.

**Our board does not have the chip.** Both sides of the PCB were gone over: there is no transceiver
package, no crystal for one, no matching network, and no second antenna. The single antenna is the
black whip on the coax that disappears under the glue at the module's edge — that is the U.FL feed
of the ESP32-WROOM-32UE, i.e. Wi-Fi/BLE. Nor is there an unpopulated footprint waiting for a radio.

This is consistent with the firmware itself: `RF mode:NO` exists as a first-class option beside 868
and 915, and the mode is selected in the main state machine. `Display_esp32` is one image shipped
across a product family, and this unit is a build of that family without the radio.

Practical consequence: **do not go looking for a sub-GHz link on this hardware.** The pins the RF
driver configures (GPIO23, GPIO4, GPIO35) are free on our board.

## How the segment display is actually driven — decoded

The bottom of the stack is three small functions, and together they say exactly how to talk to the
panel:

```c
gpio_set_level(2, 0);                       // FUN_400e6f44 — latch down
                                            // FUN_400e6f54 — one 16-bit word out
trans.length  = 0x10;                       //   16 bits
trans.flags   = 1;                          //   payload lives in the descriptor
trans.tx_data = bswap16(value);             //   byte-swapped, so MSB goes out first
spi_device_polling_transmit(tube_handle, &trans);
gpio_set_level(2, 1);                       // FUN_400e6f8c — latch up, data appears
```

So: **GPIO2 is the latch (RCLK/STB) of the shift-register chain**, and each refresh is a single
**16-bit** word over SPI — eight bits of segments and eight of digit select, which is what the
SOIC-16 packages are there for.

### The bus itself — there are two, and only one is the display

This is the trap. `FUN_400e6884` sets up **VSPI (host 2), MOSI GPIO26, SCLK GPIO14** — and that bus
belongs to the **CMT2300A**, which is not fitted on our board. Wiring a display driver to those pins
produces exactly nothing, which is how the mistake announces itself.

The display has its own bus, brought up in `FUN_400e6fb0` — a function Ghidra never recognised, so
it has to be read as assembly (`tube-driver.asm`):

```asm
gpio_set_direction(0x1a, 2)   ; GPIO26 as output
gpio_set_direction(0x02, 2)   ; GPIO2  as output
gpio_set_level(0x1a, 1)       ; GPIO26 HIGH — an enable, held for the duration
...
mosi = 0x19 = GPIO25
sclk = 0x21 = GPIO33
spi_bus_initialize(1, ...)    ; host 1 = HSPI
spi_bus_add_device(1, ...)
```

So the panel is **HSPI: MOSI GPIO25, SCLK GPIO33, latch GPIO2, enable GPIO26 high**. Confirmed on
the hardware.

For reference, the radio bus config, which is what the earlier version of this file wrongly gave as
the display:

```c
bus.mosi_io_num = 0x1a;      // GPIO26
bus.miso_io_num = -1;        // unused
bus.sclk_io_num = 0x0e;      // GPIO14
bus.quadwp/quadhd = -1;
bus.max_transfer_sz = 0x80;  // 128
spi_bus_initialize(2, &bus, 0);        // host 2 = VSPI

dev.clock_speed_hz = 400000;           // 400 kHz
dev.spics_io_num   = -1;               // no hardware CS — selects are driven by hand
dev.flags          = 0x14;             // HALFDUPLEX | 3WIRE
dev.queue_size     = 1;
spi_bus_add_device(2, &dev, &handle);
```

**Half-duplex 3-wire** means the data line is bidirectional, so the bus *could* read back — but
nothing in the image ever does: the tube handle is used by exactly one function, and that one only
writes.

### The refresh loop

`FUN_400dfff0` is a task that does nothing else:

```c
do {
    send16( framebuffer[pos] );   // one 16-bit word
    pos = (pos + 1) & 0xff;
    if (pos > 3) pos = 0;         // exactly four digits
    vTaskDelay(2);                // 2 ms each -> ~125 Hz per digit
} while (1);
```

The framebuffer is four 16-bit words at `0x3ffc5ac0`. (An earlier note in this file said there was no
display task — that was wrong; this task is created through a different call than the eight plain
`xTaskCreate` sites.)

### The segment encoding — recovered

There is no font table in the image because the codes are **immediates in the instruction stream**.
The digit renderer is a chain of compares, each clearing the bits it does not need and OR-ing in its
pattern:

```asm
400e0102  beqi   a3, 0x1, ...      ; dispatch on the digit
400e0105  movi   a2, -0x41         ; clear segment g
400e0108  and    a8, a8, a2
400e010b  movi.n a2, 0x3f          ; digit 0 -> 0x3F
400e010d  or     a8, a8, a2
```

Collected across the chain:

| Digit | Code | Digit | Code |
|---|---|---|---|
| 0 | `0x3F` | 5 | `0x6D` |
| 1 | `0x06` | 6 | `0x7D` |
| 2 | `0x5B` | 7 | `0x07` |
| 3 | `0x4F` | 8 | `0x7F` |
| 4 | `0x66` | 9 | `0x6F` |

That is the **standard common-cathode table**, so the bit order is the ordinary one:

```
bit 0 = a,  bit 1 = b,  bit 2 = c,  bit 3 = d,  bit 4 = e,  bit 5 = f,  bit 6 = g
bit 7 = decimal point            (cleared by the blank path, which ANDs with 0x80)
bit 8 = colon                    (set/cleared on its own by FUN_400e007c, mask 0x100)
```

Blanking a position ANDs the word with `0xFF80`, i.e. it wipes the seven segments and the dot while
leaving the higher bits alone — more evidence that the bits above 8 are separate indicators.

One thing to be careful about: the renderer only ever touches bits 0..8, and the digit is chosen by
**which** framebuffer word is being sent, not by anything the renderer writes. So the digit-select
bits must already sit in the upper byte of each word, put there when the framebuffer is set up. What
exactly those bits are has not been read out — reading the four words at `0x3ffc5ac0` on a running
board would settle it in one look.

So the whole panel is now specified well enough to write from scratch:

```
VSPI: MOSI GPIO26, SCLK GPIO14, 400 kHz, half-duplex 3-wire, no hardware CS
per digit:  GPIO2 low -> one 16-bit word, MSB first -> GPIO2 high
word:       bits 0..6 segments a..g (standard), bit 7 dot, bit 8 colon
cycle:      four digits, 2 ms each
```

Above that sits a small class with a vtable. `FUN_400d9944(char *s)` is the public "show" call: it
takes a **four-character string** and, per position 1..4,

```c
if (c >= '0' && c <= '9') digit(pos, c - '0');   // FUN_400d8618 -> vtable[0x20]
else                      symbol(pos, c);        // FUN_400d85ec -> vtable[0x24]
```

That second branch is the answer to where `Err` comes from: letters are not digits, so they go out
through the symbol method. The panel is fed plain ASCII and the driver decides how to render it,
which is also why **there is no font table in the image** — the mapping lives behind the vtable,
built at run time, not in a const array.

`88888888` and `tube scan` sit in the string table as the panel's self-test patterns.

## The segment display is on SPI, not on bit-banged pins

The component is literally called `spi_tube_driver` ("tube" = the segment digits). That matches the
three SOIC-16 packages on the board: shift registers fed from the SPI bus. It also explains two
things that had been puzzling: there is **no 7-segment font table anywhere in the image** (checked
for every bit ordering, for 8/16/32-bit entries, and allowing constant extra bits), and there is
**no direct write to the GPIO output registers** — the base `0x3ff44000` never appears in the code.

The radio sits on the same SPI bus: `cmt2300a_hal.c` sends bytes with
`spi_device_polling_transmit(handle, &trans)` at 8 bits per transfer.

## Pins established from the code

These come from explicit `gpio_set_level(pin, level)` and `gpio_set_direction(pin, mode)` calls with
constant arguments (`FUN_4012cf40` and `FUN_4012d0e8` respectively, both identified from their own
`__FUNCTION__` strings):

| Pin | Direction | Established use |
|---|---|---|
| **GPIO17** | out | UART1 TX — the J2 link to the mainboard |
| **GPIO16** | in | UART1 RX — the J2 link |
| **GPIO2** | out | **segment-display latch** (RCLK/STB of the shift-register chain) |
| **GPIO25** | out | **display data** — HSPI MOSI, 400 kHz |
| **GPIO33** | out | **display clock** — HSPI SCLK |
| **GPIO26** | out | **display enable**, held high while the panel is in use |
| **GPIO21 / GPIO22 / GPIO19** | in, pull-up | **the three buttons the panel reads itself** |
| **GPIO27** | out | **buzzer** — pulsed three times with 500 ms gaps on the fault path, and driven from the key/settings code |
| **GPIO23** | out | CMT2300A chip select — **not populated on our board** |
| **GPIO4** | out | CMT2300A second chip select (CSB/FCSB) — not populated |
| **GPIO35** | in | CMT2300A interrupt line — not populated (input-only pad) |
| **GPIO18** + **GPIO5** | out | **rain-sensor electrode drive**, pushed in opposite phase (anti-electrolysis) |
| **GPIO36** | in (ADC1_CH0) | **rain-sensor measurement** (SENSOR_VP) |
| **J3 / J4** | — | rain-sensor electrodes (springs), read through the ADC |

The J2 link is fully characterised:

```c
uart_param_config (1, &cfg);              // baud 0x38400 = 230400, 8 data bits
uart_set_pin      (1, 0x11, 0x10, -1);    // TX GPIO17, RX GPIO16, no flow control
uart_driver_install(1, 0x400, 0x400, 8, &queue, 0);
```

**230400 independently confirms the figure reached from the mainboard side**, where the same link is
named `DB`. J1, by contrast, is UART0 on the default pads — the console and the programming header.

## Tasks

`xTaskCreate` is called eight times; the task names in the image are `main_task`, `wifi_task`,
`mqtt_task`, `Factory_task`, `bt thread`, `RF thread`, `rain detect thread`, `log thread`,
`update thread`, `ipc_task`, `tcpip_thread`.

There is **no display task** — the segments are not serviced by a thread of their own.

## The buttons — the panel reads three of them itself

An earlier version of this file claimed the opposite, and it was wrong. The mistake is worth keeping
on record because it is an easy one to repeat: grepping the decompile for `GPIO_IN_REG`
(`0x3ff4403c`) finds nothing, and it is tempting to conclude that no input is ever read. But
`gpio_get_level` takes the peripheral base from a struct rather than a literal, so the constant
never appears — the read shows up as `*(base + 0x3c) >> pin`.

Found that way, `FUN_400dafb0` reads **GPIO21, GPIO22 and GPIO19**, packs them into a mask, debounces
with a counter to seven and beeps on a press. Three call sites, so three buttons; the fourth (`ON`,
by elimination) goes out on the harness as a power-on request rather than being read here.

What still holds: the display's SPI device is opened half-duplex 3-wire, which would have allowed
reading the keys back through the panel driver, but no code does it — the tube handle is write-only.
So the keys are plain GPIO inputs, not scanned through the display chip.

`KeyNum = %d` appears both here and in the state machine that owns the UART1 link, so key events are
read locally and also reported to the mainboard.

Practical consequence for reuse: **the buttons are ours already.** They need no rewiring — our
firmware only has to read GPIO21, GPIO22 and GPIO19 with pull-ups.

Read the harness row carefully — `OK · STA · GND · ↓ · ↑ · ON · 5V`. **The two arrows are the UART**,
data in and data out, the same link that was tapped to sniff the protocol; they are not switches.
That leaves `OK`, `STA` and `ON` as the discrete lines, i.e. three of them against four buttons on
the panel, so the mapping from buttons to harness lines is not settled — `ON` in particular reads
more like a power-on request to the mainboard than a plain key.

Consequence for our own firmware: **the buttons will not work as-is.** They are physically on this
PCB, but their signals leave for the mainboard rather than reaching the ESP32. They have to be
jumpered from the harness pads to spare pins — and GPIO23, GPIO4 and GPIO35 are free precisely
because the radio those were meant for is not fitted.

## Rain sensor — the whole chain

Analogue, and driven, not merely sampled:

```c
FUN_400df0e0(1):  gpio_set_level(18, 1);  gpio_set_level(5, 0);   // one polarity
FUN_400df0e0(2):  gpio_set_level(18, 0);  gpio_set_level(5, 1);   // the other
value = adc1_get_raw(0);                                          // ADC1 channel 0 = GPIO36
```

**GPIO18 and GPIO5 are the electrode drive**, pushed in opposite phase so the pair is excited with
alternating polarity — the standard way to keep a resistive rain sensor from electrolysing itself.
The measurement is **ADC1 channel 0, i.e. GPIO36 (SENSOR_VP)**, and the reading is smoothed by a
running average (`acc += new - avg; avg = acc >> 2`) before being compared against a threshold with a
counter that saturates at 15.

Around it: its own `rain detect thread`, an error path `initialze robot rain failed`, the settings
`rain_en`, `rain_delay`, `rain_delay_set`, `rain_delay_left`, `rain_state`, `rain_status`, and the
robot states `rain dock`, `rain wait`, `rain delay`.

## Factory test mode

The firmware answers a set of commands that exercise each peripheral in turn: `ft-key-1`, `ft-lcd-`,
`ft-rain-1`, `ft-beep-`, `ft-ble-1`, `ft-wifi-1`, `ft-rf-1`, `ft-uart-1`, entered with `ft-into` /
`Factory`. Useful if the pins ever need confirming on live hardware without a scope: run one command
at a time and watch what the board does.

## Still open

- **Which SPI bus and which SCLK/MOSI pins the tube driver and the radio share.** The call to
  `spi_bus_initialize` with the bus config has not been pinned down — the application reaches the
  IDF drivers through function pointers held in literal pools, so there are no call references to
  follow. The pins above were recovered because those particular calls pass constants.
- The pins for the buttons and the buzzer.
- Module attribution for GPIO18/GPIO5, GPIO27 and GPIO2 is by neighbourhood, not proven.

---

## Verified on the hardware (2026-08-10)

Our own firmware now drives this board, which settles the parts that could only
be inferred from the image — and corrects one thing the image hid.

**The display needs its chip select.** The device config carries
`spics_io_num = 0x20`, i.e. **GPIO32**. Miss it and the panel ignores the bus
entirely: data clocks out, nothing latches, and the board reads as dead. That was
the longest detour in this bring-up, so it is worth stating loudly.

Working set, all confirmed by lighting real segments:

```
bus     HSPI (host 1), MOSI GPIO25, SCLK GPIO33, 400 kHz
select  GPIO32 low across the frame
latch   GPIO2 pulsed around the transfer
GPIO26  left LOW — the factory drives it high during init and drops it at the end
frame   16 bits, most significant byte first
```

Word layout, established a bit at a time:

| Bits | Meaning |
|---|---|
| 0..6 | segments a..g, ordinary order |
| 7 | decimal point |
| 8 | colon |
| 9 | nothing |
| 10 | digit 4 — and it also lights digit 3's dot |
| 11 | digit 3 |
| 12 | digit 2 |
| 13 | digit 1 |
| 14, 15 | nothing |

The selects are neither in position order nor contiguous, and bit 10 dragging in
a neighbour's dot is the kind of thing that comes of laying a panel out for
convenience. Guessing this from the image was never going to work; it took eight
probes on the glass.

**Three of the four buttons are read by the panel**, on GPIO22, GPIO21 and
GPIO19 — they debounce and beep. `ON` is not among them, which confirms it
leaves on the harness as a power-on request to the mainboard rather than being
read here.

One practical note for anyone repeating this: the board **browns out on a
USB-TTL's 3.3 V regulator** once Wi-Fi is up. The symptom is a reset loop with
`Brownout detector was triggered` and no console output at all. Feed it from
something that can actually source a few hundred milliamps.
