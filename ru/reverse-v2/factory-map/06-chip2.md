# 06 — CHIP2: второй контроллер = сенсорная плата границы (bdboard)

Источники: `reverse-v2/chip2/{decompiled_all.c(17424 стр,430 функций),strings.txt,symbols.txt,
functions_index.csv,memory_map.txt}`, дамп `dist/gd32-mainboard-chip2-dump-v1.bin` (LE),
для роли-сверки — `reverse-v2/chip1/strings.txt`.

**ВЫВОД ОДНОЙ СТРОКОЙ:** chip2 — это **контроллер сенсорной платы обнаружения границы**
(в исходниках зовётся *bd / bdboard / board sensor / mboard*). Он оцифровывает сигнал
периметрального провода (АЦП+DMA), детектирует «волну» (wave) границы и состояние подъёма
косилки (lift), и по **UART** отдаёт эти данные главному chip1 в виде **cJSON-строк**.
К силовым рейлам моторов/колёс он **не подключён** — только сенсорика и безопасность.

---

## (1) ЧТО ЗА МК + ПЕРИФЕРИЯ

**МК: GD32F4xx (класс GD32F407/F405), тот же чип-семейство, что и chip1.**
Основания (`memory_map.txt`): полный набор F4-периферии — ENET_MAC/DMA/PTP, USBFS (GLOBAL/HOST/
DEVICE/PWRCLK), EXMC, CAN0+CAN1, **три АЦП** (ADC0 @0x40012400, ADC1 @0x40012800, ADC2 @0x40013c00),
DAC @0x40007400, SPI0..2, USART0..2 + UART3..4, I2C0/I2C1, таймеры TIMER0..13, RCU/FMC/CRC, NVIC.
Flash-регион `08000000–0803ffff` (256 КБ). Дамп: SP=`0x20000988`, reset-вектор=`0x080001b4`
(FUN_080001b4). SRAM `20000000–20017fff` (96 КБ).

**ПО (по вшитым путям исходников `..\src\...`):**
- RTOS: **FreeRTOS** (`platform\freertos\{tasks,queue,port,heap_4}.c`).
- Лог: **EasyLogger v2.2.99** (`platform\easylogger\elog*.c`) — `elog init success`.
- Сериализация линка: **cJSON** (форматтеры `%1.15g`/`%1.17g`, Create/AddItem/Print/Delete).
- Безопасность: **IEC60730** self-test (CPU / FLASH-CRC32 / clock / **ADC** / RAM, PreRun+Run-Time).
- Прикладные модули: `application\{rw_bdboard_init,process_deal_board,process_comm}.c`,
  `driver\driver_board_sensor\driver_bdsensor.c`, `driver\driver_port\driver_mboard_port_snk_v2.c`.

**Что включает периферия:**
- **АЦП + DMA** — приёмник сигнала границы. `FUN_08019d34` конфигурит АЦП+DMA-канал
  (конфиг-структура: 0x200, `base voltage`=0x800=**2048** по умолчанию, 0x2000; при ошибке —
  строка `base voltage =%d error, set default 2048`), ставит приоритет NVIC для IRQ **0xB**.
  `FUN_08019e34`/`FUN_08019eb8` выделяют буферы сэмплов по полю `+0x92` (**800** либо **235**
  сэмплов) — это окна захвата «волны».
- **UART + DMA** — линк с chip1 (см. раздел 3). GPIO/AF настраивает `FUN_08019bf4`
  (два GPIO-порта `DAT_08019cf0`/`DAT_08019cf4`, маски пинов 0x60000/0xe0000/0x7000).
- **Таймер** — «timer driver» (`FUN_08019340`), периодический запуск сбора данных.
- **GPIO lift** — «lift driver» (`FUN_080192c8`), входы датчиков подъёма (лево/право).

---

## (2) КАРТА ФУНКЦИЙ  FUN_ → имя → действие

