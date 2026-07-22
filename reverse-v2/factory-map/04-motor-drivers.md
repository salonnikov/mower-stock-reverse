# 04 — chip1 motor drivers (blade / left / right) + A4963

Full analysis of the three A4963 drivers in the factory firmware MI 302 (chip1, GD32F4xx).
Source: `reverse-v2/chip1/decompiled_all.c` + dump `dist/gd32-mainboard-dump-v1.bin`.
Goal: find the blade-vs-wheels asymmetry, candidates for "wheel power/gate".

> **Decompilation status.** Fully decompiled and authoritative is the **LEFT** driver
> (`FUN_0805aee0` + run/brake `FUN_0805f2xx..f5xx`). The constructors of the **blade** (`0805a858`)
> and the **right** (`0805b820`) fell into unresolved Ghidra gaps — their fields were recovered
> from the ops tables (function-pointer vtable) and from the common helpers they actually call.
> This is honestly flagged below. All three motors use the same A4963 write path.

---

## 1. FUNCTION MAP

### 1.1 Driver creation (rw_init `FUN_080477c4`, order: tilt→battery→**BLADE→RIGHT→LEFT**)
Drivers are created via getters, each filling an "ops" object (vtable) with function pointers:

| Motor | ops-filler | init-function (ops[0]) | set_dir (SPI) | run/coast (GPIO) | PWM channel TIMER2 |
|---|---|---|---|---|---|
| BLADE (blade) | `FUN_0806e0b4` / `FUN_0806dff8` | `FUN_0805a858`* | `FUN_08022a80`→`FUN_0801f4c6` | **none** | CH1 (`FUN_0807f696(...,1,..)`) |
| RIGHT (right)| `FUN_0806e79c` / `FUN_0806e6d0` | `FUN_0805b820`* | (SPI, analogous) | **`FUN_0806fc2c`** (PD4, bit 0x10) | CH2 |
| LEFT (left) | `FUN_0806e4e4` / `FUN_0806e418` | **`FUN_0805aee0`** | `FUN_0805f300`→`FUN_0801f4c6` | **`FUN_0805f3d8`** (PD7, bit 0x80) | CH3 (`FUN_0807f474/696(...,3,..)`) |

`*` — not decompiled (gap), address taken from ops[0] in the filler.
Two fillers per motor = two board versions (board type `0x50dc` and `0x708`, selected in rw_init).

### 1.2 Common A4963/SPI/GPIO primitives
- **A4963 write+verify** `FUN_0808408c(handle, word)`: writes `word`, reads the reg back
  (`FUN_0806caac`, reg address = `word & 0xe000`), compares `word&0xfff == readback&0xffff`,
  up to 0x31 retries. → every config write is verified.
- **A4963 transfer** `FUN_080840d8(handle, word)`: CS assert = `(*handle[0])()`, sends
  `word | 0x1000` (**WR bit=0x1000**) via `FUN_0807dd98`, CS deassert = `(*handle[1])()`.
- **A4963 direction** `FUN_0801f4c6(handle, cfg, dir)`: edits **bit 0x2 of the word cfg+0xe** (= reg7),
  rewrites reg7 over SPI. Direction = bit1 of reg7. Brake — the neighboring helper `FUN_0801f4b8`.
- **GPIO set/clear** `FUN_080534f8(port,mask)` writes `port+0x14`; `FUN_080534fc(port,mask)`
  writes `port+0x10` (set/reset-latch pair). **This is the driver's only "discrete" output.**
- **GPIO mode/AF** `FUN_08053558` (CTL), `FUN_08053608` (AF-mux). **RCU-enable** `FUN_0806c77c(id)`
  (id>>6 = offset of the EN register, id&0x1f = bit).
- **PWM (TIMER2 CHxCV/CHCTL)** channels: `FUN_0807f474` (config CH), `FUN_0807f696` (duty),
  `FUN_0807f86c` (TIMER3 channel config), `FUN_0807f648/6b4/7cc/3f8` — enable/polarity/start.
- **Tacho (TIMER3 input-capture)** `FUN_08062c64(ch,cb)` + `FUN_08062c8c(ch,ctx)` —
  capture callback registration. **Called ONLY by the wheel inits.**

---

## 2. A4963 CONFIG WORDS

A4963 word format: **bits 15:13 = register address (0..7)**, bit 12 = WR (added in the
transfer as `|0x1000`), bits 11:0 = data. Below — the 8 words (reg0..reg7) that the LEFT init
(`FUN_0805aee0`) places into the object at offsets +0x30..+0x3e and then sends as 7 writes
(reg0..reg6 + reg7; reg5 is skipped in the loop — indices 0,1,2,3,4,5?,7 are written, see the code in §4).

