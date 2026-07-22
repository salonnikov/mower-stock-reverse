# ARCHITECTURE.md — Unified map of the MI 302 lawn-mower robot

Assembled from `reverse-v2/reports/subsystem-findings.md` (6 subsystem analyses) + cross-check against `chip1/{decompiled_all.c,symbols.txt,strings.txt}` and `chip2/`.
Marks: **✔** — confirmed by code/string in the fresh decompilation; **ⓗ** — hypothesis, needs verification on a live chip/re-dump.
We do not trust the old `docs/`.

> ⚠️ **CORRECTION 2026-07-13:** read mentions of «Allegro A4963» as **Fortior FU6832N** —
> there is physically no A4963 on the board, the «A4963 protocol» is implemented by the FU6832 firmware (SPI slave).
> The SPI reverse from the chip1 side is valid; the semantics from the A4963 datasheet are not.
> See `reports/PLAN-2026-07-13-coils-wheels-fu6832.md` and memory `fu6832-drive-architecture`.

---

## 1. System overview

Three MCUs + Allegro A4963 BLDC drivers.

> ⚠️ **CORRECTION 2026-07-13:** throughout the document below, read «Allegro A4963» as **three Fortior FU6832N**
> (smart BLDC controller 8051+FOC, emulating the A4963 protocol over SPI). There is no physical A4963 on the board.
> The SPI reverse from the chip1 side is valid; conclusions from the A4963 datasheet are not. See `reports/PLAN-2026-07-13-coils-wheels-fu6832.md`.

```
            ┌───────────────────────┐
            │  DISPLAY BOARD (UI)    │  buttons/LED, ledport protocol
            └───────────┬───────────┘
                        │ UART, frame &..#, XOR 0x5B, CRC8/MAXIM  (bidir.)
                        ▼
   ┌──────────────────────────────────────────────────────┐
   │  chip1  GD32F305  — MAIN BRAIN                         │
   │  FreeRTOS (11 tasks), FSM 3..10, command dispatchers,  │
   │  wheel+blade drive (PWM+SPI A4963), odometry (tacho),  │
   │  IMU BNO055 (I2C), BMS (serial), navigation/border     │
   └───────┬───────────────────────────────┬───────────────┘
           │ USART2 (0x40004800)           │ SPI  (motor_spi_mutex)
           │ frame &..#, XOR 0x5B, CRC8      │ addr<<13|data (verify+retry)
           │ bidirectional                 ▼
           ▼                        ┌──────────────────────────┐
   ┌───────────────────────┐        │  A4963 ×3: L-wheel /      │
   │ chip2 GD32F30x — COILS │        │  R-wheel / blade (BLDC)   │
   │ border wave reception, │        └──────────────────────────┘
   │ DSP→area/str, lift ×2  │
   └───────────────────────┘
```

**Physical links:**
- **chip1 ↔ chip2 (borders):** USART2 `0x40004800` (chip2 side), DMA, mutex+timeout, ≤128 bytes. Frame: `&`(0x26) + body + CRC8 + `#`(0x23); **body XOR 0x5B**; CRC8 = Dallas/MAXIM poly 0x8C. **Bidirectional** (chip2 parses start-search/set-version). ✔
- **chip1 ↔ display (UI/commands):** same frame format (XOR 0x5B, `&..#`, CRC8). RX feeds the JSON dispatcher. ✔
- **chip1 → A4963 ×3:** SPI (shared `motor_spi_mutex`), word `addr<<13|data`, verify+retry. + PWM into the timer's CHxCV. ✔
- **chip1 → external SPI flash:** BB/DB/LB firmware sub-images (OTA store). ✔

---

## 2. chip1 map (GD32F305) — main brain

### 2.1 Flash layout

| Region | Address | Contents |
|---|---|---|
| Bootloader | `0x08000000–0x08017FFF` (96K) | vector@0x08000000 (SP=0x20016ed8, Reset=**FUN_08011a3c** @0x08011a3c), POST, USB-updater, EasyFlash ENV, sub-programmers, HW-CRC, boot FSM, jump-to-app ✔ |
| Boot CRC word | `0x08017FFC` | expected bootloader CRC ⓗ (boundary matches) |
| MB Application | `0x08018000`+ (max 0xE8000) | 2nd vector table, reset=*(0x08018004)=**FUN_08018440** ✔ |
| Sub-images (ext. SPI flash) | BB@0x100000, DB@0x130000, LB@0x430000 | SetFirmInfo `FUN_0800214c` ✔ |

