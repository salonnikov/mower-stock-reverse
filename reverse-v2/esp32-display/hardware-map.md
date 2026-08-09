# What the stock firmware tells us about the display board's hardware

Extracted from `decompiled_all.c` (see [README](README.md)). The point is not the vendor's logic —
it is the wiring and the peripheral inventory, so that our own firmware can drive this board.

## Established

### The J2 link to the mainboard — UART1, 230400 8N1, TX = GPIO17, RX = GPIO16

`FUN_400e61e4` sets the link up end to end:

```c
uart_param_config (1, &cfg);              // cfg.baud_rate = 0x38400 = 230400, 8 data bits
uart_set_pin      (1, 0x11, 0x10, -1);    // TX = GPIO17, RX = GPIO16, no RTS/CTS
uart_driver_install(1, 0x400, 0x400, 8, &queue, 0);   // rx 1024, tx 1024, event queue 8
xTaskCreate(...);                         // the link task, priority 0x18
```

Two things follow. The link is **UART1**, not the UART0 that J1 exposes — J1 is the console and the
programming header, J2 is the machine link. And **230400** independently confirms the figure
reached from the mainboard side, where the display link is named `DB` and clocked at 230400.

### J1 — UART0 on the default pins

UART0 keeps the default pads (GPIO1 TX / GPIO3 RX), which is what `T` / `R` on J1 are. `P` on J1 is
IO0, confirmed by taking the dump through it.

### Peripheral inventory, from the factory-test command set

The firmware carries a factory mode whose commands enumerate what the vendor considers testable
hardware on this board:

| Command | Peripheral |
|---|---|
| `ft-key-1` | the four front buttons |
| `ft-lcd-` | the 4-digit segment display |
| `ft-rain-1` | rain sensor |
| `ft-beep-` | buzzer |
| `ft-ble-1` | Bluetooth LE |
| `ft-wifi-1` | Wi-Fi |
| `ft-rf-1` | a separate RF path — **not** Wi-Fi/BLE, worth identifying |
| `ft-uart-1` | the J2 link |
| `ft-into`, `Factory` | enter factory mode |

`ft-rf-` is the surprise: it implies a radio path distinct from the ESP32's own Wi-Fi and BLE.

### Rain sensor — analogue, with its own task

Log lines `rain adc value:%d` and `rain sensor : %d`, a thread named `rain detect thread`, an error
path `initialze robot rain failed`. It is a measured value, not a dry contact, and it is debounced
in software: settings `rain_en`, `rain_delay`, `rain_delay_set`, `rain_delay_left`, `rain_state`,
`rain_status`, and robot states `rain dock`, `rain wait`, `rain delay`. There is a menu entry for
it (`add rain menu`).

### Lighting and zones

`led_en`, `led_mode`, `led_night`, `led_start`, `led_end` — scheduled lighting, with a night window.
`zone_en`, `zone_ex` and a multizone setting sit next to them in the same NVS handler
(`FUN_400d9ddc` / `FUN_400db588`).

### Settings held in NVS

`snk_mqtt`, `robot_ssid`, `robot_password`, `robot_name`, `robot_sn`, `Robot_env`, plus the rain,
led and zone keys above. Cloud side: MQTT to `server.sk-robot.com` (and `test1..3`,
`mergemqtt-wired`), pairing AP prefix `Mower_`.

## Not established — and the cheap way to get it

**The GPIO numbers for the display, the buttons, the buzzer and the rain input are still unknown.**
Tracing them out of the decompile stalls for a specific reason: the application calls the IDF
drivers **indirectly**, through function pointers loaded from literal pools
(`(*(code *)PTR_FUN_400d0504)(...)`), so there are no direct call references to follow, and the pins
are not passed as literal constants at the few call sites that are visible. `uart_set_pin` was the
exception, which is why the link is fully known.

Two ways forward that do not need more static analysis:

1. **Drive the factory test and watch the pins.** The stock firmware exercises each peripheral on
   command (`ft-lcd-`, `ft-key-1`, `ft-beep-`, `ft-rain-1`). With the board powered and a scope or
   logic analyser on the module pads, each command shows exactly which pins move.
2. **Attach JTAG.** `JTAG_DISABLE` is not blown, so the GPIO matrix and IO_MUX registers can be read
   live while the stock firmware runs — that is the pin map, read straight out of the silicon
   rather than inferred.

Either is faster than continuing to chase indirect calls through the pseudo-C.
