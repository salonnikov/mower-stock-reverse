# mower-own — STANDALONE firmware for the mower mainboard (GD32F305)

This is **our own application firmware**, which **replaces the factory app**. It is
**not** a hook/injection into the factory FSM (that was `rc-payload`). The GD32
bootloader runs first and jumps to our reset vector; from there this is the only
code running on chip1.

Status: **All 3 motors (2 wheels + blade) driven; buttons, battery ADC, auto-
sleep, and rich mailbox telemetry** (hardware-inventory report, 2026-07). Chip1
is the core, controlled/monitored by a Raspberry-Pi dashboard over the SWD
mailbox (no on-board display link — the display is a separate ESP32, handled
later). Builds clean with `mower-fwbuild`. `main()`:
1. raises **PE12 HIGH (MAIN power latch)** + PB0 HIGH (secondary) immediately —
   mandatory, or the mower powers off when the button is released;
2. clock init to **120 MHz** (8 MHz HXTAL → PLL, 2 flash WS);
3. peripheral clock enables (GPIOB/C/D/E, AFIO, ADC0, DMA0, TIMER2, SPI1, PMU);
4. **SPI1** init (PB13 SCK / PB14 MISO / PB15 MOSI, CTL0=0x0B2F). SPI1 uses
   software-NSS — per-motor CS are GPIO;
5. **PB12 HIGH** — the shared A4963 gate-driver enable (enables ALL 3 drivers);
6. configures + RUNs the two wheel A4963 (BLADE configured but left UN-RUN), each
   on its own GPIOD CS (PD5/PD4/PD3);
7. TIMER2 full remap (PC6..PC9) + CH1 (BLADE) / CH2 (RIGHT) / CH3 (LEFT) PWM;
   **ADC0** battery init (PC5/PC4);
8. loop (every iteration, non-blocking): poll **power button** (long-press →
   power off), read **charge** (PE8), sample **battery** (~1 s), publish all
   **telemetry** (batt mV/%, charge, actual CH1/2/3 duties, button, fw_state),
   run **auto-sleep** (~10 min inactivity → off), then the motor path — read
   mailbox → set DIR + duty for both wheels, gate blade by `blade_on`, dead-man.

**SAFETY: the blade defaults OFF and its A4963 is left UN-RUN. It spins ONLY when
the host sets `blade_on`.** The button/charge/battery/telemetry/auto-sleep service
runs every loop before any drive gate, so power-off and liveness never depend on
arming state.
Coil sensors remain stubs (on chip2); chip1 ADC0 = battery; odometry ISRs TODO.

### Corrected wiring (VERIFIED)

| signal | pin | port | notes |
|--------|-----|------|-------|
| **MAIN power latch** | **PE12** | GPIOE | GP PP out, HIGH — hold system power (mandatory, first); power-off = hold LOW forever |
| secondary latch | PB0 | GPIOB | GP PP out, HIGH (bootloader sets it on key confirm); LOW alone does NOT cut power |
| A4963 shared **enable** | **PB12** | GPIOB | GP PP out, HIGH — enables all 3 drivers (NOT an SPI pin) |
| SPI1 SCK | PB13 | GPIOB | AF PP 50 MHz |
| SPI1 MISO | PB14 | GPIOB | input floating |
| SPI1 MOSI | PB15 | GPIOB | AF PP 50 MHz |
| LEFT **CS** | **PD5** | GPIOD | GP PP out, idle HIGH, active-low (mask 0x20) |
| RIGHT **CS** | **PD4** | GPIOD | GP PP out, idle HIGH, active-low (mask 0x10) |
| BLADE **CS** | **PD3** | GPIOD | GP PP out, idle HIGH, active-low (mask 0x08) |
| LEFT PWM | PC9 | GPIOC | TIMER2 CH3 (CH3CV 0x40000440) |
| RIGHT PWM | PC8 | GPIOC | TIMER2 CH2 (CH2CV 0x4000043c) |
| BLADE PWM | PC7 | GPIOC | TIMER2 CH1 (CH1CV 0x40000438) |
| power button | PE10 **+** PE11 | GPIOE | inputs, active-low pull-up; pressed = level(PE10)&level(PE11)==0 |
| charge detect | PE8 | GPIOE | input, active-HIGH pull-down (charger present) |
| pack voltage | PC5 | GPIOC | analog, ADC0 ch15 (pack_mV = raw × 5.4277) |
| pack current | PC4 | GPIOC | analog, ADC0 ch14 (read; not yet exposed) |