| FUN_ | имя (по строкам) | действие |
|---|---|---|
| FUN_080001b4 | Reset_Handler | точка входа (reset-вектор) |
| FUN_080175e8 | **init_task / rw_bdboard_init** | главный init: malloc, вызывает init_all_driver, логирует `hardwave version=%d, software version=%d`, создаёт процессы init_board (id 0x17) и comm (id 0x14); при ошибке — вечный цикл с `init error=%d` |
| FUN_0801715c | **init_all_driver** | регистрирует+инициализирует драйверы: timer(FUN_08019340) → board_sensor(FUN_08019240) → port(FUN_08019300) → lift(FUN_080192c8) → factory(FUN_08019288); каждый = объект с vtable {init,…,start}; при сбое — `init … driver failed` |
| FUN_08019240 | get board_sensor driver | объект АЦП-драйвера границы |
| FUN_08019300 | get port(mport) driver | объект UART-линка |
| FUN_080192c8 | get lift driver | объект датчиков подъёма |
| FUN_08019340 | get timer driver | объект таймера сбора |
| FUN_08019d34 | **bdsensor_init (ADC+DMA)** | конфиг АЦП+DMA, base voltage 2048, NVIC IRQ 0xB |
| FUN_08019e34/eb8 | alloc wave buffers | буферы сэмплов (800 / 235) под захват волны |
| FUN_08019bf4 | **mport GPIO/AF init** | AF-настройка пинов UART TX/RX + DMA |
| FUN_08019a18 | **send_string_dpport_snk_v2** | TX ≤128 Б по DMA-UART; мьютекс (FUN_0801b246 take/FUN_0801b232 give, obj+6); memset 0x80, memcpy(FUN_08010912), DMA: FUN_08013d96 en-канал → FUN_08013f74 set-count → FUN_08013db6 старт; ошибки `send len=0`/`wait send string overtime`/`send string length=%d too long` |
| FUN_08019cc? (FUN_080199cc) | port TX-обёртка | формирует и выдаёт строку через vtable порта |
| ~строки 14060–14232 | **build_border_message (cJSON)** | собирает JSON: лев/прав `wave` (area+направление cVar1∈{0,1}), lift-state, версии → сериализует (FUN_08012fd4) → отправляет; ошибки `create cjson config data failed`, `left wave data error, area=%d, str=%d` |
| ~строки 7500+ | **deal_lift_sensor** | читает датчики подъёма → `left lifted`/`right lifted`/`both lifted`/`lift none` |
| FUN_08012e20 | cJSON_CreateObject | — |
| FUN_08012d78 | cJSON_AddItemToObject | добавить поле |
| FUN_08010e52 / FUN_08010e74 | cJSON_CreateNumber (int/bool) | числа границы |
| FUN_08012fd4 / FUN_08012e38 | cJSON_PrintUnformatted / Delete | сериализация/освобождение |
| FUN_080141e8 | **elog_output (лог-обёртка)** | (indegree 18) все `..\src\...` логи |
| FUN_08011a1a | log/printf-ядро | (indegree 27) |
| ~строки 15100+ | **IEC60730_selftest** | CPU/FLASH-CRC32/clock/`adc test`/RAM тесты; при провале `====== IEC60730 Test Fail ======` |

Имена процессов/функций перечислены таблицей строк в дампе @`0801c910`+: `get_process_comm`,
`comm_task_init`, `comm_task`, `deal_lift_sensor`, `send_border_message`, `send_version_message`,
`send_seach_border_message`, `send_follow_border_message`, `port_receive_callback`,
`init_board_task`, `follow_border_check_wave`, `start_collect_data`,
`delay_and_start_next_collect_data`, `follow_check_subwave`, `bdsensor_init`,
`send_string_dpport_snk_v2`.

---

## (3) ЛИНК С CHIP1  (шина / формат / данные)

**Шина:** асинхронный **UART** (half-duplex-подобный, DMA на TX; сторона chip2 —
`driver_mboard_port_snk_v2.c` = «mport»). Точный USART/пины в декомпиляции абстрагированы за
драйвер-объект (базы приходят параметром), физику см. GPIO-init `FUN_08019bf4` +
`reference/FACTORY-GPIO-PERIPHERAL-MAP.md`.

**Формат кадра:** **cJSON-строка** (текст), кадр ≤ **128 байт** (жёсткий лимит в TX
`FUN_08019a18`, при превышении — `send string length=%d too long`). Обе стороны используют cJSON
(на chip1 тоже `create cjson config data failed`).

**Мастер = chip1.** Со стороны chip1 это подсистема **bdport** (`service_bdport.c` /
`driver_bdport.c` / `deal_message.c`, теги `bdport srv`/`bdport drv`/`dpport drv`). chip1 —
инициатор, chip2 — ведомый сенсор.

