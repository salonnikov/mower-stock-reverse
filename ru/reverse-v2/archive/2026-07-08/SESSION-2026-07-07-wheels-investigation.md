# Сессия 2026-07-06/07 — расследование «колёса не едут» (полный контекст)

Чтобы НЕ потерять контекст. Читать вместе с:
- `reverse-v2/reference/FACTORY-GPIO-PERIPHERAL-MAP.md` (GPIO/периферия/алгоритм — живой замер завода)
- память: `a4963-run-default-por`, `bms-usart2-spec`, `SESSION-HANDOFF-2026-07-06`, `own-fw-flashed-drive-isolated`

## TL;DR (где стоим)
Своя прошивка **mower-own** залита, бежит, читает батарею, heartbeat. **НОЖ КРУТИТСЯ** нашей прошивкой. **КОЛЁСА (лев+прав) — мертвы.** За сессию исключён гигантский объём; осталась узкая цель: включить **коммутируемый драйв-рейл 20U** колёс, который под нашей fw выключен, а enable его — НЕ firmware-GPIO и не periph-enable. Топ-лид: конфиг **TIMER0**; альтернатива — железо.

## Что ТВЁРДО установлено (живьём)
1. **A4963 auto-run с EEPROM (RUN=1).** Тест-сборка без SPI-конфига вообще → **нож всё равно крутится.** Завод в force-capture крутил и колёса без создания драйверов. ⇒ для езды нужно ТОЛЬКО питание+ШИМ; SPI-арм/конфиг не нужен. verify=0xFFFF (даже у живого ножа) = наш SPI-read не работает, но это НЕважно.
2. **Нож работает нашей прошивкой** ⇒ VBB (ножевого рейла) есть, PWM (TIMER2 CH1), CS (PD3), enable (PB12) — всё рабочее.
3. **Колёса мертвы даже когда наша fw ТОЧНО совпадает с заводским state-5:** PD7=0, PD4=0 (run-lines LOW), ch2/ch3=0x1000 (PWM). ⇒ дело НЕ в GPIO.
4. **Периф-дифф (наша-на-ходу vs завод):** RCU почти идентичен; лишние у завода = TIMER3/TIMER4/I2C1 (тахо/IMU, не питание). ⇒ дело НЕ в periph-enable.
5. Мотор-PWM = **TIMER2** (CH1 нож, CH2 прав, CH3 лев) — подтверждено force-capture (ch2/ch3 0x681→0x1125 = колёса крутились у завода).
6. Мотор-рейл ≈ **20U** (из 20U_BATT через ключ Q10/Q17/Q14) — по фото платы SNK_MAINBOARD_CP_U1.
7. Колёса и нож — **разные силовые пути** (нож на живом рейле, колёса на коммутируемом 20U). Подтверждено пользователем + поведением.

## Что ИСКЛЮЧЕНО (не переоткрывать)
- «Нет VBB глобально» — нож крутится.
- Пак/BMS по USART2 — реализован (линк установился, connected=1, ответ 3A A3), НО опрос пака **VBB не включает**. Байты connect: `1C A1 03 C1 01 2E` (подтверждены живьём @0x200005e0). Спека: `bms-usart2-spec`.
- SPI-арм/конфиг колёс — A4963 auto-run, SPI не нужен.
- Все GPIO: idle-дифф + state5-дифф + прямое совпадение исчерпаны. Кандидаты PA0/PA4/PB9/PD8=comms, PE0=dpport, PD7/PE0 тестированы, PE9/PD11 (мы зря HIGH→поправлено), PE1/PE5=I2C IMU(bootloader). PD7/PD4 run-line — реализован, не помог.
- Ранний бут / rw_init / departure-FSM: НЕТ драйв-специфичного power-GPIO (3 реверса).
- База: косилка ехала на заводской И БЕЗ базы ⇒ не база.
- Periph-enable: только тахо/IMU лишние.

