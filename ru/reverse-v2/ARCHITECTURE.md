# ARCHITECTURE.md — Единая карта робота-газонокосилки MI 302

Собрано из `reverse-v2/reports/subsystem-findings.md` (6 подсистемных разборов) + сверка по `chip1/{decompiled_all.c,symbols.txt,strings.txt}` и `chip2/`.
Пометки: **✔** — подтверждено кодом/строкой в свежей декомпиляции; **ⓗ** — гипотеза, требует проверки на живом чипе/ре-дампе.
Старым `docs/` не доверяем.

> ⚠️ **ПОПРАВКА 2026-07-13:** упоминания «Allegro A4963» читать как **Fortior FU6832N** —
> на плате физически нет A4963, «A4963-протокол» реализует прошивка FU6832 (SPI slave).
> Реверс SPI со стороны chip1 валиден; семантика из даташита A4963 — нет.
> См. `reports/PLAN-2026-07-13-coils-wheels-fu6832.md` и память `fu6832-drive-architecture`.

---

## 1. Обзор системы

Три MCU + BLDC-драйверы Allegro A4963.

> ⚠️ **ПОПРАВКА 2026-07-13:** «Allegro A4963» ниже по всему документу читать как **три Fortior FU6832N**
> (умный BLDC-контроллер 8051+FOC, эмулирует A4963-протокол по SPI). Физического A4963 на плате нет.
> SPI-реверс со стороны chip1 валиден; выводы из даташита A4963 — нет. См. `reports/PLAN-2026-07-13-coils-wheels-fu6832.md`.

```
            ┌───────────────────────┐
            │  ДИСПЛЕЙНАЯ ПЛАТА (UI) │  кнопки/LED, ledport-протокол
            └───────────┬───────────┘
                        │ UART, кадр &..#, XOR 0x5B, CRC8/MAXIM  (двунапр.)
                        ▼
   ┌──────────────────────────────────────────────────────┐
   │  chip1  GD32F305  — ГЛАВНЫЙ МОЗГ                       │
   │  FreeRTOS (11 задач), FSM 3..10, диспетчеры команд,    │
   │  привод колёс+нож (PWM+SPI A4963), одометрия (тахо),   │
   │  IMU BNO055 (I2C), BMS (serial), навигация/граница     │
   └───────┬───────────────────────────────┬───────────────┘
           │ USART2 (0x40004800)           │ SPI  (motor_spi_mutex)
           │ кадр &..#, XOR 0x5B, CRC8      │ addr<<13|data (verify+retry)
           │ двунаправленный               ▼
           ▼                        ┌──────────────────────────┐
   ┌───────────────────────┐        │  A4963 ×3: L-колесо /     │
   │ chip2 GD32F30x — КОИЛЫ │        │  R-колесо / нож (BLDC)    │
   │ приём волны границы,   │        └──────────────────────────┘
   │ DSP→area/str, lift ×2  │
   └───────────────────────┘
```

**Физические линки:**
- **chip1 ↔ chip2 (границы):** USART2 `0x40004800` (сторона chip2), DMA, mutex+timeout, ≤128 байт. Кадр: `&`(0x26) + тело + CRC8 + `#`(0x23); **тело XOR 0x5B**; CRC8 = Dallas/MAXIM poly 0x8C. **Двунаправленный** (chip2 парсит start-search/set-version). ✔
- **chip1 ↔ дисплей (UI/команды):** тот же формат кадра (XOR 0x5B, `&..#`, CRC8). RX питает диспетчер JSON. ✔
- **chip1 → A4963 ×3:** SPI (общий `motor_spi_mutex`), слово `addr<<13|data`, verify+retry. + PWM в CHxCV таймера. ✔
- **chip1 → внешняя SPI-флешка:** суб-образы прошивок BB/DB/LB (OTA-хранилище). ✔

---

## 2. Карта chip1 (GD32F305) — главный мозг

### 2.1 Раскладка флеша

| Регион | Адрес | Содержимое |
|---|---|---|
| Bootloader | `0x08000000–0x08017FFF` (96К) | vector@0x08000000 (SP=0x20016ed8, Reset=**FUN_08011a3c** @0x08011a3c), POST, USB-updater, EasyFlash ENV, суб-программаторы, HW-CRC, boot FSM, jump-to-app ✔ |
| Boot CRC-слово | `0x08017FFC` | ожидаемый CRC загрузчика ⓗ (граница совпадает) |
| MB Application | `0x08018000`+ (макс 0xE8000) | 2-я vector-таблица, reset=*(0x08018004)=**FUN_08018440** ✔ |
| Суб-образы (внешн. SPI-флеш) | BB@0x100000, DB@0x130000, LB@0x430000 | SetFirmInfo `FUN_0800214c` ✔ |