Architecture: chip1 is the **core**, controlled/monitored by a Raspberry-Pi
dashboard over the SWD mailbox. The physical display is a separate ESP32 board,
handled later — there is **no on-board display UART link** in this firmware.

Correction history: an earlier pass assumed CS=PB12 and PD5=LEFT-enable (wrong —
CS is per-motor on GPIOD, PB12 is the shared enable). A display link over USART2
(PD8/PD9) was added then **removed**: USART2 (0x40004800) is actually the battery
smart-pack link, not the display, so that code was on the wrong UART. Battery is
now read via **ADC0** (PC5/PC4). The TIMER2 motor remap (0x001A0C00) is unchanged.

### Buttons + charge (VERIFIED — `button.c`)

- **Power button** = PE10 (0x400) AND PE11 (0x800), **active-low**. `button_poll()`
  (every loop) reads ISTAT; pressed when `level(PE10) & level(PE11) == 0`. Pins are
  input-with-**pull-UP** (ODR bits set high in `hal_gpio_init`) — active-low needs
  pull-up, and the GD32F1 model picks pull direction from ODR.
- **Boot-release latch (C1):** the user powers ON by *holding* the key, so at boot
  the key is pressed. `button.c` will NOT accumulate the hold counter or power off
  until the key has been sampled RELEASED for ≥3 consecutive polls at least once
  (`s_seen_released`). A still-held power-on press therefore cannot self-trigger
  power-off.
- **Loop pacing (C1):** the main loop paces to ~1 ms at the **top** of `for(;;)`
  (`LOOP_PACE_ITERS`), so every path — including the disarmed `continue` — is
  paced. The button hold threshold `BTN_LONGPRESS_LOOPS` = 1500 (~1.5 s). (Before
  the fix, the disarmed path was unpaced → the 1.5 s tripped in <100 ms at boot.)
- **Long-press power-off** (mirrors the factory APP shutdown 0x080714d8, reached
  from process_power_off @0x08069c94): after ~10 debounce samples, held
  continuously ~1.5 s → **TIMER2 CH1/CH2/CH3 = 0** (motors off) → **PB12 LOW**
  (A4963 enable) → **PE9, PD11, PE7 LOW** (aux rails) → PB0 LOW (our secondary
  latch) → short watchdog-fed delay → **`for(;;){ PE12 LOW; feed FWDGT; }`** —
  the MAIN latch held LOW until the rail collapses. **NO SYSRESETREQ**: a reset
  re-runs the bootloader, which re-asserts PE12 HIGH → the mower would reboot
  instead of turning off (that was the original power-button bug). Short press
  ignored. Thresholds are loop-iteration counts — tune once a real ms tick exists.
- **Charge detect** = PE8 (0x100), **active-high**, input pull-DOWN. Exposed as
  mailbox telemetry byte `charge` at +0x17 (firmware writes; host reads).

### Battery (ADC0) — `battery.c`

- **ADC0** ch15 (**PC5**) = pack voltage, ch14 (**PC4**) = current. Software-
  triggered single conversions, polled EOC (no DMA), sample time 239.5 cycles.
  ADC clock = PCLK2 / 8 = 15 MHz (≤ 40 MHz). Clocks: ADC0 (APB2 b9), GPIOC (b4),
  GPIOB (b3); DMA0 (AHB b0) reserved for future ADC DMA.
- **Voltage** (VERIFIED scale): `pack_mV = raw × 5.4277` (factory uses 2 summed
  samples × 2.7138548; single sample → ×5.4277). Fixed-point `raw*54277/10000`.
- **Percent** — **TODO(batt)**: approximate 5-cell Li-ion linear map, 15000 mV=0 %
  … 21000 mV=100 %, clamped. `battery.c` `BATT_MV_EMPTY/FULL`. Calibrate on bench.
- **Smart-pack** over USART2 (0x40004800) with CRC-8 would be more accurate — left
  as a TODO note; ADC voltage→% is enough now.
- Robustness: EOC waits are bounded (return 0 on timeout, never wedge).

### Auto-sleep (power-save) — `main.c`

