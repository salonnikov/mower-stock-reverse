# ПЛАН ЗАЛИВКИ своей прошивки (own_fw.bin) — v2 после ревью Fable5

Составлено 2026-07-04, обновлено после адверсариального ревью. Заливать ТОЛЬКО по явному «go»,
физическое — по подтверждению. Косилка засыпает — заход быстрый. Регистр ≠ физика.

## КЛЮЧЕВОЕ РЕШЕНИЕ (ревью): ШЬЁМ ТОЛЬКО PAGE 1 (0x08018000)
Наш app (<2КБ) целиком в ОДНОЙ 2КБ-странице `0x08018000` (последний байт 0x0801871a). Наш код
НЕ проверяет app-CRC `0x080ffffc`, бутлоадер app-регион не верифицирует → **достаточно заменить
только page 1** (наш vector table + код). Заводской app (pages 2..208 + bank1) НЕ трогаем — он
дремлет (наши векторы к нему не ведут). Итог: brick-окно **~6с вместо ~20с**, и **откат тривиален**
(восстановить 1 страницу), никакого рискованного стирания 900КБ / openocd-flash-драйвера / bank1.

## ЧТО ЗАЛИВАЕМ / ЧЕМ ОТКАТ
- Заливка: `flash_own.cfg` — erase page1 + program наш app (910 полуслов) + verify + reset run.
- Откат: `flash_revert_page1.cfg` — erase page1 + program ЗАВОДСКУЮ page1 (из дампа) + reset run
  → векторы завода (SP 0x20017ff8, Reset 0x08018441), заводской app снова грузится (его pages 2..208
  и bank1 мы не трогали, self-CRC снова сходится).
- Оба — проверенный manual-FMC bank0, ОДНА страница. Генератор `tools/bench/gen_flash_own.py`.

## БЕЗОПАСНОСТЬ, вшитая в cfg (правки ревью)
1. **wdg-freeze с ASSERT В СЕССИИ:** halt → DBGMCU=0x300 → `mdw`-проверка: если не 0x300 → `shutdown`
   ДО стирания (не начнём заливку с незамороженным wdg). Плюс форма `mem2array+expr` (сработала в flash_proof).
2. **BUSY-poll STAT** после стирания (не фиксированный sleep) — не продолжим, пока FMC занят; таймаут → abort.
3. **Program** = проверенный «PG-раз + mwh + sleep6» (одна страница, все полуслова в ней).
4. **READ-BACK VERIFY** всех 512 слов page1 перед `reset run`: mismatch → `shutdown` БЕЗ reset (page1
   частично записан = ОБРАТИМО через revert; факт: сбой ловится до точки невозврата).
5. Бутлоадер НЕ трогается (grep-подтверждено: все адреса ≥0x08018000, в пределах page1).

## ПРАВКИ ПРОШИВКИ (ревью H1/H2/H3/M2/M3/M4 — вносит разраб)
- **H2:** main() на старте (после PB0-латча) ОБНУЛЯЕТ mailbox (magic/seq/скорости/blade) → протухший
  SRAM после reset не крутанёт моторы; привод только после явного arm+bump seq хостом.
- **H3:** нож НЕ в RUN с init (конфиг шлём, RUN — только по команде blade_on) → нож не крутанёт от мусора.
  + ФИЗИЧЕСКИ ОТКЛЮЧИТЬ нож на всё время bring-up.
- **M3:** startup гасит прерывания (disable_irq, SysTick off, NVIC clear) до конфигурации.
- **M2/M4:** таймауты во всех busy-wait (clock STB, SPI TBE/RBNE) — зависший init не выглядит бриком.
- **H1 (IWDG):** наш main IWDG не кормит. После `reset run` IWDG (если активен) сбросит → reset-loop.
  ⇒ **после заливки ОБЯЗАТЕЛЬНО физический power-cycle** (POR останавливает IWDG). Проверить, кормить ли
  IWDG в цикле, если он активен при старте нашего app.

## PRE-FLIGHT (ДО заливки, НЕ трогая флеш)
- **A. wdg-freeze:** poll-telnet → halt → `mem2array cr 32 0xE0042004 1; mww 0xE0042004 [expr {$cr(0)|0x300}]`
  → `mdw 0xE0042004` == 0x00000300; держать 5с halted → косилка НЕ ребутнулась = wdg реально заморожен.
  Если не встаёт → СТОП (иначе даже page1-заливка рискует, хоть окно и мало).
- Revert-драйвер тестировать НЕ надо (откат = manual-FMC page1, проверенный метод).

## ПРОЦЕДУРА (по «go», косилка в idle, на питании; всё уже на /tmp малины)
1. `sudo systemctl stop mower-openocd`.
2. `sudo openocd -f /tmp/flash_own.cfg` detached с логом. Мониторить: «VERIFY OK» + дошло до shutdown, без «ABORT».
   Если «ABORT: verify failed» — page1 частичен, НЕ грузить: сразу `flash_revert_page1.cfg` (вернуть завод).
3. `sudo systemctl start mower-openocd`.
4. **Физический POWER-CYCLE косилки** (обязательно — POR останавливает IWDG, чистый старт нашего app).
5. Проверка: `own_test.py hb` → heartbeat РАСТЁТ = наша прошивка исполняется.
6. ТЕСТ ПРИВОДА (колёса ВЫВЕШЕНЫ, нож ФИЗИЧЕСКИ ОТКЛЮЧЁН): `own_test.py fwd 0x400` + держать seq
   (`own_test.py hold` в фоне) → смотреть колёса ГЛАЗАМИ. Стоп: `own_test.py stop`. Нож — отдельно, осознанно.

## АБОРТ / ОТКАТ
- «ABORT» в логе flash_own (wdg/erase/verify) → page1 не финализирован → `flash_revert_page1.cfg`.
- Косилка не грузится / heartbeat не растёт → `flash_revert_page1.cfg` → power-cycle → завод.
- Чип не виден (брик) → catch-and-hold (recover_run.sh стиль) + в окне питания залить revert.
- Дамп-эталон на малине: `/tmp/gd32-mainboard-dump-v1.bin` (sha 82b8bb5a...).

## ФАЙЛЫ (на /tmp малины + в гите)
- `own_fw.bin` (образ), `flash_own.cfg` (заливка page1), `flash_revert_page1.cfg` (откат page1),
  `gd32-mainboard-dump-v1.bin` (эталон/откат), `~/mower-bench/own_test.py` (управление).
- Генератор `tools/bench/gen_flash_own.py`, упаковка `pack_own.py`, `app_crc.py`.
</content>
