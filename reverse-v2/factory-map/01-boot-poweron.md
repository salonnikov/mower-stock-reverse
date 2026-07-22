# 01 — BOOT and POWER-ON (factory chip1)

**Reverse branch:** boot / power-on. Full timeline from reset to idle (state 3), with a focus on
power latches and rail-enable. Source: `chip1/decompiled_all.c` + DAT_ resolution against
`dist/gd32-mainboard-dump-v1.bin` (flash base 0x08000000, LE). Cross-checked against the live map:
`reference/FACTORY-GPIO-PERIPHERAL-MAP.md`.

---

## 0. IMPORTANT: the real chip and peripheral addresses

Resolving the base pointers from the dump showed that **this is NOT the F4 layout from the spec.**
The chip is the **GD32F30x** family (Cortex-M4, "GD32F303-class"), F1-compatible peripherals:

| Peripheral | Base (fact from the dump) | Note |
|---|---|---|
| GPIOA | `0x40010800` | (spec erroneously said 0x40020000) |
| GPIOB | `0x40010c00` | |
| GPIOC | `0x40011000` | |
| GPIOD | `0x40011400` | |
| GPIOE | `0x40011800` | |
| AFIO  | `0x40010000` | |
| RCU   | `0x40021000` | (spec erroneously said 0x40023800) |
| DMA0/DMA1 | `0x40020000` / `0x40020400` | AHB |
| TIMER0 | `0x40012c00` | advanced |
| ADC0 | `0x40012400` | battery |
| SPI0 | `0x40013000`, USART0 `0x40013800` | |
| APB1: TIMER1 `0x40000000`, TIMER2 `0x40000400` (motor PWM), TIMER3 `0x40000800` (tacho), USART1 `0x40004400`, USART2 `0x40004800` (BMS), UART3 `0x40004c00`, SPI1 `0x40003800`, I2C1 `0x40005800` | as in the spec |

**GPIO registers (F1 style, 4 bits/pin):** `CTL0=+0x00` (pins 0-7), `CTL1=+0x04` (pins 8-15),
`ISTAT=+0x08` (input), `OCTL=+0x0c` (output), `BOP=+0x10` (set-bit → HIGH), `BC=+0x14` (clear-bit → LOW).

**RCU registers:** `AHBEN=+0x14`, `APB2EN=+0x18`, `APB1EN=+0x1C`, `RSTSCK=+0x24` (reset-cause flags).

### How to decode the HAL calls (the key to the whole timeline)

- `FUN_08010cd0(id)` / `FUN_0806c77c(id)` — **RCU clock enable**. GD32 `REGIDX_BIT` encoding:
  `reg = RCUbase + (id>>6)`, bit `= id & 0x1f`, `*reg |= 1<<bit`.
  → `0x600`=AFIO, `0x602`=GPIOA, `0x603`=GPIOB, `0x604`=GPIOC, `0x605`=GPIOD, `0x606`=GPIOE
    (all on APB2EN off 0x18); `0x701`=TIMER2, `0x70e`=SPI1 (APB1EN off 0x1C); `0x506`=CRC (AHBEN).
- `FUN_0800ca10(base,mask)` → `*(base+0x14)=mask` = **BC** → pins LOW.
- `FUN_0800ca14(base,mask)` → `*(base+0x10)=mask` = **BOP** → pins HIGH.
- `FUN_0800ca22(base,p2,p3,mask)` → CTL0/CTL1 mode config (a 4-bit nibble per pin).
  Nibble `= (CNF<<2)|MODE`. Observed `p2`: `0x10`→nibble `3` (out-PP 50MHz);
  `0x14`→nibble `7` (out-OD 50MHz); `0x18`→nibble `0xb` (AF-PP 50MHz); `4`→nibble `4` (in-float).
- `FUN_08010a54(id)` → read bit `*(RCUbase+(id>>6)) & (1<<bit)`; for `0x91x` reads **RCU_RSTSCK**
  (reset-cause flags).
