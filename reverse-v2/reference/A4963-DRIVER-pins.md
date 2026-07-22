# A4963 driver: CS / enable / SPI1 pins / PWM binding

Byte-level disassembly from `dist/factory-full.asm` (base 0x08000000) and dump
`dist/gd32-mainboard-dump-v1.bin`. Labels: **[V]** proven by asm byte/string,
**[I]** plausible, **[U]** not fully dug out.

Peripheral bases (from project anchors, confirmed by usage):
GPIOD=0x40011400 (BOP +0x10 = set/HIGH, BC +0x14 = reset/LOW, OCTL +0x0C).
GPIOB=0x40010C00. GPIOE=0x40011800. SPI1=0x40003800. TIMER2=0x40000400.

---

## Key architectural fact: CS lives in the object's callbacks, not in the SPI body

The A4963 SPI frame is sent by `FUN_080840d8` (send, offset 227102). It does NOT touch GPIO
directly — CS is toggled via two function pointers in object `r7` (arg0):

| asm offset | instruction | meaning |
|---|---|---|
| 0x80840fa | `ldr r0,[r7,#0]; blx r0` | **CS assert** (before frame) |
| 0x8084136 | `ldr r0,[r7,#4]; blx r0` | **CS deassert** (after frame) |

send literal pool (offset 227158-227162):
`0x8084160`→**0x000186a0** (timeout 100000), `0x8084164`→**0x20000580** (SPI wrapper),
`0x8084168`→**0x40003800** (SPI1). **[V]** SPI1 = 0x40003800 confirmed by pool byte.

Frame: verify-write `FUN_0808408c` (offset 227079-227080): `r7 = frame & 0xFFF`
(12 data bits), `r8 = frame & 0xE000` (3 high bits = A4963 register address).
send OR's `r1 | 0x1000` (offset 227106) — bit12 = A4963 write command.
Result: each "motor" has ITS OWN driver object with ITS OWN CS callbacks; motor
selection = which object you call, not a bit in the frame. **[V]**

---

## 1. CS of each motor  ✅ RESOLVED

Each create writes the pair of CS thunks into the object at offsets +0x28/+0x2c
(the same as +0/+4 inside the SPI object). The thunks are tiny functions:
`movs r1,#mask; ldr r0,=GPIOx; b.w gpio_helper`.
gpio helpers: `0x80534f8` = `str r1,[r0,#20]` = **BC (+0x14) = LOW/select**;
`0x80534fc` = `str r1,[r0,#16]` = **BOP (+0x10) = HIGH/deselect** (offset 142283/142285).

### LEFT motor — CS = **PD5** [V]
- create `FUN_0805aee0` (offset 155576), object `[0x2000054c]`.
- Setting the callbacks (offset 155660-155662): `[obj+0x28]=0x0805fdc9` (assert),
  `[obj+0x2c]=0x0805fdbd` (deassert). Pool offset 155806/155807.
- Assert `FUN_0805fdc8` (offset 163846): `movs r1,#0x20` (bit5); `ldr r0`=pool@0x805fdd0=**0x40011400** (GPIOD); `b.w 0x80534f8` → **PD5 LOW**.
- Deassert `FUN_0805fdbc` (offset 163841): `movs r1,#0x20`; `ldr r0`=pool@0x805fdc4=**0x40011400**; `b.w 0x80534fc` → **PD5 HIGH**.
- **[V] Left CS = GPIOD bit5 = PD5**, active-low.

### RIGHT motor — CS = **PD4** [V]
- create around 0x805b6xx-0x805b9xx, object `[0x2000057c]` (from set_right).
- Setting (offset 156825/156823): `[obj+0x28]=0x080705a5` (assert),
  `[obj+0x2c]=0x08070599` (deassert). Pool@0x805ba70/0x805ba6c.
- Assert thunk `LAB_080705a4` (offset 192341): `movs r1,#0x10` (bit4); `ldr r0`=pool@0x80705ac=**0x40011400** (GPIOD); `b.w 0x80534f8` → **PD4 LOW**.
- Deassert thunk `LAB_08070598` (offset 192330): `movs r1,#0x10`; `ldr r0`=**0x40011400**; `b.w 0x80534fc` → **PD4 HIGH**.
- **[V] Right CS = GPIOD bit4 = PD4**, active-low.

### BLADE — CS = **PD3** [V]  ⚠️ CORRECTED (this agent's initial conclusion was wrong)
- ❌ The initial conclusion "blade = plain PWM, not A4963" was an ERROR: the agent took the WRONG
  function (`FUN_0805a720`, malloc 32B — that is a different object). The real blade driver =
  **`FUN_0805a858`** (`driver_blade_motor_snk_v2.c`, malloc **64B**, object `[0x20000540]`).
- **Verified byte-wise (cross-checked, file B + direct disasm):**
  - `movw #0xee0d` (A4963 reg7) occurs **exactly 3 times**: 0x805a8be (blade),
    0x805af34 (left), 0x805b874 (right) → **three A4963 drivers**, blade among them.
  - blade CS thunk: assert `0x08022fd8` (`movs r1,#8; ldr r0=0x40011400; b.w 0x80534f8`=BC/LOW),
    deassert `0x08022fcc` (`movs r1,#8; …0x40011400; b.w 0x80534fc`=BOP/HIGH).
    Mask `#8`=bit3, port `0x40011400`=GPIOD ⇒ **PD3, active-low**.