Inactivity timer: **no activity for ~10 min → power off** (same path as a long-
press: the factory shutdown — motors/rails off, PE12 held LOW until the rail
dies). "Activity" = a fresh mailbox `seq` bump (a
command) **OR** the power button pressed **OR** charging (PE8 high). The firmware
**never sleeps while charging or while armed+driving**. Timeout = `AUTOSLEEP_LIMIT`
(600000 paced ~1 ms loops ≈ 10 min). Mirrors the factory auto-off.

## Architecture

```
bootloader (0x08000000..0x08017FFF, WE DON'T TOUCH)
   └─ loads MSP from *(0x08018000), jumps to *(0x08018004)
        └─ startup.c: Reset_Handler
             ├─ copy .data (flash→RAM), zero .bss
             ├─ SystemInit() : set VTOR = 0x08018000
             └─ main()
                  ├─ hal_clock_init()   (RCU clock enables; PLL = TODO(hw))
                  ├─ hal_gpio_init()     (pins = TODO(hw))
                  ├─ spi_a4963_init()    (SPI mode/baud = TODO(hw))
                  ├─ pwm_init()          (TIMER2, CAR=0x1C20)
                  ├─ motor_init_all()    (A4963 CONFIG0..5 + reg7 RUN)
                  └─ loop:
                       ├─ heartbeat++            (SWD-observable liveness)
                       ├─ read RC mailbox @0x20014000
                       ├─ safety: magic + dead-man staleness gate
                       └─ motor_set_wheel(left, right)
```

Files:

| file | role |
|------|------|
| `ld/app.ld`       | linker: FLASH 0x08018000..0x080FFFF8 (CRC word reserved), RAM 96 K |
| `src/startup.c`   | vector table @0x08018000, Reset_Handler, Default_Handler, SystemInit (VTOR) |
| `src/gd32_regs.h` | peripheral register map (RCU/SPI/TIMER2/GPIO/ADC/CRC) |
| `src/hal.c/.h`    | clock/GPIO/SPI/PWM primitives + `a4963_write()` |
| `src/motor.c/.h`  | A4963 config + RUN, signed-speed → dir+PWM (the crux, see file header) |
| `src/button.c/.h` | power button (PE10+PE11) long-press-off, charge detect (PE8) |
| `src/battery.c/.h`| ADC0 pack voltage (PC5) + current (PC4), mV + percent |
| `src/main.c`      | bring-up + poll loop (button/charge/battery/telemetry + auto-sleep + motor safety) |
| `Makefile`        | builds `build/app.elf` + `build/app.bin` |

## Memory map

| region | range | notes |
|--------|-------|-------|
| bootloader | 0x08000000 – 0x08017FFF | not ours; runs first |
| **app (this fw)** | **0x08018000 – 0x080FFFF8** | vector table at start |
| app CRC-32 word | 0x080FFFFC | written by packer (`tools/bench/app_crc.py`) |
| pad | to 0x08100000 | 0xFF fill |
| SRAM | 0x20000000 – 0x20018000 | 96 KB; stack top `_estack` = 0x20018000 |
| RC mailbox | 0x20014000 | host writes over SWD |

Verified: vector table lands at 0x08018000, word0 = 0x20018000 (SP),
word1 = 0x08018195 (Reset_Handler, thumb). Packing an `app.bin` into a 1 MB
image and running `app_crc.py --set` produces a valid CRC at 0x080FFFFC.

## RC mailbox (SRAM 0x20014000)

Struct in `main.c` (`rc_mailbox_t`), 32 bytes total. The Raspberry-Pi dashboard
WRITES the **control** fields; the firmware WRITES the **telemetry** fields every
loop. Speeds are **signed 16-bit** (sign = direction, magnitude = PWM duty
0..0x1C20). A compile-time `_Static_assert(sizeof==0x20)` guards the layout.