- `FUN_0800f5e0(irq)` → `NVIC_ICER` — **disable** IRQ (used during bootloader teardown).

---

## 1. FUNCTION MAP (FUN → name → action)

### Bootloader (0x08000xxx)

| FUN | Name | What it does to the hardware |
|---|---|---|
| `FUN_08008960` | `boot_sys_init` | Top-level spin-up: LED/log → `FUN_08000f38` (GPIO+latches) → `FUN_08001b4c` (secondary latch) → delay → CRC-clock. Order in §2. |
| `FUN_08000f38` | `boot_gpio_init` | **The main power-on GPIO of the bootloader.** Enables clocks for GPIOA-E+AFIO, configures all pins, **raises PE12 (main latch) and PE7 (aux rail)**. Breakdown in §3. |
| `FUN_08001b4c` | `boot_power_latch` | Reads the reset cause (`FUN_0800c8e0`); if not "1" → **PB0=HIGH (secondary latch)** via BOP GPIOB, sets RAM flag `0x40` @ `0x20000018`, logs `reset_power_on`. |
| `FUN_0800c8e0` | `reset_cause` | Reads RCU_RSTSCK (`0x91a..0x91f`), returns the reset-source code (por/ext/sw/wdg). |
| `FUN_080012d0` | `jump_to_app` | Validates the application vector (`*app & mask == 0x20000000`), logs `jump_to_app`, calls teardown `FUN_08000e48`, sets MSP, jumps to the app. |
| `FUN_08000e48` | `boot_deinit` | Before the jump: disables IRQs (0x43,0x1c,0x19,0x25,0x26,0x34), resets TIMER0/DMA0/DMA1, deinits SPI0/USART1/UART3, GPIOB; `SYST_CSR &= ~3` (stops SysTick). Does NOT touch power (latches remain). |
| `FUN_08000f06` | `boot_rcu_x2` | Enables clocks `0x500`,`0x501` (AHBEN DMA0/DMA1). |
| `FUN_08001bb8` | `boot_early` | Clocks `0x71b`,`0x71c` (APB1 UART3/UART4), UART init `FUN_0800fd60`. |

### Application (0x08061xxx / main)

| FUN | Name | What it does to the hardware |
|---|---|---|
| `FUN_08019430` | `Reset_Handler` (app C-runtime) | Copies .data, clears .bss, calls `main` through `thunk_FUN_08061714`. |
| `FUN_08061714` | `main` | Body: `FUN_0807f010(); FUN_0806bbd8();` — that's all. |
| `FUN_0807f010` | `pre_scheduler_init` | **Enables clocks for GPIOE (`0x606`) and GPIOB (`0x603`)** (the ports of latches PE12/PE7/PB0), then flash/CRC check (`FUN_0802abbc`), on mismatch — `FUN_0807ef68`. Does NOT configure the GPIO pins. |
| `FUN_0806bbd8` | `rtos_start` | FreeRTOS: creates task **"initial"** (`FUN_080849c0`, stack 0x1000, prio 0xf, entry `0x0805bc29`) and **"IDLE"**, configures SysTick/PendSV (`SHPR3 |= 0xc0000000`), starts the scheduler (`FUN_08018194`). Does not return. |
| `FUN_080477c4` | `rw_init` | **Driver bring-up** (in task "initial"). Determines the board type, creates the driver objects in order **tilt → battery → BLADE → RIGHT → LEFT**, calls their init/enable methods. Breakdown in §4. |
| `FUN_0807976c` | `fsm_set_state` | `if(state<0xb) *(0x200000ac+0x10)=state`. Writes the FSM state (`0x200000bc`). **3 = idle**. |
| `FUN_08063368` | `enable_event` | App-level "enable event" (object model, flag+counter increment). Associated with PE0 in the live map; not a direct GPIO drive. |

---

## 2. TIMELINE (reset → idle) step by step

### Stage A — BOOTLOADER (right after reset, before the app)

