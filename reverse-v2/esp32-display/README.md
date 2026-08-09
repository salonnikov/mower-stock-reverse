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

## Decompilation

**6192 functions found, 6191 decompiled** (one failure). Ghidra **12.1.2 carries its own Xtensa
processor module** — contrary to the earlier assumption, no third-party extension is needed.

| File | What it is |
|---|---|
| `decompiled_all.c` | pseudo-C for every function (5.1 MB) |
| `functions_index.csv` | `addr,name,size,decomp_ok` |
| `symbols.txt` | all symbols, including the ROM map |
| `strings.txt` | strings with addresses |
| `memory_map.txt` | the memory layout the analysis ran against |

Reproduce with [`tools/ghidra-esp32/run.sh`](../../tools/ghidra-esp32/run.sh); the layout script is
[`reverse-v2/ghidra-scripts/Esp32Segments.java`](../ghidra-scripts/Esp32Segments.java).

Two things matter for getting sane output:

- **The image is not flat.** Its six segments load at unrelated addresses, so the raw file is
  imported as seg3 (IROM) at `0x400d0020` and the other five are mapped in as separate blocks,
  plus uninitialised regions for ROM, BSS and the peripherals. Loading the file linearly produces
  garbage.
- **ROM calls are named** from `esp32.rom.ld` of the *same* ESP-IDF the firmware was built with
  (v4.4.3): 1591 symbols, so calls land on `rom_phy_disable_agc` and friends rather than bare
  addresses.

Docker note: the image is built for **linux/amd64 on purpose**. The Ghidra release has native
decompiler binaries for linux_x86_64 / mac_* / win_x86_64 but none for linux_arm_64, so an arm64
image comes up with no decompiler at all. On Apple Silicon this needs Rosetta enabled in Docker
Desktop.

## Open

- **Function names.** The firmware is stripped: 6155 of 6192 functions are `FUN_xxxxxxxx`. The
  image does carry IDF assert material — source paths such as `/IDF/components/vfs/vfs_uart.c`
  next to identifiers such as `uart_write` — so a second pass could recover real names for a good
  part of the IDF-side code.
- A handful of functions hit `Unable to resolve constructor` warnings — Xtensa opcodes the SLEIGH
  spec does not model. They decompile, but those spots should be read as assembly.
- What actually travels over J2 (the link to the mainboard) has not been worked out.
