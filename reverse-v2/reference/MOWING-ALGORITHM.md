# Driving/mowing algorithm — factory firmware CHIP1 (GD32F305, main)

> Source: `reverse-v2/chip1/decompiled_all.c`, `strings.txt`, `symbols.txt` (dump `dist/gd32-mainboard-dump-v1.bin`, base 0x08000000).
> Confidence notation: **[V]** verified (code citation / address), **[I]** inferred (plausible), **[U]** unknown.
> All facts are about **CHIP1** (the main controller), unless explicitly stated otherwise.
> A huge advantage of this firmware: it **documents itself** with debug strings (`FUN_08048adc(level, "cutting", "..\src\app\process\process_cutting.c", ...)`). The logic below is reconstructed from code + from these strings.

---

## 0. Key objects

| Object | Where | What | Conf |
|---|---|---|---|
| Global application context `ctx` | `*(int*)(DAT+4)` across many modules (e.g. `_DAT_080392e0`, `_DAT_0802fd08`) | Pointer to the application struct; it holds state, sub-modules (motion, borders, blade, battery), counters | [V] |
| **State number (state)** | `ctx + 0x10` | Holds the current top-level mode 0..10 | [V] `FUN_0807976c` writes `*(uint*)(DAT_08079778+0x10)=param` |
| Cut sub-step (cut main-step) | `ctx_cut + 0x138` | Sub-mode within cutting: 1=front/normal, 2/3/4="random move to escape", 6=blade-reverse-recover | [I] from code 41214-41331 |
| Border cycle sub-step | `ctx_cut + 0x134` | 0..6, set by `FUN_0802f97c` | [V] 41142 |
| Random-move attempt/iteration counter | `ctx_cut + 0x148` | `%3==0`→blade reverse; `>0xe(14)`→error | [V] 41295-41310 |
| **Motion command queue** | `*(ctx_move+4) + 0xe4` | write-pointer into a ring of 0x24-byte records; primitives place a record and do `ptr += 0x24` | [V] 30884-30895 |
| Motion module vtable | `*(int*)(ctx+4)+0xc` → methods `+0x60/+0x84/+0x88/+0x98/+0xa0` | cycle/turn, get-heading, get-distance | [V] 42092/42264/48417 |

**List of states** (argument to `FUN_0807976c`, from call sites + names `process_*_run` in `strings.txt` 4262-4367):

| state | process (_run name) | meaning |
|---|---|---|
| 0/1/2 | security / waiting-init | start/security/waiting |
| 3 | process_waiting_run | idle (parked at base/waiting) |
| **4** | process_departure_smooth_run | **leaving the base** |
| **5** | process_cutting_run | **mowing (drives forward + blade)** |
| **6** | process_find_bd_run | **border search** |
| **7** | process_docking_smooth_run | **docking to the base** |
| **8** | process_charging_run | **charging** |
| 9 | process_error_run | error (emergency stop) |
| 10 | process_power_off_run | power-off |

Conf.: **[V]** — the state↔process correspondence is confirmed by call sites of `FUN_0807976c(N)` and by logs (`s____src_app_process_process_*` nearby).

---

## 1. Dispatcher / high-speed loop — `FUN_08053af0` @ 0x08053af0

This is **not** a switch on state, but a **multi-frequency cooperative scheduler** (RTOS-like). It spins an infinite loop and on each "tick" calls arrays of function-pointers from tables in the `ctx->sched` struct (`*(int*)(iVar1+4)`), with different frequency dividers. **[V]** 66398-66505.

```
do {
  FUN_0808368c(&local_8, 10);          // barrier ~10 ms → base tick = 100 Hz   [V] 66421
  // group @off 0x08 (count @0x6c): EVERY tick  (~100 Hz)                        [V] 66422-66429
  // group @off 0x30 (count @0x6e): every 2 ticks (~50 Hz, flip iVar8)           [V] 66430-66444
  // group @off 0x1c (count @0x6d): counter 1..9  → period ~10 ticks (~10 Hz)    [V] 66445-66459
  // group @off 0x44 (count @0x6f): counter 3..0x13(19) → ~17 ticks              [V] 66460-66474
  // group @off 0x50 (count @0x70): counter 4..0x18(24) → ~21 ticks              [V] 66475-66489
  // group @off 0x5c (count @0x71): counter 5..99 → ~95 ticks (~1 Hz)            [V] 66490-66504
} while(true);
```

