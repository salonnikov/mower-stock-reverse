# reverse-v2 — чистая переразборка прошивок косилки MI 302 (2026-07-02)

Полная **повторная** декомпиляция с нуля, без доверия к первой итерации (`docs/fw/`, `docs/fw-chip2/`, `docs/live-findings.md`).
Инструмент: **Ghidra 12.1.2 native-декомпилятор** (не r2 pdg) в Docker + карта периферии из SVD GD32F30x_CL.

## Что здесь
- `chip1/` — GD32F305 (главная плата, 1 МБ). **1931 функция, 100% декомпилировано.**
- `chip2/` — GD32F30x (плата коилов, 256 КБ). **435 функций, 100% декомпилировано.**
- Каждая папка: `decompiled_all.c` (весь псевдо-C), `functions_index.csv`, `symbols.txt`, `strings.txt`, `memory_map.txt`.
- `ghidra-scripts/` — headless-скрипты (Prep.java: SVD-карта+SRAM+посев векторов; ExportDecomp.java: экспорт).
- `GD32F30x_CL.svd` — карта периферии (62 блока, 1026 именованных регистров применены).

## Как воспроизвести (Docker, без рантаймов на хосте)
```
docker run --rm -v <ws>:/work -e MAXMEM=6G \
  --entrypoint /ghidra/support/analyzeHeadless blacktop/ghidra:latest \
  /work/proj chip1 -import /work/in/chip1.bin \
  -processor "ARM:LE:32:Cortex" -loader BinaryLoader -loader-baseAddr 0x08000000 \
  -scriptPath /work/scripts \
  -preScript Prep.java /work/svd/GD32F30x_CL.svd \
  -postScript ExportDecomp.java /work/out-chip1 -deleteProject
```

## Подтверждено этой итерацией (✔ = из свежего кода/строк)
- ✔ Оба дампа — валидная **незашифрованная** прошивка Cortex-M4, таблица векторов @0x08000000.
  chip1: SP=0x20016ed8, Reset=0x08011a3c. chip2: SP=0x20000988, Reset=0x080001b4.
- ✔ chip1 — многообразная система: **загрузчик + IAP/OTA** нескольких образов (тэги ENV `BB_*`, `DB_*`, `LB_*` = отдельные MCU-образы: mainboard / display / led), EasyFlash ENV, FreeRTOS, cJSON.
- ✔ Периферия по SVD: ADC0 @0x40012400, TIMER0 @0x40012c00, TIMER7 @0x40013400, USART0 @0x40013800, CRC @0x40023000 — все как volatile MMIO.

## ⚠️ Расхождения с первой итерацией (перепроверяется, Phase 3/4)
- ⚠️ **CRC приложения — НЕ аппаратный.** Аппаратный CRC-блок (0x40023000) в коде **не используется** ни разу. Первая итерация утверждала «HW CRC-32 @0x40023000 poly 0x04C11DB7». → CRC программный/табличный, пересчёт при патче будет другим.
- ⚠️ **CRC сверяется в пути OTA-обновления**, а не (пока не доказано) на каждом бутоне. Три функции-гейта `FUN_08006658/08006ac0/…` сравнивают посчитанный CRC свежезалитого блока с ожидаемым из ENV (`BB_CRC`/`DB_CRC`/`LB_CRC`) и печатают `CRC check error: app CRC / get CRC`. → надо доказать, есть ли boot-time проверка перед прыжком в app.
- ⚠️ **База приложения ≠ подтверждена как 0x08018000** — прямых ссылок нет. Механизм перехода загрузчик→app ещё не локализован.
- Прежние «CCR TIMER2 = руль колёс», адреса моторов/одометрии/state — **не подтверждены заново**, ждут Phase 3 (сверка с живым чипом).

## Дальше
Phase 3: сверка каждого старого утверждения с этим кодом (✔/ⓗ/❌).
Phase 4: точный механизм CRC/boot → процедура патч→пересчёт→заливка + откат.
Phase 5: ручное/автономное управление + тулзы и инструкции прошивки.
