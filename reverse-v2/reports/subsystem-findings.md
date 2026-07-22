# Subsystem findings (reverse-v2, 2026-07-03) — 6 parallel analyses of chip1/chip2

Source: only the fresh decompilation `reverse-v2/chip1|chip2`. ✔ confirmed by code/string, ⓗ hypothesis. We do not trust the old docs/.

---

## 1. BOOT / BOOTLOADER / FLASH / CRC / OTA (chip1)

**Flash layout:**
- Bootloader `0x08000000–0x08017FFF` (96 KB): vector @0x08000000 (SP=0x20016ed8, Reset=0x08011a3c), POST, USB updater, EasyFlash ENV, sub-board programmers, HW-CRC, boot.c FSM, jump-to-app. ✔
- Expected bootloader CRC — word `0x08017FFC`. ⓗ (boundary matches)
- **MB Application `0x08018000`+** (max 0xE8000): 2nd vector table @0x08018000, reset=*(0x08018004)=FUN_08018440. ✔ (SetFirmInfo case1 base 0x18000)
- Sub-images in the EXTERNAL SPI flash: BB@0x100000, DB@0x130000, LB@0x430000 (SetFirmInfo `FUN_0800214c`). ✔

**Jump-to-app = `FUN_080012d0`:** loads the MSP from *(0x08018000), jumps to *(0x08018004). Checks only: (SP & mask)==0x20000000 and base<reset. **VTOR is not written by the bootloader.** ✔

**CRC = hardware GD32 @0x40023000** (poly 0x04C11DB7, init 0xFFFFFFFF, 32-bit big-endian words, no reversal / final XOR):
- compute `FUN_08008e1c` (feeds CRC_DATA), reset `FUN_08008e38` (CRC_CTL bit0). ✔
- **(a) Boot-gate — ONLY the bootloader:** in Reset `FUN_08011a3c`: CRC over 0x08000000..0x08017FF8 (0x5fff words), compared against *(0x08017FFC); mismatch → `FUN_08011946` eternal halt. **The application is NOT CRC'd at startup.** ✔
- **(b) App launch — WITHOUT CRC.** ✔
- **(c) Three "CRC check error" = OTA checks of the SUB-BOARDS** (BB/DB/LB, `FUN_08006658/08006ac0/08006d3c`): expected from ENV *_CRC, get = the CRC computed by the remote board. Only during OTA. ✔
- **(d) MB self-OTA `FUN_08006fe8`:** the only place where the chip CRCs its own application — during an update (mb_sv<MB_BVER) it compares against ENV MB_CRC. Not the boot-gate. ✔