| reg (15:13) | NORMAL (fast) | SLOW mode | notes on the A4963 fields |
|---|---|---|---|
| reg0 `000` | `0x03E8` (=1000) | `0x01A8` (=424) | data 0x3E8/0x1A8 — timings (off-time / t_blank) |
| reg1 `001` | `0x22DF` | `0x2ADF` | data .df — dead-time / blank; in slow the high nibble is 0x2A |
| reg2 `010` | `0x4753` | `0x47D3` | VDS/current-limit threshold; slow changes one bit |
| reg3 `011` | `0x6721` | `0x6721` | identical — fixed field |
| reg4 `100` | `0x8735` | `0x8745` | bemf/commutation; slow differs |
| reg5 `101` | `0xA736` | `0xA736` | identical |
| reg6 `110` | `0xC000` | `0xC000` | identical — data=0 (mask/mode) |
| reg7 `111` | **`0xEE0D`** | **`0xEE0D`** | **RUN register: RUN held, direction=bit1, brake — edited at runtime** |

Decode of reg7 `0xEE0D` = `1110 1110 0000 1101`b: address=111 (reg7), data=0xE0D.
Direction is changed by `FUN_0801f4c6` (bit 0x2). **reg7 holds RUN — the chip, once powered,
must spin (see the investigation conclusion about auto-run).**

> **Blade and right:** their inits are not decompiled, but (a) they use the same `FUN_0808408c`
> write path, (b) their direction goes through the same `FUN_0801f4c6` (reg7 bit1), (c) per project
> memory reg7=0xEE0D is common to all three. A separate config-word table for blade/right requires an
> ARM-thumb disassembler (unavailable on the host); architecturally they are identical to the left.

---

## 3. run / brake TIMELINE

### 3.1 LEFT init `FUN_0805aee0` (authoritative)
1. malloc the object 0x48, zero the fields.
2. Reads the platform mode `FUN_08050578()[0x54]` → selects **fast** (0/2) or **slow** (1/3) config set → writes 8 words into +0x30..+0x3e.
3. `FUN_0805f4ac()` — left GPIO/PWM config: RCU `0x600`, port `0x605`/`0x604`; sets pins bit 0x20 (**PD5 = left CS**), bit 0x40 and AF (0x48) — TIMER2 CH3 PWM (PC9).
4. `FUN_0801f2f4(obj+0x28)` — A4963 SPI1-handle init.
5. `FUN_0806c77c(0x701)` — RCU-enable; then **TIMER2 CH3**: `FUN_0807f474(TIMER2,3,cfg)`,
   `FUN_0807f696(TIMER2,3,0)` duty=0, `FUN_0807f648(TIMER2,3,0x60)`, `_6b4`, `_3f8`, `_7cc` — start the channel.
6. **TIMER3 (tacho)**: `FUN_0807f86c(TIMER3,1,cfg)`, `_9a0/_998`, and
   **`FUN_08062c64(4, cb)` + `FUN_08062c8c(4, ctx)`** — wheel input-capture registration.
7. **8× `FUN_0808408c(obj+0x28, reg_word)`** — flash A4963 config (reg0..reg6, reg7) with verify.
   Any failure → log `a4963` + return -1.

### 3.2 LEFT run/brake
- **direction/start** `FUN_0805f300(dir)`: `FUN_0801f4c6(obj+0x28, obj+0x30, dir)` — reg7 bit1 over SPI.
- **run/coast GPIO** `FUN_0805f3d8(on)`: if start → `FUN_080534f8/fc(DAT_0805f430, **0x80**)`
  = **PD7** set/clear (by the polarity flag obj+9). Map: **PD7 LOW = run**.
- **PWM duty**: `FUN_0807f696(TIMER2, 3, duty)` (CH3CV).

### 3.3 RIGHT run/coast `FUN_0806fc2c(on)`
Exact analogue of the left, but GPIO bit **0x10 = PD4** (`FUN_080534f8/fc(DAT_0806fc84, 0x10)`),
PWM = CH2, tacho channel same mechanism. direction — via `FUN_0801f4c6` (reg7).

### 3.4 BLADE run/brake (blade)
- **direction** `FUN_08022a80(dir)`: **only** `FUN_0801f4c6(obj+0x20, obj+0x28, dir)` — reg7 over SPI.
- **speed/brake** `FUN_08022dc8(speed)`: on speed<200 → `FUN_0801f4b8` (A4963 brake over SPI) +
  `FUN_0807f696(TIMER2, **1**, 0)`; otherwise PWM duty on **CH1**.
