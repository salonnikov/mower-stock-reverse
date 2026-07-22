# End-to-end trace of the wheel drive (chip1 GD32F305) — 2026-07-03

> ⚠️ **CORRECTION 2026-07-13 (board photo):** there is physically NO Allegro A4963 on the board — the drive =
> **three Fortior FU6832N** (8051+FOC, own firmware), the "A4963 protocol" is emulated by the FU6832 firmware.
> All **✔D** marks (semantics per the A4963 datasheet: POR defaults, RUN=1, VBB, etc.) are **annulled**.
> The **✔C** marks (what chip1 sends over SPI: frames, CS, timings, registers) remain valid.
> Current context: `reports/PLAN-2026-07-13-coils-wheels-fu6832.md` + memory `fu6832-drive-architecture`.

Assembled from statics (decompiled_all.c + force-disasm of gaps + raw dump material) and **cross-checked against the Allegro A4963 datasheet** (2 independent open-source drivers, bit-for-bit). 
**Legend strictly:** ✔C = proven by dump code · ✔D = semantics per the A4963 datasheet · ❓ = NOT physically verified (needs a clean bench measurement) · ⓗ = inference/guess.
Rule: we do NOT trust old live findings; no "live" observation from past sessions is used as confirmation.

---

## 1. Drive hardware
- Wheels + blade = **3× BLDC on Allegro A4963** (sensorless), shared `motor_spi_mutex`. Drivers `driver_{left,right,blade}_motor_snk_v2.c`, `a4963_snk_v2.c`. ✔C
- The A4963 is controlled over **SPI** (config/direction/mode) + a **hardware PWM pin** (speed setpoint). ✔C/✔D

## 2. A4963 SPI frame ✔C
`FUN_0808408c` (verify-write) / `FUN_080840d8` (send): word = `[15:13]=reg address(0..7) | [12]=W(set by |0x1000 on send) | [11:0]=data`. Readback check `(word&0xfff)==readback`, up to 0x31 attempts.

## 3. A4963 register map (per datasheet) ✔D
| addr | reg | key fields |
|---|---|---|
|0|CONFIG0|dead-time, blank-time, recirc|
|1|CONFIG1|VDS threshold, current-limit, invert-PWM, fast-decay|
|2|CONFIG2|PWM period, overspeed, pos-P|
|3|CONFIG3|hold-time/duty, pos-I|
|4|CONFIG4|start-speed, startup-duty, speed-P|
|5|CONFIG5|phase-adv, **SMX[6:4]=max-speed(gear)**, FG output, speed-I|
|7|**RUN**|**RUN[0], DIR[1], BRK[2], restart[3], demand DC[8:4], stop-on-fail[9], CM[11:10]=mode**|

## 4. Left motor init: 8 A4963 words ✔C+✔D
Written from shadow `+0x30..+0x3e`; **reg6/MASK is not sent** (stays default). The reg7 (RUN) word = `0xee0d`:
- **RUN=1** (output stage ENabled), DIR=0 (forward), **BRK=1**, restart=1, **demand DC=0**, stop-on-fail=1, **CM=11 (ClosedLoopSpeed)**.
→ **The A4963 is in RUN already from init and stays there** (there is no separate "turn RUN on later"). Serial-demand stays 0.

## 5. What actually sets the speed — PWM on TIMER2 CH3 ✔C/✔D
- Channel init: `FUN_0807f474(TIMER2,3,…)` → CHxMS=00 (**output mode**)+CHxEN; `FUN_0807f648(…,0x60)` → OCxM=PWM; `FUN_0807f696(TIMER2,3,0)` → CH3CV=0. TIMER2=`0x40000400`, CH3CV=`0x40000440`. ✔C
- `set_left_motor_speed` = **`FUN_0805f620`** (force-disassembled, 1114 b) and mirror `set_right` = **`FUN_0806fe66`** (1070 b): "gearbox" `pwm = speed*0x1c20/gear_max`, gear_max∈{6141,3069,718 in PWM units}, on a gear change — `FUN_0801f4dc`; finale `FUN_0805f300`(dir)+`FUN_0807f696(TIMER2,3,pwm)`. ✔C
- Serial-demand in RUN (DC[8:4]) stays 0 → **speed goes through the PWM pin, not over SPI.** ✔C/✔D

