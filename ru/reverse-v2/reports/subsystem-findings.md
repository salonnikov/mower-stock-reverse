# Подсистемные находки (reverse-v2, 2026-07-03) — 6 параллельных разборов chip1/chip2

Источник: только свежая декомпиляция `reverse-v2/chip1|chip2`. ✔ подтверждено кодом/строкой, ⓗ гипотеза. Старым docs/ не доверяем.

---

## 1. BOOT / BOOTLOADER / FLASH / CRC / OTA (chip1)

**Раскладка флеша:**
- Bootloader `0x08000000–0x08017FFF` (96 КБ): vector @0x08000000 (SP=0x20016ed8, Reset=0x08011a3c), POST, USB-updater, EasyFlash ENV, суб-плата-программаторы, HW-CRC, boot.c FSM, jump-to-app. ✔
- Ожидаемый CRC загрузчика — слово `0x08017FFC`. ⓗ (граница совпадает)
- **MB Application `0x08018000`+** (макс 0xE8000): 2-я vector-таблица @0x08018000, reset=*(0x08018004)=FUN_08018440. ✔ (SetFirmInfo case1 base 0x18000)
- Суб-образы во ВНЕШНЕЙ SPI-флешке: BB@0x100000, DB@0x130000, LB@0x430000 (SetFirmInfo `FUN_0800214c`). ✔

**Jump-to-app = `FUN_080012d0`:** грузит MSP из *(0x08018000), прыгает на *(0x08018004). Проверяет только: (SP & mask)==0x20000000 и base<reset. **VTOR не пишется загрузчиком.** ✔

**CRC = аппаратный GD32 @0x40023000** (poly 0x04C11DB7, init 0xFFFFFFFF, 32-бит слова big-endian, без реверса/финального XOR):
- compute `FUN_08008e1c` (кормит CRC_DATA), reset `FUN_08008e38` (CRC_CTL bit0). ✔
- **(a) Boot-гейт — ТОЛЬКО загрузчик:** в Reset `FUN_08011a3c`: CRC над 0x08000000..0x08017FF8 (0x5fff слов), сверка с *(0x08017FFC); mismatch → `FUN_08011946` вечный halt. **Приложение при старте НЕ CRC-ится.** ✔
- **(b) App launch — БЕЗ CRC.** ✔
- **(c) Три «CRC check error» = OTA-проверки СУБ-ПЛАТ** (BB/DB/LB, `FUN_08006658/08006ac0/08006d3c`): expected из ENV *_CRC, get = CRC посчитанный удалённой платой. Только при OTA. ✔
- **(d) MB self-OTA `FUN_08006fe8`:** единственное место, где чип CRC-ит своё приложение — при обновлении (mb_sv<MB_BVER) сверяет с ENV MB_CRC. Не boot-гейт. ✔

