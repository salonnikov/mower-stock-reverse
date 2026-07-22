# tools/bench — ready-made scripts for the drive bench test (zero improvisation)

They run **on the Pi** (python3 is there). They talk to the running `mower-openocd`
over telnet `:4444` (mdw/mww, without halt). They implement the phases from
`reverse-v2/reports/bench-test-plan.md`. On the bench — just run them in order and
read the verdict.

## Pre-flight checklist (DO BEFORE running the scripts)
Hardware:
- [ ] The drive wheels are **suspended** (spin freely, will not touch anything).
- [ ] The blade is **physically disconnected** (the blade motor connector).
- [ ] Power is stable (the mower on the charger; the Pi from a power bank).
- [ ] SWD is connected to chip1.
- [ ] A hand is near the power cut-off.

Software (on the Pi):
- [ ] `sudo systemctl status mower-openocd` — up.
- [ ] `sudo systemctl stop mower-webctl` — free the single telnet client.
- [ ] `cd` into the directory with these scripts.
- [ ] Link: `python3 swd.py` → should print `LINK OK` and the current state.
      If "Target not examined/halted" — run `targets` in the openocd console; do NOT halt.

## Run order

| # | Command | What it does | Wait / decision |
|---|---|---|---|
| link | `python3 swd.py` | sanity-read of the state | `LINK OK` |
| Ph0 | `python3 phase0_baseline.py` | reads state/TIMER2/slots | **GO** = PWM path active; **NO-GO** = bring up the service/change the state |
| Ph1a | `python3 phase1_find_odom.py save` | snapshot of the wheel objects | "snapshot #1 saved" |
| — | *turn both wheels by hand ~10 rev.* | (physically) | say "done" |
| Ph1b | `python3 phase1_find_odom.py diff` | finds the tick address | write down the L and R tick ADDRESS |
| Ph2 | `python3 phase2_watch_pwm.py` | is CH3CV stable on its own | STABLE → Ph3 directly; JITTERY → Ph3 `--loop` |
| Ph3 | `python3 phase3_drive.py L 500 <odomL>` | small duty on the left | ticks grow + the wheel spins = PWM rotates ✔ |
| stop | `python3 estop.py` | zero all duty | at any moment of alarm |

Ph3 on `JITTERY`: `python3 phase3_drive.py L 500 <odomL> --loop`.
Increase the duty (1000, 2000…) only if the small one passed cleanly. One wheel at a time.

## What each verdict means
- **Ph0 GO** → A4963 configured, TIMER2 CH3 active → writing CH3CV is meaningful.
- **Ph3 ticks grow + rotation** → **the "duty rotates" hypothesis is confirmed LIVE** (not from old docs).
- **Ph3 no ticks / the wheel stands still** → duty alone does not rotate → Phase 4 of the plan (the role of A4963 RUN/direction); we do not prepare a script for it in advance — it depends on the Ph3 result.

## Immediate STOP criteria (hit estop.py + cut the power)
- The WRONG wheel moves / both at once.
- An unusual sound/vibration/heat/smell.
- Ticks grow on a command of 0.
- The mower changed state on its own (went to departure/cutting) → we caught an auto-start: cut the power, come back, do not enter state 4/5/6.

## Boundaries (what the scripts do NOT do)
- They do NOT write to flash, do NOT halt/reset/shutdown, do NOT touch the blade.
- They work only with TIMER2 CHxCV and reading RAM — everything is undone by removing the power.
- The right channel CH2CV (0x4000043c) — a hypothesis (ⓗ); confirm via Ph0 (which CHxCV changes during a normal drive) before running the right wheel.

## Addresses (cross-check, from reverse-v2)
- state `0x200000bc` · movebase `*0x20000338` · lmdrv `*0x2000054c` · rmdrv `*0x2000057c`
- TIMER2: CTL0 `0x40000400` · CHCTL2 `0x40000420` · CAR `0x4000042c` · CNT `0x40000424`
- CH0..3CV `0x40000434/38/3c/40` — **LEFT wheel = CH3CV `0x40000440`**
