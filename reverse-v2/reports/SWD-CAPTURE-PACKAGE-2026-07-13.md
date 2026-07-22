# Offline package for ONE SWD session with the factory firmware (2026-07-13)

Goal: in a single bench session, capture EVERYTHING that closes the open wheel items, so we do not
have to run to the hardware many times. Method — the P1-logger approach (SWD snapshots into a file on the Pi, NOT streaming;
see memory p1-logger-service). Each item: address → what to read → why.

Context correction: the SPI slave of the wheels = the **FU6832N** firmware (not A4963). So `MISO=0xFFFF` under
our fw = FU6832 does not answer. The factory SPI1_DATA at the moment of rotation will show what a LIVE
FU6832 answers — that is the reference for our debugging. The A4963 datasheet is no longer a source of semantics.

---

## BLOCK 1 — static RAM snapshots (target halted/stopped, any moment after init)

Read as words (LE). Pointers → then deref the object.

| Address | What | Expectation | Why |
|---|---|---|---|
| `0x20000540` | ptr BLADE a4963-object | ≠0 | base for the blade reg block (base+0x28) |
| `0x2000054c` | ptr LEFT object | ≠0 | LEFT base (reg block base+0x30) |
| `0x2000057c` | ptr RIGHT object | ≠0 | RIGHT base (reg block base+0x30) |
| `*0x20000580` | SPI1 init-once flag | 0 (initialized) | prove that SPI1 is up |
| `[LEFT]+0x30..0x3e` | shadow reg0..reg7 LEFT | see below | WHAT the factory actually wrote into the wheel FU6832 |
| `[RIGHT]+0x30..0x3e` | shadow reg0..reg7 RIGHT | same | same for the right |
| `[BLADE]+0x28..0x36` | shadow reg0..reg7 blade | reference (live) | compare with the wheels |
| `[LEFT/RIGHT]+0x22` | byte state (0xFF=dir unknown?) | — | driver state |
| `[LEFT/RIGHT]+0x18/+0x08/+0x10` | flags | 2/1/1 | object integrity |

**Shadow regs — REFERENCE values from the dump (check whether they match live):**
- fast set LEFT/RIGHT: reg0..5 = `03E8 22DF 4753 6721 8735 A736`, reg7=`EE0D`, reg6(not sent)=`C000`.
- slow set: reg0..5 = `01A8 2ADF 47D3 6721 8745 A736`.
- **★ The fast/slow selection key = `FUN_08050578()[0x54]` (board variant 0/2 vs 1/3).** Capture the live value
  of this byte → that is how we find out whether the factory really flashes fast or slow on THIS board. Our fw is hard fast
  (motor.c s_a4963_cfg); A/B fast/slow is already runtime since commit bed655f — use exactly that to check.

## BLOCK 2 — SPI1 peripheral (base 0x40003800), capture live

| Address | Reg | Why |
|---|---|---|
| `0x40003800` | SPI1_CTL0 | compare with our 0x0B2F (hal.c). Full match? |
| `0x40003804` | SPI1_CTL1 | mode, DMA, NSS |
| `0x40003808` | SPI1_STAT | busy/rxne |
| `0x4000380C` | SPI1_DATA | ★ last received 16-bit word = diag of the LIVE FU6832 (capture right after the verify transaction; upper bits = status). Reference against our 0xFFFF |

## BLOCK 3 — TIMER2 (PWM, base 0x40000400), capture AT THE MOMENT the wheels rotate

Wheels suspended, blade removed, start from the base into "cut". Capture fully, not just CHxCV:

| Reg (offset) | Why |
|---|---|
| CTL0 `+0x00`, CTL1 `+0x04` | counter enable, mode |
| CHCTL0 `+0x18`, CHCTL1 `+0x1C` | channel mode (PWM mode), preload |
| CHCTL2 `+0x20` | ★ output-enable bits of the channels (CH2/CH3 wheels, CH1 blade) — CCR≠0 is useless when the output is off |
| PSC `+0x28`, CAR `+0x2C` | frequency/period |
| CH1CV `+0x38` (blade), CH2CV `+0x3C` (right), CH3CV `+0x40` (left) | duty of each channel |
| DMAINTEN `+0x0C`, AFIO remap (see GPIO map) | completeness |

Compare with our fw at a non-zero duty. A delta in CHCTL2/CTL0 = candidate for "why it does not spin".

## BLOCK 4 — GPIO choreography of the start (1–2 s before rotation)

Capture the IDR/ODR of the ports that we do NOT fully replicate: **GPIOC, GPIOE, GPIOF** (+ D for CS/NSS).
Method: a series of ODR snapshots of all ports during the idle→drive transition. Look for the pin that toggles
simultaneously with the start of rotation = candidate for the hardware power switch of the FU6832 channels.
Reference: `measurements/force-capture-2026-07-06.txt`, `reference/FACTORY-GPIO-PERIPHERAL-MAP.md`.

## BLOCK 5 — BMS/USART2 (close along the way)

- The connect frame is already known: `1C A1 03 C1 01 2E` (factory-map/03-fsm §2). Verify in the RAM
  USART2 buffer (`0x200005e0` area — 6 bytes of the connect request) as live confirmation.
- Capture whether the wheel VBB comes up under an active keep-alive (CE→connect→C3+53). But per the decompile
  there is NO explicit "enable discharge FET" command (03-fsm §6, 05-bms) — probably the discharge switch is held
  by the pack itself on wake. That is consistent with the "dead link gates the wheels" hypothesis.

---

## MAIN FORK (decided by a multimeter on VDD5, a NON-SWD block)

Before/parallel to SWD: measure "5V"(=VDD5 internal LDO) on the L/R wheel headers vs the blade
(factory idle / factory driving / our fw). This is the primary sorter:
- **wheels VDD5=0, blade=5V** → the wheel FU6832 is NOT powered. SWD blocks 2/3 are secondary, the key is BLOCK 4
  (GPIO switch) + BLOCK 5 (BMS). We look for how the factory supplies VCC to the wheel chips.
- **wheels VDD5=5V** → the chips are powered, silent by protocol/config. The key is BLOCKS 1/2/3
  (reg set fast/slow, SPI CTL, TIMER2 output-enable). Reversing the "enable" = reversing the config.

Bench order: (1) VDD5 measurement with probes 10 min → pick the branch; (2) SWD session over the needed blocks;
(3) if needed, a "while driving" measurement with a solder-on tap.

## Sources
- reference/A4963-DRIVER-bringup-ram.md (object addresses, reg layout, SPI1, diag method)
- reports/WHEELS-ROOT-CAUSE-AUDIT-2026-07-10.md (open deltas fast/slow, SPI, TIMER2)
- factory-map/03-fsm-mow-home.md §6, factory-map/05-bms-pack.md (candidates for the 20U rail, BMS)
- memory: fu6832-drive-architecture, bms-usart2-spec, p1-logger-service
