# A4963 — bring-up, driver RAM layout, diag address (chip1)

Dump: `dist/gd32-mainboard-dump-v1.bin` (base 0x08000000, file_offset = addr − 0x08000000). Disassembly: `dist/factory-full.asm`. Each fact carries a [V]/[I]/[U] tag and evidence (offset/asm line). A4963 SPI-frame format (from `reports/drive-chain.md`, confirmed): word = `[15:13]=register address (0..7) | [12]=W (set via |0x1000 on send) | [11:0]=data`. Read verification: they send `word & 0xE000` (address, W=0 ⇒ read) and compare `(word&0xFFF)==readback`.

---

## 0. Three A4963 driver instances and their config functions

| Driver | Config/ctor func | Source line | Object size | Global pointer (RAM) | CS pin |
|---|---|---|---|---|---|
| BLADE (blade) | `FUN_0805a858` | `driver_blade_motor_snk_v2.c` @flash 0x0805aa1c | 0x40=64B (malloc @0x805a86a `movs r0,#64`) | **0x20000540** | **PD3** |
| LEFT (left wheel) | `FUN_0805aee0` | `driver_left_motor_snk_v2.c` @flash 0x0805b0c0 | 0x48=72B (malloc @0x805aef4 `movs r0,#72`) | **PD5** |
| RIGHT (right wheel) | `FUN_0805b820` | `driver_right_motor_snk_v2.c` @flash 0x0805ba00 | 0x48=72B (malloc @0x805b834 `movs r0,#72`) | **PD4** |

[V] Global pointers (RAM word, holds pointer to the malloc object):
- BLADE `*(0x20000540)` — literal @0x805a9f4 = `20000540` (offset 0x5a9f4).
- LEFT  `*(0x2000054c)` — literal @0x805b09c = `2000054c` (offset 0x5b09c).
- RIGHT `*(0x2000057c)` — literal @0x805b9dc = `2000057c` (offset 0x5b9dc).

> Note: the anchor "0x20000544 / 0x2000054c" — LEFT is definitely 0x2000054c [V]; BLADE = 0x20000540 (not 0x20000544) [V]; RIGHT = 0x2000057c [V].

---

## 1. Bring-up orchestrator

### 1.1 Top level — `FUN_080477c4` (`src/app/rw_init.c`) [V]
Order (by the error strings in the body):
1. tilt-sensor init; 2. battery init;
3. **BLADE** motor init: `(*puVar6[0])()` → err-string `blade_motor_driver_init_failed` (rw_init.c:0x353), then vtbl `[0x13]` (register/post);
4. **RIGHT** motor init: `(*puVar8[0])()` → `right_motor_driver_init_failed` (:0x362), vtbl `[0x13]`, on state==4 → `[0x15](0)`;
5. **LEFT** motor init: `(*puVar7[0])()` → `left_motor_driver_init_failed` (:0x371), vtbl `[0x13]`, on state==4 → `[0x15](0)`;
6. `(*puVar6[2])(); (*puVar7[2])(); (*puVar8[2])();` — common post-start of all three.

The manager objects are chosen by board type: `FUN_0806e0b4`/`FUN_0806e79c`/`FUN_0806e4e4` (and branches for other board-types). vtbl[0] of each = low-level a4963-init (`FUN_0805aee0` for LEFT, etc.).

### 1.2 Low-level init (LEFT example — `FUN_0805aee0`) [V]
Step by step (decompiled_all.c:70008):
1. `malloc(0x48)` → object; zero the fields, `byte[+0x22]=0xFF`, `byte[+0x18]=2`, `[+0x08]=1`, `[+0x10]=1`.
2. `FUN_08050578()`→`[+0x54]()` — read the board variant (0/2 vs 1/3) → choose the reg-value set.
3. Fill the shadow reg-words in the object (offsets 0x30..0x3e, see §3) + CS-function pointers into `[+0x28]`/`[+0x2c]`.
4. `FUN_0805f4ac()` — miscellaneous.
5. **`FUN_0801f2f4(obj+0x28)` — SPI1 HW init** (§1.3). Returns 0 ⇒ continue.
6. PWM/timebase: `FUN_0807f474/…(TIMER2=0x40000400,…)`, `FUN_0807f86c(TIMER3=0x40000800,…)` — wheel duty channels.
7. `FUN_0801f4a8()` — PB12(NSS)→HIGH (idle).
8. **Send A4963 registers** in a loop `FUN_0808408c(obj+0x28, *(obj+0x30..))`: reg0,reg1,reg2,reg3,reg4,reg5,**reg7** (offsets 0x30,0x32,0x34,0x36,0x38,0x3a,**0x3e**). **reg6 (0x3c) is NOT sent** [V] (asm 0x805afbe..; decompiled 70109-70118). Any failure → log `a4963` + `A4963 init failed`.

