# Factory chip1 — GPIO + peripheral map + diff against our firmware

**Fresh version 2026-07-08** (old one → `reverse-v2/archive/2026-07-08/`). Clean current snapshot:
only what is confirmed, and an honest "tried / not tried" status.

GPIO source: **live measurement** of the factory firmware over SWD (idle state=3 + one snapshot of state5 driving) —
CTL0/CTL1/OCTL of all ports decoded. Peripherals: RCU EN registers (live) + reverse engineering.
Context: we are looking for what enables the **switched 20U power rail** of the wheels (the blade on a live rail spins
even under our fw, the wheels are dead). Full investigation status: `reports/WHEELS-STATE-2026-07-08.md`.

## 1. Factory GPIO map (live, idle state=3)

OUT-PP=push-pull output (=1/0 level); AF=alt-func; IN=input; AIN=analog; OD=open-drain.

| Pin | Factory (live) | Purpose (reverse) | Our fw | Match? |
|---|---|---|---|---|
| PA0 | OUT-PP HIGH | UART DE (comms) | OUT LOW | not crit (comms) |
| PA2 | AF | USART1 TX | — | |
| PA4 | OUT-PP HIGH | SPI0 CS (comms) | — | not crit (comms) |
| PA5/6/7 | AF/IN/AF | SPI0 SCK/MISO/MOSI | — | |
| PA9 | AF | USART0 TX | — | |
| PA13/14/15 | SWD | debug | — | |
| PB0 | OUT-PP HIGH | secondary power latch | OUT HIGH | = |
| PB1 | AIN | analog (current?) | — | |
| PB9 | OUT-PP HIGH | UART DE (comms) | — | not crit (comms) |
| PB10/11 | AF-OD | I2C1 (IMU) SCL/SDA | — | |
| PB12 | OUT-PP HIGH | A4963 common enable | OUT HIGH | = |
| PB13/14/15 | AF(SPI1) | SPI1 to A4963 | AF | = |
| PC4/5 | AIN | battery V/I | AIN | = |
| PC7/8/9 | AF | TIMER2 PWM CH1/2/3 (blade/right/left) | AF | = |
| PC10 | AF | UART3 TX | — | |
| PD3 | OUT-PP HIGH | blade CS | OUT HIGH | = |
| PD4 | OUT-PP HIGH | right CS **+ right run/coast** | OUT HIGH | = |
| PD5 | IN-float (idle) | left CS | OUT HIGH | ≠ (not crit) |
| PD7 | OUT-PP HIGH→**LOW when driving** | left run/coast (LOW=run) | implemented LOW when driving | = (did not help) |
| PD8 | OUT-PP HIGH | USART2 TX (BMS, half-duplex) | AF | (same link) |
| PD9 | — | USART2 RX (BMS) | AF | |
| PE7 | OUT-PP HIGH | aux rail | OUT HIGH | = |
| PE9 | OUT-PP **LOW** | (role unclear) | LOW (corrected) | = (did not help) |
| PD11 | OUT-PP **LOW** | (role unclear) | LOW (corrected) | = (did not help) |
| PE0 | OUT-PP HIGH (app) | enable event (FUN_08063368) | tried HIGH | ≠→test did not help |
| PE1/PE5 | OUT-OD LOW | (set by the bootloader, we inherit it) | = | = |
| PE12 | OUT-PP HIGH | MAIN power latch | OUT HIGH | = |

## 2. Factory peripherals (RCU EN, live)
- **APB2EN=0x527d:** AFIO, GPIOA-E, ADC0 (battery), TIMER0 (advanced), USART0.
- **APB1EN=0x184e000f:** TIMER1, **TIMER2 (motor PWM)**, **TIMER3 (wheel tacho)**, TIMER4, USART1,
  **USART2 (BMS pack)**, UART3, **I2C1 (IMU ICM-42688)**, DAC, PMU.