1. **Reset** → bootloader startup → `FUN_08008960` (`boot_sys_init`):
   1. `FUN_0800f68c(0x500)` — early initialization (log/LED channel).
   2. `FUN_080025a8()` — system init.
   3. `FUN_08001bb8()` — clocks UART3/UART4 (APB1 `0x71b/0x71c`), UART init for the log.
   4. **`FUN_08000f38()` (`boot_gpio_init`) — THE KEY POWER STEP:**
      - RCU: `FUN_08010cd0(0x602/0x606/0x603/0x604/0x605/0x600)` → enables clocks for **GPIOA, GPIOE, GPIOB, GPIOC, GPIOD, AFIO**.
      - Pin config (full breakdown §3). Notable:
        - **PE12 → OUT-PP 50MHz, then BOP HIGH ⇒ THE MAIN POWER LATCH is raised.**
        - **PE7 → OUT-PP 50MHz, then BOP HIGH ⇒ THE AUX RAIL is raised.**
        - PB0 → OUT-PP, BC LOW (still LOW; step 5 raises it).
        - PE0/PE1/PE5 → OUT-OD, LOW.
        - Comms pins: PA9/PA10 (USART0), PC10/PC11 (UART3), PA5/6/7+PA4 (SPI0), PA0/PB9/PB6 (OUT low, UART-DE).
        - Inputs: PE10/PE11, PC0/PC1, PA3, PE8.
   5. **`FUN_08001b4c()` (`boot_power_latch`):** reads the reset cause; if ≠1 →
      **BOP GPIOB bit0 ⇒ PB0=HIGH (SECONDARY LATCH)**, RAM flag `0x40`, log `reset_power_on`.
   6. `FUN_08002614(100)` — delay ~100 (power/latch stabilization).
   7. `FUN_08001e5c()`, `FUN_08000f06()` (clocks DMA0/DMA1), `FUN_08003fc0()`.
   8. `FUN_08010cd0(0x506)` — CRC clock.
2. The bootloader checks/selects the image and calls **`FUN_080012d0` (`jump_to_app`)**:
   - validates the app vector, logs `jump_to_app`, → **`FUN_08000e48` (`boot_deinit`)**: disables IRQs,
     resets TIMER0/DMA/SPI0/USART1/UART3/GPIOB, stops SysTick. **Latches PE12/PE7/PB0 remain raised.**
   - sets MSP from the app vector, jumps to the app Reset.

### Stage B — APPLICATION

3. **App Reset** `FUN_08019430`: copies .data, clears .bss → `main` `FUN_08061714`.
4. **`FUN_0807f010` (`pre_scheduler_init`):**
   - `FUN_0806c77c(0x606)` + `FUN_0806c77c(0x603)` → re-enables clocks for **GPIOE and GPIOB**
     (the latch ports; the app keeps them powered). It does not touch other RCU/GPIO here.
   - flash/CRC self-check (`FUN_0802abbc`), on mismatch `FUN_0807ef68`.
5. **`FUN_0806bbd8` (`rtos_start`):** creates tasks "initial" and "IDLE", configures SysTick+PendSV,
   starts the scheduler. Control passes to task "initial" (entry `0x0805bc28`).
6. **Task "initial"** → service config (`FUN_0805bb4c`: run_param/OTA flags) → calls
   **`FUN_080477c4` (`rw_init`)** — driver bring-up (§4): tilt → battery → BLADE → RIGHT → LEFT.
7. After a successful bring-up the FSM is moved into the working loop; **idle = state 3** (`fsm_set_state(3)`),
   the system waits for a command (unlock/departure).

---

## 3. KEY CODE — `FUN_08000f38` (boot_gpio_init), annotated