- Base tick = **10 ms** (argument 10 in `FUN_0808368c`) → main loop **100 Hz**. **[V]** (the address of the returning barrier is [I]).
- Into these tables the **process manager registers the `run` function of the current state** and the services (motion, blade, border sensors, battery). A state change via `FUN_0807976c` results in the `run` of the new process running on the following ticks. **[I]** (the table-registration code itself is not separately cited — **[U]** exact moment the pointers are set).
- `FUN_08054274` @0x08054274 — **an example scheduler task**: waits until the drive settles into idle (`vtable+0xc+0xc()==0`) and the counter `+0x40≥+0x42`, then `FUN_0807976c(5)` → **departure→cutting transition**. **[V]** 67074-67092.

---

## 2. Start via "mow" — command `0x300100de`

A control frame arrives at the command dispatcher; the low part of the id = **`0x100de`**. **[V]** 55577.

```c
if (iVar6 == 0x100de) {                                   // "set control command"
    iVar17 = FUN_08023f80(param_1, &DAT_0804709c);        // parse the frame
    if (iVar17 != 0) {
        iVar6 = FUN_08050448();
        (**(code**)(iVar6+0x14))(*(u8*)(iVar17+0x14));     // <-- control BYTE of the command
        ...  "set control command=%d"                      // [V] 55584
```
- The actual **verb** (start-mow / dock / stop / restart) is the **sub-byte** `*(iVar17+0x14)`, passed into `FUN_08050448()->vtable[0x14]`. The value `0x20` = restart (seen in the neighboring branch `0x101dc`, 55598). **[V]** for the mechanics, **[U]** — the full table of numeric values for "mow/to base/stop".
- This handler **sets a deferred command** which `process_wait` reads in idle:
  `FUN_08028910` @0x08028910 returns `-1` if there is a pending command (`vtable+0x44()==-1`). **[V]** 55? / 28910-block.
- Further, in `process_wait` (`FUN_080270a8`, `FUN_080391e4`), based on this command and checks, **state 4 (departure)** is set:
  `FUN_0807976c(4)` (36424, 36461, 36557) with logs `auto departure delay time is reached, goto departure`, `Robot on schedule, start work`. **[V]**.
- Strings confirming the path: `manual start command, change to departure` (0x08065f18), `robot in station, change to departure` (0x08066670), `start departure` (0x08067778). **[V]** strings; binding to the exact branch — **[I]**.

---

## 3. Leaving the base — state 4, `process_departure_smooth` (0x08032f20…0x08038xxx)

Narrative (from strings 0x08031e..0x08038 + code 48340-48455):

1. **Backing out of the station** — the drive goes back (`back`), measuring the covered distance `vtable+0xa0` (get-distance). Control: `leave station lifted again`, `leave station hitted`, `leave station back distance too short=%d and hitted`, `leave station hit overtime, back distance=%dmm`. **[V]** strings 1091-1104.
2. **Stop + turn in place** — upon reaching the distance a "cycle" (turn) is done: calls `(**(vtable+4)+0x60))(100,1,0x2d0)` = cycle(dist=100, dir=1, angle=0x2d0=720). **[V]** 48417/48439. Logs `departure cycle hit, change to random move`, `departure cycle finish, not in border, change to random move`. **[V]** 1100-1104.
3. **Border check** — if after the turn the robot is not "in border": `departure cycle finish, no border, continue cycle right`. If rain: `departure rain, find border` / `departure rain and on border, dock`. **[V]** 1205-1208.
4. **Blade start** — `leave station finish, start blade` (0x0803208c). **[V]**.
5. **Transition into mowing** — when the drive has finished and settled (see `FUN_08054274`), `FUN_0807976c(5)`. **[V]** 67088.

> Departure is a large sub-state-machine with a `case` switch of steps (`case 5/6` visible in 48450-48455). Step names in `strings.txt` 4306-4315: `departure_step_follow_bd`, `departure_step_leave`, `departure_step_start_blade`, `cut_step_departure_station`. The exact numbering of departure sub-steps is **[I]** (the full machine was not analyzed step by step).

---

## 4. Mowing — state 5, `process_cutting` (0x0802bcb4…0x080305e0)

Two levels of sub-states:
- **main-step** `ctx_cut+0x138`: normal forward driving vs "random move to escape".
- **cycle sub-step** `ctx_cut+0x134` (set by `FUN_0802f97c`, values 0..6): normal(0)/driving along the border "cycle"(1)/…/stall-recover(5)/ultrasonic-recover(6). **[V]** 41142-41155.