- **Jump-to-app** = `FUN_080012d0`: MSP←*(0x08018000), jump to *(0x08018004). Only checks `(SP&mask)==0x20000000` and `base<reset`. **VTOR is NOT written by the bootloader.** ✔
- **CRC = hardware GD32 @0x40023000**, poly 0x04C11DB7, init 0xFFFFFFFF, 32-bit big-endian, no reversal/final-XOR. compute `FUN_08008e1c`, reset `FUN_08008e38`. ✔
  - Boot gate (Reset `FUN_08011a3c`): CRC over 0x08000000..0x08017FF8 vs *(0x08017FFC); mismatch → `FUN_08011946` eternal halt. **The application is NOT CRC-checked at startup.** ✔
  - App launch — no CRC. ✔
  - 3× «CRC check error» = OTA of the BB/DB/LB sub-boards (`FUN_08006658/08006ac0/08006d3c`), only during OTA. ✔
  - MB self-OTA `FUN_08006fe8` — the only CRC of its own application, during an update (mb_sv<MB_BVER vs ENV MB_CRC). Not a boot gate. ✔
- **ENV = armink EasyFlash**, keys MB_/BB_/DB_/LB_/BTL_ (*_SIZE/_CRC/_BVER/_sv/_BRF). get `FUN_0800aa84`, set `FUN_0800ae40`. ✔

### 2.2 FreeRTOS tasks (wrapper `FUN_080849c0`, 11 tasks)

| Task | Role | Note |
|---|---|---|
| `initial` | Brings up services, then self-deletes | start |
| **`highspeed loop`** | Fast loop — **this is where the STATE MACHINE ticks** (process_*_run) | autonomy core |
| `time` | RTC / schedule | |
| `bms` | Battery (smart pack over serial) | |
| `bdport` | Border reception/parsing (coils) | |
| `map` | Map/multizones | |
| `dp send` / `dp receive` | UART protocol: RX feeds the JSON dispatcher | UI/commands link |
| `send_log` | Logs | |
| **`imu fuse`** | Sensor fusion: tilt/heading | |
| `IDLE` | FreeRTOS idle | |

`process manager` / `mems` / `ledport` are services inside the highspeed loop / imu fuse, not separate tasks. ✔

### 2.3 State machine

`state = *(0x200000bc)` (base 0x200000ac + 0x10). Setter `FUN_0807976c` (clamp **0..10**), getter `FUN_0804edd4`. No direct writes bypassing the setter found (all through ~22 calls/wrappers). ✔

| val | State | Module (process_*) | Proof |
|---|---|---|---|
| 3 | IDLE/standby | process_waiting | "change to idle", "Robot manual stopped" |
| 4 | DEPARTURE/leaving | process_departure_smooth | "start work", "goto departure" |
| 5 | CUTTING/mowing | process_cutting | "in border, change to cutting" |
| 6 | FIND BORDER | process_find_bd | "not on border, change to find border" |
| 7 | DOCKING | process_docking_smooth | "on border, change to dock" |
| 8 | CHARGING | process_charging | "in station, change to charging" |
| 9 | ERROR | process_error | abort FUN_08083d9c→set(9) |
| 10 | POWER OFF | process_power_off | "Robot manual power off" |

(0/1/2 are unused; error wrappers set 9/10 by the error-code mask.) ✔

```
                    control-byte 0x300100de (start)
          ┌──────────────────────────────────────────┐
          ▼                                           │
  [3 IDLE] ──start──► [4 DEPARTURE] ──in border──► [5 CUTTING]
     ▲  ▲                                             │
     │  │                                    not on border
     │  │                                             ▼
     │  └── stop/manual ◄────────────────────── [6 FIND BORDER]
     │                                               │ on border
     │                                               ▼
  [3 IDLE] ◄── charge done ── [8 CHARGING] ◄─in station─ [7 DOCKING]
                                                         ▲ dock cmd
  any ── error(mask) ──► [9 ERROR]
  any ── power off ──────► [10 POWER OFF]
```

