# Automatic dump of the mower's ESP32 firmware

> **Superseded — the dump exists, but it was taken another way (2026-08-09).** The display board
> was out of the mower by then, so the Wi-Fi bridge described here was unnecessary: a USB-TTL
> adapter was wired straight to the board's J1 header, `P`→`GND` put the ESP32 into the ROM
> bootloader, and `esptool.py read_flash` pulled all 4 MB. The image is **not encrypted**.
>
> Result: [`dist/esp32-display-dump-v1.bin`](../../dist/esp32-display-dump-v1.bin);
> analysis in [`reverse-v2/esp32-display/`](../../reverse-v2/esp32-display/).
>
> The procedure below stays on record for the case where the ESP32 has to be read **in place,
> inside an assembled mower** — it needs no disassembly and no physical access to J1.

Through the `mower-link` bridge (BRIDGE mode) over Wi-Fi. No USB, no manual esptool commands.
`read_flash` — READ ONLY, does not break the mower.

## Steps
1. Flash `mower-link` onto our ESP32, connect it to the mower's J1:
   `T→GPIO16, R→GPIO17, P→GPIO5, GND→GND` (power for our ESP — its own USB/power bank).
2. On the `mower-link` page (http://mower-link.local/) press **BRIDGE (dump)**.
3. On the home laptop:
   ```
   cd server/esp-dump
   BRIDGE_IP=<ip-mower-link> docker compose run --rm dump
   ```
4. When the script asks — **power-cycle the mower** (its ESP will go into the bootloader).
5. The script itself catches the ESP, reads 4 MB, saves `out/mower-esp-<date>.bin`
   and reports a **verdict**: normal firmware (protocol present) or encrypted.
6. Afterwards — return `mower-link` to **SNIFF** and power-cycle the mower (it returns to normal).

## If "possibly encrypted"
Then the dump is useless — we switch to **protocol sniffing** (mower-link SNIFF + the logger
in `server/uart-logger`), decoding the commands/telemetry from the live stream.

Not the case here: the efuses read back `FLASH_CRYPT_CNT = 0` and `ABS_DONE_0/1 = False`, so
flash encryption and secure boot are both off and the 2026-08-09 image is plaintext.