### 4a. Blade start + safety + beep — `process_cutting` start-blade sub-step (41180-41340)
- Checks before the blade rotates: **in station?** → error; **lift (raised)?** `vtable+0x34` → go to random-move; **updown==6** → `brake and goto error updown`; **hit==3** → random-move; **slope 4/2/5** → handling. **[V]** 41199-41244.
- Spin-up of the blade: `vtable+0x10 -> +0x14/+0x18` (forward/backward), success = `uVar3==2` → `start blade finish, turn to step random move`. **[V]** 41272-41276.
- **Blade reverse for self-cleaning**: counter `+0x148`, `%3==0` → main-step=6 (reverse), on `>0xe(14)` → error `start blade stall`. **[V]** 41295-41314.
- **Beep/sound**: the sound service `FUN_08077e10(3)` is pulled on every transition (this is probably the "beep" before/during blade start). There are strings `cutting_without_beep` (0x080875d4), `speaker_en`/`config product speaker` (0x080775e0). **[I]** — that `FUN_08077e10(3)` = specifically a beep is not proven byte by byte (**[U]** exact sound code).

### 4b. Normal forward driving
- In normal mode with cut sub-step=0: the drive goes forward (`front`), logs `unnormal front, set normal` / `blade normal, change to normal`. **[V]** 42301.
- Sensors are constantly polled: border, hit (bump/bumper), lift, slope/tilt, ultrasonic, stall (wheel jam). **[V]** (everywhere in 42060-42300).

### 4c. REACTION TO THE BORDER (in→out) — core `FUN_08030128` @0x08030128 (size 3618)
This is "border situation front" — what to do when the border sensors show going past the wire. **[V]** 42111-42298.

Key sensor polls:
- `FUN_0805deec()==0` → **right sensor went past the border** → `right border out, cycle left` (or `…back then cycle`). **[V]** 42111-42128.
- `FUN_0805df08()==0` → **left one went out** → `left border out, cycle right`. **[V]** 42130-42147.
- `FUN_0805e140()==0` → **both went out** → `border all out, cycle left/right` (the side is chosen by `FUN_0802c424`). **[V]** 42149-42177.
- Narrow corridor: `narrow front border, cycle left/right` / `narrow but no saved action`. **[V]** 42180-42233.
- No signal at all: `border all no signal, cycle left` (`vtable+0x88`). **[V]** 42274-42277.
- Near the station: `front but block station, back cycle left, angle=135/45`. **[V]** strings 0x0802ceaf/cf18.

Primitives called from here:
- `FUN_0802c1c8(dir, 0x2d0)` — **issue a "cycle" command (turn along/away from the border)**, dir=1/2, angle-param 0x2d0=720. Internally decides "cycle immediately" (`vtable+0x88`) or "first back, then cycle" based on `FUN_0804e3e0()`. **[V]** 40698-…, 42113/42132/42209.
- `FUN_0802f97c(N)` — reassign the cycle sub-step. **[V]**.
- `FUN_0804e3e0()` = byte-flag `*(DAT+3)` — "whether backing up is needed first" (in-border/on-the-wire). **[V]** 62514.

### 4d. RANDOM TURN ANGLE
- **Range RNG**: `FUN_0804f2c8(min,max)` = `rand % (max-min+1) + min`. **[V]** 63152:
  ```c
  iVar2 = (param_2 - param_1) + 1;
  return (iVar1 - iVar2*(iVar1/iVar2)) + param_1;   // == rand()%(max-min+1)+min
  ```
- Example call: `FUN_0804f2c8(0x4b,0x87)` = **random angle 75°..135°**, then `vtable+0x84 (0x104, 1, angle)`. **[V]** 42263-42264.
- **Random "left/right"**: `FUN_0804f2ee()` = `rand&1 ? 1 : 2`. **[V]** 63?/`FUN_0804f2ee`.
- Confirming strings: dozens of `... cycle left/right, angle=%d`, `back cycle, angle=%d`, `cycle slipe times=%d, back cycle, angle=%d`. **[V]** strings 895-996.

### 4e. Angle → ticks (for turn commands) — `FUN_08020520` @0x08020520
```c
return (param_1 * 0x2b9) / uVar2;   // ticks = angle * 697 / speed_param(model)
```
- `0x2b9 = 697`. `uVar2` depends on "model"/speed (field `ctx+0x32/0x3a/0x42/0x4a/0x52` by model index 0..4). **[V]** 31306-31335. Model error → log `model error, angle could not change to tick`.

---

## 5. Motion primitives and the `+0xe4` queue

All primitives place a 0x24-byte record into the ring `*(ctx_move+4)+0xe4` and advance the pointer (`*ptr += 0x24`). The consumer (the motion service, registered in the scheduler §1) pulls records and turns the wheels. **[V]** 30877-31141.