```c
void boot_gpio_init(void) {
  // --- RCU: clocks for all ports + AFIO ---
  rcu_en(0x602); // GPIOA
  rcu_en(0x606); // GPIOE
  rcu_en(0x603); // GPIOB
  rcu_en(0x604); // GPIOC
  rcu_en(0x605); // GPIOD
  rcu_en(0x600); // AFIO

  // === GPIOE (0x40011800) ===
  cfg(E, 0x10,4, 0x1000);  // PE12 = OUT-PP 50MHz
  BOP(E, 0x1000);          // *** PE12 = HIGH  → THE MAIN POWER LATCH ***
  cfg(E, 4,4, 0x400);      // PE10 = IN-float
  cfg(E, 4,4, 0x800);      // PE11 = IN-float

  // === GPIOC (0x40011000) ===
  cfg(C, 4,4, 0x1);        // PC0 = IN-float
  cfg(C, 4,4, 0x2);        // PC1 = IN-float

  // === GPIOB (0x40010c00) ===
  cfg(B, 0x10,4, 0x1);  BC(B, 0x1);      // PB0 = OUT-PP, LOW (secondary latch raised by FUN_08001b4c)
  // (GPIOB again after the GPIOA block)

  // === GPIOA (0x40010800) ===
  cfg(A, 0x10,4, 0x1);  BC(A, 0x1);      // PA0 = OUT-PP LOW (UART DE)

  cfg(B, 0x10,4, 0x200); BC(B, 0x200);   // PB9 = OUT-PP LOW (UART DE)
  cfg(B, 0x10,4, 0x40);  BC(B, 0x40);    // PB6 = OUT-PP LOW

  cfg(A, 0x18,4, 0x200);   // PA9  = AF-PP  → USART0 TX
  cfg(A, 4,4, 0x400);      // PA10 = IN     → USART0 RX
  cfg(A, 4,4, 0x8);        // PA3  = IN-float

  cfg(C, 0x18,4, 0x400);   // PC10 = AF-PP  → UART3 TX
  cfg(C, 4,4, 0x800);      // PC11 = IN     → UART3 RX

  cfg(E, 0x14,4, 0x1);  BC(E, 0x1);      // PE0 = OUT-OD LOW
  cfg(E, 0x14,4, 0x2);  BC(E, 0x2);      // PE1 = OUT-OD LOW
  cfg(E, 0x14,4, 0x20); BC(E, 0x20);     // PE5 = OUT-OD LOW

  cfg(A, 0x10,4, 0x10); BC(A, 0x10);     // PA4 = OUT-PP LOW → SPI0 CS
  cfg(A, 0x18,4, 0x20);    // PA5 = AF-PP → SPI0 SCK
  cfg(A, 0x18,4, 0x80);    // PA7 = AF-PP → SPI0 MOSI
  cfg(A, 4,4,   0x40);     // PA6 = IN    → SPI0 MISO

  cfg(E, 0x10,4, 0x80);  BOP(E, 0x80);   // *** PE7 = OUT-PP HIGH → AUX RAIL ***
  cfg(E, 4,4,   0x100);    // PE8 = IN-float
  // ...AFIO/misc tail
}
```

`FUN_08001b4c` (secondary latch):
```c
void boot_power_latch(void) {
  if (reset_cause() != 1) {          // FUN_0800c8e0: RCU_RSTSCK
    BOP(GPIOB, 0x1);                 // *** PB0 = HIGH  → THE SECONDARY LATCH ***
    *(u32*)0x20000018 |= 0x40;       // RAM flag "power-on"
    log("reset_power_on");
  }
}
```

---

## 4. `rw_init` (FUN_080477c4) — driver bring-up (object model)

The drivers are created as objects with a vtable; the factories are selected by **board type** (`iVar4`, from
`FUN_08050578()`), then the methods are called. The order and log strings confirm the sequence:

| # | Driver | Factory (board=0/`0x50dc`) | init-error log | Methods after init |
|---|---|---|---|---|
| 1 | **tilt sensor** | `FUN_0806e5a4` | `ERROR robot_tilt_sensor_initial` (0x300) | `[8](cfg)`, `[0]()` |
| 2 | **battery** | `FUN_0806e1f0`/`FUN_0806df14` | `ERROR robot_battery_initial_fail` (0x32d) | `[4](1)`,`[5](1)`,`[0]()` |
| 3 | **BLADE (blade)** | `FUN_0806e0b4` | `blade_motor_driver_init_failed` (0x353) | `[0]()`,`+0x78 hook`,`[0x13]()` |
| 4 | **RIGHT (right)** | `FUN_0806e79c` | `right_motor_driver_init_failed` (0x362) | `[0]()`,`+0x80 hook`,`[0x13]()`, if board==4: `[0x15](0)` |
| 5 | **LEFT (left)** | `FUN_0806e4e4` | `left_motor_driver_init_failed` (0x371) | `[0]()`,`+0x80 hook`,`[0x13]()`, if board==4: `[0x15](0)` |

