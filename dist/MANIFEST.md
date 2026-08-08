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
- Taken: openocd on raspberrypi (192.168.10.81) via selectelvm/WG, gd32-dump.cfg.

## esp32-display-dump-v1.bin
- Purpose: FULL flash dump of the **display board** ESP32 (original firmware), taken over UART.
- Board: SNK_DISPLAY_CP_V11 (PN 80102373-01). Chip: ESP32-D0WD-V3 rev 3.1, MAC `d8:13:2a:22:fe:10`.
- Size: 4194304 bytes (4 MB). md5: `a9d7f453a85f3ba805e0d8d88af1b1f7`. Date: 2026-08-09.
- Not encrypted: `FLASH_CRYPT_CNT = 0`, `ABS_DONE_0/1 = False`, JTAG left enabled.
- Firmware inside: project `Display_esp32` v3.02.05, ESP-IDF v4.4.3, built Mar 26 2024.
- Taken: USB-TTL (3.3 V) on the board's J1 header, `P`→`GND` for ROM download mode,
  `esptool.py v4.12.0 read_flash 0 0x400000`. No soldering. See `reverse-v2/esp32-display/`.

> Note: unlike a build repo, the factory dumps above are **tracked on purpose** — they are the
> primary reverse-engineering source material. See the note at the bottom of `.gitignore`.