| Primitive | Address | What it writes into the record | Meaning |
|---|---|---|---|
| **forward** `FUN_0801fb04(spd,dist)` | 0x0801fb04 | `+4=2, +8=2, +0xc=2`, `+0x10/0x12=spd`, `+0x14/0x1c=dist` | drive **forward** [V] 30884-30895 |
| **back** `FUN_0801fe8c(spd,dist)` | 0x0801fe8c | `+4=1, +8=1, +0xc=1` | drive **back** [V] 30970-30981 |
| **turn** `FUN_08020164(dir,spd,dist)` | 0x08020164 | dir=1→code 3, dir=2→code 4; otherwise log `turn dir param err` | **turn in place**, dir 1/2 [V] 31095-31140 |

- The higher-level "cycle" (turn with an arc/angle) goes through the **motion vtable** `+0x60(dist,dir,angle)`, `+0x84(spd,dir,angle)`, `+0x88(dir,angle)` — used in departure and in the border reaction. **[V]** 42092/42264/48417.
- Record fields: `+4` type(1=back/2=fwd/3=turnL/4=turnR), `+8` submode, `+0x10/0x12` param1 (speed/angle), `+0x14/0x1c` param2 (distance). **[V]**.

---

## 6. Blade service + sound

- **`service_blade` `FUN_08022938`** @0x08022938 — the ticking blade service (in the scheduler). Reads flags `+0x25` (requested direction), `+0x26` (force), `+0x27` (current direction), `+0x28`. Drives the blade motor via `vtable+0x10 -> +0x14 (forward) / +0x18 (backward)`. Logs `force blade foreward, speed=%d` / `force blade backward, speed=%d`. Periodically **reverses** the blade (toggles +0x27). **[V]** 32850-32920.
- Setting blade direction/brake: `FUN_08022a80` (`set blade direction`), strings `set blade direction failed`, `set blade brake failed`, `blade service not running, cutting function could not excute`. **[V]** 32928-32963, strings 606-608, 0x08023917.
- **Sound**: `FUN_08077e10(n)` is pulled on mowing transitions; `set_product_enable_speaker` (0x080861df), `speaker_en`, `cutting_without_beep`. **[I]** that this is specifically a safety beep; **[U]** exact tone generation.

---

## 7. Border / docking / charging (states 6/7/8)

- **find_bd (6)**: launched from wait/on loss of the border — strings `bdboard start search border`, `bdboard find border`, `lost bd, find border in, goto find bd`. Set by `FUN_0807976c(6)` (44479, 44509). **[V]**.
- **docking (7)**: when the "to base" command is given and the robot is on the wire — `FUN_0807976c(7)` in `FUN_080391e4`, log `dock command and on border, change ...`; `robot on border, goto docking`. The robot drives along the border to the station. **[V]** 44503, strings 2023/3002/3018.
- **charging (8)**: `FUN_0807976c(8)` — `idle dock command and in station`, `Robot push into station, start charging`, `on schedule but battery voltage low, goto charging`. Current/stage control: `change to normal/trickle/temp-protect charging step`. **[V]** 44468, 36486, strings 705-707.
- **error (9)**: forced by the helper `FUN_08083d9c` (which itself sets `FUN_0807976c(9)`, 93238) — called before every `goto error`. Also `FUN_0802bce4(...)` (state 9) on fatal blade/updown situations. **[V]** 93238, 40450.
- **power_off (10)**: `FUN_08025be0/FUN_08025c16`. **[V]** 35405-35429.

---

## 8. Protections (lift / tilt / stall / hit / ultrasonic)

Polled every tick in cutting/departure; the reaction is going into "cycle/back cycle/random move":
- **lift (wheels raised)**: `find border but lift, reverse cycle right/left`, `back lift, cycle …, angle=%d`, `start blade but lift, turn to step random move to escape`. **[V]** strings.
- **tilt/slope**: `back slope up, pitch=%d,roll=%d, cycle …`, `find border but slope, pitch=%d,roll=%d, reverse cycle …`. **[V]**.
- **stall (wheel jam)**: `front, wheel stall, back cycle …`, `wheel block time=%d, back cycle`, `torque back wheel stall, blade brake and escape`, `cycle stall recover`. **[V]** 42066-42079, strings 958/993.
- **hit (bump/bumper)**: `torque cycle hit, blade brake and escape`, `back hit, cycle …, angle=%d`, `front but block station …`. **[V]**.
- **ultrasonic (obstacle)**: `front ultrasonic, back cycle …`, `cycle ultrasonic recover`. **[V]** 42090-42102.
- General technique: on hit/stall with torque — **blade brake + "escape"** (`blade brake and escape`). **[V]**.

---

## 9. Step-by-step cycle table (user narrative → code)

