# Automatic dump of the mower's ESP32 firmware

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
