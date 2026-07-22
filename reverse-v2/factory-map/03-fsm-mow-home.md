# 03 — FSM "MOW" and "HOME" + motion service (factory chip1, GD32F4xx)

FSM branch: idle→departure→cutting and return/dock. Goal — to find what the factory does at the
START OF DRIVING that would enable the switched wheel power rail "20U" and which we lack
(the blade spins with our fw, the left+right wheels are dead).

Sources: `chip1/decompiled_all.c`, `strings.txt`, `analysis/callgraph-chip1.tsv`, the dump
`dist/gd32-mainboard-dump-v1.bin`. Investigation status: `reports/WHEELS-STATE-2026-07-08.md`.
Pin map: `reference/FACTORY-GPIO-PERIPHERAL-MAP.md`.

> IMPORTANT (architecture): the firmware is service-oriented (framework/service/*). The FSM processes
> do NOT write registers directly — they poke services through a **vtable** (indirect calls
> `(**(obj+off))()`). Because of this the callgraph does not see edges to motion/motor/BMS, and the real
> writes to GPIO/TIMER/SPI are hidden inside driver methods. The links below are traced by hand.

---

## 1. FUNCTION MAP

### FSM core
| Function | Address | Module | Role |
|---|---|---|---|
| FUN_0807976c | 0807976c | (core) | **FSM setter**: `*(DAT_08079778+0x10)=state` (state<0xb). 3=idle,4=departure,5=cutting,6/7/8=dock-substates,9=move/err,10=docked/charge |
| FUN_080270a8 | 080270a8 | process_wait.c | Idle handler (state3): safety/schedule checks → `FUN_0807976c(4)` |
| FUN_08054274 | 08054274 | process_wait/manager | departure→cutting transition detector → `FUN_0807976c(5)` |
| FUN_0803a7a0 | 0803a7a0 | process_cutting.c | Main state dispatcher (switch over state) |
| FUN_080679f8 | 080679f8 | process_departure_smooth.c | Main process loop |
| FUN_0802fa90 | 0802fa90 | process_cutting.c | Mowing controller: `FUN_08077e10(3)`+`FUN_0802bce4` (blade) |
| FUN_080391e4 | 080391e4 | process (dock) | Dock command: state 6/7/8 |
| FUN_08025fd4 | 08025fd4 | process_charging.c | Charge/dock loop (state 10), exits into state 9 |

### Motion service (movebase_snk_v10.c) + motor commands
| Function | Address | Role |
|---|---|---|
| FUN_08055f98 | 08055f98 | **movebase "set path / go"**: writes the speed-profile table (off 0x2c..0x52) by station type/direction, sets running-flag=1, `FUN_08078ee8(0)` |
| FUN_08077e10 | 08077e10 | **Motion/loop** (param 1=departure, 2=blade, 3=cutting): computes speed `FUN_0804f2c8(0x3c,0x87)`, vtable call to wheels, "enable" vtable @+0xa8 |
| FUN_08078ee8 | 08078ee8 | change_speed_model (case 0..4): profile selection, vtable @(obj+0xc)+0xc |
| FUN_08051f2c | 08051f2c | movebase service singleton |
| FUN_08079020 | 08079020 | mm→ticks recompute (wheel_perimeter, reduction ratio) |
| FUN_0802bce4 | 0802bce4 | Blade start: vtable +0x54 (prepare), +0x34 (enable), `FUN_080784bc(speed)` |

### Motor drivers (driver_left/right/blade_motor_snk_v2.c, a4963_snk_v2.c)
| Function | Address | Role |
|---|---|---|
| FUN_0805aee0 | 0805aee0 | **LEFT motor INIT**: RCU `FUN_0806c77c(0x701)`, GPIO AF-init, A4963 config (7 words) via `FUN_0808408c` |
| FUN_0805f300 | 0805f300 | **Left direction**: SPI-write A4963 `FUN_0801f4c6(dev+0x28,cfg+0x30,dir)` (bit1 reg+0x0e) |
| FUN_0805f3d8 | 0805f3d8 | **Left run/coast**: GPIO `FUN_080534f8/fc(DAT=GPIOD, 0x80)` = **PD7** |
| FUN_0806fc2c | 0806fc2c | **Right run/coast**: GPIO `FUN_080534f8/fc(DAT, 0x10)` = **PD4** |
| FUN_0806fb54 | 0806fb54 | Right direction: `FUN_0801f4c6(...)` (SPI A4963) |
| FUN_08022a80 / FUN_08022dc8 | — | Blade direction/enable (A4963, same path) |
| FUN_0801f4c6 | 0801f4c6 | A4963-write: reg(cfg+0x0e) bit1 ±, then `FUN_0808408c(dev,word)` |
| FUN_0801f4dc | 0801f4dc | A4963-write mode bits [6:4] reg(cfg+0x0a): 1→0x30,0→0x20,2→0x10. **NOT called statically (dead/indirect)** |
| FUN_0808408c | 0808408c | **A4963 SPI write+verify**: writes `word|0x1000` (WR bit), reads back, compares the low 12 bits; up to 0x31 attempts |
| FUN_080840d8 | 080840d8 | One SPI transfer to A4963 (CS assert cb[0], word, CS deassert cb[1]) |
| FUN_0806c77c | 0806c77c | RCU periph-enable: `*(base+(id>>6)) |= 1<<(id&0x1f)` |

### BMS / pack (service_bms.c)
| Function | Address | Role |
|---|---|---|
| FUN_08056980 | 08056980 | **bms_service init**: enable-value param_2[2]∈{0,1}→ +0xa5; creates the BMS task; vtable (+0x20)+0x1c(always), (+0x20)+0x20 if state==10 |
| FUN_08072948 | 08072948 | **BMS state command** (task): ONLY if +0xa5==1 → by bits +0xa6 sends: +0x30 into_charge, +0x34 exit_charge, +0x38/+0x3c/+0x40 other |

---

## 2. "MOW" TIMELINE: state3 → state4 → state5

### state3 (idle) — FUN_080270a8 (process_wait.c)
1. `FUN_08052d34()` — sensor/lid status → flag +0x38.
2. `FUN_0804f758(...)` — **safety-check** (deal_safety.c). On error — log, stay in idle.
   ⚠️ Here there is a "can not discharger" gate (see §6): when the pack is protected the factory does not move.
3. `FUN_0805deec()` — schedule/start gate (condition `+0x24==0 && +0x2c==1`).
4. `FUN_0805034c()+0x14` — border-service (perimeter), NOT power.
5. **Transition:** when ready `FUN_0807976c(4)`.
   Logs: "auto departure delay time is reached, goto departure" @0x08027514,
   "Robot on schedule, start work" @0x0802756c.
   → NO writes to GPIO/TIMER/SPI at the transition itself. Only the FSM state.

### state4 (departure) — FUN_0803a7a0/FUN_0802fa90 → **FUN_08077e10(1)**
The actual start of driving. FUN_08077e10(1):
- `*(mv+0x130)=1` (departure direction), flags `+299=0xff`, `+0x145=0xff`.
- `FUN_0804e3e0()` — obstacle status; selects the wheel vtable method (+0x7c or +0x84).
- **speed** `uVar2=FUN_0804f2c8(0x3c,0x87)` (PWM range ~0x3c..0x87).
- **WHEEL COMMAND:** `(*pcVar6)(600, dir, speed)` — motion vtable (600=ramp ms).
- **"ENABLE":** `(**(mv+0xc)+0xa8)()` — vtable call "go" AFTER setting the speed.
- Reset counters +0x4e/+0x12f/+0x158.

Where these vtable calls lead (movebase→driver), i.e. the REAL HARDWARE:
- **TIMER2 (0x40000400)** CH2CV(+0x3c)=right, CH3CV(+0x40)=left — PWM duty.
- **A4963 direction** — SPI1 write `FUN_0801f4c6` → `FUN_0808408c` (`word|0x1000`).
- **Run/coast GPIO** — PD7 (left, FUN_0805f3d8) / PD4 (right, FUN_0806fc2c) via set/reset.
> Not a single SEPARATE "rail-enable" pin/register beyond PWM+direction+run — NOT found.

### state4 → state5 — FUN_08054274
- Condition `*(s+0x42) < *(s+0x40)` (departure distance covered) → `FUN_0807976c(5)`.

### state5 (cutting) — FUN_0802fa90 → FUN_08077e10(3) + blade FUN_0802bce4
- Blade: vtable prepare(+0x54), enable(+0x34), `FUN_080784bc(speed)` (writes `*(DAT_080784c4+8)`).
- Wheels: FUN_08077e10(3) — same path (PWM+dir+run), plus a big reset of cycle counters.

**"Mow" summary:** idle(safety/schedule) → FSM=4 → FUN_08077e10(1) [speed+vtable enable] →
[moving: TIMER2 CHx PWM + A4963 dir SPI + PD7/PD4 run] → distance covered → FSM=5 → blade+cycles.

---

## 3. "HOME" / DOCK TIMELINE

- "Home" command: FUN_080391e4 — depending on border presence sets state 6/7/8 (`FUN_0807976c(6/7/8)`),
  then `FUN_08078b18()` (clear flag). Logs "dock command…".
- Return/movement to base goes through the same **movebase** path (FUN_08077e10 / FUN_08055f98):
  PWM CH2/CH3 + A4963 dir + PD7/PD4 run. There is no separate "rail" action.
- Dock/charge — FUN_08025fd4 (process_charging.c), state 10. Here the **charge** commands to the
  pack are activated (into/exit charge, see §6), but this is CHARGING, not wheel power.
- ⚠️ The full bodies of process_docking_smooth.c / process_charging init (addresses 0x08085xxx) are BEYOND
  the decompile (the file ends ~0x08085/line 94394); not parsed in detail. A source limitation.

**"Home" summary:** the same motor path as "mow". Nothing new for the 20U rail was found.

---

## 4. MOVEBASE — how it moves (movebase_snk_v10.c)

- **FUN_08055f98** = "set path & go": by station type (0..4) and direction fills the speed-profile
  table in the object (off 0x2c..0x52: values 8000..21000 mm/s·profiles, 0x188..0x311 —
  accelerations), computes target distances `FUN_08079020` (mm→ticks via wheel_perimeter/reduction),
  sets `**(srv+4)=1` (running), `FUN_08078ee8(0)` (speed model 0). Threshold `param_1>=0x20`.
- Wheel speed is set NOT directly into a register, but as a "model" (FUN_08078ee8 case 0..4) + target
  profiles; the driver, via PID/tacho (TIMER3), drives the PWM (TIMER2 CHx). The duty value comes into
  CH2CV/CH3CV. Factory measurement on the move: ch2/ch3 0x681→0x1125 (force-capture) — this is the
  movebase output.
- At start (FUN_08077e10) — vtable "enable" @+0xa8. Traced as "start the regulator/apply PWM+run",
  NOT as a separate power-gate.

**Conclusion on movebase:** the motion engine forms a speed profile → PWM + direction(SPI A4963) + run(PD7/PD4).
movebase does not touch any additional "enable wheel rail" pin/bus.

---

## 5. KEY CODE (annotated)

### A4963 SPI write+verify (FUN_0808408c) — the only path to configure the motor chip
```c
// param_2 = A4963 word; on write the WR bit 0x1000 is set; read back, compare [11:0]
FUN_080840d8(dev, word);                 // CS↓; SPI1 TX (word|0x1000); CS↑
FUN_0806caac(dev, word&0xe000, &rd);     // read the register (address in [15:13])
if ((word&0xfff) != (rd&0xffff)) retry;  // up to 0x31 attempts, else -1
```
> On our side verify=0xFFFF even for the LIVE blade ⇒ our SPI1-READ does not work. If the wheel A4963 (unlike
> the blade) POR-default is NOT "RUN+forward", then a SPI config that never landed = a stationary wheel. See §6.

### Left motor: direction (SPI) and run/coast (GPIO PD7)
```c
FUN_0805f300: FUN_0801f4c6(dev+0x28, cfg+0x30, dir);   // A4963 config bit1 (reg+0x0e) = direction
FUN_0805f3d8: FUN_080534f8/fc(GPIOD, 0x80);            // PD7 set/reset = run/coast
FUN_0806fc2c: FUN_080534f8/fc(GPIOD?, 0x10);           // PD4 = right run/coast
```

### Motion start (FUN_08077e10, departure)
```c
*(mv+0x130)=1;                            // direction
speed = FUN_0804f2c8(0x3c,0x87);          // PWM speed
(*pcVar6)(600, dir, speed);               // wheel motion vtable (600ms ramp)
(**(mv+0xc)+0xa8)();                       // vtable "enable/go"  ← candidate, but = regulator+PWM+run
```

### BMS: enable-gate for pack commands (FUN_08072948)
```c
if (*(bms+0xa5)=='\x01') {                 // enable flag (set in FUN_08056980 from param_2[2])
  if (bits & 1)     (**(bms+0x20)+0x30)(); // send_cmd_into_charge
  if (bits<<0x1e<0) (**(bms+0x20)+0x34)(); // send_cmd_exit_charge
  ... +0x38 / +0x3c / +0x40                // other battery-state commands
}
```
> These are CHARGE commands (into/exit charge), not "enable discharge to the motor rail". There is NO separate
> "enable discharge FET" command in the chip1 strings (only discharge PROTECTION/monitoring, §6).

---

## 6. ★ CANDIDATES FOR ENABLING THE 20U RAIL (ranked)

The firmware motion path (idle→departure→cutting and dock) does NOT contain a separate action
"enable wheel rail" beyond PWM(TIMER2 CHx) + direction(A4963 SPI) + run(PD7/PD4) — all of which
we have already reproduced (the blade confirmed it). What remains:

### ★1. The wheel A4963 requires SPI config, and our SPI1-READ/verify does not work (NEW angle, firmware-testable)
- FACT: `FUN_0808408c` writes config with verify; on our side verify=0xFFFF ⇒ **SPI1 read does not work**, which
  means the write may NOT have applied (or the chip does not respond).
- The blade spins WITHOUT SPI (the blade A4963 POR-default = "spin"). BUT the wheel A4963 POR-default
  direction/mode may be different (coast/standby) ⇒ without SPI config that actually landed
  (words 0x30..0x3e: reg0..reg7, incl. `0xee0d`=reg7 RUN and `FUN_0801f4c6` direction) the wheel stays put.
- TO CHECK: fix SPI1 (CTL0/CTL1, MISO PB14, bit order/CPOL/CPHA) → achieve verify!=0xFFFF
  on the WHEEL A4963 → confirm that the 7 config words and the direction are actually written and verified.
  This is the only firmware element of the driving start that we definitely have NOT completed (verify fails).

### ★2. BMS/pack: the enable flag gates commands to the pack; movement may require an active link/keep-alive
- `FUN_08056980` puts enable(param_2[2]) into +0xa5; `FUN_08072948` without enable==1 sends NOTHING to the pack.
- The commands found = charge (into/exit), NOT an explicit "discharge FET on". A direct string
  "enable discharge output" is absent in chip1 — probably the discharge switch is held by the pack itself (via wake/keep-alive).
- Consistent with what is already done (keep-alive "we hold the motor VBB", connect `1C A1 03 C1 01 2E`): the link exists,
  but the wheel VBB has not risen yet. TO REFINE: (a) send enable=1 and the periodic `FUN_08072948`-equivalent;
  (b) look in the factory traffic on the move for a frame we lack (SWD dump of the pack RAM / factory USART2 log).
- Medium priority: overlaps with what is exhausted, but not closed byte-precisely.

### ★3. Hardware high-side switch for the 20U rail (Q10/Q17/Q14)
- If neither the wheel A4963 SPI config nor the BMS raises the VBB — a hardware 20U gate remains,
  controlled NOT by firmware GPIO (per prior conclusions). Ring out the gate of Q10/Q17/Q14 factory-on-the-move vs ours.
- Low priority per the user's decision (firmware first), but this is the default outcome if ★1/★2 miss.

### Removed / not confirmed
- A separate "rail-enable" pin/register in the FSM/movebase at the driving start — NOT found (re-checked).
- vtable "enable" @+0xa8 (FUN_08077e10) — this is starting the regulator (PWM+run), not a power-gate.
- BMS into/exit-charge — about CHARGING, not about wheel power.

---

## Limitations
- process_docking_smooth.c / process_charging init (0x08085xxx) — beyond the decompile.
- The real writes to GPIO/TIMER/SPI are hidden behind driver vtables; the motion links are traced by hand,
  but the exact CTL0/CTL1 of SPI1 and the wheel A4963 config order require a live check (★1).