- **Jump-to-app** = `FUN_080012d0`: MSP←*(0x08018000), прыг на *(0x08018004). Проверка только `(SP&mask)==0x20000000` и `base<reset`. **VTOR загрузчиком НЕ пишется.** ✔
- **CRC = аппаратный GD32 @0x40023000**, poly 0x04C11DB7, init 0xFFFFFFFF, 32-бит big-endian, без реверса/фин-XOR. compute `FUN_08008e1c`, reset `FUN_08008e38`. ✔
  - Boot-гейт (Reset `FUN_08011a3c`): CRC над 0x08000000..0x08017FF8 vs *(0x08017FFC); mismatch → `FUN_08011946` вечный halt. **Приложение при старте НЕ CRC-ится.** ✔
  - App launch — без CRC. ✔
  - 3× «CRC check error» = OTA суб-плат BB/DB/LB (`FUN_08006658/08006ac0/08006d3c`), только при OTA. ✔
  - MB self-OTA `FUN_08006fe8` — единственная CRC своего приложения, при обновлении (mb_sv<MB_BVER vs ENV MB_CRC). Не boot-гейт. ✔
- **ENV = armink EasyFlash**, ключи MB_/BB_/DB_/LB_/BTL_ (*_SIZE/_CRC/_BVER/_sv/_BRF). get `FUN_0800aa84`, set `FUN_0800ae40`. ✔

### 2.2 FreeRTOS-задачи (wrapper `FUN_080849c0`, 11 задач)

| Задача | Роль | Прим. |
|---|---|---|
| `initial` | Поднимает сервисы, затем самоудаляется | старт |
| **`highspeed loop`** | Быстрый цикл — **тут тикает МАШИНА СОСТОЯНИЙ** (process_*_run) | ядро автономии |
| `time` | RTC / расписание | |
| `bms` | Батарея (smart-пак по serial) | |
| `bdport` | Приём/парс границы (коилы) | |
| `map` | Карта/мультизоны | |
| `dp send` / `dp receive` | UART-протокол: RX питает диспетчер JSON | линк UI/команды |
| `send_log` | Логи | |
| **`imu fuse`** | Сенсор-фьюжн: наклон/курс | |
| `IDLE` | FreeRTOS idle | |

`process manager` / `mems` / `ledport` — сервисы внутри highspeed loop / imu fuse, не отдельные задачи. ✔

### 2.3 Машина состояний

`state = *(0x200000bc)` (база 0x200000ac + 0x10). Сеттер `FUN_0807976c` (кламп **0..10**), геттер `FUN_0804edd4`. Прямых записей мимо сеттера НЕ найдено (все через ~22 вызова/обёртки). ✔

| val | Состояние | Модуль (process_*) | Доказательство |
|---|---|---|---|
| 3 | IDLE/standby | process_waiting | "change to idle", "Robot manual stopped" |
| 4 | DEPARTURE/выезд | process_departure_smooth | "start work", "goto departure" |
| 5 | CUTTING/кошение | process_cutting | "in border, change to cutting" |
| 6 | FIND BORDER | process_find_bd | "not on border, change to find border" |
| 7 | DOCKING | process_docking_smooth | "on border, change to dock" |
| 8 | CHARGING | process_charging | "in station, change to charging" |
| 9 | ERROR | process_error | аборт FUN_08083d9c→set(9) |
| 10 | POWER OFF | process_power_off | "Robot manual power off" |

(0/1/2 не используются; обёртки-ошибки ставят 9/10 по маске кода ошибки.) ✔

```
                    control-байт 0x300100de (start)
          ┌──────────────────────────────────────────┐
          ▼                                           │
  [3 IDLE] ──start──► [4 DEPARTURE] ──в границе──► [5 CUTTING]
     ▲  ▲                                             │
     │  │                                    не на границе
     │  │                                             ▼
     │  └── stop/manual ◄────────────────────── [6 FIND BORDER]
     │                                               │ на границе
     │                                               ▼
  [3 IDLE] ◄── charge done ── [8 CHARGING] ◄─в станции─ [7 DOCKING]
                                                         ▲ dock cmd
  любое ── ошибка(маска) ──► [9 ERROR]
  любое ── power off ──────► [10 POWER OFF]
```