## 6. Corrections to earlier labels ✔C+✔D
- **`FUN_0801f4dc` ≠ "brake to dir".** It writes CONFIG5 SMX[6:4] = **max-speed selection (gear)**: 1→1533, 2→3069, 3→6141 rpm. Matches the log constants `6141`/`1533`. This is a gear-selector, NOT direction and NOT brake.
- **Direction** = `FUN_0801f4c6` (bit DIR reg7). set_left_dir=`FUN_0805f300`.
- **Motor "brake"** = NOT clearing RUN and NOT the BRK bit (BRK is always 1), but **resetting the PWM demand to 0** → with RUN=1&BRK=1 the A4963 gives electrodynamic braking/hold (per datasheet). ✔D
- Coasting (coast, BRK=0) is NOT in the code.

## 7. Default state after init ✔D
`RUN=1, BRK=1, CH3 duty=0%` → PWM inactive → the wheel in **braking/hold** (not spinning, not coasting). **The only thing gating rotation is a nonzero duty in CH3CV.** ← this is a MODEL (code+datasheet), NOT a live measurement.

## 8. Motor-loop gating ✔C
- **hs-loop `FUN_08053af0`** — a multi-rate callback dispatcher (step 10ms): array `+8` (fastest, motor loop) every 10ms, `+0x30` every 20ms, `+0x6d` less often (state machine). ✔C
- **Motor-command queue** `pri_data+0xe4` (movebase pri_data, RAM singleton `*0x20000338`): ring 6×0x24, cursor `+0xe4`. Writers `FUN_0801fb04`(mode2)/`FUN_0801fe8c`(mode1)/`FUN_08020164`(turn). The executor (consumer) — the body of the executor task (in a gap ⓗ). ✔C(writers)
- **Border navigation is NOT a separate path**: the reactive cycle ("border out, cycle left") is inside `FUN_08030128` (process_cutting run, state 5) and process_find_bd (state 6), poking the same primitives. → **the border-cycle is active ONLY in state 5/6.** ✔C
- **`manual set` (0x30000006/07)** writes a byte flag (0=manual/0xFF=quit), the motor loop does NOT read it → does NOT control the drive (it silences the auto-start by schedule). ✔C
- **Direct speed injection in the firmware**: `FUN_0803b6f4(L,R)` in state {1,7,8} and the heading controller `FUN_0805fdd4` (enable byte==1) set the wheel speed, bypassing the queue/border. (The exact vtable offset of the set-speed method from statics is ambiguous — `+0x38` turned out to be a getter; the reliable CH3CV writers are set_left `FUN_0805f620`/set_right `FUN_0806fe66`.) ✔C(writers)/ⓗ(vtable point)

## 9. Full chain (end-to-end, with the marked gap)
```
hs-loop FUN_08053af0 (10ms, callbacks +8)
  └► process_*_run (state-dispatch) ─┐
       state 5/6: FUN_08030128 border-cycle ─┤ write the queue +0xe4 via
       direct: FUN_0803b6f4 (state 1/7/8)   ─┘ primitives FUN_0801fb04/fe8c/08020164
            └► movebase vtable +0x38 (set L,R)         [queue consumer = executor task, ⓗ gap]
                 └► set_left FUN_0805f620 / set_right FUN_0806fe66  (gear-map)
                      ├► FUN_0801f4dc  → A4963 CONFIG5 SMX (gear) over SPI
                      ├► FUN_0805f300→FUN_0801f4c6 → A4963 RUN.DIR over SPI
                      └► FUN_0807f696(TIMER2,3,pwm) → CH3CV duty  ──► A4963 PWM pin ──► motor
odometry (feedback): tacho A4963 ──► TIMER3 (0x40000800) input-capture
                             ──► ISR FUN_0805fbc0/FUN_0805fd45 → ticks/speed
```

## 10. ❓ WHAT WE DON'T KNOW (needs a clean bench measurement)
- Whether a nonzero CH3CV **physically** spins the wheel; whether CH3CV=0 brakes it. This is a code+datasheet model, NOT a measurement. The earlier "CCR=0 stopped it" = a false positive (jammed against the border wire).
- The exact role of the A4963 PWM pin in ClosedLoopSpeed mode (demand vs enable) — ⓗ.
- The CH3CV→rpm scale; the body of the executor task (queue consumer).

## 11. Clean bench-test protocol (when the hardware is available)
1. Wheels suspended, blade physically disconnected.
2. Put the mower into a **non-cutting** state (NOT 5/6): e.g. state 1/7/8 (then `FUN_0803b6f4` writes `+0x38` directly) or observe in state 4 (departure, straight driving). manual-set is not required.
3. Over SWD, independently vary: (a) only CH3CV TIMER2 (0x40000440) with a known A4963 state; (b) A4963 RUN.DIR/CONFIG5 over SPI. Watch the odometry ticks (TIMER3 capture) + physical rotation.
4. This way isolate what exactly spins it: only the PWM duty, or something else over SPI is also needed.
</content>