**ENV = armink EasyFlash**, ключи MB_*/BB_*/DB_*/LB_*/BTL_* (*_SIZE/*_CRC/*_BVER/*_sv/*_BRF). get `FUN_0800aa84`, set `FUN_0800ae40`. ✔

**🎯 ВЫВОД:** патч ПРИЛОЖЕНИЯ (0x08018000+) + заливка по SWD → грузится БЕЗ пересчёта CRC. Условия: валидный SP@0x08018000 и reset@0x08018004, и не висит pending OTA (mb_sv≥MB_BVER, иначе перезальёт из внешней флешки). Патч ЗАГРУЗЧИКА → надо пересчитать HW-CRC над 0x08000000..0x08017FF8 → записать в 0x08017FFC.

---

## 2. ПРИВОД КОЛЁС + ОДОМЕТРИЯ (chip1)

**Колёса = BLDC на внешнем sensorless-драйвере Allegro A4963** (3 шт: лев.колесо/прав.колесо/нож, общий `motor_spi_mutex`). Файлы: `a4963_snk_v2.c`, `driver_left/right/blade_motor_snk_v2.c`. ✔
- **Скорость → PWM** в регистр таймера CHxCV: `FUN_0807f696(base,ch,val)` пишет 0x34/0x38/0x3c/0x40 (=CH0..3CV). Левое колесо = **CH3**, база в литерале DAT_0805b144 (номер таймера не выгружен — нужны сырые байты). ✔/ⓗ
- **Направление/тормоз/конфиг → SPI A4963** с verify+retry: `FUN_0808408c(spi,word)` (word: addr<<13|data). Init льёт 8 конфиг-регистров. ✔
- **brake-to-dir `FUN_0801f4dc`:** переключает биты режима RUN/BRAKE/COAST в теневом рег. A4963 и льёт по SPI. set_direction `FUN_0801f4c6`, set_left_dir `FUN_0805f300`. ✔
- **Крутить только PWM'ом нельзя** — пока A4963 не сконфигурен по SPI и не в RUN, PWM игнорируется (объясняет прошлое «CCR не крутит»). ✔
- **set_motor_speed `FUN_0805f620`** ✔ (форс-дизасм, 1114 байт) — «коробка передач»: `pwm = speed*0x1c20 / gear_max`, gear_max ∈ {`0x17fd`=6141, `0xbfd`=3069, `0x2ce`=718}, передача 0/1/2 в `*(ctx+0x10)`, при смене — `FUN_0801f4dc` brake-to-dir. Финал: `FUN_0805f300(dir)` + **`FUN_0807f696(_DAT_0805fac4, 3, pwm)`** — запись PWM в **CH3** таймера левого колеса, шэдоу в `*(*ctx+2)`. Насыщение до `0x1c20`=7200. Файл: `reverse-v2/chip1/forced_functions.c`.

**Одометрия — НЕ фикс-адреса.** Старые `0x2000be28/be58` в бинаре ОТСУТСТВУЮТ (структуры колёс на malloc). Реальный механизм: тахо/FG-сигнал A4963 → **input-capture таймера**; ISR-колбэки `FUN_0805fbc0`/`FUN_0805fc7c` инкрементят тики по направлению (+0x18 fwd/+0x1c back) и считают скорость по периоду (capture чит `FUN_0807f402`). Регистрация колбэков `FUN_08062c64/8c`. ✔

**Drive(speed,angle) = `FUN_0804bb0c`** через vtable движ-базы (reset/set_speed/set_target/start). Угол→тики `FUN_08020520`: `angle*697/K` (K пер-модельная). ✔

---

## 3. НОЖ / IMU / СЕНСОРЫ (chip1)

**Нож:** сервис `FUN_08022938` (service_blade.c). Драйвер `driver_blade_motor_snk_v2` = тот же A4963-путь (`FUN_0808408c`): команда 16-бит, enable=бит 0x1000, dir=бит 0x2, speed=биты 0x0FFF. НЕТ отдельного GPIO/таймера ножа — всё по serial. Скорость из cfg blade_sp/blade_max_sp. Защиты: stall→reverse, highload→смена скорости, overslope/lift/torque→brake&escape, blade_check_en. ✔

**IMU = Bosch BNO055** по I²C (dev 0x50=7-bit 0x28), chip-id `A0 FB 32 0F` @рег0x00. `driver_mems_snk_v13.c`. Углы: рег 0x1A, 6 байт (EUL heading/roll/pitch), /16 → **целые градусы**. Диапазоны heading 0..360/roll±90/pitch±180. Объект углов: heading+0x18/roll+0x1a/pitch+0x1c (+ avg +0x1e/+0x20/+0x22). Slope-детект использует **pitch+roll** (не heading). Альтернатива ICM-42688 (TDK) тоже вкомпилена (WHOAMI 0x47, вероятно SPI). ✔
- I2C0(0x40005400) vs I2C1(0x40005800) — не разрешено. ⓗ

**Батарея = умный пак по serial с CRC-фреймом** (driver_battery_snk_v1/v2, service_bms). Пер-cell напряжения, химия (sony/samsung/eve), telemetry `battery vol=%dmV, percent=%d, min/max, temp=%d`. SoC по OCV. Пороги из cfg voltage points. НЕ ADC. ✔

**Дождь** = service_rain (rain_en/rain_delay, digital вход через port-service). **Lift** = service_lift (lift_en, дебаунс-счётчик). **Updown/переворот** = вычисляется из pitch/roll IMU. **Температура** = из пака + die-темп IMU (отдельного NTC ADC нет). ✔/ⓗ (GPIO-пины rain/lift за абстракцией)

---

## 4. КАТУШКИ / ГРАНИЦА (chip2 + приём chip1)

**✅ ИСПРАВЛЕНО (пост-ревизия): дамп chip2 ПОЛНЫЙ, ADC-тракт катушек В НЁМ ЕСТЬ. Ре-дамп НЕ нужен.** Первичный агент дал ложноотрицательный вывод (искал по SVD-метке `ADC0`, а код идёт через указатель `_DAT_08019cf0`=ADC0, `_DAT_08019cf4`=ADC1; в сырых байтах базы ADC0/ADC1/DMA0 присутствуют).
- **Оцифровка `FUN_08019bf4` (dual-ADC, вызывается):** две катушки = **ADC0/канал5 + ADC1/канал9**, sample-time 7, dual (0x20000), DMA, буфер 800 отсч. ✔ (`FUN_08012a04` проверено = конфиг RSQ/SAMPT; объект RAM 0x20000078, 7-vtable в дампе). Какая ЛЕВО/ПРАВО — из ADC не следует, по разводке/send_border (ⓗ).
- **Baseline `FUN_0801a1f8`:** сумма 8 отсчётов /8 → `*DAT_0801a250`; валид [1906..2191]. ✔
- Приложение chip2 берёт волну через vtable провайдера `FUN_08016ba8` (на сторону: type-код 0/1/2/3 + магнитуда). DSP-обработка/классификация: `process_deal_board.c` (`FUN_080164bc/08016784`), очередь волн, отсев «disturb». Выход: `area`(0/1/2/3) + `str`. ✔

**Линк chip2→chip1:**
- `send_border_message = FUN_08019914` (chip2): cJSON {area/str L+R + lift-флаг}. TX `FUN_08019a18`: ≤128 байт, mutex+timeout, DMA. USART2 (0x40004800). ✔
- **Кадр `&`(0x26)+тело+CRC8+`#`(0x23)**, тело **XOR 0x5B** (`FUN_08013ae4`), CRC8/MAXIM poly 0x8C (`FUN_08013008`). НЕ голый JSON. ✔
- **Линк ДВУНАПРАВЛЕННЫЙ** — chip2 тоже парсит входящие (start-search, set-version). ✔
- **Lift на chip2**: 2 переключателя (лев/прав), дебаунс 10, `deal_lift_sensor FUN_08013970`. ✔

**Приём на chip1:** deal_message `FUN_0801fb80` (build), CRC8 `FUN_080241d0`, cJSON `FUN_08023f80`. bdport-колбэк `FUN_08021bb4` (version/reset/log/find-border). Модель границы (service_border.c, `FUN_0804d4c8`): L str+0x20/area+0x24, R str+0x28/area+0x2c. **area∈{0,1,2}=снаружи/внутри/на-линии, str=магнитуда** (=ls/rs). ✔
**Near-station** — композит (корректность border-сигнала + напряжение + дебаунс), не простой порог. Пересечение in→out → манёвры «cycle left/right» навигации. ✔

---

## 5. ДИСПЕТЧЕР КОМАНД / ПРОТОКОЛ (chip1)

**RX-пайплайн:** UART → де-XOR 0x5B `FUN_0803953c` → дефреймер `FUN_0806e938` (ищет `&`…`#`, CRC8) → cJSON `FUN_08023fe6` → диспетчер `FUN_080470f0`. TX: build `FUN_0801fb80` + XOR `FUN_08039e8c`. ✔
**CRC8** = Dallas/Maxim `FUN_080241d0` (poly 0x8C reflected, init 0, битовый). ✔
**Диспетчеры:** dpport `FUN_080470f0` (16 слотов, +0x60 cat/+0x64 handler), bdport `FUN_08063880` (5), ledport `FUN_0806396c` (5). Категория = старший байт cmd `&0xff000000`. ✔
**Таблица {cat→handler} = статическая const в .rodata** — кодом не заполняется, указатели хендлеров из псевдо-C не вытащить (нужны сырые .rodata). Активная категория **0x30 → `FUN_08044c80`**. ✔

**Команды 0x30xxxxxx** (FUN_08044c80): 06/07=manual-set вкл/выкл (только флаг!), 09=login pwd, 10=set pwd, 11-16=RTC/расписание, 17=rain, 18=multizone, 1a=name, 1b/1c=SN, 20=**OTA**, 21=wifi, 22=ble, 23=reset pwd; 82-8d=config-сеттеры (schedule/rain/led/ultrasonic/map); **0x300100de=CONTROL-байт** (start/stop/dock/charge → `FUN_08050448`+0x14), **0x300101dc=restart** (байт 0x20). ✔

**🎯 ПРЯМОЙ КОМАНДЫ ДВИЖЕНИЯ/JOG В ПРОТОКОЛЕ НЕТ.** Моторы двигаются только через control-байт → автономные сценарии. Примитивы движения `FUN_08020164` (turn/dir), `FUN_0801fb04`/`FUN_0801fe8c` (fwd/back) пишут в **очередь мотор-команд +0xe4**, но вызываются только автономной FSM через vtable, из протокола недостижимы. → RC: (A) свой хендлер в свободный слот диспетчера (напр. cat 0x31) дёргает примитивы; (B) по SWD писать в очередь +0xe4 / вызывать примитив. ✔

---

## 6. МАШИНА СОСТОЯНИЙ / ЗАДАЧИ / КОНФИГ (chip1)

**state = `0x200000bc`** (база 0x200000ac + 0x10). Сеттер `FUN_0807976c` (кламп **0..10**), геттер `FUN_0804edd4`. **Прямых записей мимо сеттера НЕ найдено** (все через 22 вызова + обёртки-переходы). ✔

| val | Состояние | Модуль | Доказательство |
|----|----|----|----|
| 3 | **IDLE/standby** | process_waiting | "change to idle", "Robot manual stopped" |
| 4 | **DEPARTURE/выезд** | process_departure_smooth | "start work", "goto departure" |
| 5 | **CUTTING/кошение** | process_cutting | "in border, change to cutting" |
| 6 | **FIND BORDER** | process_find_bd | "not on border, change to find border" |
| 7 | **DOCKING** | process_docking_smooth | "on border, change to dock" |
| 8 | **CHARGING** | process_charging | "in station, change to charging" |
| 9 | **ERROR** | process_error | аборт FUN_08083d9c→set(9) |
| 10 | **POWER OFF** | process_power_off | "Robot manual power off" |

(0/1/2 не используются. Обёртки-ошибки берут маску кода ошибки, состояние ставят 9/10.) ✔

**FreeRTOS-задачи** (wrapper `FUN_080849c0`, 11 задач): `initial` (поднимает сервисы, самоудаляется), **`highspeed loop`** (быстрый цикл — здесь тикает МАШИНА СОСТОЯНИЙ process_*_run), `time` (RTC/расписание), `bms`, `bdport` (граница), `map`, `dp send`/`dp receive` (RX питает диспетчер JSON)/`send_log`, **`imu fuse`** (сенсор-фьюжн наклон/курс), `IDLE`. `process manager`/`mems`/`ledport` — сервисы внутри highspeed loop/imu fuse, не отдельные задачи. ✔

**Конфиг:** парсер `FUN_08076b2c` (~110 ключей), вход по control-байту 0xa5 из "cfgstr". Геометрия: perimeter→+0x64, ratio→+0x74(double), wheel_dis→+0x66, blade_diameter→+0x68, bmotor_polar/cmotor_polar→+0x60/+0x61, blade_sp→+0x6c. Флаги *_en: pwd/rain/fslip/ult/led/gps/sch/zone/com/hit/lift/mems/border/mtrack/wlch/auto_off/blade_check/speaker/smooth/rtt/qdock/area_limit/map_update/rb_en_mag/lboard. ✔

---

## Сводка исправлений первой итерации
- ❌→✔ «CRC не аппаратный» (моя ранняя ошибка) → **аппаратный** GD32 CRC, poly 0x04C11DB7.
- ✔ Boot-CRC гейтит **только загрузчик**, не приложение (первая итерация: «гейт над app, обход 1 байт» — неверно; приложению обход НЕ нужен).
- ✔ Колёса = **A4963 BLDC** (PWM+SPI), не прямой CCR-руль.
- ✔ Одометрия — **malloc**, адреса 0x2000be28/be58 в бинаре отсутствуют (input-capture тахо, не EXTI/encoder).
- ✔ Линк коилов — **кадр+CRC8+XOR 0x5B, двунаправленный** (не «голый JSON односторонний»).
- ✔ Состояния переопределены: 3=idle,4=departure,5=cutting,6=findbd,7=dock,8=charge,9=error,10=poweroff.
- ✅ Дамп chip2 ПОЛНЫЙ — ADC-тракт есть (ADC0/ch5 + ADC1/ch9, `FUN_08019bf4`, объект RAM 0x20000078), ре-дамп НЕ нужен (был ложноотрицательный вывод re-run агента). L/R-привязка каналов — ⓗ.
- ✅ set_wheel_speed FUN_0805f620 — форс-дизасм закрыл дыру (gear-map, PWM→CH3).
</content>
