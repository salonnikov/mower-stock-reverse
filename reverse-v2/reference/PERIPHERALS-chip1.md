# CHIP1 (GD32F305, main brain of MI-302) — peripheral / IO map

Date: 2026-07-06. Chip: **chip1** = GD32F305xC (main board, "brain"). Dump: `dist/gd32-mainboard-dump-v1.bin`, base `0x08000000`, length 1 MB. Disasm: `dist/factory-full.asm`. Decompilation: `reverse-v2/chip1/`.

**Trust legend:** `[V]` verified (offset in the dump / asm line / literal quote) · `[I]` inferred (plausible from reverse engineering / hardware logic / live diff, NOT proven in this pass) · `[U]` unknown/disputed.

⚠️ All facts below concern **chip1**. Do not confuse with chip2 (coil board, which has its own USART2 and its own ADC pair). Where something is also present on chip2 — marked explicitly.

---

## 0. Inventory of used peripherals (census by base-address literals) `[V]`

Method: scanning the raw dump for 4-byte little-endian literals of peripheral base addresses. The count = how many times the base occurs in the image (a rough measure of usage). Zero ⇒ the peripheral is **not addressed** by the code.

| Peripheral | Base | Literal (LE) | Count | Conclusion |
|---|---|---|---|---|
| GPIOA | 0x40010800 | `00080140` | 24 | used `[V]` |
| GPIOB | 0x40010C00 | `000c0140` | 49 | used (most loaded port) `[V]` |
| GPIOC | 0x40011000 | `00100140` | 16 | used `[V]` |
| GPIOD | 0x40011400 | `00140140` | 28 | used `[V]` |
| GPIOE | 0x40011800 | `00180140` | 30 | used `[V]` |
| **GPIOF** | 0x40011C00 | `001c0140` | **0** | **NOT used** `[V]` |
| **GPIOG** | 0x40012000 | `00200140` | **0** | **NOT used** `[V]` |
| SPI1 | 0x40003800 | `00380040` | 3 | A4963 bus `[V]` |
| SPI0 | 0x40013000 | `00300140` | 5 | used (external SPI flash of sub-images?) `[V]`/`[I]` |
| USART0 | 0x40013800 | `00380140` | 12 | OTA/display board `[V]`(base)/`[I]`(purpose) |
| USART1 | 0x40004400 | `00440040` | 9 | bdport→border(chip2) `[V]`(base)/`[I]`(purpose) |
| USART2 | 0x40004800 | `00480040` | 24 | battery/BMS `[V]` |
| UART3 | 0x40004C00 | `004c0040` | 13 | LED board `[V]`(base)/`[I]`(purpose) |
| UART4 | 0x40005000 | `00500040` | 2 | lightly used `[V]`/`[U]`(purpose) |
| **I2C0** | 0x40005400 | `00540040` | **0** | **NOT used** `[V]` |
| **I2C1** | 0x40005800 | `00580040` | **8** | **IMU bus** `[V]` |
| ADC0 | 0x40012400 | `00240140` | 9 | used `[V]`, purpose `[U]` |
| **ADC1** | 0x40012800 | `00280140` | **0** | **NOT used** `[V]` |
| TIMER0 (adv) | 0x40012C00 | `002c0140` | 8 | used `[V]`, purpose `[U]` |
| TIMER2 | 0x40000400 | `00040040` | 38 | **motor PWM** `[V]` |
| TIMER3 | 0x40000800 | `00080040` | 23 | odometry/timebase `[V]` |
| TIMER4 | 0x4000_0C00 | `000c0040` | 7 | timebase/tacho `[V]`/`[I]` |
| TIMER7 (adv) | 0x40013400 | `00340140` | 3 | used `[V]`, purpose `[U]` |
| PMU | 0x40007000 | `00700040` | 6 | power-mgmt `[V]` |
| **DAC** | 0x40007400 | `00740040` | **0** | **NOT used** `[V]` |
| DMA0 | 0x40020000 | `00000240` | 29 | used `[V]` |
| DMA1 | 0x40020400 | `00040240` | 11 | used `[V]` |
| CRC | 0x40023000 | `00300240` | 5 | HW-CRC (boot-gate/OTA) `[V]` |