### 1.3 SPI1 HW init — `FUN_0801f2f4` (init-once) [V]
- Init-once flag: `if (*DAT_0801f424 == -1)` → `DAT_0801f424 = 0x20000580` (offset 0x1f424 = `20000580`). After init `*ptr='\0'` (0), mutex handle in `*(0x20000584)`.
- SPI1 base `DAT_0801f42c = 0x40003800` [V] (offset 0x1f42c).
- SPI1 pins on **GPIOB** (`DAT_0801f428 = 0x40010c00`, offset 0x1f428): `FUN_08053558(GPIOB,0x10,3,0x1000)`=PB12, `…0x2000`=PB13(SCK), `…0x4000`=PB14(MISO), `…0x8000`=PB15(MOSI). `FUN_080534f8(GPIOB,0x1000)` = PB12→LOW (BC).
- **PB12 = hardware NSS of SPI1, software-controlled** (init: LOW; `FUN_0801f4a8` = `FUN_080534fc(GPIOB,0x1000)`=PB12→HIGH). ⚠️ This is NOT "A4963 enable" — chip select is done by the per-chip CS (PD3/4/5). [V]
- TIMER2 (PWM reference) `DAT_0801f434 = 0x40000400` [V].

### 1.4 Where the A4963 actually "spins"
- **Driver enable = VBB power + reg7 (RUN) at init.** There is NO separate A4963 GPIO-enable in the SPI path [V] (the only GPIOs around it are the CS PD3/4/5 and NSS PB12).
- **Moment of motion:** PWM duty (TIMER2 CH) 0→N via `FUN_0807f696(...)` + direction/brake via reg7 over SPI. RUN is already asserted at init. I.e. the "spinning state" = nonzero duty on an already-RUN A4963, NOT a separate SPI RUN command at drive start. [V] (see §2, §4-drive)

---

## 2. reg7 / RUN — semantics and WHEN it is written

**reg7 value on all three drivers = `0xEE0D`** [V]:
- BLADE @0x805a912 `strh r8,[r0,#54]` r8=0xEE0D (movw @0x805a8be).
- LEFT  @0x805afba `strh.w ip,[r0,#62]` ip=0xEE0D (movw @0x805af34); decompiled `*(iVar3+0x3e)=0xee0d`.
- RIGHT @0x805b8fa `strh.w ip,[r0,#62]` ip=0xEE0D (movw @0x805b874).

All the places that write a word with `[15:13]=111` (constant `0xEE0D`) in the dump: **only these three init functions** (`grep ee0d` → 0x805a8be/0x805af34/0x805b874). There is NO separate reg7 constant at drive start. [V]

**When:** reg7=0xEE0D is written **ONCE at init** (in the common reg0..reg7 send loop, last) and held. [V]

**Runtime edits to reg7 (they modify ONLY individual bits of the same reg7 shadow word = base+0x0e, then re-send):** [V]
- `FUN_0801f4c6(spi, regblk, …)` = **direction**: `regblk[+0xe] |= 2` (fwd) or `&= 0xFFFD` (rev) → bit1 of reg7 = DIR. (strings `set_blade_direction_failed` and the wheel analogues.)
- `FUN_0801f4b8(spi, regblk, …)` = **brake**: `regblk[+0xe] &= 0xFE0F` (clears bits [8:4]) → "brake" (string `set_blade_brake_failed`).

