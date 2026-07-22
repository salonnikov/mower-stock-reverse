# chip1: identify the firmware + roll back to factory (2026-07-14)

A ready package for the Pi (SWD bitbang to chip1 GD32F305). Two iterations.
chip1 pins: **SWCLK=GPIO25 (pin22), SWDIO=GPIO24 (pin18), GND (pin6)** → `swd_nums 25 24`.

## What is established (host-side analysis)
- Our flashers write the app from `0x08018000`. Our app SP = **0x20018000**, the factory one = **0x20017ff8** —
  one word reliably distinguishes "ours" from "factory".
- **The last build (testO, 10.07) erased 3 pages: 0x18000, 0x18800, 0x19000** and wrote up to 0x080192f6.
  All the old `flash_revert*.cfg` roll back only **2** pages (0x18000..0x18ffe) → **page 3
  would remain non-factory → the CRC of the whole app (@0x080ffffc) would not match → brick loop.** That is why here — a proper
  **3-page** rollback, byte-for-byte from `dist/gd32-mainboard-dump-v1.bin`.

## Files
| file | what | writes? |
|---|---|---|
| `chip1-identify.cfg` | identify the firmware + link + check the high regions | no (read-only) |
| `run_identify.sh` | detached runner for identify, log `/home/pi/identify.log` | no |
| `flash_factory_restore_3page.cfg` | roll back 3 app pages to factory (3071 halfwords) | **YES** |
| `run_restore.sh` | detached runner for the rollback + auto-verify, log `/home/pi/restore.log` | **YES** |
| `verify_after.cfg` | read-back check that 3 pages = factory | no |

## Push to the Pi
```bash
scp -r tools/rpi-swd/restore-2026-07-14 pi@<pi>:/home/pi/mower-swd/restore/
ssh pi@<pi> 'chmod +x /home/pi/mower-swd/restore/*.sh'
```

## ITERATION 1 — IDENTIFY (read-only, safe)
The mower must be **powered** (not asleep), otherwise chip1 is not visible (DPIDR=deadbeef/zeros).
```bash
ssh pi@<pi> 'nohup /home/pi/mower-swd/restore/run_identify.sh >/dev/null 2>&1 &'
sleep 8
ssh pi@<pi> 'cat /home/pi/identify.log'
```
Read the output:
- **LINK: CPUID/DBG_ID** non-zero → the link is up. All deadbeef/0 → the mower is asleep/no power.
- **`>>> FIRMWARE = ...`** → factory or our own_fw.
- Lines PAGE3 / HOOK / PAYLOAD / CRC: if all = **FACTORY**, but SP = **OURS** — only the 3 lower
  pages are touched → the 3-page rollback = exact and sufficient.
- If HOOK/PAYLOAD/CRC are marked `!!!` — the high regions are touched (bank1/injection) → **do not flash**
  the 3-page rollback, tell me: I will generate an extended one (needs `dist/gd32-mainboard-dump-v1.bin` on the Pi).

## ITERATION 2 — ROLL BACK TO FACTORY (by a separate command)
Only after identify has confirmed "only 3 pages touched". **Keep the mower powered.**
```bash
ssh pi@<pi> 'nohup /home/pi/mower-swd/restore/run_restore.sh >/dev/null 2>&1 &'
sleep 60
ssh pi@<pi> 'cat /home/pi/restore.log'
```
Wait in the log for `FLASH rc=0`, then verify: `>>> OK: 3 pages = FACTORY`.
Then — a **physical POWER-CYCLE of the mower** (not `reset run` — it desyncs the boards → E88).

## If the flash is interrupted (WiFi/power)
A flash interruption is reversible (verified): the catch-and-hold trap in the power window (the user holds the button) —
see `reverse-v2/reports/flash-procedure.md` §5. Restarting `run_restore.sh` is idempotent
(erase+write of the same 3 pages).