## ОСТАЛОСЬ (2 варианта)
1. **TIMER0 (advanced, 0x40012C00)** — конфигурация. Включён (bootloader) у обоих, но завод может гнать им charge-pump/bootstrap/gate-drive для high-side ключа 20U; наша fw оставляет idle. ★ ТОП-ЛИД, в духе «всё включается прошивкой».
2. **Железо** — гейт ключа 20U (Q10/Q17/Q14): прозвонить затвор.

### Следующий замер (firmware-путь)
Залить заводскую → на ходу (state5) снять полный **TIMER0** (CTL0/CTL1/SMCFG/CHCTL0-2/CTL2? CAR/PSC/CH0-3CV/CCHP/DMAINTEN) и **TIMER3**; сравнить с нашими (у нас idle). Если завод конфигурит/гоняет — воспроизвести в hal, залить, проверить колёса.

## Состояние прошивки mower-own (на диске, НЕ закоммичено)
Свежие правки (5 файлов src): gd32_regs.h/hal.c/hal.h/main.c/motor.c.
- **bms.c/bms.h** — USART2 пак-линк (v2, читает ответ) — закоммичено ранее (26d7d95/012cefa).
- **hal.c:** `hal_motor_run(motor,run)` — драйвит PD7(лев)/PD4(прав) LOW=run/HIGH=coast (WHEEL_RUN_PIN_* в gd32_regs.h). hal_aux_power_on: PE7 HIGH, PE9/PD11→LOW (как завод). PD7 сконфижен выходом в hal_gpio_init.
- **motor.c:** drive_wheel зовёт hal_motor_run(run=1) при езде, (0) при стопе; motor_all_stop coast'ит; A4963_SKIP_CONFIG=0.
- **main.c:** BENCH_NO_AUTOSLEEP=1 — авто-сон отключён (чтобы не засыпала на стенде). ⚠️ вернуть 0 для поля.
Сборка -Werror чистая (text≈3808). На чипе СЕЙЧАС: эта fix-сборка (авто-сон off).

## Инфраструктура заливки (РАБОТАЕТ — важно)
- openocd 0.10 на малине давится на Tcl (`[expr]`/`mem2array`/`if`/`while`) в файловом cfg → usage-ошибка. **РЕШЕНИЕ: «плоский» cfg** — только plain mww/mwh/sleep, `poll off` после init, без reset. Генератор: `dist/gen_flash_own.py flash` → затем transform (strip Tcl, split `;`, add `poll off`) → `flash_own_flat.cfg`. Заводской откат = `flash_revert_flat.cfg` (из flash_revert_own.cfg тем же transform).
- Пайплайн: `pack_own.py` (образ own_fw.bin) → gen_flash_own → flatten → scp на малину → `openocd -f flat.cfg` standalone (СНАЧАЛА убить сервис mower-openocd + pkill openocd).
- **ВСЕГДА проверять DPIDR (не 0xdeadbeef/DAP-init-failed) + verify (dump_image 0x08018000 0x1000, сравнить с own_fw.bin) ПЕРЕД «готово».** Если target dead — косилка выключена/спит ИЛИ SWD-провод отошёл.
- Standalone `-c`-команды работают всегда; файловый Tcl — нет. Верификация/чтение — отдельным standalone openocd (init/poll off/halt/mdw|dump_image/shutdown).
- Reset run заводскую/нашу чисто НЕ поднимает → нужен физический power-cycle.
- Малина: pi@192.168.10.81, ~/mower-swd (flash cfgs), ~/mower-bench (swd.py, drive_read.py, p1_snapshot.py). Сервис mower-openocd (poll.cfg, :4444, Restart=always — перед флешем стоп+pkill).

## Грабли сессии (чтобы не повторять)
- Авто-сон нашей fw (~10 мин) выключал косилку между тестами — теперь BENCH_NO_AUTOSLEEP=1.
- «Флеш прошёл» при выключенной косилке = ЛОЖЬ (DAP init failed, ничего не записано). ВСЕГДА verify.
- Тестить оба колеса + нож, не только левое. Один чистый тест на прогон (лаг наблюдения + время раскрутки).
- Заводская сама рулит GPIO → SWD-запись пинов на живой заводской перетирается (тест «тыкать пины» невалиден). Менять — на НАШЕЙ прошивке + флеш.