| off | type | dir | field | meaning |
|-----|------|-----|-------|---------|
| 0x00 | u32 | fw→host | `heartbeat` | firmware increments every loop — liveness |
| 0x04 | u32 | host→fw | `magic` | host sets `= 0x5243414D` to **arm**; else all motors off |
| 0x08 | u32 | host→fw | `seq` | host increments on each new command (**dead-man / activity**) |
| 0x0C | i16 | host→fw | `left_speed` | signed; + fwd, − rev, 0 = stop. \|v\| = duty (0..0x1C20) |
| 0x0E | i16 | host→fw | `right_speed` | signed, same units |
| 0x10 | u8  | host→fw | `blade_on` | **0 = OFF (safe default)**; ≠0 = spin blade |
| 0x11 | u8  | — | `pad0` | padding |
| 0x12 | u16 | — | `pad1` | padding (aligns batt_mV to 0x14) |
| 0x14 | u16 | fw→host | `batt_mV` | pack voltage (mV) |
| 0x16 | u8  | fw→host | `batt_pct` | state of charge (0..100) — TODO(batt) curve |
| 0x17 | u8  | fw→host | `charge` | 1 = charger present (PE8) |
| 0x18 | u16 | fw→host | `ch3_duty` | LEFT actual PWM (TIMER2 CH3CV) |
| 0x1A | u16 | fw→host | `ch2_duty` | RIGHT actual PWM (CH2CV) |
| 0x1C | u16 | fw→host | `ch1_duty` | BLADE actual PWM (CH1CV) |
| 0x1E | u8  | fw→host | `btn` | power button pressed 0/1 |
| 0x1F | u8  | fw→host | `fw_state` | 0=disarmed, 1=armed-idle, 2=driving |

**Safety (defense in depth):**
- **Boot-clear (H2):** `main()` zeroes the mailbox control fields (magic, seq,
  speeds, blade_on) immediately after the power latch, *before* bring-up. A warm
  `reset run` or SRAM-retaining power cycle can leave a valid magic + nonzero
  speeds from a prior test; wiping them means nothing can move until the host
  re-arms from scratch. heartbeat is preserved.
- **Fresh-seq gate (H2):** even once armed, no motor drives until the firmware
  observes at least one *fresh* `seq` bump by the host (a transition away from the
  cleared baseline). Stale SRAM with a matching seq can never drive the first
  loops.
- **Dead-man:** any motor moves only while `magic` is correct AND `seq` keeps
  changing. If `seq` stalls for `STALE_LIMIT` (200 loops ≈ 200 ms), all motors
  force-stop.
- **Auto-sleep:** no activity (seq bump / button / charging) for ~10 min → power
  off. Never sleeps while charging or armed+driving.
- **Blade positive-off (H3):** the blade A4963 is *configured but left UN-RUN*
  (output stage OFF) at init — the two wheels RUN at init (duty 0), the blade does
  NOT. The blade is only put into RUN when the host sets `blade_on`, and is taken
  back out of RUN on off / any stop. So the blade physically cannot spin from
  init, garbage, or a glitch — only from an explicit command.
- **Startup IRQ quiesce (M3):** `SystemInit` masks IRQs, kills SysTick, and
  disables+clears all NVIC lines before setting VTOR, so a bootloader-left IRQ
  can't vector into the halt handler and look bricked.
- **Bounded waits (M2/M4):** all clock-stabilization and SPI TBE/RBNE waits have
  timeouts — a dead crystal/PLL falls back to HSI, a dead SPI returns an error;
  init never wedges before reaching the heartbeat loop.

Drive wheels (wheels raised, blade physically disconnected, on power):
```
# confirm we're alive:
mdw 0x20014000            # heartbeat should be incrementing
# left fwd 0x600, right fwd 0x600 (two i16 packed in one word @0x0C):
#   0x0C low half = left_speed, high half = right_speed
mww 0x2001400c 0x06000600   # left=+0x600, right=+0x600
mwb 0x20014010 0x00         # blade_on = 0 (OFF)
mww 0x20014008 0x00000001   # seq = 1 (fresh)
mww 0x20014004 0x5243414d   # magic = ARM (last!)
# keep moving: bump seq periodically (mww 0x20014008 2, 3, ...)
# reverse left:            mww 0x2001400c 0x0600fa00  (left=-0x600, right=+0x600)
# stop:                    mww 0x20014004 0x00000000  (disarm)
```

## Build

```
docker run --rm -v /Users/denis/MOWER/firmware/mower-own:/w mower-fwbuild \
  bash -c 'cd /w && make'
```
Output: `build/app.elf`, `build/app.bin`. Flags mirror `rc-payload`
(`-mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -ffreestanding -nostartfiles`,
`-Wall -Wextra -Werror`). Current size: **text 2948 / data 0 / bss 44**
(`app.bin` = 2948 bytes).

**Flash footprint:** app.bin = 2948 bytes → **2 pages of 2 KB** (0x08018000..
0x08018FFF; last app byte at 0x08018B83). `gen_flash_own.py` auto-covers the
occupied pages (flash + revert both target these 2 pages), so multi-page is fine.

