# reverse-v2/reports — индекс актуальных отчётов

Только живые документы. Устаревшее/замещённое вынесено в `reverse-v2/archive/2026-07-13/`
(ничего не удалено — история в git). Дата в скобках = актуальность содержания.

## Актуальные (читать эти)

| Файл | О чём | Статус |
|---|---|---|
| [PLAN-2026-07-13-coils-wheels-fu6832.md](PLAN-2026-07-13-coils-wheels-fu6832.md) | **Главный текущий план.** Три трека: косить (chip2+катушки), замеры FU6832, реверс включения колёс | ✅ актуально |
| [SWD-CAPTURE-PACKAGE-2026-07-13.md](SWD-CAPTURE-PACKAGE-2026-07-13.md) | Оффлайн-пакет для ОДНОГО SWD-захода к заводу: адреса → что читать → зачем | ✅ актуально |
| [CHIP2-BORDER-SENSITIVITY-2026-07-13.md](CHIP2-BORDER-SENSITIVITY-2026-07-13.md) | chip2: как перенастроить детекцию границы под новые катушки (пороги, base voltage, CRC) | ✅ актуально |
| [WHEELS-ROOT-CAUSE-AUDIT-2026-07-10.md](WHEELS-ROOT-CAUSE-AUDIT-2026-07-10.md) | Аудит «колёса не крутятся»: причина НЕ доказана, список незакрытых различий завод↔наша fw | ✅ актуально |
| [drive-chain.md](drive-chain.md) | Трасса привода chip1 по SPI | ⚠️ ✔C валидно, ✔D (даташит A4963) аннулировано открытием FU6832 |
| [subsystem-findings.md](subsystem-findings.md) | 6 подсистемных разборов chip1/chip2 | ⚠️ база верна, «A4963»→FU6832 |
| [flash-procedure.md](flash-procedure.md) | Заливка своей прошивки + восстановление (проверено на стенде) | ✅ актуально |
| [hw-session-runbook.md](hw-session-runbook.md) | Runbook захода к железу / откат | ✅ актуально |
| [rc-payload-design.md](../archive/2026-07-13/rc-payload-design.md) | — | ⤴ в архиве (инъекционный подход заброшен) |

## В архиве (`../archive/2026-07-13/`) — историческое, НЕ руководство к действию

`SESSION-HANDOFF.md`, `WHEELS-STATE-2026-07-08.md`, `bench-test-plan.md`,
`BENCH-TEST-PLAN-2026-07-09.md`, `MEASUREMENT-PLAN-factory-drive.md`,
`own-fw-flash-plan.md`, `fw-review-2026-07-06.md`, `vs-remote-audit.md`,
`rc-payload-design.md`.

Замещены текущим планом. Многие оперируют даташитом A4963 как источником семантики —
это опровергнуто (привод = FU6832N). Открытые технические пункты по колёсам перенесены
в `WHEELS-ROOT-CAUSE-AUDIT-2026-07-10.md` и `PLAN-2026-07-13-*`.