**ENV = armink EasyFlash**, keys MB_*/BB_*/DB_*/LB_*/BTL_* (*_SIZE/*_CRC/*_BVER/*_sv/*_BRF). get `FUN_0800aa84`, set `FUN_0800ae40`. ✔

**🎯 CONCLUSION:** patching the APPLICATION (0x08018000+) + flashing over SWD → boots WITHOUT recomputing the CRC. Conditions: a valid SP@0x08018000 and reset@0x08018004, and no pending OTA (mb_sv≥MB_BVER, otherwise it re-flashes from external flash). Patching the BOOTLOADER → the HW-CRC over 0x08000000..0x08017FF8 must be recomputed → written to 0x08017FFC.

---

## 2. WHEEL DRIVE + ODOMETRY (chip1)

**Wheels = BLDC on the external sensorless Allegro A4963 driver** (3 of them: left wheel/right wheel/blade, shared `motor_spi_mutex`). Files: `a4963_snk_v2.c`, `driver_left/right/blade_motor_snk_v2.c`. ✔
- **Speed → PWM** into the timer register CHxCV: `FUN_0807f696(base,ch,val)` writes 0x34/0x38/0x3c/0x40 (=CH0..3CV). Left wheel = **CH3**, base in the literal DAT_0805b144 (timer number not extracted — raw bytes needed). ✔/ⓗ
- **Direction/brake/config → SPI A4963** with verify+retry: `FUN_0808408c(spi,word)` (word: addr<<13|data). Init pours in 8 config registers. ✔
- **brake-to-dir `FUN_0801f4dc`:** toggles the RUN/BRAKE/COAST mode bits in the shadow A4963 register and pours over SPI. set_direction `FUN_0801f4c6`, set_left_dir `FUN_0805f300`. ✔
- **You cannot spin with PWM alone** — until the A4963 is configured over SPI and in RUN, the PWM is ignored (explains the past "CCR doesn't spin"). ✔
- **set_motor_speed `FUN_0805f620`** ✔ (force-disasm, 1114 bytes) — the "gearbox": `pwm = speed*0x1c20 / gear_max`, gear_max ∈ {`0x17fd`=6141, `0xbfd`=3069, `0x2ce`=718}, gear 0/1/2 in `*(ctx+0x10)`, on change — `FUN_0801f4dc` brake-to-dir. Final: `FUN_0805f300(dir)` + **`FUN_0807f696(_DAT_0805fac4, 3, pwm)`** — writing the PWM into **CH3** of the left wheel timer, shadow in `*(*ctx+2)`. Saturates to `0x1c20`=7200. File: `reverse-v2/chip1/forced_functions.c`.

**Odometry — NOT fixed addresses.** The old `0x2000be28/be58` are ABSENT from the binary (wheel structs are on malloc). The real mechanism: A4963 tacho/FG signal → **timer input-capture**; ISR callbacks `FUN_0805fbc0`/`FUN_0805fc7c` increment the ticks by direction (+0x18 fwd/+0x1c back) and compute speed by period (capture read `FUN_0807f402`). Callback registration `FUN_08062c64/8c`. ✔

**Drive(speed,angle) = `FUN_0804bb0c`** via the motion base vtable (reset/set_speed/set_target/start). Angle→ticks `FUN_08020520`: `angle*697/K` (K per-model). ✔

---

## 3. BLADE / IMU / SENSORS (chip1)

**Blade:** service `FUN_08022938` (service_blade.c). Driver `driver_blade_motor_snk_v2` = the same A4963 path (`FUN_0808408c`): 16-bit command, enable=bit 0x1000, dir=bit 0x2, speed=bits 0x0FFF. NO separate blade GPIO/timer — everything over serial. Speed from cfg blade_sp/blade_max_sp. Protections: stall→reverse, highload→speed change, overslope/lift/torque→brake&escape, blade_check_en. ✔

**IMU = Bosch BNO055** over I²C (dev 0x50=7-bit 0x28), chip-id `A0 FB 32 0F` @reg0x00. `driver_mems_snk_v13.c`. Angles: reg 0x1A, 6 bytes (EUL heading/roll/pitch), /16 → **integer degrees**. Ranges heading 0..360/roll±90/pitch±180. Angle object: heading+0x18/roll+0x1a/pitch+0x1c (+ avg +0x1e/+0x20/+0x22). Slope detection uses **pitch+roll** (not heading). The alternative ICM-42688 (TDK) is also compiled in (WHOAMI 0x47, probably SPI). ✔
- I2C0(0x40005400) vs I2C1(0x40005800) — not resolved. ⓗ

**Battery = smart pack over serial with a CRC frame** (driver_battery_snk_v1/v2, service_bms). Per-cell voltages, chemistry (sony/samsung/eve), telemetry `battery vol=%dmV, percent=%d, min/max, temp=%d`. SoC by OCV. Thresholds from cfg voltage points. NOT ADC. ✔

**Rain** = service_rain (rain_en/rain_delay, digital input via the port service). **Lift** = service_lift (lift_en, debounce counter). **Updown/flip** = computed from IMU pitch/roll. **Temperature** = from the pack + IMU die temp (no separate NTC ADC). ✔/ⓗ (rain/lift GPIO pins are behind the abstraction)

---

## 4. COILS / BORDER (chip2 + reception on chip1)

**✅ CORRECTED (post-revision): the chip2 dump is COMPLETE, the coil ADC path IS IN IT. No re-dump needed.** The primary agent gave a false-negative conclusion (it searched by the SVD tag `ADC0`, while the code goes through the pointer `_DAT_08019cf0`=ADC0, `_DAT_08019cf4`=ADC1; in the raw bytes the ADC0/ADC1/DMA0 bases are present).
- **Digitization `FUN_08019bf4` (dual-ADC, called):** two coils = **ADC0/channel5 + ADC1/channel9**, sample-time 7, dual (0x20000), DMA, 800-sample buffer. ✔ (`FUN_08012a04` verified = RSQ/SAMPT config; RAM object 0x20000078, 7-vtable in the dump). Which is LEFT/RIGHT — does not follow from the ADC, by routing/send_border (ⓗ).
- **Baseline `FUN_0801a1f8`:** sum of 8 samples /8 → `*DAT_0801a250`; valid [1906..2191]. ✔
- The chip2 application takes the waveform via the provider vtable `FUN_08016ba8` (per side: type-code 0/1/2/3 + magnitude). DSP processing/classification: `process_deal_board.c` (`FUN_080164bc/08016784`), a waveform queue, "disturb" rejection. Output: `area`(0/1/2/3) + `str`. ✔

**Link chip2→chip1:**
- `send_border_message = FUN_08019914` (chip2): cJSON {area/str L+R + lift flag}. TX `FUN_08019a18`: ≤128 bytes, mutex+timeout, DMA. USART2 (0x40004800). ✔
- **Frame `&`(0x26)+body+CRC8+`#`(0x23)**, body **XOR 0x5B** (`FUN_08013ae4`), CRC8/MAXIM poly 0x8C (`FUN_08013008`). NOT raw JSON. ✔
- **The link is BIDIRECTIONAL** — chip2 also parses incoming (start-search, set-version). ✔
- **Lift on chip2**: 2 switches (left/right), debounce 10, `deal_lift_sensor FUN_08013970`. ✔

**Reception on chip1:** deal_message `FUN_0801fb80` (build), CRC8 `FUN_080241d0`, cJSON `FUN_08023f80`. bdport callback `FUN_08021bb4` (version/reset/log/find-border). Border model (service_border.c, `FUN_0804d4c8`): L str+0x20/area+0x24, R str+0x28/area+0x2c. **area∈{0,1,2}=outside/inside/on-line, str=magnitude** (=ls/rs). ✔
**Near-station** — a composite (border-signal correctness + voltage + debounce), not a simple threshold. An in→out crossing → "cycle left/right" navigation maneuvers. ✔

---

## 5. COMMAND DISPATCHER / PROTOCOL (chip1)

**RX pipeline:** UART → de-XOR 0x5B `FUN_0803953c` → deframer `FUN_0806e938` (looks for `&`…`#`, CRC8) → cJSON `FUN_08023fe6` → dispatcher `FUN_080470f0`. TX: build `FUN_0801fb80` + XOR `FUN_08039e8c`. ✔
**CRC8** = Dallas/Maxim `FUN_080241d0` (poly 0x8C reflected, init 0, bitwise). ✔
**Dispatchers:** dpport `FUN_080470f0` (16 slots, +0x60 cat/+0x64 handler), bdport `FUN_08063880` (5), ledport `FUN_0806396c` (5). Category = the high byte of cmd `&0xff000000`. ✔
**Table {cat→handler} = a static const in .rodata** — not filled by code, the handler pointers cannot be pulled out of the pseudo-C (raw .rodata needed). Active category **0x30 → `FUN_08044c80`**. ✔

**Commands 0x30xxxxxx** (FUN_08044c80): 06/07=manual-set on/off (flag only!), 09=login pwd, 10=set pwd, 11-16=RTC/schedule, 17=rain, 18=multizone, 1a=name, 1b/1c=SN, 20=**OTA**, 21=wifi, 22=ble, 23=reset pwd; 82-8d=config setters (schedule/rain/led/ultrasonic/map); **0x300100de=CONTROL byte** (start/stop/dock/charge → `FUN_08050448`+0x14), **0x300101dc=restart** (byte 0x20). ✔

**🎯 THERE IS NO DIRECT MOVE/JOG COMMAND IN THE PROTOCOL.** Motors move only via the control byte → autonomous scenarios. Motion primitives `FUN_08020164` (turn/dir), `FUN_0801fb04`/`FUN_0801fe8c` (fwd/back) write into the **motor-command queue +0xe4**, but are called only by the autonomous FSM via the vtable, unreachable from the protocol. → RC: (A) our own handler in a free dispatcher slot (e.g. cat 0x31) pokes the primitives; (B) over SWD, write into the queue +0xe4 / call a primitive. ✔

---

## 6. STATE MACHINE / TASKS / CONFIG (chip1)

**state = `0x200000bc`** (base 0x200000ac + 0x10). Setter `FUN_0807976c` (clamp **0..10**), getter `FUN_0804edd4`. **No direct writes bypassing the setter found** (all via 22 calls + wrapper trampolines). ✔

| val | State | Module | Evidence |
|----|----|----|----|
| 3 | **IDLE/standby** | process_waiting | "change to idle", "Robot manual stopped" |
| 4 | **DEPARTURE/exit** | process_departure_smooth | "start work", "goto departure" |
| 5 | **CUTTING/mowing** | process_cutting | "in border, change to cutting" |
| 6 | **FIND BORDER** | process_find_bd | "not on border, change to find border" |
| 7 | **DOCKING** | process_docking_smooth | "on border, change to dock" |
| 8 | **CHARGING** | process_charging | "in station, change to charging" |
| 9 | **ERROR** | process_error | abort FUN_08083d9c→set(9) |
| 10 | **POWER OFF** | process_power_off | "Robot manual power off" |

(0/1/2 are not used. The error wrappers take the mask of the error code, set the state to 9/10.) ✔

**FreeRTOS tasks** (wrapper `FUN_080849c0`, 11 tasks): `initial` (brings up services, self-deletes), **`highspeed loop`** (fast loop — this is where the STATE MACHINE process_*_run ticks), `time` (RTC/schedule), `bms`, `bdport` (border), `map`, `dp send`/`dp receive` (RX feeds the JSON dispatcher)/`send_log`, **`imu fuse`** (tilt/heading sensor fusion), `IDLE`. `process manager`/`mems`/`ledport` — services inside the highspeed loop/imu fuse, not separate tasks. ✔

**Config:** parser `FUN_08076b2c` (~110 keys), entry via the control byte 0xa5 from "cfgstr". Geometry: perimeter→+0x64, ratio→+0x74(double), wheel_dis→+0x66, blade_diameter→+0x68, bmotor_polar/cmotor_polar→+0x60/+0x61, blade_sp→+0x6c. Flags *_en: pwd/rain/fslip/ult/led/gps/sch/zone/com/hit/lift/mems/border/mtrack/wlch/auto_off/blade_check/speaker/smooth/rtt/qdock/area_limit/map_update/rb_en_mag/lboard. ✔

---

## Summary of first-iteration corrections
- ❌→✔ "CRC is not hardware" (my early mistake) → **hardware** GD32 CRC, poly 0x04C11DB7.
- ✔ The boot CRC gates **only the bootloader**, not the application (first iteration: "gate over app, 1-byte bypass" — wrong; the application needs NO bypass).
- ✔ Wheels = **A4963 BLDC** (PWM+SPI), not a direct CCR steer.
- ✔ Odometry — **malloc**, addresses 0x2000be28/be58 absent from the binary (input-capture tacho, not EXTI/encoder).
- ✔ Coil link — **frame+CRC8+XOR 0x5B, bidirectional** (not "raw one-way JSON").
- ✔ States redefined: 3=idle,4=departure,5=cutting,6=findbd,7=dock,8=charge,9=error,10=poweroff.
- ✅ The chip2 dump is COMPLETE — the ADC path is there (ADC0/ch5 + ADC1/ch9, `FUN_08019bf4`, RAM object 0x20000078), no re-dump needed (a re-run agent gave a false-negative conclusion). L/R channel binding — ⓗ.
- ✅ set_wheel_speed FUN_0805f620 — force-disasm closed the gap (gear-map, PWM→CH3).