> ⚠️ TIMER1 (0x40000000, LE `00000040`) yields ~44 matches, but `0x40000000` = also the base of the entire APB1 region ⇒ the counter is noisy, use of TIMER1 specifically is not proven → `[U]`.

---

## 1. GPIO — map of the pins used

The pin-to-function layout below is assembled from reverse engineering + hardware-pinout (default AF) + live GPIO diff of the factory firmware. **Only ports A–E are used** (F/G empty `[V]`, see §0). Pin-level assignments not confirmed by asm/census are marked `[I]`; previously mistaken ones — with an explicit note.

| Pin | Dir. | Purpose | Trust / evidence |
|---|---|---|---|
| **PE12** | OUT | **Main power latch** (power-latch: HIGH=hold, LOW=off). | `[V]` behaviorally: our firmware drops PE12 → mower turns off; the power-off button works (motor-FACTS A#1). |
| **PB0** | OUT | Secondary power-latch line. | `[I]` (power-off reverse; not isolated in asm in this pass). |
| **PE10** | IN | Button (active-low), half of the pair. | `[I]` |
| **PE11** | IN | Button (active-low), half of the pair. | `[I]` |
| **PE8** | IN/OUT | Charging / charge-detect (`@charge power on`). | `[I]` |
| PE7 | OUT | Aux power rail (dropped in power-off). | `[I]` (power-off sequence reverse). |
| PE9 | OUT | Aux power rail (dropped in power-off). | `[I]` |
| PD11 | OUT | Declared as an aux rail. ⚠️ **DISPUTED** — PD11 is in the USART2-pin zone; the firmware previously drove it as a rail by mistake. | `[U]` (motor-FACTS: false rail, remove). |
| PD10 | — | Previously mistakenly driven as a rail/DRIVE-STATE candidate. **NOT a power rail.** | `[U]`/`[I]` (motor-FACTS: PD8-12 = USART2/AF zone, not rails). |
| **PB12** | OUT | **A4963 enable** (common to the drivers; this is the SPI1 NSS pin, repurposed as GPIO). | `[I]` (byte-for-byte reverse vs factory; motor-FACTS B). |
| **PB13** | AF | **SPI1 SCK** → A4963. | `[V]` (SPI1 non-remap default pinout + SPI1 census). |
| **PB14** | AF | **SPI1 MISO** ← A4963. | `[V]` (same). |
| **PB15** | AF | **SPI1 MOSI** → A4963. | `[V]` (same). |
| **PD3** | OUT | A4963 CS (one of three, individual chip-select). | `[I]` (reverse; 3 devices on 1 bus ⇒ 3 separate CS. The exact PD3/PD4/PD5 were not re-analyzed from asm in this pass). |
| **PD4** | OUT | A4963 CS. | `[I]` (same). |
| **PD5** | OUT | A4963 CS. | `[I]` (same; ⚠ PD5 = also USART1-remap TX — conflict unresolved `[U]`). |
| **PC6** | AF | **TIMER2_CH0** PWM (full-remap) — drive. | `[V]` remap bit, `[I]` channel→motor mapping. |
| **PC7** | AF | **TIMER2_CH1** PWM (full-remap). | `[V]`/`[I]` |
| **PC8** | AF | **TIMER2_CH2** PWM (full-remap). | `[V]`/`[I]` |
| **PC9** | AF | **TIMER2_CH3** PWM (full-remap) = **LEFT wheel speed** (CH3CV). | `[V]` (see §6, DAT_0805fac4=TIMER2, CH3). |
| **PB10** | AF-OD | **I2C1 SCL** → IMU. | `[V]` (I2C1 non-remap default + I2C1 census). |
| **PB11** | AF-OD | **I2C1 SDA** → IMU. | `[V]` (same). |
| **PD8** | AF | **USART2 TX** → battery/BMS. | `[V]` (asm 0x80213ee: gpio_init GPIOD pin 0x100=PD8; see §3). |
| **PD9** | AF | **USART2 RX** ← battery/BMS. | `[V]` (asm 0x8021404: pin 0x200=PD9). |
| PA0-3 | AF? | possible default TIMER2 channels before remap — **NOT active** (remap=full). | `[U]` |
| PC10/PC11 | AF | UART3 (LED board) TX/RX (default). | `[I]` |

**How to hunt for pins further:** the GPIO-base literals are in the dump (§0), but the decompilation (`decompiled_all.c`) hides them behind `DAT_` pointers — a grep for `GPIOx_BOP`/`_BC` there is empty. Extract pin numbers from `dist/factory-full.asm`: `ldr rN,[pc,#..]` → literal-pool halfword pair (e.g. `1800`+`4001` = 0x40011800 GPIOE), nearby `movs r1,#mask` (mask=1<<pin).

**AFIO_PCF0 (remap):** factory `0x00001C30` `[V]` (motor-FACTS A#5). Bits [11:10]=TIMER2_REMAP=`0b11` = **full remap → PC6/7/8/9**. Our firmware wrote a broken `0x001A0C00` (bug, to be fixed).

---

## 2. SPI1 — Allegro A4963 motor driver

- **SPI1 base = 0x40003800** `[V]` (census: 3 references; literal in the a4963-driver area ~0x0801F42C).
- The bus is **shared among 3× A4963** (left wheel / right wheel / blade), protected by `motor_spi_mutex`. `[V]` (subsystem-findings §2, drivers `a4963_snk_v2.c`).
- Pins: **SCK=PB13, MISO=PB14, MOSI=PB15** `[V]` (SPI1 is not remapped, default pinout). **NSS(PB12) not as NSS — as a GPIO A4963-enable** `[I]`. Individual **CS = PD3/PD4/PD5** `[I]`.
- **CTL0 ≈ 0x0B2F + SPIEN** `[I]` (motor-FACTS B; master, mode/divider set there).
- **A4963 frame format (16 bits):** `[15:13]=reg address(0..7) | [12]=W (set via |0x1000 on write) | [11:0]=data`. `[V]` (drive-chain §2).
- **Write with verify+retry:** `FUN_0808408c` (verify-write, @0x0808408c `[V]` asm line 227073) / `FUN_080840d8` (send). Readback compare `(word & 0xFFF) == readback`, up to 0x31 attempts. `[V]`. The transaction goes through the SPI HAL object (`ldr [obj,#0x24]→[+4]→FUN_0807dc18`), not through the bare base in this function `[V]` (asm 0x8084076-7a).
- Diagnostics: reading A4963 diag on all 3 = **0xFFFF, spi_status=0** (transaction completes, no timeout) `[V]` (motor-FACTS A#2) — interpretation "no VBB" = `[U]` hypothesis.

---

## 3. USART / UART

| Port | Base | Baud | Pins | Purpose | Trust |
|---|---|---|---|---|---|
| USART0 | 0x40013800 | `[U]` | TX=PA9/RX=PA10 or remap PB6/PB7 `[U]` | OTA updater / display board | base `[V]`, purpose `[I]` |
| USART1 | 0x40004400 | 115200 `[I]` | PA2/PA3 or remap PD5/PD6 `[U]` | **bdport → border (link to chip2)** | base `[V]`, purpose `[I]` |
| **USART2** | **0x40004800** | **19200 8N1** | **PD8=TX / PD9=RX** (remap) | **battery/BMS** (`driver_battery_snk`) | **`[V]`** |
| UART3 | 0x40004C00 | `[U]` | PC10/PC11 `[I]` | LED board (ledport) | base `[V]`, purpose `[I]` |
| UART4 | 0x40005000 | `[U]` | `[U]` | lightly used (2 references) | `[V]`/`[U]` |

**USART2 (battery) — fully confirmed in asm** (init `FUN` @0x080213c0, `dist/factory-full.asm`):
- line 54955-56 (0x8021464): literal-pool = `4800 4000` = **base 0x40004800** `[V]`.
- line 54926 (0x8021416): `mov.w r1, #19200 @ 0x4b00` → baud → `usart_baudrate_set` `[V]`.
- lines 54906-14: `gpio_init(GPIOD=0x40011400, …)` pin `0x100`=**PD8** and `0x200`=**PD9** `[V]`.
- Battery frame with CRC (smart pack), strings `battery connect failed crc=%d`, `can not discharger`, `@charge power on` `[V]`. Discharge is gated by a BMS handshake (open question — whether there is a discharge-enable in TX) `[U]`.

> ⚠️ **USART2 chip1 ≠ USART2 chip2.** chip2 has its own USART2 (same base 0x40004800, but pins **PB10(TX)/PB11(RX), 115200**) = the link to chip1. On chip1, PB10/PB11 are occupied by I2C1 (IMU). Do not confuse.

---

## 4. I2C — IMU

- **Bus = I2C1 (0x40005800)** `[V]` (census: I2C1=8 references, **I2C0=0 — not used** `[V]`). Resolves the old ambiguity "I2C0 vs I2C1" in favor of **I2C1**.
- **Pins: SCL=PB10, SDA=PB11** (AF open-drain, I2C1 is not remapped) `[V]`. Frequency ~400 kHz `[I]`.
- **Which physical IMU — `[U]` (conflict of two analyses):**
  - motor-FACTS (newer): **TDK ICM-42688**, address **0x68** (WR 0xD0), WHO_AM_I reg 0x75 == 0x47.
  - subsystem-findings (earlier): **Bosch BNO055**, dev 0x50 = 7-bit **0x28**, chip-id `A0 FB 32 0F`, Euler angles reg 0x1A.
  - **Both** drivers are compiled into the firmware (`driver_mems_snk_v13.c`, an ICM path exists too). Which one is soldered onto this board is not proven → `[U]`. The IMU feeds the tilt/lift FSM (safety) → it can inhibit the drive.

---

## 5. ADC — battery / other

- **Battery is NOT via ADC** — the smart pack goes over USART2/serial with a CRC frame `[V]` (§3, subsystem-findings §3). SoC is computed from the pack telemetry OCV.
- **ADC0 (0x40012400) is used** (census: 9 references `[V]`), **ADC1 is not used** (0 `[V]`). Purpose of ADC0 on chip1 — `[U]` (not battery; possibly Vrefint/internal temperature/service input). Channels/pins `[U]`.
- ⚠ Dual ADC0+ADC1 with DMA for the border coils — that is **chip2**, not chip1. On chip1 ADC1 is empty.

---

## 6. TIMER

| Timer | Base | Role | Trust / evidence |
|---|---|---|---|
| **TIMER2** | **0x40000400** | **Motor PWM** (wheel speed setting). CH3CV=0x40000440. **CH3 = left wheel.** Full-remap → outputs **PC6-9**. | `[V]` |
| **TIMER3** | 0x40000800 | **Odometry** — input-capture of the A4963 tacho/FG signal. ISR `FUN_0805fbc0`/`FUN_0805fc7c` increment the ticks by direction, speed by period (`FUN_0807f402`). | `[V]` (census 23; drive-chain §9). |
| TIMER4 | 0x40000C00 | Tacho/timebase (paired with TIMER3, 20 Hz). | `[V]`(census 7)/`[I]`(role). |
| TIMER0 (adv) | 0x40012C00 | Used (8 references), role not revealed. The advanced-PWM hypothesis dropped in favor of TIMER2. | `[V]`/`[U]` |
| TIMER7 (adv) | 0x40013400 | Used (3 references), role not revealed. | `[V]`/`[U]` |
| TIMER1 | 0x40000000 | Use **not proven** (the literal counter is noisy from the APB1 base). | `[U]` |

**Key motor-PWM proof `[V]`:** `set_left_motor_speed` (`FUN_0805f620`, forced_functions.c) writes the speed via `FUN_0807f696(_DAT_0805fac4, 3, pwm)`. The value `_DAT_0805fac4` was read from the dump: **offset 0x5FAC4 = bytes `00 04 00 40` = 0x40000400 = TIMER2**. `FUN_0807f696(base,ch,val)` writes CHxCV at offset 0x34/0x38/0x3C/**0x40**(CH3). ⇒ left wheel speed = **TIMER2 CH3CV (0x40000440)** `[V]`. The "gearbox" formula: `pwm = speed*0x1C20 / gear_max`, gear_max∈{6141,3069,718}, saturation 0x1C20=7200. set_right = `FUN_0806fe66` (mirror, another TIMER2 channel) `[V]`/`[I]`.

**Drive state model** (code + A4963 datasheet, NOT a live measurement `[U]`): after init RUN=1, BRK=1, PWM-duty=0 ⇒ electrodynamic hold; rotation is gated only by a nonzero CH3CV. The blade = a pure SPI A4963 path (no timer of its own) `[V]`.

---

## 7. DMA

- **DMA0 (0x40020000)** is used — 29 references `[V]`. **DMA1 (0x40020400)** — 11 references `[V]`.
- Channel bindings not re-analyzed in this pass → `[U]`. Typical consumers on this chip: USART TX/RX (battery, border, log), possibly I2C1. On chip2 DMA0 services the dual-ADC of the coils — but that is a different chip.

---

## 8. Other / non-IO

- **CRC (0x40023000)** — the hardware GD32 CRC (poly 0x04C11DB7). Boot-gate over the bootloader 0x08000000..0x08017FF8, compared against 0x08017FFC; the application is NOT CRC'd at startup. `compute FUN_08008e1c`, `reset FUN_08008e38`. `[V]` (subsystem-findings §1).
- **PMU (0x40007000)** — 6 references `[V]`, power-management (sleep/standby modes of the mower).
- **SPI0 (0x40013000)** — 5 references `[V]`; probably external SPI flash of the sub-images (BB/DB/LB) `[I]`, pins `[U]`.

---

## 9. Most important / unexpected + main gaps

- **[V] Motor PWM = TIMER2 CH3 (0x40000440), full-remap onto PC9.** Resolves the conflict: the "advanced-timer TIMER0/TIMER7" hypothesis from motor-FACTS is **dropped** (DAT_0805fac4@0x5FAC4 = 0x40000400).
- **[V] I2C0 and ADC1 on chip1 are not used at all; the IMU is unambiguously on I2C1 (PB10/PB11).** Ports GPIOF/GPIOG are also empty. This narrows the search field for "where the VBB-enable is hidden".
- **[V] USART2-battery fully confirmed in asm:** 0x40004800, 19200, PD8/PD9.
- **[U] Physical IMU undetermined** — ICM-42688@0x68 vs BNO055@0x28, both drivers compiled in.
- **[U] Purpose of ADC0 (9 references) on chip1** — not the battery; channels/pins unknown. A possible lead for "where VBB/current is measured".
- **[U]/[I] A4963 CS pins (PD3/4/5)** and enable **PB12** — from reverse engineering, NOT re-analyzed from raw asm in this pass; PD5 conflicts with a possible USART1-remap. Requires exact binding via asm.
- **[U] diag=0xFFFF** ("motors do not turn") — the fact of the transaction is there, the cause (no VBB / MISO routing / BMS handshake) is not proven.