**Resolution of the "RUN=1-from-init vs RUN-edge" dispute:** [V] → **RUN is configured at init (0xEE0D) and held**; it is NOT asserted by an SPI edge at drive start. At drive start only these change: (a) PWM duty of TIMER2 (0→N, that is the "throttle"), (b) bit1 DIR of reg7, (c) bits[8:4] brake of reg7. There is no separate "RUN=1" event at drive-time.

Decode `0xEE0D` (on send → `0xFE0D`): addr=7, W=1, data[11:0]=`0xE0D`. From the runtime functions: **bit1=DIR, bits[8:4]=brake field** [V]; the exact mapping of the remaining bits to datasheet fields (RUN/coast/restart/ESF) is [I], not dug out bit by bit.

---

## 3. Driver object RAM layout (shadow regs + pointers)

Common scheme (from the init functions of §0):
- **BLADE** (64B): CS functions in `obj+0x20`(assert)/`obj+0x24`(deassert); **reg-block base = obj+0x28**, reg_k = base+2k ⇒ reg0..reg7 @ 0x28..0x36.
- **LEFT/RIGHT** (72B): CS functions in `obj+0x28`(assert)/`obj+0x2c`(deassert); **reg-block base = obj+0x30**, reg0..reg7 @ 0x30..0x3e.
- Common formula for the shadow reg7 = **reg-block base + 0x0E**.

Verification of register addresses by `[15:13]` — all matched [V]:

| reg | BLADE (obj+) value | LEFT var A / var B (obj+) value | meaning (brief) |
|---|---|---|---|
| reg0 | 0x28 = 0x03E4 | 0x30 = 0x03E8 / 0x01A8 | config0 |
| reg1 | 0x2A = 0x2A5F/0x2ADF | 0x32 = 0x22DF / 0x2ADF | config1 |
| reg2 | 0x2C = 0x478D | 0x34 = 0x4753 / 0x47D3 | config2 |
| reg3 | 0x2E = 0x6723 | 0x36 = 0x6721 | config3 |
| reg4 | 0x30 = 0x8974 | 0x38 = 0x8735 / 0x8745 | config4 |
| reg5 | 0x32 = 0xA933 | 0x3A = 0xA736 | config5 (slew/mode; edited by `FUN_0801f4dc` bits[6:4]) |
| reg6 | 0x34 = 0xC000 | 0x3C = 0xC000 | **mask** — stored, NOT sent at init |
| reg7 | 0x36 = 0xEE0D | 0x3E = 0xEE0D | **RUN/DIR/brake** |

(BLADE evidence: strh chain 0x805a8f2..0x805a912; LEFT: decompiled 70055-70085 + asm 0x805af72..0x805afba; RIGHT identical to LEFT.)

CS-function pointers in the object [V]:
- LEFT: `obj+0x28 = 0x0805fdc8` (assert=CS low), `obj+0x2c = 0x0805fdbc` (deassert=CS high) — literals @0x805b12c/0x805b128.
- RIGHT: `obj+0x28 = 0x080705a4`, `obj+0x2c = 0x08070598` — @0x805ba70/0x805ba6c.
- BLADE: `obj+0x20 = 0x08022fd8`, `obj+0x24 = 0x08022fcc` — @0x805aa8c/0x805aa88.

Other init fields: `byte[+0x22]=0xFF` (LEFT/RIGHT) — state/"dir unknown" [I]; `byte[+0x18]=2`, `[+0x08]=1`, `[+0x10]=1`, `u32[+0x40]=0`, `u32[+0x44]=0` (LEFT). An "initialized" flag as a separate word in the object is NOT allocated; the actual "already created" = `*(global pointer) != 0`. [V]

### 3.1 CS functions (byte by byte) [V]
Both branches branch into the common GPIO helpers: assert→`0x080534f8` (`str r1,[r0,#0x14]`=**BC**, pin→LOW), deassert→`0x080534fc` (`str r1,[r0,#0x10]`=**BOP**, pin→HIGH). The port in all = `0x40011400`=**GPIOD**. Masks: LEFT `#32`=bit5=**PD5**, RIGHT `#16`=bit4=**PD4**, BLADE `#8`=bit3=**PD3** (asm 0x805fdbc/0x80705a4/0x8022fcc). CS is active-LOW.