### 2.4 Подсистемы

**Привод колёс = 3× BLDC на Allegro A4963** (L-колесо/R-колесо/нож), sensorless, общий `motor_spi_mutex`. Файлы `a4963_snk_v2.c`, `driver_{left,right,blade}_motor_snk_v2.c`. ✔
- Скорость→PWM: `FUN_0807f696(base,ch,val)` пишет CHxCV (0x34/38/3c/40 = CH0..3CV). **Таймер = TIMER2 `0x40000400`** (прочитано из литералов: PWM-out `DAT_0805fac4`=0x40000400, left/right/blade init-базы ВСЕ =0x40000400). Все 3 A4963 (2 колеса+нож) кормятся 4 каналами TIMER2; левое колесо = ch3 (offset 0x40). Мотор-ctx RAM=`0x2000054c`. ✔ (совпало с живой находкой «TIMER2 CCRx»)
- Направление/тормоз/конфиг→SPI A4963: `FUN_0808408c(spi,word)` word=`addr<<13|data`, verify+retry; init льёт 8 конфиг-регистров. ✔
- brake↔dir `FUN_0801f4dc` (RUN/BRAKE/COAST в теневом рег.), set_direction `FUN_0801f4c6`, set_left_dir `FUN_0805f300`. ✔
- **Только PWM крутить нельзя** — пока A4963 не сконфигурен по SPI и не в RUN, PWM игнорируется. ✔
- **set_motor_speed `FUN_0805f620`** ✔ (форс-дизасм закрыл дыру): `pwm = speed*0x1c20/gear_max`, gear_max ∈ {6141,3069,718}, передачи 0/1/2, brake-to-dir при смене, финал `FUN_0805f300`+`FUN_0807f696(timer,3,pwm)`. См. `chip1/forced_functions.c`.

**Одометрия — отдельный таймер TIMER3 `0x40000800`** (НЕ тот, что PWM), режим **input-capture** кан.1 (`FUN_0807f86c(...,1,0x10002)`). ISR `FUN_0805fbc0`(0x0805fbc0)/`FUN_0805fd45` инкрементят тики по направлению (+0x18 fwd/+0x1c back), скорость по периоду (capture-чит `FUN_0807f402`). Регистрация колбэков `FUN_08062c64/8c(4,...)`. Старые фикс-адреса 0x2000be28/be58 в бинаре ОТСУТСТВУЮТ (структуры колёс на malloc). ✔
> **Что доказано СТАТИЧЕСКИМ кодом (только это):** `FUN_0807f474` ставит CH3 в output mode (CHxMS=00)+CHxEN, `FUN_0807f648(…,0x60)`=OCxM PWM, `FUN_0805f620`→`FUN_0807f696` пишет duty в CH3CV. Т.е. путь скорости в коде идёт через регистр CH3CV TIMER2 (это output-канал, НЕ input-capture; одометрия на отдельном TIMER3). ✔ (только статика)
> **НЕ доказано (нет чистого живого замера):** что запись в CH3CV ФИЗИЧЕСКИ крутит/тормозит колесо; что «CCR=0 останавливает». Прежнее «живое подтверждение» из старых live-findings ОТБРОШЕНО как false-positive — косилка тогда встала, упёршись в ограничительный провод (border-стоп), а не из-за записи в CCR. Физический эффект CCR и роль SPI-RUN A4963 — требуют чистого стендового замера (вывешенные колёса, БЕЗ border-навигации). ⓗ

**Drive(speed,angle)** = `FUN_0804bb0c` через vtable движ-базы (reset/set_speed/set_target/start). Угол→тики `FUN_08020520`: `angle*697/K` (K пер-модельная). ✔

**Нож:** сервис `FUN_08022938` (service_blade), драйвер = тот же A4963-путь (`FUN_0808408c`): 16-бит команда — enable=бит 0x1000, dir=бит 0x2, speed=биты 0x0FFF. Отдельного GPIO/таймера ножа НЕТ. Скорость cfg blade_sp/blade_max_sp. Защиты: stall→reverse, highload→смена скорости, overslope/lift/torque→brake&escape, blade_check_en. ✔

