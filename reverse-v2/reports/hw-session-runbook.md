# HARDWARE SESSION RUNBOOK — RC-firmware check / revert (prepare in advance!)

## ★ V2 SESSION (2026-07-04): v1→v2 upgrade + energization diagnostics (current plan)

**What's new:** payload v2 = heartbeat + **RPC** (call any function by address from the mailbox) + RC drive.
RPC is needed to LIVE-confirm the missing energization step (hypothesis: `FUN_0805f604` left enable)
WITHOUT reflashes and WITHOUT guessing. Goal: find what actually spins the wheel.

**Ready files (host `dist/`, copy to the Pi):** `rc_fw.bin`(v2, CRC 613a4a93), `flash_v2.cfg`(upgrade:
halt+freeze+overwrite ONLY bank1 payload+CRC+**resume** — bank0 hook is already in place from v1, no reset needed→no E88),
`flash_finish.cfg`(recovery, reset), `flash_revert.cfg`(revert to stock). Test: `firmware/rc-payload/rc_test.py`.

**Order (each step — on "go", physical — on confirmation):**
0. (me, read-only) `rc_test.py state` — is the mower in idle? heartbeat growing (v1 alive)? If asleep/no response — I ask to wake it (I do NOT guess).
1. (me, on "go") FLASH v2 (bank1 only, resume): stop mower-openocd → `openocd -f /tmp/flash_v2.cfg` → start mower-openocd. Safer than v1: bank0 not touched, resume without desync. If interrupted → recovery (catch-and-hold + flash_finish.cfg).
2. (me) `rc_test.py state` — payload=4a02b500, CRC=613a4a93, heartbeat growing = v2 alive.
3. **DRIVE DIAGNOSTICS (on "go", wheels SUSPENDED, blade disconnected):**
   - `rc_test.py arm`
   - `rc_test.py init-left` → ret should be **0x0** (A4963 init success). If 0xffffffff — init failed (SPI/mutex), that is already a finding.
   - `rc_test.py enable-left` (RPC FUN_0805f604 — the HYPOTHESIZED missing enable)
   - `rc_test.py drive 0x600` → **WATCH THE WHEEL** (you by eye: spinning or not). `rc_test.py ch` — CH3CV.
   - `rc_test.py stop`.
   - If it spun up after enable-left — hypothesis confirmed, we finalize the firmware. If not — via RPC we try other candidates (without reflashing).
4. Stop always `rc_test.py stop` or the mower button/power.

⚠️ The register (CH3CV) ≠ physics: "spinning" — ONLY on your word. Do not let the mower fall asleep between steps.

---


Composed 2026-07-04 after the first RC-payload flash. **Rules:** hardware only on the user's explicit "go";
physical actions (button/power-cycle) — only after explicit confirmation; all preparation
done in advance; the mower FALLS ASLEEP when idle → the pass is fast, no pauses.

## CURRENT FLASH STATE (verified by reading)
A **valid RC firmware** is flashed (the image is consistent):
- hook `0x08053b0e` = `bl 0x08090000` (our veneer). payload `0x08090000` = veneer. CRC `0x080ffffc` = `fd0f0f91` (target).
- heartbeat reached 0x8ae (our hook executed ~2222 times) — the code WORKS.
- BUT after `reset run` the mower went into **E88** and froze. Hypothesis (grounded in the firmware strings
  "bdboard disconnect / power on overtime / process deal overtime"): E88 = an overtime error of communication with
  the subsystems (chip2/display), desynced by `reset run` (a reset of ONLY the core). It is NOT proven that our firmware is at fault.

## LESSONS OF THIS FLASH (accounted for in cfg/plan)
1. **wdg-freeze ONLY direct** `mww 0xE0042004 0x00000300` (the variant with `[expr {$cr(0)|0x300}]` in the config CRASHED
   with a usage error → the watchdog was not frozen → IWDG reset mid-flash → brick). All ready cfgs already use the direct form.
2. **NOT `reset run` to check the firmware** — it desyncs the display/boards → E88/overtime/power-off.
   For a clean start — a **full physical power-cycle** (all boards reset together).
3. **The mower falls asleep** when idle/on error → chip1 goes dark (DPIDR deadbeef / all zeros over SWD). Don't confuse "asleep"
   with "brick": with live power DPIDR=0x2ba01477.
4. A flash interruption is reversible (catch-and-hold proven): catch it in the power window (`openocd -f hold.cfg` in a loop, the user
   holds the button) → finish it. The bank0 page with hs-loop code → write ONLY under halt.

## WHAT'S PREPARED (on the host, copy to the Pi at the start of the pass)
- `dist/rc_fw.bin` — the target RC image (CRC fd0f0f91). Already on the Pi `/tmp/rc_fw.bin` (sha b98ee58c…).
- `dist/flash_finish.cfg` — finish bank1 payload+CRC (if it interrupts again). Already `/tmp/flash_finish.cfg`.
- `dist/flash_revert.cfg` — **REVERT to stock** (hook→2ff0bdfd, payload erase, CRC→0f69a878). Copy to `/tmp/`.
- `firmware/rc-payload/rc_test.py` — RC control via mailbox. Copy to `~/mower-bench/`.
- On the Pi already: `~/mower-bench/verify.py`, `catch2.py`, `hold.cfg`, `recover_run.sh`.

## SESSION PLAN (step by step; each — on "go", physical — on confirmation)

**Step 0 (me, quick, read-only):** is poll active? read the `verify.py` state (boot/hook/payload/CRC/hb).
  We expect a valid RC image. If all zeros/deadbeef → the mower is asleep → **I ask you to turn on power**, wait for confirmation.

**Step 1 (you, on confirmation):** **full POWER-CYCLE** of the mower (turn it off completely, ~3s, turn on). Say "on".

**Step 2 (me, immediately, quick — don't let it fall asleep):** `rc_test.py state` + `rc_test.py hb`.
  - ✅ state=3 (idle) and **heartbeat GROWING continuously** → the firmware loads cleanly, hook alive, E88 was a reset-run desync. → Step 3.
  - ❌ E88/state=9 on a CLEAN start, or heartbeat not growing → the desync hypothesis is wrong, the firmware interferes with the load → **REVERT step**.

**Step 3 — RC test (on "go", wheels SUSPENDED, blade disconnected):**
  - `rc_test.py hb` (confirm once more that it's growing).
  - `rc_test.py drive 0x600` → should: energize the left driver + spin the LEFT wheel forward (CH3CV≠0).
  - Observe the wheel/CH3CV. `rc_test.py stop` → wheel stop.
  - (reverse: `rc_test.py rev 0x600`.) Stop always `rc_test.py stop` (or the mower button/power).

**REVERT step (if Step 2 = ❌, on "go"):**
  - Copy `flash_revert.cfg` → `/tmp/`. Stop `mower-openocd`.
  - `sudo openocd -f /tmp/flash_revert.cfg` (halt+freeze+revert 3 regions+reset run). If the mower is in error/asleep —
    run in a loop `recover_run.sh`-style with `flash_revert.cfg`, you hold the button.
  - Bring up poll, `verify.py`: hook=2ff0bdfd, payload=FF, CRC=0f69a878 → **full power-cycle** → factory boot.

## AFTER THE PASS
Update memory with facts (worked/E88 outcome/RC result). If the left RC works — add the right wheel
(pin down the right driver's energize) in the next pass.
