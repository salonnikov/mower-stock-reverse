# archive/ — старое, опровергнутое, замещённое

Сюда убрано всё, что мешало и/или уже неверно. Ничего не удалено — только перемещено (git хранит историю). Реорганизация 2026-07-06.

## Что где

| Папка | Что | Почему в архиве |
|---|---|---|
| `injection-approach/` | rc-payload, mower-executor, их прошивальщики | Подход «инъекция в заводскую FSM» заброшен — FSM не обойти хуками (разбирается в памяти проекта). Замещён своей прошивкой `firmware/mower-own/`. |
| `old-decompiles/docs-fw/`, `docs-fw-chip2/`, `r2out/` | Ранние Ghidra/radare2 декомпилы chip1/chip2 | Замещены чистым каноническим декомпилом `reverse-v2/chip1|chip2/`. Декомпилили ~5 раз — это старые проходы. |
| `old-docs/` | live-findings.md, firmware-gd32-*-reverse.md, phase1-capture.md | Доки первой итерации с ошибками (память прямо помечает «НЕ доверять»). Актуальное — в `reverse-v2/`. |
| `old-firmware/` | mower-auto, mower-link, sniffer-rtos, uart-sniffer-esp32, usb-sniffer, recon | Разведочные прошивки (снифферы UART/USB, авто-эксперименты). Не часть текущего пути. |
| `old-flashers/` | flash_revert_page1.cfg, flash_revert_full.cfg, flash_bank1_*.cfg, flash_a3_test.cfg, flash_proof.cfg, a3_run.sh | Сломанные (форма `[expr]` заморозки watchdog падает на openocd 0.10) или PoC-эксперименты де-риска bank1. Рабочие прошивальщики — см. `dist/FLASHERS.md`. |
| `old-fw-binaries/` | proof_fw.bin, rc_fw.bin, mower-auto-v1.bin, sniffer-*.bin | Собранные образы заброшенных веток. |

## Что осталось каноническим (НЕ в архиве)
- Своя прошивка: `firmware/mower-own/` · будущая работа по коилам: `firmware/coil-scope/` · ESP-дисплей: `firmware/esphome/`
- Реверс: `reverse-v2/` (декомпил + отчёты + `reference/` + `analysis/`)
- Дампы/образы: `dist/*.bin`, `dist/factory-full.asm` — см. `dist/FLASHERS.md`
- Прошивальщики: `dist/flash_own.cfg`, `dist/flash_revert_own.cfg`, генераторы `tools/bench/gen_flash_own.py`/`pack_own.py`/`app_crc.py`, рекавери `tools/bench/recovery.cfg`

## Дополнение 2026-07-13 (вторая волна чистки)

| Папка/файл | Что | Почему в архиве |
|---|---|---|
| `old-docs/planning-esp32/` | `docs/00..07-*.md`, `backlog.md` | Планировочные доки подхода «ESP32-пересадка мозга». Подход заброшен → прямой контроль штатного GD32 своей прошивкой. |
| `old-firmware/esp32-brain/` | `firmware/src/main.cpp`, `platformio.ini`, `README.md` | Каркас ESP32-S3-«мозга» (Ardumower-порт). Тот же заброшенный подход. Активная прошивка — `firmware/mower-own/`. |
| `mower-repo-snapshot-20260701.tar.gz` | Снапшот всего репо от 01.07 (79 МБ) | Полная копия — история и так в git. Держим как офлайн-бэкап, из корня убран. |
| `../reverse-v2/archive/2026-07-13/` | 9 отчётов (SESSION-HANDOFF, WHEELS-STATE-07-08, bench-планы, MEASUREMENT-PLAN, own-fw-flash-plan, fw-review, vs-remote-audit, rc-payload-design) | Замещены текущим планом `PLAN-2026-07-13-*`. Многие опирались на даташит A4963 как семантику — опровергнуто (привод = FU6832N). |

Актуальная точка входа проекта — корневой `README.md` + `reverse-v2/reports/README.md`.