| # | Scenario step | Function(address) | What it does | Conf |
|---|---|---|---|---|
| 1 | "mow" command | cmd `0x100de` → `FUN_08050448`+0x14 (0x080504xx) | accept the control byte, set a pending command | [V] 55577 |
| 2 | idle decides to leave | `process_wait` `FUN_080270a8`/`FUN_08028910` → `FUN_0807976c(4)` | pending → **state 4** | [V] 36424/28910 |
| 3 | Backed out of the base | departure `back` + `vtable+0xa0` | drive back, measure distance | [V] 48417 |
| 4 | Stopped | distance reached / drive idle | stop | [V] 48411-48417 |
| 5 | Turn in place | `vtable+0x60(100,1,0x2d0)` (cycle) | turn around | [V] 48417 |
| 6 | Border check + beep | `FUN_08077e10(3)`, "in border" check | sound/log, check | [I] |
| 7 | Turned the blade on | start-blade sub-step (41180-41340), `service_blade FUN_08022938` | spin up the blade, safety check | [V] 41199-41276 |
| 8 | Drives forward | cut sub-step=0, `forward FUN_0801fb04` (via the service) | normal driving | [V] 42301 |
| 9 | departure→cutting transition | `FUN_08054274` → `FUN_0807976c(5)` | **state 5** | [V] 67088 |
| 10 | Hit the wire (in→out) | `FUN_08030128`: `FUN_0805deec/df08/e140` | detect going past the border | [V] 42111-42177 |
| 11 | Stop + backed up | `FUN_0804e3e0()` → "back then cycle" / `back FUN_0801fe8c` | reverse away from the border | [V] 42124/62514 |
| 12 | Turn by a random angle | `FUN_0804f2c8(75,135)` + `FUN_0804f2ee()` + `FUN_0802c1c8(dir,720)` | random angle/side, cycle | [V] 42263/63152 |
| 13 | Angle→ticks | `FUN_08020520` = `angle*697/spd` | recompute into turn ticks | [V] 31335 |
| 14 | Forward again | cut sub-step→0, `forward` | continue mowing | [V] |
| 15 | Border again → repeat 10-14 | the same `FUN_08030128` | mowing looped | [V] |
| 16 | "To base"/discharge/schedule | `FUN_0807976c(6/7/8)` | find_bd / docking / charging | [V] 44479/44503/44468 |
| 17 | Fault | `FUN_08083d9c`→`FUN_0807976c(9)` | stop/error | [V] 93238 |

---

## 10. Where exactly the "loop" and conditions are

- **Main eternal loop**: `FUN_08053af0` `do{…}while(true)` — ticks services + the `run` of the current state @100 Hz. **[V]** 66420-66505.
- **The mowing loop** (border→turn→forward→border) is physically realized by the fact that `process_cutting_run` each tick calls `FUN_08030128` again, and the cut sub-step transitions (`FUN_0802f97c`) keep the robot in state 5 until a command/discharge/error arrives. There is no explicit `for/while` "perimeter traversal" — it is an event-driven state machine. **[V/I]**.
- **Conditions for changing the top-level state**: only through `FUN_0807976c(N)` (see the call map §0/§7). Within cutting the change is through `+0x138`/`+0x134`, not through `0x10`.

---

## 11. Honest gaps [U]

1. **[U] Exact table of numeric values of the control byte** of command `0x100de` (which number = "mow", "to base", "stop"). Only the mechanism is proven (`FUN_08050448`+0x14) and that `0x20`=restart.
2. **[U] Moment/code of registering the `run` functions and services** into the scheduler tables (`ctx->sched` off 0x08/0x1c/0x30/0x44/0x50/0x5c). It is visible that the tables are called, but the code filling them is not cited — the state↔slot link is inferred from logs ([I]).
3. **[U] Full numbering of departure sub-steps** (the `case` machine in 0x08037xxx) — only the key branches (leave/cycle/start_blade) are analyzed, not all.
4. **[U] What exactly `FUN_08077e10(n)` does** — presumably sound/beep, but not confirmed byte by byte; tone generation was not analyzed.
5. **[I] Correspondence dir 1↔left / 2↔right** in cycle: from the strings `cycle left/right` next to the dir arguments, but an unambiguous "1==left" is not fixed in the code.
6. **[U] Full list of fields of the `+0xe4` queue record** beyond +4/+8/+0xc/+0x10/+0x14 (submode semantics of +8) — partial.
7. Drive physically: this file describes the **logic**, not why on OUR firmware the wheel does not drive (see `motor-investigation-FACTS.md`). The A4963/RUN gate is not considered here.