**IMU = Bosch BNO055** I²C (dev 0x50 = 7-bit 0x28), chip-id `A0 FB 32 0F`@рег0x00. `driver_mems_snk_v13.c`. Углы рег 0x1A, 6 байт (EUL heading/roll/pitch), /16→целые градусы. Объект: heading+0x18/roll+0x1a/pitch+0x1c (avg +0x1e/0x20/0x22). Slope-детект = pitch+roll (не heading). Альтернатива ICM-42688 (WHOAMI 0x47, вероятно SPI) тоже вкомпилена. I2C0(0x40005400) vs I2C1(0x40005800) — не разрешено. ✔/ⓗ

**BMS = умный пак по serial с CRC-фреймом** (driver_battery_snk_v1/v2, service_bms). Пер-cell напряжения, химия (sony/samsung/eve), telemetry "battery vol=%dmV, percent=%d, min/max, temp=%d". SoC по OCV, пороги из cfg voltage points. **НЕ ADC.** ✔

**Коилы-приём (граница)** на chip1: deal_message `FUN_0801fb80`, CRC8 `FUN_080241d0`, cJSON `FUN_08023f80`, bdport-колбэк `FUN_08021bb4` (version/reset/log/find-border). Модель границы (service_border `FUN_0804d4c8`): L str+0x20/area+0x24, R str+0x28/area+0x2c. **area∈{0,1,2} = снаружи/внутри/на-линии, str = магнитуда.** Near-station — композит (корректность border + напряжение + дебаунс). Пересечение in→out → манёвры «cycle left/right». ✔

**Дождь** = service_rain (rain_en/rain_delay, digital вход через port-service). **Lift** = service_lift (lift_en, дебаунс). **Updown/переворот** — из pitch/roll IMU. **Температура** — из пака + die-темп IMU (отдельного NTC ADC нет). ✔/ⓗ (GPIO rain/lift за абстракцией)

---

## 3. Карта chip2 (GD32F30x) — плата коилов границы

**✅ Дамп ПОЛНЫЙ (256 КБ), ADC-тракт катушек в нём есть.** (Ранний вывод «нужен ре-дамп» был ложноотрицательным — код обращается к ADC через указатель из литерал-пула, а не через SVD-метку.)

- **Оцифровка `FUN_08019bf4` (dual-ADC, вызывается из `FUN_08012b6c`):** `_DAT_08019cf0`=ADC0(0x40012400), `_DAT_08019cf4`=ADC1(0x40012800), DMA0(0x40020000). `FUN_08012a04(ADC0,rank0,ch5,st7)` + `FUN_08012a04(ADC1,rank0,ch9,st7)` → **две катушки = ADC0/канал5 + ADC1/канал9**, sample-time 7, dual (0x20000), DMA, буфер 800 отсч. ✔ (`FUN_08012a04` = конфиг RSQ/SAMPT, проверено по регистрам). Какая из них ЛЕВО/ПРАВО — по разводке/`send_border`, ⓗ.
- Объект-сборщик коилов: RAM `0x20000078`, 7-функциональная vtable (`DAT_08016c28..40` → код в дампе), `FUN_08016ba8`. ✔
- **Baseline `FUN_0801a1f8`:** сумма 8 отсчётов ADC → среднее /8 → `*DAT_0801a250`; валид [1906..2191] (~2048). ✔
- Волна через vtable провайдера `FUN_08016ba8` (type-код 0/1/2/3 + магнитуда). DSP/классификация: `process_deal_board.c` (`FUN_080164bc`/`FUN_08016784`), очередь волн, отсев «disturb». Выход: **area(0/1/2/3) + str**. ✔
- Линк chip2→chip1: `send_border_message FUN_08019914` (cJSON {area/str L+R + lift-флаг}), TX `FUN_08019a18` (≤128 байт, mutex+timeout, DMA, USART2 0x40004800). ✔
- Кадр `&`+тело+CRC8+`#`, тело XOR 0x5B (`FUN_08013ae4`), CRC8/MAXIM poly 0x8C (`FUN_08013008`). Двунаправленный. ✔
- Lift: 2 переключателя (лев/прав), дебаунс 10, `deal_lift_sensor FUN_08013970`. ✔

---

## 4. Сквозные потоки данных

