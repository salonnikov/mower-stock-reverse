# dist — firmwares (what is what). Flash to `0x0` via esptool-js.

## ACTIVE
| File | Purpose | Pins | baud | md5 |
|---|---|---|---|---|
| **`sniffer-2line-rtos-v2.bin`** | **The sniffer "done right".** FreeRTOS 2 cores (capture does not depend on Wi-Fi → does not hang) + static 48K buffer (survives dropouts) + auto-reconnect + half-open socket detection + **LOCAL web UI on the ESP**. | ↑→GPIO17, ↓→GPIO16, GND→GND | 230400 | `64f8961d864ebd33fe2d417108bf5c46` |

**Local interface:** Wi-Fi access point **`mower-sniff`** (password `mower1234`) → http://192.168.4.1/ — status + live frames EVEN if the VPS/home Wi-Fi is unavailable. Or by the ESP's IP on the home network.
**VPS dashboard:** http://107.173.50.198:8080/

## Others / archive
- `sniffer-2line-rtos-v1.bin` — without the local UI (there was a bug with connectivity).
- `sniffer-2line-nobuf-v1.bin`, `mower-auto-v1.bin`, archive/* — old.

## gd32-mainboard-dump-v1.bin
- Purpose: FULL flash dump of the mainboard (original firmware), taken over SWD from RPi2.
- Chip: GD32F305 (Cortex-M4), 1 MB flash @0x08000000. RDP was not set.
- Size: 1048576 bytes. md5: 316927a42857b6f28bd7a0ad2d070de5. Date: 2026-06-29.
- Do NOT commit/push (proprietary firmware) — in .gitignore.
- Taken: openocd on raspberrypi (192.168.10.81) via selectelvm/WG, gd32-dump.cfg.
