# ESP32 display board — flash dump and analysis

Board **SNK_DISPLAY_CP_V11** (P/N 80102373-01, dated 2024-04-08), module **ESP32-WROOM-32UE**.
Dumped 2026-08-09 through the J1 header, with no soldering.

## How it was dumped

- USB-TTL (PL2303, 3.3 V logic) on J1: `RX→T`, `TX→R`, `GND→GND`; the board is powered from the
  same adapter's `3V3` pin.
- **`P` on J1 is IO0** (confirmed in practice): jumper `P`→`GND` plus power-up enters download mode.
  EN is not broken out and is not needed — the reset is done by cycling power.
- `esptool.py v4.12.0 --before no_reset --after no_reset read_flash 0 0x400000`.

## Chip

| | |
|---|---|
| Chip | ESP32-D0WD-V3 rev 3.1, 40 MHz crystal |
| MAC | `d8:13:2a:22:fe:10` |
| Flash | 4 MB (mfr `5e`, dev `4016`) |
| Dump | `dist/esp32-display-dump-v1.bin`, md5 `a9d7f453a85f3ba805e0d8d88af1b1f7` |

**No protection is set:** `FLASH_CRYPT_CNT = 0`, `ABS_DONE_0/1 = False`, `JTAG_DISABLE = False`.
The image is plaintext, not ciphertext.

## Partitions

| Partition | Offset | Size |
|---|---|---|
| `nvs` | 0x009000 | 16 K |
| `otadata` | 0x00d000 | 8 K |
| `phy_init` | 0x00f000 | 4 K |
| **`ota_0`** | **0x010000** | **0x170000 (1.44 MB)** — the live image |
| `ota_1` | 0x180000 | 0x170000 — holds no valid image (no `e9` magic) |
| `coredump` | 0x2f0000 | 64 K |
| `panic_out` | 0x300000 | 4 K |

## Firmware (app descriptor in ota_0)

- project_name: **`Display_esp32`**
- version: **`3.02.05`**
- built: **Mar 26 2024, 10:34:26**
- ESP-IDF: **`v4.4.3-dirty`**

## What the strings show (`strings.txt`)

This is a vendor **SK-Robot** network node, not just an LED-panel driver:

- MQTT: `mqtt://server.sk-robot.com`, `mqtt://mergemqtt-wired.sk-robot.com`, `test1..test3.sk-robot.com`
- NVS keys: `snk_mqtt`, `robot_ssid`, `robot_password`, `robot_name`, `robot_sn`, `Robot_env`
- AP prefix `Mower_`, password/token checks during pairing
- log line `robot poweron,esp32 reset: %d,cpu0 reset: %d, cpu1 reset: %d`

## Open

- Decompilation: stock Ghidra does not handle Xtensa LX6 — a third-party processor module is needed.
- What actually travels over J2 (the link to the mainboard) has not been worked out.