### 2.4 Subsystems

**Wheel drive = 3× BLDC on Allegro A4963** (L-wheel/R-wheel/blade), sensorless, shared `motor_spi_mutex`. Files `a4963_snk_v2.c`, `driver_{left,right,blade}_motor_snk_v2.c`. ✔
- Speed→PWM: `FUN_0807f696(base,ch,val)` writes CHxCV (0x34/38/3c/40 = CH0..3CV). **Timer = TIMER2 `0x40000400`** (read from the literals: PWM-out `DAT_0805fac4`=0x40000400, left/right/blade init bases ALL =0x40000400). All 3 A4963 (2 wheels + blade) are fed by the 4 channels of TIMER2; the left wheel = ch3 (offset 0x40). Motor ctx RAM=`0x2000054c`. ✔ (matched the live finding «TIMER2 CCRx»)
- Direction/brake/config→SPI A4963: `FUN_0808408c(spi,word)` word=`addr<<13|data`, verify+retry; init pours 8 config registers. ✔
- brake↔dir `FUN_0801f4dc` (RUN/BRAKE/COAST in a shadow reg.), set_direction `FUN_0801f4c6`, set_left_dir `FUN_0805f300`. ✔
- **PWM alone cannot spin it** — until the A4963 is configured over SPI and is in RUN, the PWM is ignored. ✔
- **set_motor_speed `FUN_0805f620`** ✔ (force-disasm closed the hole): `pwm = speed*0x1c20/gear_max`, gear_max ∈ {6141,3069,718}, gears 0/1/2, brake-to-dir on change, final `FUN_0805f300`+`FUN_0807f696(timer,3,pwm)`. See `chip1/forced_functions.c`.

**Odometry — a separate timer TIMER3 `0x40000800`** (NOT the PWM one), **input-capture** mode channel 1 (`FUN_0807f86c(...,1,0x10002)`). ISR `FUN_0805fbc0`(0x0805fbc0)/`FUN_0805fd45` increment ticks by direction (+0x18 fwd/+0x1c back), speed from the period (capture-read `FUN_0807f402`). Callback registration `FUN_08062c64/8c(4,...)`. The old fixed addresses 0x2000be28/be58 are ABSENT from the binary (wheel structs on malloc). ✔
> **What is proven by STATIC code (only this):** `FUN_0807f474` puts CH3 into output mode (CHxMS=00)+CHxEN, `FUN_0807f648(…,0x60)`=OCxM PWM, `FUN_0805f620`→`FUN_0807f696` writes the duty into CH3CV. That is, the speed path in the code goes through the CH3CV register of TIMER2 (this is an output channel, NOT input-capture; odometry is on a separate TIMER3). ✔ (static only)
> **NOT proven (no clean live measurement):** that writing to CH3CV PHYSICALLY spins/brakes the wheel; that «CCR=0 stops». The former «live confirmation» from the old live-findings is DISCARDED as a false-positive — the mower back then stopped because it ran into the boundary wire (border-stop), not because of the write into CCR. The physical effect of CCR and the role of the SPI-RUN of the A4963 require a clean bench measurement (wheels lifted, WITHOUT border navigation). ⓗ

**Drive(speed,angle)** = `FUN_0804bb0c` via the movement-base vtable (reset/set_speed/set_target/start). Angle→ticks `FUN_08020520`: `angle*697/K` (K is per-model). ✔

**Blade:** service `FUN_08022938` (service_blade), driver = the same A4963 path (`FUN_0808408c`): 16-bit command — enable=bit 0x1000, dir=bit 0x2, speed=bits 0x0FFF. There is NO separate GPIO/timer for the blade. Speed cfg blade_sp/blade_max_sp. Protections: stall→reverse, highload→speed change, overslope/lift/torque→brake&escape, blade_check_en. ✔