- **AHBEN=0x57:** DMA0, DMA1, SRAM, FMC, CRC.
- Our fw enables: GPIOA-E, AFIO, ADC0, **TIMER2**, SPI1, USART2, DMA0.
  We do not enable: TIMER0/1/3/4, USART0/1/UART3, I2C1, DAC.
  Extra on the factory side while driving = **TIMER3/TIMER4 (tacho) + I2C1 (IMU)** — sensors, not the power path.

## 3. Key code entry points (reverse, confirmed)
- reset → C-runtime `FUN_08019430` → **main `FUN_08061714`** → pre-scheduler `FUN_0807f010`
  (RCU+flash, does not touch GPIO; RCU-enable via `FUN_0806c77c(id)`) → FreeRTOS `FUN_0806bbd8`
  (task "initial") → body @0x0805bc28 → **rw_init `FUN_080477c4`** (creates drivers: tilt→battery→BLADE→RIGHT→LEFT).
- Power latches: **PE12 HIGH** (main, bootloader) + **PB0 HIGH** (secondary). They power the whole board, not drive-specific.
- FSM setter **`FUN_0807976c(state)`**: 3=idle/unlock, 4=departure, 5=cutting, 9=move?, 10=docked/charge.
- Motor drivers: left `FUN_0805aee0`, right `FUN_0806fc2c`; A4963 SPI-write `FUN_0808408c`
  (write+readback, WR bit `word|0x1000`), transfer `FUN_080840d8`. Modules: `driver_left/right/blade_motor_snk_v2.c`, `a4963_snk_v2.c`.

## 4. MAIN CONCLUSION (current)
**"Wheels do not drive" ≠ firmware GPIO/SPI/arm/pack/periph-enable — all of that has been checked and ruled out.**
- The blade SPINS under our fw (even without SPI config) ⇒ the blade VBB is present, A4963 auto-run (RUN=1 in EEPROM),
  PWM (TIMER2), CS — all working. To drive, ONLY power+PWM is needed.
- Our fw MATCHED the factory in the driving state (PD7/PD4 LOW + TIMER2 PWM) and periph-enable. The wheels are still dead.
⇒ **The wheel power rail 20U is OFF under our fw, and its enable is neither a firmware-GPIO nor a periph-enable.**
The blade is on a separate (live) rail. What remains: (A) hardware gate of the 20U high-side switch (Q10/Q17/Q14);
(B) discharge FET of the power motor tap in the pack (BMS); (C) the unexplored module `movebase_snk_v10.c` (the drive branch).

## 5. What we TRIED and it did NOT work (do not reopen)
- GPIO shotgun over all pins; direct match to the factory state5 (PD7/PD4 LOW + PWM).
- Candidates **PA4/PB9 HIGH** — turned out to be UART/SPI comms (not power); the test did not help.
- **PE9/PD11 → LOW** (were erroneously HIGH; the factory holds them LOW) — corrected, did not help.
- **PE0 → HIGH** (like the factory) — did not help.
- **PD7/PD4 run/coast LOW when driving** (exact copy of state5) — did not help.
- Pack/BMS over USART2: link established (connected=1, response 3A A3), connect bytes `1C A1 03 C1 01 2E` —
  but polling/handshake **does NOT enable the wheel VBB** (in its current form). See `reports/WHEELS-STATE-2026-07-08.md`.
- **TIMER0 config** (was the top lead) — offline analysis: TIMER0 = a delay timer, not gate-drive;
  the motor driver uses TIMER2+TIMER3, not TIMER0. Analysis in the archive `archive/2026-07-08/SESSION-2026-07-07-TIMER0-analysis.md`.
- Base station: the mower drove on the factory fw even WITHOUT the base ⇒ not the base.

## 6. What we did NOT try (open directions)
- **`movebase_snk_v10.c`** — the wheel motion service: not reversed, may toggle something when driving starts. ★ first.
- **BMS discharge-enable** — whether there is an "enable power discharge" command to the pack and when (the telemetry parser
  does not set it; "set charge current" / into-exit-charge as a side-effect enable have not been checked). ★.
- Hardware gate of the 20U switch (Q10/Q17/Q14) — continuity check of the gate factory-while-driving vs ours (per the user's decision — low priority).
