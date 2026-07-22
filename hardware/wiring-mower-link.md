# Connecting our ESP32 (mower-link) to the mower's display board (J1)

Our board: ESP32 DOIT DevKit, 30 pin, USB at the bottom.
All 4 wires go to the RIGHT row (D5, TX2, RX2 in sequence + GND below).

```
                 ESP32 DOIT DevKit (30 pin)          J1 of the mower display board
                    ┌──────────────────┐             ┌──────────────────────┐
              EN ──►┤                  ├──◄ D23      │  3V3  ── do not connect│
          VP(36) ──►┤                  ├──◄ D22      │   T   ──► ESP RX2/16   │
          VN(39) ──►┤     ESP32        ├──◄ TX0      │   R   ──► ESP TX2/17   │
             D34 ──►┤    WROOM-32      ├──◄ RX0      │  GND  ──► ESP GND      │
             D35 ──►┤                  ├──◄ D21      │  GND  ── (any GND)     │
             D32 ──►┤                  ├──◄ D19      │   P   ──► ESP D5       │
             D33 ──►┤                  ├──◄ D18      └──────────────────────┘
             D25 ──►┤                  ├──◄ D5     ◄═══════ J1 "P"  (IO0, dump)
             D26 ──►┤                  ├──◄ TX2/17 ◄═══════ J1 "R"
             D27 ──►┤                  ├──◄ RX2/16 ◄═══════ J1 "T"
             D14 ──►┤                  ├──◄ D4
             D12 ──►┤                  ├──◄ D2
             D13 ──►┤                  ├──◄ D15
             GND ──►┤                  ├──◄ GND    ◄═══════ J1 "GND" (common ground!)
             VIN ──►┤                  ├──◄ 3V3
                    └────────┬──────┬──┘
                          ═══╡ USB  ╞═══  ◄── ESP power: power bank/computer
                             └──────┘
```

| J1 of the mower | → ESP32 | purpose |
|---|---|---|
| **T**   | **RX2 / GPIO16** | display→mainboard (and RX for the dump) |
| **R**   | **TX2 / GPIO17** | mainboard→display (and TX for the dump) |
| **P**   | **D5 / GPIO5**   | enter the bootloader (dump only) |
| **GND** | **GND**          | common ground — required |

ATTENTION: do NOT connect J1 `3V3` (the ESP is powered by its own USB/power bank).
Firmware: mower-link (SNIFF/BRIDGE modes). Pins are defined in firmware/mower-link/src/main.cpp.