**IMU = Bosch BNO055** I²C (dev 0x50 = 7-bit 0x28), chip-id `A0 FB 32 0F`@reg0x00. `driver_mems_snk_v13.c`. Angles reg 0x1A, 6 bytes (EUL heading/roll/pitch), /16→integer degrees. Object: heading+0x18/roll+0x1a/pitch+0x1c (avg +0x1e/0x20/0x22). Slope detect = pitch+roll (not heading). The ICM-42688 alternative (WHOAMI 0x47, probably SPI) is also compiled in. I2C0(0x40005400) vs I2C1(0x40005800) — not resolved. ✔/ⓗ

**BMS = smart pack over serial with a CRC frame** (driver_battery_snk_v1/v2, service_bms). Per-cell voltages, chemistry (sony/samsung/eve), telemetry "battery vol=%dmV, percent=%d, min/max, temp=%d". SoC from OCV, thresholds from cfg voltage points. **NOT ADC.** ✔

**Coil reception (border)** on chip1: deal_message `FUN_0801fb80`, CRC8 `FUN_080241d0`, cJSON `FUN_08023f80`, bdport callback `FUN_08021bb4` (version/reset/log/find-border). Border model (service_border `FUN_0804d4c8`): L str+0x20/area+0x24, R str+0x28/area+0x2c. **area∈{0,1,2} = outside/inside/on-line, str = magnitude.** Near-station — a composite (border correctness + voltage + debounce). Crossing in→out → «cycle left/right» maneuvers. ✔

**Rain** = service_rain (rain_en/rain_delay, digital input via port-service). **Lift** = service_lift (lift_en, debounce). **Updown/flip** — from IMU pitch/roll. **Temperature** — from the pack + IMU die-temp (no separate NTC ADC). ✔/ⓗ (rain/lift GPIO behind the abstraction)

---

## 3. chip2 map (GD32F30x) — border coil board

**✅ The dump is COMPLETE (256 KB), the coil ADC path is in it.** (The earlier conclusion «a re-dump is needed» was a false-negative — the code accesses the ADC via a pointer from the literal pool, not via an SVD label.)

- **Digitization `FUN_08019bf4` (dual-ADC, called from `FUN_08012b6c`):** `_DAT_08019cf0`=ADC0(0x40012400), `_DAT_08019cf4`=ADC1(0x40012800), DMA0(0x40020000). `FUN_08012a04(ADC0,rank0,ch5,st7)` + `FUN_08012a04(ADC1,rank0,ch9,st7)` → **two coils = ADC0/channel5 + ADC1/channel9**, sample-time 7, dual (0x20000), DMA, buffer 800 samples. ✔ (`FUN_08012a04` = RSQ/SAMPT config, verified against the registers). Which of them is LEFT/RIGHT — from the wiring/`send_border`, ⓗ.
- Coil collector object: RAM `0x20000078`, 7-function vtable (`DAT_08016c28..40` → code in the dump), `FUN_08016ba8`. ✔
- **Baseline `FUN_0801a1f8`:** sum of 8 ADC samples → average /8 → `*DAT_0801a250`; valid [1906..2191] (~2048). ✔
- Wave via the provider vtable `FUN_08016ba8` (type code 0/1/2/3 + magnitude). DSP/classification: `process_deal_board.c` (`FUN_080164bc`/`FUN_08016784`), wave queue, «disturb» rejection. Output: **area(0/1/2/3) + str**. ✔
- Link chip2→chip1: `send_border_message FUN_08019914` (cJSON {area/str L+R + lift flag}), TX `FUN_08019a18` (≤128 bytes, mutex+timeout, DMA, USART2 0x40004800). ✔
- Frame `&`+body+CRC8+`#`, body XOR 0x5B (`FUN_08013ae4`), CRC8/MAXIM poly 0x8C (`FUN_08013008`). Bidirectional. ✔
- Lift: 2 switches (left/right), debounce 10, `deal_lift_sensor FUN_08013970`. ✔

---

## 4. End-to-end data flows