**Направления и содержимое (по строкам chip1):**
- **chip1 → chip2 (команды):** `bdboard start search border` (+`becauseof %d`),
  `bdboard reset by flag=%d`, запрос версии (`set bdboard version`), follow-border.
- **chip2 → chip1 (данные/ответы):**
  - версия при подключении: chip1 читает `bdboard hardware version=%d, software version=%d`
    (chip2 логирует свою `hardwave version=%d, software version=%d` в init_task);
  - данные границы: `bdboard find border`; JSON с левым/правым `wave` (area + знак направления),
    lift-состоянием;
  - таймауты/связь: chip1 фиксирует `bdboard send overtime`, `bdport service receive overtime`,
    `bdboard disconnect`, `reconnect bdport`.

Итого: **chip1 (bdport, master) ⇄ UART/cJSON ⇄ chip2 (mport, sensor-slave).** chip2 непрерывно
оцифровывает провод периметра, находит «волну» границы, и по команде/периодически шлёт chip1
результат детекции границы + подъёма; chip1 на этом строит навигацию «искать/следовать границе».

---

## (4) ТАЙМЛАЙН РАБОТЫ

1. **Reset** → `FUN_080001b4` → clock/RCU, **IEC60730 PreRun** (CPU, FLASH-CRC32, clock, ADC, RAM);
   при провале — `IEC60730 Test Fail` / зависание.
2. **FreeRTOS** старт → задача `initial` (@`0801884f`) → **`init_task`/`rw_bdboard_init`**
   (`FUN_080175e8`): malloc pri-data, лог версий.
3. **`init_all_driver`** (`FUN_0801715c`): timer → **board_sensor(ADC+DMA)** → **port(UART+DMA)** →
   **lift(GPIO)** → factory. `bdsensor_init` (`FUN_08019d34`): base voltage=2048, IRQ 0xB.
4. Создаются процессы **init_board (0x17)** и **comm (0x14)**.
5. **Рабочий цикл:**
   - `comm_task`: рукопожатие с chip1 по UART (отдать версию), приём команд
     (`port_receive_callback`), диспетч (`start search border` / `follow border` / `reset`).
   - `board_task`: `start_collect_data` → АЦП+DMA копит окно сэмплов (800/235) → `check_wave` /
     `follow_border_check_wave` / `follow_check_subwave` детектит границу → `deal_lift_sensor`
     читает подъём → **build_border_message (cJSON)** → **`send_string_dpport_snk_v2`** в chip1;
   - `delay_and_start_next_collect_data` → следующая итерация.
   - Фоном: IEC60730 Run-Time (FLASH-CRC/RAM) периодически.

---

## (5) СВЯЗЬ С ПИТАНИЕМ / ТОКОМ МОТОРОВ

**Прямой причастности НЕТ.** chip2 не управляет ни рейлом колёс 20U, ни драйверами A4963, ни BMS —
в его strings/периферии нет ни `a4963`, ни `motor`, ни `bms`, ни силовых GPIO. Он читает **АЦП
сигнала границы** (не токовые шунты моторов) и **GPIO датчиков подъёма**.

Косвенные факторы безопасности (стоит держать в уме для «колёс»):
- **Lift-сенсор**: chip2 сообщает `left/right/both lifted`. Приподнятая косилка — штатный стоп
  ножа/движения на стороне chip1. Если lift читается как «поднято», chip1 может блокировать
  привод. Проверить, что при наших тестах lift=none.
- **Зависимость FSM от bdport-линка**: chip1 логирует `bdboard disconnect`/`reconnect bdport`/
  `send overtime`. Если chip1 требует живого/валидного bdboard перед разрешением движения, мёртвый
  UART-линк с chip2 мог бы держать FSM вне ходового состояния. НО: под нашей fw нож крутится ⇒ FSM
  доходит до рабочего состояния ⇒ это **не** главный блокер колёс. Оставить как второстепенную
  проверку, приоритет остаётся на железе рейла 20U / BMS discharge (см. MEMORY).

**Вердикт:** chip2 не влияет на «почему колёса не едут» напрямую; это сенсор границы+подъёма.
Единственные его рычаги над приводом — сигнал lift и здоровье bdport-линка, оба вторичны.
