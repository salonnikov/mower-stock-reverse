# Plan 2026-07-13: mow (chip2+coils) ∥ measurements (FU6832) ∥ reverse wheel enable

Context discovery: the drive is physically = **three Fortior FU6832N** (smart BLDC controller, 8051+FOC,
own firmware 16kB), there are no A4963 on the board. The "A4963 protocol" is implemented by the FU6832 firmware
(SPI slave: P0.5/SCLK, P0.6/MOSI, P0.7/MISO). Reversing SPI from the chip1 side remains valid.
Each FU6832 has a 5-pin header (labeled 5V/GND + 3 signals). Per the FU6832 datasheet
(scratchpad/fu6832_full.pdf): debug FICE = FICEK on pin **RSTN** + FICED on **P1.2**;
**VDD5 = output of the INTERNAL LDO** (VCC 5–28V) ⇒ presence of 5V on the header = "chip is powered".
The board is under conformal coating; only connectors, headers, test pads are accessible (can solder to pads).

User priority: **mow FIRST** (assemble the mower on the factory firmware with new
border coils), measurements — in parallel, reverse — in the background.

---

## Track A (priority): factory + new border coils → mow

Facts: chip2 = GD32F4xx bdboard, ADC+DMA, "base voltage" config (default 2048 = mid-scale
offset, NOT gain), capture windows 800/235 samples, wave/area detection, result → chip1 over
UART/cJSON. One old coil was cut up by the blade; the two new ones are a different rating ⇒ a different
signal amplitude/resonance.

### A1. Offline (no hardware, can do right away)
- In the chip2 decompile (`reverse-v2/chip2/decompiled_all.c`) trace: where "base voltage"
  comes from (flash config? chip1 command? constant) and where the wave/area detection thresholds are
  (`follow_border_check_wave`, `check_wave`, `follow_check_subwave`). Output: a list of
  "sensitivity knobs" with addresses (flash constants / RAM variables).
- In chip1 (bdport, `deal_message.c`) — check whether it sends chip2 any config
  (besides `start search border` / `reset` / version). If a config exists — that is the cleanest
  tuning path.
- Note: chip2 runs the **IEC60730 FLASH-CRC32 self-test** ⇒ any chip2 firmware patch
  requires recomputing/neutralizing the CRC — check how it is computed (the dump has everything).

### A2. Bench: measure what chip2 actually sees from the coils
- Measurement method — **SWD into chip2** (as we did the dump), preferably via the P1-logger approach (not streaming):
  read the RAM sample buffer (800/235) and the wave/area result.
- Reference: the old surviving coil near the perimeter wire (base on) → take the levels.
- Then a new coil in the same position → take the levels. The amplitude difference = the compensation factor.

### A3. Decision based on the A2 result (from simple to complex)
1. **Hardware at the coil connector** (outside the coating!): if the receiver circuit is resonant, tune it
   with a parallel capacitor right at the coil connector/wires up to peak amplitude;
   or a divider/series resistor if the signal is too big instead.
2. **Config**, if A1 finds a configuration channel (base voltage / thresholds) — set the correct value.
3. **Patch chip2 thresholds** (last resort): edit the thresholds in the firmware + solve the CRC32 question.
- Verification: lift sensors = "none" (chip2 sends these too), chip1 finds the border
  (`bdboard find border`), search/follow works. Then assembly and mowing.

---

## Track B (in parallel, multimeter): FU6832 headers + test pads

Preparation: macro photo of each header (pin labels may be on the silkscreen — in photo #1 only
5V/GND are visible, capture the other three). **The blade channel = a live reference** — compare the wheel
headers to the blade header pairwise, pin by pin, even without knowing the pin functions.

States:
- **S1** — factory, idle (on the table).
- **S2** — factory, actually driving (start from base "mow"; wheels off the ground, BLADE REMOVED/blocked!).
  For S2 it is useful to solder wires to the needed pads/pins in advance — you can't hit them with probes while running.
- **S3** — our firmware (rc_fw), drive command (the wheel should spin, but it is silent).

Measurements (DC, relative to header GND):
| Point | Expectation | What the result tells |
|---|---|---|
| "5V" (=VDD5) on the blade header | 5.0V in all S | confirmation of the method |
| "5V" on the L/R wheel headers | **MAIN MEASUREMENT** | 0V in S1/S3, 5V in S2 → channel power is gated, looking for a switch (→ Track C4). 5V everywhere → chips are powered, it's config/protocol (→ C1–C3). 0V even in S2 → method error / pin is not VDD5 |
| 3 unknown pins, all 3 headers | RSTN ≈ VDD5 (pull-up); data ~0/5 | wheel RSTN=0 while blade=5 → chips held in reset (new branch) |
| Pads 20V, 20V_BATT, 5V, 3V3 | at rating | rail map; 20V vs 20V_BATT in S1/S3 — is there a separate switched motor rail |
| Pad STOP | ? | understand the stop-circuit logic |
| LEFT_I / RIGHT_I / BLADE_I | ≈0 at rest | reference points of the current shunts (useful later) |

Order: 20 minutes S1+S3 with probes → already the fork of the whole tree. S2 — on a second pass (soldering).

---

## Track C (background, offline + one SWD pass): how the factory enables the wheels

Base: `reports/WHEELS-ROOT-CAUSE-AUDIT-2026-07-10.md` — the open items remain, now with
the correction "slave = FU6832 firmware, the A4963 datasheet is not an argument".

1. **From the factory live (P1-logger, one pass):** platform[0x54] (fast/slow selection),
   the actual CONFIG0..7 of both wheels after init (RAM bring-up, see
   `reference/A4963-DRIVER-bringup-ram.md`), SPI1 CTL0/CTL1/STAT, the full TIMER2
   (CTL0/CHCTL0..2/PSC/CAR/CHxCV/remap) at the moment of physical wheel rotation.
   Also: the 6 missing bytes of the BMS connect request @0x200005e0 (see bms-usart2-spec).
2. **Diff against mower-own** (motor.c: s_a4963_cfg fast set hard-coded; hal.c: CTL0=0x0B2F) →
   bring to byte match. A/B fast/slow already at runtime (commit bed655f) — use it.
3. **GPIO choreography of the start:** via force-capture + FACTORY-GPIO-PERIPHERAL-MAP find which
   chip1 legs change at the moment "the wheels started spinning" (1–2 sec before) — candidate for the channel
   power switch. In particular: whether anything toggles on ports C/E/F that we don't replicate.
4. **BMS B0..B4 / discharge:** if the B measurement shows VDD5 gating — reverse which event
   the factory uses to turn on the power rail (a BMS command? a 20U GPIO switch?), and reproduce it in rc_fw.

Branching by the B result: "power is gated" → C4 main; "power is present" → C1–C3 main.

---

## Rules (reminder)
- Hardware — only on "go"; prepare everything offline in advance; the mower falls asleep.
- Test BOTH wheels + blade; ALWAYS verify the flash (DPIDR≠deadbeef); reset run drops the target —
  only a physical power-cycle.
- S2 (driving) — remove the blade. Coil experiments — base on, perimeter closed.
