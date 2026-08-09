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
| **GPIO27** | out | **buzzer** — pulsed three times with 500 ms gaps on the fault path, and driven from the key/settings code |
| **GPIO23** | out | CMT2300A chip select — **not populated on our board** |
| **GPIO4** | out | CMT2300A second chip select (CSB/FCSB) — not populated |
| **GPIO35** | in | CMT2300A interrupt line — not populated (input-only pad) |
| **GPIO18** + **GPIO5** | out | driven as a pair from inside the RF driver — unidentified, and moot if the radio is absent |
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

## Rain sensor

Analogue, not a dry contact: `rain adc value:%d`, `rain sensor : %d`, its own `rain detect thread`,
an error path `initialze robot rain failed`. Debounced in software — `rain_en`, `rain_delay`,
`rain_delay_set`, `rain_delay_left`, `rain_state`, `rain_status` — with robot states `rain dock`,
`rain wait`, `rain delay`.

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