- Method `[0]()` = the driver's hardware init; `[2]()` — later start; `[0x13]()` — registration/enable in the framework.
- The motor drivers (`driver_{left,right,blade}_motor_snk_v2.c`) internally poke A4963 over SPI1
  (`FUN_0808408c` write+readback) and the CS/run-coast GPIO. The detailed breakdown of the motor path is in the
  motor/A4963 branch (see `MEMORY: a4963-driver-RESOLVED`, `a4963-RUN-default-por`). What matters here: **rw_init
  does NOT raise any additional power rail-GPIO** beyond what the bootloader already did.
- **Difference blade vs wheels in init:** the blade (BLADE) spins successfully even with our fw ⇒ its VBB is present;
  the wheels (RIGHT/LEFT) are constructed by the same code, but their 20U power rail stays off — this
  confirms that the wheel rail enable is OUTSIDE this firmware path.

---

## 5. LINK TO WHEEL POWER — rails / enable / latch (separately)

**What the firmware actually turns on along the power-on path (the full list of "into power" drives):**

| GPIO | Where it is set | Level | Role | Powers |
|---|---|---|---|---|
| **PE12** | bootloader `FUN_08000f38` (1st) | HIGH (BOP) | **main power latch** | the whole board (self-hold after button/base) |
| **PE7**  | bootloader `FUN_08000f38` | HIGH (BOP) | **aux rail** | auxiliary power (logic/sensors) |
| **PB0**  | bootloader `FUN_08001b4c` (by reset cause) | HIGH (BOP) | **secondary latch** | the board (duplicates the hold) |

- The app (`FUN_0807f010`) only **re-enables the GPIOE/GPIOB clocks** to keep driving these
  pins; it does not raise any new power GPIO. The bootloader `boot_deinit` before the jump does NOT touch the latches.
- **PE0/PE1/PE5** are set to OUT-OD **LOW** (bootloader) — these are not power rails (open-drain, low).
- **GPIOD** is not configured at all in the bootloader; the GPIOD clock (`0x605`) is enabled, but the pins
  (motor CS/run-coast PD3/4/5/7) are configured later by the motor driver in `rw_init`.

**MAIN CONCLUSION ON WHEELS (consistent with `WHEELS-STATE-2026-07-08`):**
Along the whole path reset → idle the firmware raises exactly THREE "into power" lines — **PE12 (main latch),
PE7 (aux), PB0 (secondary latch)**. All of them are board-wide, not drive-specific, and our fw
reproduces them. **There is NO separate firmware GPIO / periph-enable for the wheels' 20U power rail on
the boot path.** The blade sits on a live rail (VBB present → it spins), while the wheel 20U rail is switched
in hardware (high-side switch Q10/Q17/Q14) or gated by the pack's discharge-FET via the BMS — outside this timeline.
The next direction = hardware/BMS, not the boot firmware.

---

## Appendix — DAT_ resolution (from the dump)

```
RCU base          DAT_08010ce8/DAT_0806c794 = 0x40021000
GPIO input base   FUN_08010a54 → RCU (RSTSCK @ +0x24)
FUN_08000f38 ports: E=0x40011800 C=0x40011000 B=0x40010c00 A=0x40010800
FUN_08001b4c latch: DAT_08001b90=0x40010c00 (GPIOB), flag @ 0x20000018 |= 0x40
FSM state var:    0x200000ac + 0x10 = 0x200000bc  (3=idle)
initial task entry: DAT_0806bc30 = 0x0805bc29 (thumb → 0x0805bc28)
```