---

## 4. ★ Address of the A4963 "last read diag"

**Conclusion [V]: the factory driver does NOT store a diag word in persistent RAM.** The only path that reads A4963 MISO is write verification:
- `FUN_0808408c` (verify-write) → inside, `FUN_0806caac(spi, addr, &local_20)` places the 16-bit reply into `*param_3`.
- In `FUN_0808408c` `param_3 = &local_20` — a **stack local**; after comparing `(word&0xFFF)==(local_20&0xFFF)` the word is discarded. The high bits [15:12] (A4963 diagnostics) are NOT preserved. [V]
- `FUN_0806caac` — the only call from `FUN_0808408c` (grep: 2 matches — definition + this call). `FUN_0807dd92` (reading `SPI_DATA` = base+0xC) is called only in verify paths and in a different SPI (`FUN_0806cc24`, another peripheral). A periodic A4963 health-poll that stores diag into an object/global was NOT found. [V]/[U]

**How to nevertheless capture the factory driver's live diag (to calibrate our 0xFFFF):**
1. **[recommended] Read `SPI1_DATA` = `0x4000380C`** (SPI1 base 0x40003800 + 0x0C) — over SWD at/right after the transaction: the last received 16-bit word sits there (high bits = A4963 diag). [V-address, I-timing-dependent]
2. **Breakpoint on the return of `FUN_0806caac`** (0x0806cb28 area) or on `*param_3=uVar3` — capture `local_20`/the register holding diag. [I]
3. Shadow reg-words (§3) — read statically: they show what the driver *sent* (not what it received), but confirm that the init-config reached the object.

> ⚠️ Key: do diag-calibration through the SPI1_DATA peripheral / breakpoint, not through a "diag RAM field" — the factory driver has no such field.

---

## 5. Table "RAM/periph address → what to read in the P1 snapshot"

| Address | What it is | How to read | Tag |
|---|---|---|---|
| `0x20000540` | pointer to the BLADE a4963 object | word; then deref | [V] |
| `0x2000054c` | pointer to the LEFT a4963 object | word; then deref | [V] |
| `0x2000057c` | pointer to the RIGHT a4963 object | word; then deref | [V] |
| `*(0x20000580)` | SPI1 init-once flag (0=initialized) | word @0x20000580 | [V] |
| `*(0x20000584)` | SPI-mutex handle | word @0x20000584 | [V] |
| `[LEFT_obj]+0x3e` | shadow **reg7 (RUN/DIR/brake)** LEFT, expect 0xEE0D±bits | deref LEFT +0x3e, u16 | [V] |
| `[LEFT_obj]+0x30..0x3c` | shadow reg0..reg6 LEFT | u16×7 | [V] |
| `[RIGHT_obj]+0x30..0x3e` | shadow reg0..reg7 RIGHT | u16×8 | [V] |
| `[BLADE_obj]+0x28..0x36` | shadow reg0..reg7 BLADE | u16×8 | [V] |
| `[obj]+0x22` (LEFT/RIGHT byte) | state/flag (init 0xFF) | byte | [I] |
| `[obj]+0x28`/`+0x2c` (wheels), `+0x20`/`+0x24` (blade) | CS assert/deassert pointers (validate the object) | word | [V] |
| **`0x4000380C`** | **SPI1_DATA — last received word (A4963 diag in the high bits)** | u16, at the moment of the transaction | [V]-address / [I]-timing |
| `0x40003800`+0x00/0x08 | SPI1 CTL0/STAT (cross-check the mode) | u16 | [V] |
| GPIOD OCTL `0x4001140C` bits 3/4/5 | current CS BLADE/RIGHT/LEFT | word | [V] |
| GPIOB OCTL `0x40010C0C` bit 12 | NSS SPI1 (idle=1) | word | [V] |

**Open [U]:** the exact bit-by-bit decode of reg7 (which bit exactly = RUN/coast/restart) is not verified against the A4963 datasheet; a periodic diag-poll with storage in RAM was not found (possibly there is none — then only SPI1_DATA/breakpoint).