### 4.1 «Граница» (коил → манёвр)
```
коил (аналог) ─► chip2 factory-driver ADC/DSP  [В ДАМПЕ НЕТ ⚠️]
   ─► vtable провайдер FUN_08016ba8 (type 0..3 + магнитуда)
   ─► process_deal_board FUN_080164bc/08016784 (очередь, отсев disturb)
   ─► area(0..3)+str  ─► send_border_message FUN_08019914 (cJSON L/R + lift)
   ─► TX FUN_08019a18: &+тело+CRC8+#, тело XOR 0x5B  ─── USART2 ───►
chip1: де-XOR ─► дефреймер (&..#, CRC8 FUN_080241d0) ─► cJSON FUN_08023f80
   ─► deal_message FUN_0801fb80 ─► bdport-колбэк FUN_08021bb4
   ─► service_border FUN_0804d4c8 (L str+0x20/area+0x24, R str+0x28/area+0x2c)
   ─► навигация: in→out ⇒ манёвр «cycle left/right»  ─► drive (см. 4.3)
```

### 4.2 «Команда» (дисплей → FSM)
```
дисплей ── UART ──► chip1
   ─► де-XOR 0x5B FUN_0803953c ─► дефреймер FUN_0806e938 (&..#, CRC8)
   ─► cJSON FUN_08023fe6 ─► ДИСПЕТЧЕР dpport FUN_080470f0 (16 слотов, +0x60 cat/+0x64 handler)
   ─► категория = cmd&0xff000000 = 0x30 ─► FUN_08044c80
   ─► control-байт 0x300100de (start/stop/dock/charge) ─► FUN_08050448 (+0x14)
   ─► set-state FUN_0807976c ─► [FSM 3..10]
   (0x300101dc = restart, байт 0x20)
```
Диспетчеры: dpport `FUN_080470f0`(16), bdport `FUN_08063880`(5), ledport `FUN_0806396c`(5). Таблица {cat→handler} — статическая const в .rodata (кодом не заполняется; указатели из псевдо-C не вытащить → нужны сырые .rodata). ✔

Команды 0x30xxxxxx (FUN_08044c80): 06/07=manual-set вкл/выкл (**только флаг!**), 09=login pwd, 10=set pwd, 11-16=RTC/расписание, 17=rain, 18=multizone, 1a=name, 1b/1c=SN, **20=OTA**, 21=wifi, 22=ble, 23=reset pwd; 82-8d=config-сеттеры; **0x300100de=CONTROL**, **0x300101dc=restart**. ✔

### 4.3 «Движение» (FSM → колесо → обратная связь)
```
FSM (highspeed loop, process_*_run)
   ─► drive(speed,angle) FUN_0804bb0c ─► vtable движ-базы (reset/set_speed/set_target/start)
   ─► примитивы: turn/dir FUN_08020164, fwd FUN_0801fb04, back FUN_0801fe8c
   ─► пишут в ОЧЕРЕДЬ мотор-команд +0xe4
   ─► set_motor_speed FUN_0805f620 (кламп 1533..6141) [ДЫРА декомпайлера ⚠️]
   ─► PWM: CHxCV FUN_0807f696 (L=CH3)  +  SPI A4963 dir/brake FUN_0801f4dc/FUN_0808408c
   ─► КОЛЕСО (BLDC RUN)
Обратная связь:
   тахо/FG A4963 ─► input-capture ─► ISR FUN_0805fbc0/FUN_0805fc7c
   ─► тики +0x18 fwd/+0x1c back, скорость по периоду (FUN_0807f402)
   ─► одометрия (malloc-структуры) ─► навигация
```
**Прямой команды JOG/движения в протоколе НЕТ** — очередь +0xe4 дёргается только автономной FSM через vtable, из протокола недостижима. ✔

### 4.4 «Нож»
```
FSM/service_blade FUN_08022938 ─► A4963-путь FUN_0808408c
   ─► 16-бит: enable 0x1000 | dir 0x2 | speed 0x0FFF (cfg blade_sp/blade_max_sp)
   ─► защиты: stall→reverse, highload→скорость, overslope/lift/torque→brake&escape
```

### 4.5 «Зарядка / BMS»
```
FSM 7 DOCKING ─(на границе, near-station композит)─► FSM 8 CHARGING (process_charging)
BMS-задача ◄─ serial CRC-фрейм пак ─► per-cell V, SoC(OCV), temp, химия
   ─► пороги cfg voltage points ─► charge done ─► FSM 3 IDLE
```

---

## 5. Единая таблица ключевых адресов