### 4.1 «Border» (coil → maneuver)
```
coil (analog) ─► chip2 factory-driver ADC/DSP  [NOT IN THE DUMP ⚠️]
   ─► vtable provider FUN_08016ba8 (type 0..3 + magnitude)
   ─► process_deal_board FUN_080164bc/08016784 (queue, disturb rejection)
   ─► area(0..3)+str  ─► send_border_message FUN_08019914 (cJSON L/R + lift)
   ─► TX FUN_08019a18: &+body+CRC8+#, body XOR 0x5B  ─── USART2 ───►
chip1: de-XOR ─► deframer (&..#, CRC8 FUN_080241d0) ─► cJSON FUN_08023f80
   ─► deal_message FUN_0801fb80 ─► bdport callback FUN_08021bb4
   ─► service_border FUN_0804d4c8 (L str+0x20/area+0x24, R str+0x28/area+0x2c)
   ─► navigation: in→out ⇒ «cycle left/right» maneuver  ─► drive (see 4.3)
```

### 4.2 «Command» (display → FSM)
```
display ── UART ──► chip1
   ─► de-XOR 0x5B FUN_0803953c ─► deframer FUN_0806e938 (&..#, CRC8)
   ─► cJSON FUN_08023fe6 ─► dpport DISPATCHER FUN_080470f0 (16 slots, +0x60 cat/+0x64 handler)
   ─► category = cmd&0xff000000 = 0x30 ─► FUN_08044c80
   ─► control-byte 0x300100de (start/stop/dock/charge) ─► FUN_08050448 (+0x14)
   ─► set-state FUN_0807976c ─► [FSM 3..10]
   (0x300101dc = restart, byte 0x20)
```
Dispatchers: dpport `FUN_080470f0`(16), bdport `FUN_08063880`(5), ledport `FUN_0806396c`(5). The {cat→handler} table — a static const in .rodata (not filled by code; the pointers cannot be pulled from the pseudo-C → raw .rodata is needed). ✔

Commands 0x30xxxxxx (FUN_08044c80): 06/07=manual-set on/off (**flag only!**), 09=login pwd, 10=set pwd, 11-16=RTC/schedule, 17=rain, 18=multizone, 1a=name, 1b/1c=SN, **20=OTA**, 21=wifi, 22=ble, 23=reset pwd; 82-8d=config setters; **0x300100de=CONTROL**, **0x300101dc=restart**. ✔

### 4.3 «Motion» (FSM → wheel → feedback)
```
FSM (highspeed loop, process_*_run)
   ─► drive(speed,angle) FUN_0804bb0c ─► movement-base vtable (reset/set_speed/set_target/start)
   ─► primitives: turn/dir FUN_08020164, fwd FUN_0801fb04, back FUN_0801fe8c
   ─► write into the motor-command QUEUE +0xe4
   ─► set_motor_speed FUN_0805f620 (clamp 1533..6141) [DECOMPILER HOLE ⚠️]
   ─► PWM: CHxCV FUN_0807f696 (L=CH3)  +  SPI A4963 dir/brake FUN_0801f4dc/FUN_0808408c
   ─► WHEEL (BLDC RUN)
Feedback:
   tacho/FG A4963 ─► input-capture ─► ISR FUN_0805fbc0/FUN_0805fc7c
   ─► ticks +0x18 fwd/+0x1c back, speed from the period (FUN_0807f402)
   ─► odometry (malloc structs) ─► navigation
```
**There is NO direct JOG/motion command in the protocol** — the +0xe4 queue is driven only by the autonomous FSM through the vtable, and is unreachable from the protocol. ✔

### 4.4 «Blade»
```
FSM/service_blade FUN_08022938 ─► A4963 path FUN_0808408c
   ─► 16-bit: enable 0x1000 | dir 0x2 | speed 0x0FFF (cfg blade_sp/blade_max_sp)
   ─► protections: stall→reverse, highload→speed, overslope/lift/torque→brake&escape
```

### 4.5 «Charging / BMS»
```
FSM 7 DOCKING ─(on border, near-station composite)─► FSM 8 CHARGING (process_charging)
BMS task ◄─ serial CRC-frame pack ─► per-cell V, SoC(OCV), temp, chemistry
   ─► cfg voltage points thresholds ─► charge done ─► FSM 3 IDLE
```

---

## 5. Unified table of key addresses