- **There is NO GPIO run/coast line.** Blade control = purely A4963 SPI config + PWM CH1.

---

## 4. KEY CODE (annotated)

```c
// A4963: write a config word with the WR bit and verification (common to the 3 motors)
FUN_0808408c(handle, word) {
  do { transfer(handle, word);                 // sends word|0x1000 (WR)
       read_reg(handle, word & 0xe000, &rb);    // reads the same reg (address=top3)
  } while (word&0xfff != rb&0xffff && ++n<0x31);// verify low-12 bits
}

// A4963 direction = reg7 bit1 (same for blade and wheels)
FUN_0801f4c6(handle, cfg, dir){
  w = *(u16*)(cfg+0xe);                          // cfg+0xe = reg7 (=0xEE0D)
  w = dir ? (w|2) : (w & ~2);
  *(u16*)(cfg+0xe)=w;  FUN_0808408c(handle, w);  // re-flash reg7 over SPI
}

// ★ WHEELS: physical GPIO run/coast (ABSENT on the blade)
FUN_0805f3d8(on){   // LEFT
  if (start) (flag==-1) ? SET(PORT,0x80) : CLR(PORT,0x80);  // 0x80 = PD7
}
FUN_0806fc2c(on){   // RIGHT
  ... SET/CLR(DAT_0806fc84, 0x10);                          // 0x10 = PD4
}

// BLADE: direction/brake — only A4963 SPI, no discrete enable
FUN_08022a80(dir){ FUN_0801f4c6(obj+0x20, obj+0x28, dir); } // that's all
```

---

## 5. ★ BLADE vs WHEELS ASYMMETRY

| Trait | BLADE (blade) | WHEELS (left/right) | What it means |
|---|---|---|---|
| A4963 config (reg0-7) | same path `FUN_0808408c`, reg7 RUN-held | the same | **symmetric** — not the cause |
| direction | SPI reg7 bit1 (`FUN_0801f4c6`) | SPI reg7 bit1 (the same helper) | symmetric |
| **discrete GPIO run/coast** | **NONE** | **PRESENT: PD7 (left), PD4 (right)** | ★ the only discrete output on the wheels; but **already tested LOW=run — did not help** (WHEELS-STATE) |
| PWM TIMER2 | CH1 | CH2 (right), CH3 (left) | symmetric (different channels) |
| **TIMER3 tacho** (`FUN_08062c64/8c`) | none | **present** (wheel input-capture) | speed sensor, **not a power path** |
| ops-slot +0x24, +0x54 | none (has puVar4[7]) | **present** (fptr `0807894x`/`0807afxx`) | these addresses are in the `dpport/report` service area → **speed report/odometry, not power** |
| power rail | live (blade spins under our fw) | switched **20U**, off under our fw | ★ cause of "the wheels are dead" |

### Candidates for "wheel power/enable" — conclusion
1. **There is NO firmware power asymmetry.** All discrete/peripheral differences blade-vs-wheels
   are exhausted by: (a) run/coast GPIO **PD7/PD4** — already checked, does not enable the rail;
   (b) TIMER3 tacho and (c) ops report callbacks `0807894x/0807afxx` — these are **sensing/telemetry**,
   not a power path. No per-wheel power/gate-GPIO exists in the firmware.
2. This **confirms** the hypothesis from `WHEELS-STATE-2026-07-08.md`: **the 20U rail enable is NOT a firmware-GPIO**.
   The blade sits on an always-live rail (hence it spins auto-run from reg7=RUN), the wheels — on a
   switched 20U whose switch is outside the GD32.
3. **Remaining non-firmware / non-flashed candidates** (in priority order):
   - **BMS discharge-enable over USART2** — enabling the pack's power discharge FET (the blade is powered
     differently/earlier). ★ next step — reverse the "enable discharge" command.
   - **`movebase_snk_v10.c`** (wheel motion service, not analyzed) — may toggle something at
     drive start on top of the driver.
   - **Hardware high-side gate of the 20U key** (Q10/Q17/Q14) — probing the gate (low priority).

### What could NOT be extracted (limitation)
The config words reg0-7 of the **blade** (`0805a858`) and the **right** (`0805b820`) — their inits are in Ghidra
gaps, and an ARM-thumb disassembler is unavailable on the host. Recovered indirectly (common write
path + reg7=0xEE0D). For a byte-precise blade/right table an object disassembler is needed in a container.
