# reverse-v2/reports — index of the current reports

Living documents only. The obsolete/superseded ones are moved to `reverse-v2/archive/2026-07-13/`
(nothing deleted — the history is in git). The date in parentheses = the currency of the content.

## Current (read these)

| File | About | Status |
|---|---|---|
| [PLAN-2026-07-13-coils-wheels-fu6832.md](PLAN-2026-07-13-coils-wheels-fu6832.md) | **The main current plan.** Three tracks: mow (chip2+coils), FU6832 measurements, reverse the wheel enable | ✅ current |
| [SWD-CAPTURE-PACKAGE-2026-07-13.md](SWD-CAPTURE-PACKAGE-2026-07-13.md) | Offline package for ONE SWD session with the factory: addresses → what to read → why | ✅ current |
| [CHIP2-BORDER-SENSITIVITY-2026-07-13.md](CHIP2-BORDER-SENSITIVITY-2026-07-13.md) | chip2: how to retune the border detection for the new coils (thresholds, base voltage, CRC) | ✅ current |
| [WHEELS-ROOT-CAUSE-AUDIT-2026-07-10.md](WHEELS-ROOT-CAUSE-AUDIT-2026-07-10.md) | Audit of "wheels do not spin": the cause is NOT proven, a list of unclosed differences factory↔our fw | ✅ current |
| [drive-chain.md](drive-chain.md) | Trace of the chip1 drive over SPI | ⚠️ ✔C valid, ✔D (A4963 datasheet) annulled by the FU6832 discovery |
| [subsystem-findings.md](subsystem-findings.md) | 6 subsystem analyses of chip1/chip2 | ⚠️ the base is correct, "A4963"→FU6832 |
| [flash-procedure.md](flash-procedure.md) | Flashing our firmware + recovery (verified on the bench) | ✅ current |
| [hw-session-runbook.md](hw-session-runbook.md) | Runbook for a hardware session / rollback | ✅ current |
| [rc-payload-design.md](../archive/2026-07-13/rc-payload-design.md) | — | ⤴ in the archive (the injection approach abandoned) |

## In the archive (`../archive/2026-07-13/`) — historical, NOT a guide to action

`SESSION-HANDOFF.md`, `WHEELS-STATE-2026-07-08.md`, `bench-test-plan.md`,
`BENCH-TEST-PLAN-2026-07-09.md`, `MEASUREMENT-PLAN-factory-drive.md`,
`own-fw-flash-plan.md`, `fw-review-2026-07-06.md`, `vs-remote-audit.md`,
`rc-payload-design.md`.

Superseded by the current plan. Many operate on the A4963 datasheet as a source of semantics —
that is disproven (the drive = FU6832N). The open technical items on the wheels are moved
to `WHEELS-ROOT-CAUSE-AUDIT-2026-07-10.md` and `PLAN-2026-07-13-*`.