Legend: ✔ confirmed • ⓗ hypothesis/partial • ⚠️ hole/re-dump needed.

**Boot / CRC / OTA / ENV (chip1)**
| Function | Address | Role | |
|---|---|---|---|
| Reset (bootloader) | FUN_08011a3c | POST + boot-CRC gate | ✔ |
| halt | FUN_08011946 | eternal halt on CRC mismatch | ✔ |
| jump-to-app | FUN_080012d0 | MSP+jump to the application | ✔ |
| App reset | FUN_08018440 | application entry point @*(0x08018004) | ✔ |
| SetFirmInfo | FUN_0800214c | sub-image addresses BB/DB/LB | ✔ |
| CRC compute / reset | FUN_08008e1c / FUN_08008e38 | HW-CRC GD32 @0x40023000 | ✔ |
| OTA of sub-boards | FUN_08006658/08006ac0/08006d3c | CRC BB/DB/LB | ✔ |
| MB self-OTA | FUN_08006fe8 | CRC of its own application (OTA only) | ✔ |
| ENV get / set | FUN_0800aa84 / FUN_0800ae40 | EasyFlash keys | ✔ |

**Protocol / dispatcher (chip1)**
| Function | Address | Role | |
|---|---|---|---|
| de-XOR / XOR | FUN_0803953c / FUN_08039e8c | XOR 0x5B RX/TX | ✔ |
| deframer | FUN_0806e938 | search for &..#, CRC8 | ✔ |
| cJSON parse | FUN_08023fe6 / FUN_08023f80 | | ✔ |
| CRC8 MAXIM | FUN_080241d0 | poly 0x8C | ✔ |
| dpport / bdport / ledport | FUN_080470f0 / FUN_08063880 / FUN_0806396c | dispatchers | ✔ |
| cat 0x30 handler | FUN_08044c80 | commands 0x30xxxxxx | ✔ |
| control-byte handler | FUN_08050448 (+0x14) | start/stop/dock/charge | ✔ |
| build msg / TX | FUN_0801fb80 | frame assembly | ✔ |

**FSM / tasks / config (chip1)**
| Function | Address | Role | |
|---|---|---|---|
| state var | 0x200000bc | current state | ✔ |
| set-state / get-state | FUN_0807976c / FUN_0804edd4 | clamp 0..10 | ✔ |
| abort→error | FUN_08083d9c | set(9) | ✔ |
| RTOS wrapper | FUN_080849c0 | creation of 11 tasks | ✔ |
| config parser | FUN_08076b2c | ~110 keys, input 0xa5 | ✔ |

**Drive / odometry / blade (chip1)**
| Function | Address | Role | |
|---|---|---|---|
| drive(speed,angle) | FUN_0804bb0c | movement-base vtable | ✔ |
| angle→ticks | FUN_08020520 | angle*697/K | ✔ |
| primitives turn/fwd/back | FUN_08020164 / FUN_0801fb04 / FUN_0801fe8c | write into the +0xe4 queue | ✔ |
| set_motor_speed | FUN_0805f620 | clamp 1533..6141 | ⚠️ hole |
| PWM CHxCV | FUN_0807f696 | L=CH3 | ✔/ⓗ base |
| A4963 SPI | FUN_0808408c | addr<<13\|data verify+retry | ✔ |
| brake↔dir / set_dir / set_left_dir | FUN_0801f4dc / FUN_0801f4c6 / FUN_0805f300 | RUN/BRAKE/COAST | ✔ |
| capture ISR L/R | FUN_0805fbc0 / FUN_0805fc7c | ticks +0x18/+0x1c | ✔ |
| capture read / registration | FUN_0807f402 / FUN_08062c64/8c | period→speed | ✔ |
| service_blade | FUN_08022938 | blade | ✔ |

**Border / IMU (chip1)**
| Function | Address | Role | |
|---|---|---|---|
| service_border | FUN_0804d4c8 | L/R str+area | ✔ |
| bdport callback | FUN_08021bb4 | version/reset/log/find-border | ✔ |