## Pack (host, after build)

`pack_own.py` takes the stock dump (keeps the bootloader), erases the app region
to 0xFF, drops `app.bin` at 0x08018000, computes the app CRC-32 and writes it at
0x080FFFFC:
```
docker run --rm -v /Users/denis/MOWER:/work python:3.12-slim \
  python /work/tools/bench/pack_own.py \
    /work/dist/gd32-mainboard-dump-v1.bin \
    /work/firmware/mower-own/build/app.bin \
    /work/dist/own_fw.bin
```
Latest run: SP = `0x20018000`, Reset = `0x080181c9` (thumb), app CRC =
`0x904385bb` (match=True). Bootloader 0x08000000..0x08017FFF = stock (untouched).

Flash cfgs regenerated: `dist/flash_own.cfg` (2 pages) + `dist/flash_revert_own.cfg`.

---

## VERIFIED (filled) — all 3 motors + power latch

Filled with verified values (report 2026-07-04):

- **Power latch**: PB0 = GP push-pull output, driven HIGH first in `main()`.
- **Clock**: 8 MHz HXTAL → PLL → 120 MHz SYSCLK/HCLK/APB2, APB1=60 MHz; PMU
  high-drive handshakes; FMC 2 wait states. Exact ordered CFG0/CFG1/CTL writes.
- **Peripheral clocks**: AFIO, GPIOB/C/D/E, ADC0 (APB2); TIMER2, SPI1, PMU
  (APB1); DMA0 (AHB). ADC prescaler /8 (ADCCLK = 15 MHz).
- **SPI1** (0x40003800): PB13=SCK, PB14=MISO, PB15=MOSI; CTL0=0x0B2F (mode3,
  master, /64, MSB-first, SW-NSS, 16-bit) + SPIEN. TBE/RBNE handshake.
- **Shared A4963 enable**: PB12 = GP push-pull output, HIGH (enables all 3).
- **Per-motor CS** (software-NSS): LEFT=PD5, RIGHT=PD4, BLADE=PD3, active-low.
- **A4963 config** (Variant A, all verified):
  - LEFT & RIGHT: reg0=0x03e8, reg1=0x22df, reg2=0x4753, reg3=0x6721, reg4=0x8735,
    reg5=0xa736, reg7=0xee0d.
  - BLADE: reg0=0x03e4, reg1=0x2a5f, reg2=0x478d, reg3=0x6723, reg4=0x8974,
    reg5=0xa933, reg7=0xee0d.
  - reg6 not sent. reg7 = RUN=1,DIR=0,BRK=1,CM=11. Direction = flip reg7 bit1;
    magnitude = PWM duty.
- **TIMER2 PWM**: AFIO full remap (PC6..PC9); PSC=9, CAR=0x1C20; CH1 (BLADE, PC7)
  + CH2 (RIGHT, PC8) + CH3 (LEFT, PC9) OCxM=PWM0 + preload, CHxEN; ARSE + CEN.

## ⚠️ TODO(hw) — remaining (NOT needed to drive the 3 motors)

- **Sensors**: coils are on **chip2** (inter-board serial), not chip1. chip1 ADC0 =
  **battery** lines — kept a stub. Bumper/lift/rain via chip2 link. Button PE10 is
  configured as input (polled active-low) but not yet used.
- **Odometry ISRs** (`startup.c`): TIMER3 input-capture tacho — vectors are all
  Default_Handler for now.
- **Blade run duty** (`motor.c` `MOTOR_BLADE_DUTY` = 0x1000): tune to the factory
  `blade_sp` if a specific cutting RPM is required. (Blade already spins when
  `blade_on` is set; this is just the magnitude.)
- **Loop pacing** (`main.c`): crude nop-spin; move to a SysTick/timer 10 ms tick.

### First-drive procedure
Flash `own_fw.bin`, confirm `heartbeat` at 0x20014000 increments, then arm the
mailbox (set speeds, then **bump `seq`**, write `magic` last — a fresh seq bump is
required before any drive). The two WHEEL A4963s are in RUN from init (duty 0);
PB12 enable is up; per-motor CS + PWM duty drive each wheel. The BLADE A4963 is
left UN-RUN until `blade_on` is set.
**Wheels raised, blade physically disconnected, on power. Keep `blade_on` = 0.**
