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

## The board carries a sub-GHz radio — CMT2300A

`ft-rf-1` in the factory-test set is neither Wi-Fi nor BLE. It is a **HopeRF CMT2300A** transceiver,
and the firmware switches it between bands: `RF mode:NO`, `RF mode:868`, `RF mode:915`.

What it is for shows up in the log strings: `RF open Wireless Charge` / `RF close Wireless Charge`,
and in NVS the driver keeps a pair of pairing seeds — `STA_seed` and `MOW_seed`
(`nvs_set_u16(seed_nvs_handler, "STA_seed", *s_seed)`). So the display board is the node that talks
to the charging station over the air and holds the mower↔station pairing.

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
| **GPIO23** | out | CMT2300A chip select, driven high at init |
| **GPIO4** | out | CMT2300A second chip select (the part has CSB and FCSB) |
| **GPIO35** | in | input from the RF section — input-only pad on the ESP32, consistent with the radio's interrupt line |
| **GPIO18** + **GPIO5** | out | driven as a pair from one function inside the RF driver's neighbourhood |
| **GPIO27** | out | driven from a boolean; called from four places in the settings code |
| **GPIO2** | out | driven low by a pair of functions near the RF HAL |

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