Легенда: ✔ подтверждено • ⓗ гипотеза/частично • ⚠️ дыра/нужен ре-дамп.

**Boot / CRC / OTA / ENV (chip1)**
| Функция | Адрес | Роль | |
|---|---|---|---|
| Reset (bootloader) | FUN_08011a3c | POST + boot-CRC гейт | ✔ |
| halt | FUN_08011946 | вечный halt при CRC mismatch | ✔ |
| jump-to-app | FUN_080012d0 | MSP+прыжок на приложение | ✔ |
| App reset | FUN_08018440 | точка входа приложения @*(0x08018004) | ✔ |
| SetFirmInfo | FUN_0800214c | адреса суб-образов BB/DB/LB | ✔ |
| CRC compute / reset | FUN_08008e1c / FUN_08008e38 | HW-CRC GD32 @0x40023000 | ✔ |
| OTA суб-плат | FUN_08006658/08006ac0/08006d3c | CRC BB/DB/LB | ✔ |
| MB self-OTA | FUN_08006fe8 | CRC своего приложения (только OTA) | ✔ |
| ENV get / set | FUN_0800aa84 / FUN_0800ae40 | EasyFlash ключи | ✔ |

**Протокол / диспетчер (chip1)**
| Функция | Адрес | Роль | |
|---|---|---|---|
| де-XOR / XOR | FUN_0803953c / FUN_08039e8c | XOR 0x5B RX/TX | ✔ |
| дефреймер | FUN_0806e938 | поиск &..#, CRC8 | ✔ |
| cJSON parse | FUN_08023fe6 / FUN_08023f80 | | ✔ |
| CRC8 MAXIM | FUN_080241d0 | poly 0x8C | ✔ |
| dpport / bdport / ledport | FUN_080470f0 / FUN_08063880 / FUN_0806396c | диспетчеры | ✔ |
| cat 0x30 handler | FUN_08044c80 | команды 0x30xxxxxx | ✔ |
| control-байт handler | FUN_08050448 (+0x14) | start/stop/dock/charge | ✔ |
| build msg / TX | FUN_0801fb80 | сборка кадра | ✔ |

**FSM / задачи / конфиг (chip1)**
| Функция | Адрес | Роль | |
|---|---|---|---|
| state var | 0x200000bc | текущее состояние | ✔ |
| set-state / get-state | FUN_0807976c / FUN_0804edd4 | кламп 0..10 | ✔ |
| abort→error | FUN_08083d9c | set(9) | ✔ |
| RTOS wrapper | FUN_080849c0 | создание 11 задач | ✔ |
| config parser | FUN_08076b2c | ~110 ключей, вход 0xa5 | ✔ |

**Привод / одометрия / нож (chip1)**
| Функция | Адрес | Роль | |
|---|---|---|---|
| drive(speed,angle) | FUN_0804bb0c | vtable движ-базы | ✔ |
| angle→ticks | FUN_08020520 | angle*697/K | ✔ |
| примитивы turn/fwd/back | FUN_08020164 / FUN_0801fb04 / FUN_0801fe8c | пишут в очередь +0xe4 | ✔ |
| set_motor_speed | FUN_0805f620 | кламп 1533..6141 | ⚠️ дыра |
| PWM CHxCV | FUN_0807f696 | L=CH3 | ✔/ⓗ база |
| A4963 SPI | FUN_0808408c | addr<<13\|data verify+retry | ✔ |
| brake↔dir / set_dir / set_left_dir | FUN_0801f4dc / FUN_0801f4c6 / FUN_0805f300 | RUN/BRAKE/COAST | ✔ |
| capture ISR L/R | FUN_0805fbc0 / FUN_0805fc7c | тики +0x18/+0x1c | ✔ |
| capture read / регистрация | FUN_0807f402 / FUN_08062c64/8c | период→скорость | ✔ |
| service_blade | FUN_08022938 | нож | ✔ |

**Граница / IMU (chip1)**
| Функция | Адрес | Роль | |
|---|---|---|---|
| service_border | FUN_0804d4c8 | L/R str+area | ✔ |
| bdport колбэк | FUN_08021bb4 | version/reset/log/find-border | ✔ |