**Coils (chip2)**
| Function | Address | Role | |
|---|---|---|---|
| wave provider | FUN_08016ba8 | type 0..3 + magnitude | ✔ |
| DSP deal_board | FUN_080164bc / FUN_08016784 | area/str, disturb rejection | ✔ |
| send_border_message / TX | FUN_08019914 / FUN_08019a18 | cJSON, USART2 | ✔ |
| XOR / CRC8 | FUN_08013ae4 / FUN_08013008 | 0x5B / poly 0x8C | ✔ |
| deal_lift_sensor | FUN_08013970 | 2 switch, debounce 10 | ✔ |
| **ADC/DSP factory-driver** | — | driver_bdsensor.c | ⚠️ NOT IN THE DUMP |

---

## 6. Points for customization (for future phases)

**(A) Manual RC control** — two paths:
- **Software (own handler in the dispatcher):** take a free dpport slot (e.g. category 0x31) → the handler directly drives the primitives `FUN_08020164`/`FUN_0801fb04`/`FUN_0801fe8c` or writes into the motor-command queue `+0xe4`. Requires a rebuild/patch of the application. There is NO direct JOG in the stock protocol — it has to be added. ✔
- **Over SWD (without firmware):** write into the `+0xe4` queue or call a primitive / `set_motor_speed FUN_0805f620` directly. A fast prototype, but requires physical access. ✔
- Important: before spinning, the A4963 must be configured over SPI and in RUN (otherwise the PWM is ignored). ✔

**(B) Rebuild of the application without recomputing the CRC** — **possible.** A patch of 0x08018000+ and flashing over SWD load WITHOUT CRC. Conditions: a valid SP@0x08018000 and reset@0x08018004; no pending OTA (mb_sv≥MB_BVER, otherwise the bootloader re-flashes from the external flash). A patch of the BOOTLOADER (0x08000000..0x08017FF8) requires recomputing the HW-CRC and writing it into 0x08017FFC. ✔

**(C) Tech debt — status:**
- ✅ **chip2 coil ADC/DSP** — SOLVED without a re-dump: the code is in the dump (`FUN_08019bf4` dual-ADC: ADC0 ch5=L / ADC1 ch9=R; baseline `FUN_0801a1f8`). A re-dump is NOT needed.
- ✅ **`set_motor_speed FUN_0805f620`** — SOLVED by force-disasm (gear-map, PWM→CH3). See `chip1/forced_functions.c`.
- ⚙️ **{cat→handler} tables** of the dispatchers and the **PWM timer number** (DAT_0805b144) — solvable by reading the raw literal bytes from the existing dump (not a re-dump): it is enough to read 4 bytes at the flash address of the label. TODO desk task.

---

## 7. Open questions (ⓗ) — not proven, need a live chip / re-dump

1. **Boot-CRC word @0x08017FFC** — the boundary matches, but the exact value/format is not verified on hardware. ⓗ
2. ✅ ~~chip2 ADC path~~ — SOLVED: ADC0 ch5=L / ADC1 ch9=R, sample-time 7, dual, baseline avg-8 [1906..2191] (`FUN_08019bf4`/`FUN_0801a1f8`).
3. ✅ ~~set_motor_speed FUN_0805f620~~ — SOLVED by force-disasm (gear-map pwm=speed*7200/gear_max, →CH3).
4. ✅ ~~PWM timer number~~ — SOLVED: **TIMER2 `0x40000400`**, 4 channels for 2 wheels + blade (motor ctx RAM 0x2000054c).
5. **IMU I2C bus** — I2C0(0x40005400) vs I2C1(0x40005800) not resolved. ⓗ
6. **rain/lift GPIO pins** on chip1 — behind the port-service abstraction, the specific pins are unknown. ⓗ
7. **Per-model K** in angle→ticks (`FUN_08020520`, angle*697/K) — the value from cfg/model. ⓗ
8. **Dispatcher handler pointers** (.rodata cat→handler table) — not extracted from the decompilation. ⚠️
9. **ICM-42688 IMU alternative** — compiled in, but whether it is used on the MI 302 (vs BNO055) — not proven. ⓗ
10. **The exact set of control-bytes** (besides 0x300100de/0x300101dc) and their start/stop/dock/charge subcommands — the mapping is partial. ⓗ
```
