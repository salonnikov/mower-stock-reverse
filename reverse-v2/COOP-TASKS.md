# COOP-TASKS — exchange board between the analyst and the flasher

Purpose: asynchronous exchange of tasks and results between two people.
- **Analyst** (reverse/analysis, does NOT flash): posts tasks in the "OPEN TASKS" section, provides exact addresses/context.
- **Flasher** (edits and flashes firmware, works with the hardware): performs them, writes the answer in "RESULTS / ANSWERS", and files counter-questions when needed.

## Rules
1. Each task = a `### TASK-NNN` block with a status: `OPEN` → `IN PROGRESS` → `DONE` / `BLOCKED`.
2. A task is self-contained: addresses, files, what to capture, the readiness criterion — without references to "our previous conversation".
3. The answer is written under the same number in "RESULTS / ANSWERS" with a date.
4. The flasher flashes NOTHING until a task explicitly asks to flash; by default tasks are read-only (SWD read).
5. Use an absolute date (YYYY-MM-DD).

---

## OPEN TASKS  (analyst → flasher)

### TASK-001 — capture a raw ADC window of the coils from the FACTORY firmware of chip2  · status: OPEN
**Why:** we are replacing the native border coils with two non-native (identical) ones. To recompute
the detection threshold, we need the real signal amplitude of the new coils in ADC counts.

**Bench conditions:** factory firmware on chip2 (do NOT reflash), the perimeter wire
under current/signal, the coils brought close to the wire. lift = none.

**Connection:** SWD to **chip2** (not chip1). The coil counts do not reach chip1 — only chip2.

**What to read over SWD (RAM chip2, halt → read → resume):**
- `0x20000094` — base voltage (16 bits), the auto-calibrated coil zero.
- Sensor descriptor: find the board_sensor object (in the decompile `*(int*)(DAT+4)`), fields:
  - `+0x92` — window length (expect 800 or 235),
  - `+0x48` — pointer to sample buffer #1 (short[len]),
  - `+0x68` — buffer #2 (short[len]),
  - `+0x70` — buffer #3 (int[len]).
- Capture the **window itself**: dump the buffer `+0x48` of `len` × 2 bytes, at least 3–5 repeats
  (with the wire under current and without — for contrast).

**Readiness criterion:** attach to TASK-001 in the answers: the base voltage value and
the window dump(s) (bin or CSV of counts) for the new coils. From those the analyst will compute the new
threshold (currently hardcoded at ±2500 in `FUN_0801baf8`).

**Reference for finding the addresses:** [reverse-v2/chip2/decompiled_all.c](chip2/decompiled_all.c)
— `FUN_08012b6c` (base calibration, line 6349), `FUN_08019e34/eb8` (window 800/235),
`FUN_0801baf8` (threshold ±2500). Full analysis — in the chip2 analysis section.

**Open question to the flasher:** are the L and R coils two separate ADC channels or
a multiplex of one? We need this for how the two new coils will physically sit. If it is visible from
the descriptor/schematic — note it in the answer.

### TASK-002 — a second permanent SWD harness for chip2 (its own pins, no re-plugging)  · status: OPEN
**Why:** remove the "weak spot" — the physical re-plugging of wires between chip1 and chip2.
We make two permanent harnesses; the chip choice = launching the right OpenOCD config.

**Existing (chip1, do NOT touch):** `bcm2835gpio_swd_nums 25 24`
(checked against the wiring photos [dist/wiring-rpi1-gpio-swd-v2.png](../dist/wiring-rpi1-gpio-swd-v2.png)
and [dist/wiring-rpi2-gpio-swd-v1.png](../dist/wiring-rpi2-gpio-swd-v1.png))
- SWCLK = GPIO25 = physical pin 22
- SWDIO = GPIO24 = physical pin 18
- GND   = physical **pin 6**  ← the chip1 ground is exactly on pin 6, NOT 20

**Add (chip2), the neighboring row of the same right column:**
- SWCLK2 = GPIO8 = physical pin 24
- SWDIO2 = GPIO7 = physical pin 26
- GND2   = physical pin 25

GPIO7/8 are free (SPI0 is not used, SWD = bit-bang bcm2835gpio). The 3.3 V levels of the GD32 and the Pi
match → no level shifter. 3 wires to chip2: CLK+DIO+GND. Keep the ground next to the signals.

**Config edits:** make copies of the chip2 configs ([tools/rpi-swd/chip2-*.cfg](../../tools/rpi-swd/))
replacing the line `bcm2835gpio_swd_nums 25 24` → `bcm2835gpio_swd_nums 8 7`. The chip1 configs
(`gd32-*.cfg` etc.) keep at `25 24`.

**Rules:** do not re-plug wires; bring up chip1 OR chip2 in turn (do not run two OpenOCD instances
simultaneously).

**Readiness criterion:** report here — the chip2 harness is soldered to pins 24/25/26, the chip2 config on
`8 7` reads chip2's IDCODE/DPIDR (DPIDR ≠ deadbeef). After that TASK-001 can be done without touching
the chip1 wires.

---

## RESULTS / ANSWERS  (flasher → analyst)

<!-- answers here under the task number, for example:
### TASK-001 · 2026-07-10
base voltage = ...
window: ... (file attached ...)
L/R channels: ...
-->

---

## LOG
- 2026-07-09 — file created. TASK-001 filed (capture an ADC window of the coils from the factory chip2).
- 2026-07-09 — TASK-002: a second permanent SWD harness for chip2 on pins 24/25/26 (GPIO8/7/GND),
  config `bcm2835gpio_swd_nums 8 7`. Order: first TASK-002 (harness), then TASK-001 (window capture).
