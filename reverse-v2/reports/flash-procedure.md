# Procedure for flashing our firmware + recovery (chip1 GD32F305)

Everything below is **verified live on the bench 2026-07-03** (including the full brick→recovery cycle). Infra: Pi `pi@192.168.10.81`, SWD bitbang, openocd 0.10, scripts in `tools/bench/` (on the Pi `~/mower-bench/`, configs `~/mower-swd/`).

## 0. Hardware/network — lessons
- **The Pi wifi is unstable** → run ALL flash operations **detached on the Pi** (`nohup`), with ssh only for launch+polling. An ssh drop does not interrupt it.
- The mower **auto-powers-off** after an error/idle and on an application crash (power-latch). For flashing — keep the power on (charging/woken), otherwise a power-off in the middle of a write = risk.
- **`reset run` resets only the core** → display desync (the PIN cannot be entered). Cured by a **full power-cycle**. For recovery after a write, `resume` is preferable (RAM not corrupted) or a full power-cycle.

## 1. Flash layout (verified by dump)
- Bootloader `0x08000000..0x08017FFF`; its CRC word in `0x08017FFC` (boot-gate, separate).
- **Application `0x08018000..0x080ffffc`**; the code actually goes to ~`0x08090000`, beyond that `0xFF`.
- **Free for payload: `0x08090000..0x080ffff8`** (~450 KB, all 0xFF).
- **`0x080ffffc` = CRC of the entire application image** (see §2).
- ⚠️ dual-bank: 0x08000000..0x0807FFFF = bank0 (openocd stm32f1x writes it), 0x08080000..0x080FFFFF = bank1 (only via FMC bank1, see §3).

## 2. 🔑 End-to-end CRC of the application image (CRITICAL)
The application is protected by a HW-CRC-32 over its **entire** image (including the empty tail):
```
CRC = crc32_stm32(0x08018000 .. 0x080ffffc)
      poly=0x04C11DB7, init=0xFFFFFFFF, 32-bit words LE, MSB-first, no reversal, no final XOR
stored in: 0x080ffffc (the last 4 bytes of flash)
stock image reference: 0x0f69a878 (matched the computation = mechanism confirmed)
```
**Any changed byte in 0x08018000..0x080ffffc without updating the CRC → the application crashes at startup (brick loop).** This is NOT bypassable — the CRC must be **recomputed** on every patch.

**Payload flashing recipe (without bricking):**
1. Write the payload into the free region `0x08090000..0x080ffff8`.
2. Recompute the CRC over `0x08018000..0x080ffffc` (with the payload in place, the old marker enters the computation as-is — we compute up to 0x080ffffc NOT including the marker itself... **important**: the range ends at 0x080ffffc, i.e. the marker is NOT part of the CRC; check the boundary in the implementation).
3. Write the new CRC to `0x080ffffc`.
4. Boot proceeds.

> Payload in bank1 (≥0x08080000) → write via FMC bank1 (§3). The marker 0x080ffffc is also in bank1.

## 3. Flash writing
**bank0 (0x08000000..0x0807FFFF):** openocd `stm32f1x`, but set the bank size EXPLICITLY:
`flash bank chip.flash stm32f1x 0x08000000 0x100000 0 0 chip.cpu` (autodetect by id 0x418 lies about 256K).
Need a work-area: `target create ... -work-area-phys 0x20000000 -work-area-size 0x2000`.

**bank1 (0x08080000..0x080FFFFF):** openocd stm32f1x CANNOT do it → via **FMC bank1 directly** (addresses from the vendor SVD):
- KEY1=`0x40022044` (unlock: 0x45670123, then 0xCDEF89AB)
- STAT1=`0x4002204C` (bit0 BUSY), CTL1=`0x40022050` (PG=1,PER=2,MER=4,START=0x40,LK=0x80), ADDR1=`0x40022054`
- **erase page:** unlock; CTL1=PER; ADDR1=addr; CTL1=PER|START; wait BUSY=0; CTL1=0.
- **program halfword:** CTL1=PG; `mwh addr val16`; **wait BUSY=0 (poll STAT1) after EACH one**; CTL1=0. (without waiting for BUSY, openocd `mwh` retries and writes garbage → 0x00.)
- lock: CTL1=LK.
Example: `tools/bench/flash_bank1_v2.cfg` (tcl procedures with BUSY polling).

**Common to all writes:** halt + freeze the watchdog: `mem2array cr 32 0xE0042004 1; mww 0xE0042004 [expr {$cr(0)|0x300}]` (otherwise IWDG/WWDG resets on halt).
Protection: FMC_WP `0x40022020` = 0xFFFFFFFF (no write-protect). RDP: not level-2 (reading works). Do NOT run unlock/mass_erase.

## 4. Rollback (verified)
Full dump rollback: `dist/gd32-mainboard-dump-v1.bin` (sha256 82b8bb5a...), == the current flash == 2 dumps on the Pi. Recovery = write the dump back (bank0 stm32f1x + bank1 FMC).

## 5. Recovery from "won't boot" (verified end-to-end)
If after a write the mower won't boot (display→slider→goes dark = crash before power-latch, chip1 not visible over SWD because the power drops):
1. **catch-and-hold trap:** `tools/bench/hold.cfg` + loop `hold_loop.sh` (`while: openocd -f hold.cfg` WITHOUT a timeout — on success openocd blocks and holds the halt). The user **HOLDS the power button** (the power window).
2. Once caught (`:4444` UP) — read/fix through the holding openocd (telnet :4444, `rdmem.py`, `fmc_erase.py`).
3. Erase/restore the damaged region → flash==dump (or a consistent image with the correct CRC).
4. Full **power-cycle** of the mower.
If the trap must be killed on the Pi and the wifi keeps dropping — **`sudo reboot` the Pi** (clean, poll.cfg comes up on its own). NB: `pgrep -f hold_loop` catches itself — count with `ps -eo args|grep -c "[/]tmp/hold_loop"`.

## Tools (tools/bench/)
swd.py (client :4444), rdmem.py (read), flash_run.sh (detached cfg runner), flash_bank1_v2.cfg (bank1 write with BUSY), fmc_erase.py (bank1 erase), hold.cfg/hold_loop.sh (catch-and-hold), flash_dump.py (dump).