**Коилы (chip2)**
| Функция | Адрес | Роль | |
|---|---|---|---|
| провайдер волны | FUN_08016ba8 | type 0..3 + магнитуда | ✔ |
| DSP deal_board | FUN_080164bc / FUN_08016784 | area/str, отсев disturb | ✔ |
| send_border_message / TX | FUN_08019914 / FUN_08019a18 | cJSON, USART2 | ✔ |
| XOR / CRC8 | FUN_08013ae4 / FUN_08013008 | 0x5B / poly 0x8C | ✔ |
| deal_lift_sensor | FUN_08013970 | 2 switch, дебаунс 10 | ✔ |
| **ADC/DSP factory-driver** | — | driver_bdsensor.c | ⚠️ В ДАМПЕ НЕТ |

---

## 6. Точки для кастомизации (для будущих фаз)

**(A) Ручное RC-управление** — два пути:
- **Софт (свой хендлер в диспетчере):** занять свободный слот dpport (напр. категория 0x31) → хендлер напрямую дёргает примитивы `FUN_08020164`/`FUN_0801fb04`/`FUN_0801fe8c` или пишет в очередь мотор-команд `+0xe4`. Требует пересборки/патча приложения. Прямого JOG в штатном протоколе НЕТ — надо добавлять. ✔
- **По SWD (без прошивки):** писать в очередь `+0xe4` или вызывать примитив / `set_motor_speed FUN_0805f620` напрямую. Быстрый прототип, но требует физ. доступ. ✔
- Важно: перед вращением A4963 должен быть сконфигурен по SPI и в RUN (иначе PWM игнорируется). ✔

**(B) Пересборка приложения без пересчёта CRC** — **возможно.** Патч 0x08018000+ и заливка по SWD грузятся БЕЗ CRC. Условия: валидный SP@0x08018000 и reset@0x08018004; не висит pending OTA (mb_sv≥MB_BVER, иначе загрузчик перезальёт из внешней флешки). Патч ЗАГРУЗЧИКА (0x08000000..0x08017FF8) — надо пересчитать HW-CRC и записать в 0x08017FFC. ✔

**(C) Техдолг — статус:**
- ✅ **chip2 ADC/DSP катушек** — РЕШЕНО без ре-дампа: код есть в дампе (`FUN_08019bf4` dual-ADC: ADC0 ch5=L / ADC1 ch9=R; baseline `FUN_0801a1f8`). Ре-дамп НЕ нужен.
- ✅ **`set_motor_speed FUN_0805f620`** — РЕШЕНО форс-дизасмом (gear-map, PWM→CH3). См. `chip1/forced_functions.c`.
- ⚙️ **Таблицы {cat→handler}** диспетчеров и **номер таймера PWM** (DAT_0805b144) — решаются чтением сырых байт литералов из существующего дампа (не ре-дамп): достаточно прочитать 4 байта по flash-адресу метки. TODO desk-задача.

---

## 7. Открытые вопросы (ⓗ) — не доказано, нужен живой чип / ре-дамп

1. **Boot-CRC слово @0x08017FFC** — граница совпадает, но точное значение/формат не сверены на железе. ⓗ
2. ✅ ~~chip2 ADC-тракт~~ — РЕШЕНО: ADC0 ch5=L / ADC1 ch9=R, sample-time 7, dual, baseline avg-8 [1906..2191] (`FUN_08019bf4`/`FUN_0801a1f8`).
3. ✅ ~~set_motor_speed FUN_0805f620~~ — РЕШЕНО форс-дизасмом (gear-map pwm=speed*7200/gear_max, →CH3).
4. ✅ ~~Номер таймера PWM~~ — РЕШЕНО: **TIMER2 `0x40000400`**, 4 канала на 2 колеса + нож (мотор-ctx RAM 0x2000054c).
5. **I2C-шина IMU** — I2C0(0x40005400) vs I2C1(0x40005800) не разрешено. ⓗ
6. **GPIO-пины rain/lift** на chip1 — за абстракцией port-service, конкретные пины неизвестны. ⓗ
7. **Пер-модельная K** в angle→ticks (`FUN_08020520`, angle*697/K) — значение из cfg/модели. ⓗ
8. **Указатели хендлеров диспетчеров** (.rodata таблица cat→handler) — из декомпиляции не вытащены. ⚠️
9. **ICM-42688 альтернатива IMU** — вкомпилена, но используется ли на MI 302 (vs BNO055) — не доказано. ⓗ
10. **Точный набор control-байтов** (кроме 0x300100de/0x300101dc) и их подкоманд start/stop/dock/charge — маппинг частичный. ⓗ
```