- **[V] Blade = 3rd A4963, CS = PD3.** (It ALSO has a separate PWM CH1 — speed,
  like the wheels: SPI config/direction + PWM duty.)

### Hypothesis resolution
- **PD5 = left CS, PD4 = right CS, PD3 = blade CS — ALL CONFIRMED [V]** (GPIOD, active-low).
- Our firmware (CS PD5/PD4/PD3) **matches the factory** on the CS pins.
- **PD5 vs USART1-remap**: the firmware drives PD5 as a plain GPIO output (direct
  BC/BOP on 0x40011400 bit5). So it is exactly CS that sits on PD5, USART1 is NOT
  remapped here in this firmware. **[V]**

---

## 2. Enable pin(s)  ✅ PB12 CONFIRMED

Common enable = **PB12** (GPIOB, mask 0x1000), raised HIGH (BOP/set).
- SET thunk `FUN_08062bb4` (offset ~168679): `mov.w r1,#0x1000`; `ldr r0`=pool@0x8062bc0=**0x40010C00** (GPIOB); `b.w 0x80534fc` (BOP=HIGH). There is also a paired reset thunk `LAB_08062ba4` (BC=LOW).
- Called from motor inits (BOP HIGH): offset 154852 (0x805a7ee, blade-create), 155562 (0x805aebe, before left-create), 156541 (0x805b7b6, right-create).
- **[V] PB12 = GPIOB mask 0x1000 — common enable, driven HIGH at bring-up.**

Other GPIO thunk pairs noticed nearby (NOT CS, purpose by location):
`PB9` (GPIOB 0x200, offset 161918), `PE12` (GPIOE 0x1000, pool@0x806460c, offset 171565).
**[I]** auxiliary lines (power/preparation), not A4963 CS.

---

## 3. SPI1 config (CTL0/CTL1)

- **[V]** SPI1 base = **0x40003800** (send pool offset 227162).
- **[V]** Data register = base+0x0C: `FUN_0807dd98` (offset ~216xxx):
  `str r1,[r0,#12]` — writes a byte/halfword to **SPI1_DATA (0x4000380C)**.
- **[V]** A4963 transaction = 16-bit "write" frame (`frame|0x1000`), with readback
  and comparison of the 12 data bits (verify-write FUN_0808408c).
- **[U]** Exact **CTL0/CTL1** values (CPOL/CPHA mode, divider, 8/16 bit width)
  NOT byte-localized: config goes through a HAL handle (SPI object 0x20000580,
  [+4]=handle), not a direct write of literal 0x40003800 to CTL — the literal appears in the pools
  only at send. Did not fully trace the init struct. Not making it up.

---

## 4. PWM channel ↔ wheel  ✅ CONFIRMED

Helper `FUN_0807f696(base, ch, val)` (offset 219254) — TIMER CHxCV setter:
ch0→`str [r0,#0x34]`=CH0CV, ch1→`+0x38`=CH1CV, ch2→`+0x3c`=CH2CV, ch3→`+0x40`=CH3CV.

| motor | function | call | channel | register | base (pool) |
|---|---|---|---|---|---|
| **left wheel** | set_left `FUN_0805f620` | offset 163415 (0x805f9d8): `r1=#3` | **CH3** | **TIMER2 CH3CV 0x40000440** | pool@0x805fac4=0x40000400 [V] |
| **right wheel** | set_right `FUN_0806fe66` | offset 191938 (0x80701f6) and 192089 (0x8070368): `r1=#2` | **CH2** | **TIMER2 CH2CV 0x4000043c** | pool@0x80702d8=0x40000400 [V] |
| **blade** | blade-create `FUN_0805a720` | offset 154817 (0x805a78c): `r1=#1` | **CH1** | **TIMER2 CH1CV 0x40000438** | pool@0x805a814=0x40000400 [V] |

- **[V] left=CH3 (0x40000440), right=CH2 (0x4000043c), blade=CH1 (0x40000438)** — all on TIMER2 (0x40000400).
- Objects: left `[0x2000054c]`, right `[0x2000057c]`, blade `[0x20000588]`. **[V]**
- Duty formula: **[I]** period ≈ **7200** (0x1c20). In set_left: `r6 = speed*7200`
  (offset 163069, `mul r6,r1,#0x1c20`), then `udiv` by piecewise divisors
  (0x5fd=1533, 0x17fd=6141, etc. — ramp/deadband), result `uxth`→ written to
  CHxCV as r2. Did not write out the exact piecewise-linear law byte by byte.

---

## Short conclusion (hypotheses → fact)
- **PD5/PD4 CS — CONFIRMED [V]**: PD5=left A4963 CS, PD4=right A4963 CS
  (GPIOD, active-low, assert=BC/LOW before frame, deassert=BOP/HIGH after).
- **PD3 CS = blade (3rd A4963) — CONFIRMED [V]** (corrected; see the "BLADE" section above).
- **PB12 = SPI1 hardware NSS** (GPIOB 0x1000), soft-idle HIGH — this is NOT a separate
  "A4963 enable". There is NO separate GPIO enable for the A4963: enabling = VBB power +
  reg7 RUN. (Clarified by file B; our firmware does PB12→HIGH — it matches.)
- **PWM — CONFIRMED [V]**: left=CH3, right=CH2, blade=CH1, all TIMER2.
- **SPI1**: base 0x40003800 and DATA@+0x0C [V]; exact CTL0/CTL1 (mode/divider/
  bit width) — **[U]** (HAL handle, not fully traced).
