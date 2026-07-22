# chip2 — border-detection sensitivity (for coil replacement), 2026-07-13

Task: the perimeter receiver coils were replaced with a different rating (different signal amplitude) →
how to re-tune chip2 so the border is detected again. Analyzed from the decompile
`reverse-v2/chip2/decompiled_all.c` (two independent passes).

## Main conclusion (practical)

**The correct fix is analog, at the coil connector, WITHOUT touching the firmware.** The detector is hard-
wired to `|sample| > 2500` with an auto-calibrated DC center of ~2048. The threshold is flash-immediate, and chip2
runs the IEC60730 FLASH-CRC32 self-test ⇒ a firmware patch requires recomputing the CRC, otherwise the board falls into
a self-fault. Therefore: match the new coils' signal amplitude to what the firmware expects
(amplifier/divider on the front-end), rather than changing the firmware.

## How the "wave present" decision is made (signal path)

ADC+DMA → raw buffer → laid out as signed int16 per channel (left `+0x48`, right `+0x68`),
window `+0x92` = **800** (hi-res) or **235** (lo-res) samples → detector core `FUN_08015fa8`
@0x08015fa8 → per-frame amplitude comparator `FUN_0801baf8` @0x0801baf8 → zero-crossing
`FUN_08015dcc` (by sample sign — confirms that samples are signed around the base) → area/direction
integrator `FUN_08015260` → cJSON to chip1 (`FUN_08019914`).

The string `left wave data error, area=%d, str=%d` @0x0801981c is only a sanity check "the area sign
matches the direction byte", NOT a detection threshold.

## Knobs (ranked by applicability)

### ★ KNOB 1 — amplitude threshold (THIS is the "sensitivity")
`FUN_0801baf8` @0x0801baf8 (lines 16553–16589):
```c
sVar5 = 0x9c4;  sVar6 = -0x9c4;             // = ±2500 (DEFAULT)
if (param_5 == 1) { sVar5=5000; sVar6=-5000; } // "strong-signal" mode
... if ((2500 < sample) || (sample < -2500)) { keep; count++; } else zero;
if (param_3 < count) return 2;  // too many above threshold → "saturation/interference"
if (count < param_4) return 1;  // too few → "no wave"
return 0;                       // valid wave
```
- Lower 2500 → detects a weaker signal (more sensitive). Raise → less.
- **flash-immediate, not written at runtime.**

### KNOB 2 — count gates (how many window samples must exceed KNOB 1)
Arguments in `FUN_0801baf8`: from `FUN_08015fa8` (line 10407) `upper=600, lower=5`; sub-wave
`FUN_080164bc` @0x080164bc lower=5; `FUN_08016784` @0x08016784 lower=4.
- `count>600` → return 2 (interference), `count<5` → return 1 (no wave). For weak coils fewer
  samples pass KNOB 1 ⇒ the lower gate (5) matters. Also flash-immediate.

### KNOB 3 — auto-desensitization ("strong" flag, SRAM `+0xa8`)
`FUN_08015fa8` (lines 10440–10450): while the detector returns "too many" (state 2), counter
`+0xa9` grows; after `>20` it latches `+0xa8=1` → the KNOB 1 threshold switches 2500→5000.
- **For weak coils this flag will NEVER latch** (the signal is not "too strong"), the effective
  threshold stays 2500. That is, the design is centered on 2500; the 5000 path is against a too-strong signal,
  which does not concern us. There is NO auto-raising of sensitivity for a WEAK signal.

### KNOB 4 — base voltage (DC offset) — AUTO-CALIBRATED, this is NOT gain
`FUN_08012b6c` @0x08012b6c (lines 6309–6366): averages 5 ADC measurements → `*_DAT_08012c88`. Validator
`FUN_0801a1f8` @0x0801a1f8: sums 8 conversions, valid if the average is in **[1906, 2191]**
(`0x772…0x88f`); outside the window — error, the default **2048** is substituted (string `base voltage =%d error,
set default 2048` @0x080174ef).
- Taken by a **runtime measurement**, not a constant and not a chip1 command. Self-calibrates to the actual DC bias
  of the receiver. It shifts the zero, does NOT scale the amplitude, clamped ~2048±140 ⇒ **useless as a sensitivity
  knob.** (Confirms the earlier note "2048 = mid-scale offset, not gain".)

### KNOB 5 — effective gain via accumulation (explains the number 2500)
A 12-bit ADC around 2048 gives ±2048, but the threshold 2500/5000 — so the samples are **accumulated** (not
a single raw−2048). `FUN_0801a1f8` sums 8 conversions. The window accumulation factor lives in the ADC/DMA
HAL object (`FUN_08019d34` @0x08019d34, vtable `+0x74`) — **the exact immediate not pried out** (indirect
calls). The only unclosed ambiguity; everything else is quoted from the decompile.

### KNOB 6 — window size / period (secondary)
Window `+0x92`=800/235; wave half-period `+0x78`=`0x49b4`=18868 ticks (period geometry for
area/direction, not amplitude); segment tolerances 20/25, gap 50.

## Is there a config channel with chip1? — NO (second agent, independently)
chip1 → chip2 = only commands + `{"cmd":32772,"rtc":<sec>}` (time sync). The only cJSON builder
in service_bdport.c (`FUN_0802244c` @0x0802244c) sends `cmd`+`rtc`. Neither base/threshold/gain/wave/area —
nowhere. On the receiving side chip2 also has no "set sensitivity" command. **The "tune by a command from the main
chip" path does not exist in the firmware.** Tuning — only front-end hardware or a chip2 firmware patch.

## Ranked plan for the coil replacement
1. **★ Analog front-end at the coil connector (preferred).** New coils are weaker → raise the
   gain (op-amp stage) to the old amplitude; stronger → divider/attenuator. The firmware is not touched,
   CRC/self-test intact. Target amplitude: so the peak samples confidently exceed ±2500 at the border,
   but do not give >600 samples above threshold (otherwise it is interpreted as interference).
2. **Patch KNOB 1 (0x9c4 → new) in `FUN_0801baf8`** (+ if needed the 600/5 gates and the 5000 strong mode).
   The most direct, BUT requires recomputing the IEC60730 FLASH-CRC32 and reflashing. Only if analog is impossible.
3. base voltage (KNOB 4) — no good for amplitude compensation (auto, clamped, it's an offset).

## Measurement to choose the multiplier (no scope): SWD into chip2
Read the channel signed buffers (left `[obj]+0x48`, right `+0x68`, `+0x92` samples) while bringing it near the
perimeter wire: the old surviving coil = reference peak |sample|, a new one in the same position =
new peak. The ratio = the required front-end factor. We aim so the new peak ≈ the old (confidently >2500).

## Discrepancy to check
The SVD/memory_map agent identified chip2 as **GD32F30x_CL** (12-bit ADC, important for the arithmetic above),
whereas `factory-map/06-chip2.md` says "GD32F4xx". Cross-check against the dump; it does not affect the knob conclusions
(the constants are quoted from the code directly).

## Sources
- reverse-v2/chip2/decompiled_all.c: FUN_0801baf8 (16553+), FUN_08015fa8 (10407+), FUN_08012b6c
  (6309+), FUN_0801a1f8 (14722+), FUN_08019d34, FUN_08015260/08015dcc.
- reverse-v2/chip1/decompiled_all.c: FUN_0802244c (bdport TX), FUN_08021bb4 (dispatch).
- factory-map/06-chip2.md §3.
