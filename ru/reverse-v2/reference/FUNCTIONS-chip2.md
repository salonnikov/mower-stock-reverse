# Каталог функций — CHIP2 (GD32F30x, плата коилов / border-sensor board)

> **Все факты ниже относятся к CHIP2** (плата коилов, линк к chip1 по USART2@115200).
> Источники: `reverse-v2/chip2/{functions_index.csv, symbols.txt, strings.txt, decompiled_all.c, forced_functions.c, memory_map.txt}`,
> `reverse-v2/analysis/{callgraph,indegree,outdegree}-chip2.tsv`.
> Дамп: `dist/gd32-mainboard-chip2-dump-v1.bin`, база `0x08000000`.
> Всего функций в индексе: **435** (все decomp_ok=1). Ниже разобраны все крупные/значимые кластеры + сгруппирован рантайм-балласт (libc/HAL/FreeRTOS/elog).
> Доверие: **[V]** verified (цитата кода/строки/offset), **[I]** inferred (правдоподобно, НЕ доказано), **[U]** unknown.

## 0. Ключ к «словарю» chip2 (что дают строки)

Прошивка собрана с EasyLogger + FreeRTOS + cJSON, есть отладочные `__FILE__`-строки. Модули по путям в `strings.txt` (все `..\src\...`):

| Модуль (путь строки) | Что это | Доверие |
|---|---|---|
| `application\rw_bdboard_init.c` | старт платы, регистрация драйверов, версии | [V] |
| `application\process_comm.c` | линк к chip1: приём команд, сборка ответных сообщений (cJSON) | [V] |
| `application\process_deal_board.c` | обработка волны коилов, disturb-wave, база-вольтаж | [V] |
| `platform\driver\driver_board_sensor\driver_bdsensor.c` | драйвер сенсора коила (тег `hr_bd`) | [V] |
| `platform\driver\driver_port\driver_mboard_port_snk_v2.c` | физ. порт-линк (тег `mport_drv`) SNK v2 | [V] |
| `platform\easylogger\elog*.c` | логгер EasyLogger V2.2.99 | [V] |
| `platform\freertos\{tasks,queue,port,heap_4}.c` | FreeRTOS ядро | [V] |
| IEC60730 self-test строки (`0801a52c…`) | самотест по IEC60730 (CPU/FLASH-CRC/clock/ADC/RAM) | [V] |

**Таблица имён-функций** в `strings.txt` (`0801c910…`) — это встроенный список ИМЁН (SEGGER/RTT-подобная таблица), даёт словарь ролей, но **не привязан жёстко к адресам**: `get_process_comm, comm_task_init, comm_task, deal_lift_sensor, send_border_message, send_version_message, send_seach_border_message, send_follow_border_message, port_receive_callback, init_board_task, follow_border_check_wave, start_collect_data, delay_and_start_next_collect_data, follow_check_subwave, init_task, log_init, init_all_driver, create_board_task, create_comm_task, bdsensor_init, send_string_dpport_snk_v2, elog_*`. Ниже привязки, где доказаны кодом, помечены [V]; где по смыслу — [I].

---

## 1. Старт / init / регистрация драйверов

| Адрес | Имя-догадка | Назначение | Evidence | Дов |
|---|---|---|---|---|
| 08000130 | `startup_loop` | ранний старт: крутит `FUN_08000138`/`FUN_08000194` (копирование .data/очистка .bss или libc-init) | decompiled_all.c:2-31; callgraph 08000130→08000138,→08000194 | [I] |
| 08000138 | `data_bss_init` | продолжение раннего копирования секций | decompiled_all.c:33 | [I] |
| 08000194 | `init_array_step` | шаг init | decompiled_all.c | [I] |
| 0801715c | **init_all_driver** | последовательно получает и инициализирует драйверы: timer(`FUN_08019340`), board_sensor(`FUN_08019240`), port(`FUN_08019300`), lift(`FUN_080192c8`), factory(`FUN_08019288`); логи «register/init … driver failed» | decompiled_all.c ~FUN_0801715c; строки 08017220-08017300 | [V] |
| 080175e8 | **init_board_task / rw_bdboard_init** | malloc pri-data, `elog`-init(`FUN_08017b44`), `init_all_driver`, лог «hardwave version=%d, software version=%d» (=0xC353), запуск process-init-board(cmd 0x17) и process-comm(cmd 0x14) | decompiled_all.c ~FUN_080175e8; строки 08017754,08017790,080177e0 | [V] |
| 08017b44 | `log_init / elog_start` | инициализация EasyLogger («elog init failed», «EasyLogger V%s initialize success») | ref strings 08017b9c, 080147a0; awk-attr → easylogger | [V] |
| 08019340 | `get_timer_driver` | лениво аллоцирует объект таймер-драйвера (0x18 б) + vtable из DAT_08019374.. | decompiled_all.c ~FUN_08019340 | [V] |
| 08019240 | `get_board_sensor_driver` | объект bd-sensor-драйвера (0x18 б) + 5-запись vtable | decompiled_all.c ~FUN_08019240 | [V] |
| 08019300 | `get_port_driver` | объект порт-драйвера, vtable DAT_08019330.. (через `FUN_080138dc`) | decompiled_all.c ~FUN_08019300 | [V] |
| 080192c8 | `get_lift_driver` | объект lift-драйвера (0x10 б) + 3-запись vtable | decompiled_all.c ~FUN_080192c8 | [V] |
| 08019288 | `get_factory_driver` | объект factory-драйвера (0x14 б) + 4-запись vtable | decompiled_all.c ~FUN_08019288 | [V] |
| 080138dc | `port_driver_alloc` | аллокатор объекта порт-драйвера (зовётся из get_port_driver) | callgraph FUN_08019300→FUN_080138dc | [I] |
| 08016ba8 | `get_process_init_board` | ленивый singleton структуры init-board (0x20 + 0xB8 б), заполняет vtable DAT_08016c28.. и дефолты (0x92=800 сэмплов) | decompiled_all.c ~FUN_08016ba8 | [V] |
| 08016d20 | `get_process_comm` | ленивый singleton comm-объекта (0x18 + 0x1C б), vtable DAT_08016da8.. | decompiled_all.c ~FUN_08016d20; имя `get_process_comm` в таблице | [V] |
| 08016ab4 | `get_factory_driver_obj` | геттер factory-драйвера (используется в comm/board-таске) | callgraph FUN_08017884→FUN_08016ab4; ref «get factory driver failed» | [I] |
| 08016c98 | `get_wave_queue_obj` | геттер объекта очереди волны (`return *DAT_08016ca0`) | decompiled_all.c ~FUN_08016c98 | [I] |

## 2. Задачи (FreeRTOS-таски) и главные циклы

| Адрес | Имя-догадка | Назначение | Evidence | Дов |
|---|---|---|---|---|
| 08013680 | **comm_task_init_bd / process_comm task** | ждёт готовности bd-таска (флаг +0xD), крутит `FUN_0801c618` (queue-receive), обрабатывает; логи «comm_task_init bd task wait failed» | decompiled_all.c ~FUN_08013680; строки 0801376f,08013810 | [V] |
| 08017884 | `wait_bd_ready / init_bd_service` | до 0x50 попыток дёргает factory-драйвер(`FUN_08016ab4`) vtable+4; счётчики готовности | decompiled_all.c ~FUN_08017884 | [I] |
| 08019914 | **send_border_message / follow_border_check_wave** | строит cJSON с данными волны, классифицирует left/area/str, шлёт на chip1; лог «left wave data error, area=%d, str=%d», «create cjson config data failed» | decompiled_all.c 14121-14191; строки 0801981c,080197cc | [V] |
| 08013970 | **deal_lift_sensor** | опрос двух lift-датчиков через vtable, дебаунс (порог 10), состояния both/left/right/none lifted | decompiled_all.c ~FUN_08013970; строки 08013a7f,08013abc,08013ac8,08013ad8 | [V] |
| 08012b6c | **bdsensor_init / base-voltage калибровка** | конфиг портов(`FUN_08019148` ×6), конфиг таймера-триггера(0x40000000=TIMER1), 5× чтение ADC → усреднение → база-вольтаж (deflt 2048); лог тег `hr_bd` «base voltage=%d» | decompiled_all.c ~FUN_08012b6c; строки 08012c8b,08012ce0 | [V] |

## 3. ADC-коилы / DSP-обработка волны (process_deal_board.c + driver_bdsensor.c)

| Адрес | Имя-догадка | Назначение | Evidence | Дов |
|---|---|---|---|---|
| 08016a90 | `adc_read_raw` | `return *DAT_08016a98` — чтение последнего сырого ADC-сэмпла | decompiled_all.c ~FUN_08016a90 | [V] |
| 0801a1f8 / thunk_FUN_0801a1f8 | `adc_start_convert` | запуск/ожидание одиночной ADC-конверсии (в цикле калибровки, ждёт ==1) | decompiled_all.c в FUN_08012b6c | [I] |
| 08016fa0 | `wave_buffers_init` | аллокация буферов волны по полю 0x92(=800)×2, настройка структуры обработки (поля 0x78=0x49B4 и пр.), 2 списка по 0x28×0x10 | decompiled_all.c ~FUN_08016fa0 | [I] |
| 080164bc | **enqueue_wave / disturb_wave** | аллок буфер param2×2, `FUN_0801baf8`(порог), `FUN_08015dcc`(сэмпл), `FUN_08014e88`(фичи), `FUN_0801499e`(enqueue), `FUN_080159a0`(классиф.); логи «enqueue wave to disturb wave failed», «create wave queue error», «disturb find error size=%d» | decompiled_all.c ~FUN_080164bc; строки 080166d3,0801672c,08016888 | [V] |
| 0801baf8 | `wave_threshold_window` | оконный порог на волну: ±2500 (или ±5000 при param5==1) — детект знака/амплитуды магнит-поля | decompiled_all.c ~FUN_0801baf8 | [I] |
| 08015dcc | `wave_sample_filter` | 9-арг DSP-проход по волне (short-массивы, локальные накопители) | decompiled_all.c ~FUN_08015dcc | [I] |
| 08014e88 | `wave_feature_extract` | 6-арг: извлечение фич волны (area/strength) | decompiled_all.c ~FUN_08014e88 | [I] |
| 080159a0 | `wave_classify_area_str` | 704 б: итоговая классификация area/str по фичам | decompiled_all.c ~FUN_080159a0 | [I] |
| 080179f8 | `wave_geom_calc` | 64-бит арифметика над точками волны (наклон/дистанция), проверка `<0x96` | decompiled_all.c ~FUN_080179f8; callees FUN_080108a4(div64) | [I] |
| 08016fa0..08016xxx | `deal_board cluster` | остальной кластер обработки борда (init_bd/disturb) — размечено частично | strings 08016xxx (init_bd/disturb) | [U] |

## 4. Классификация area / str + сборка сообщений (process_comm.c)

| Адрес | Имя-догадка | Назначение | Evidence | Дов |
|---|---|---|---|---|
| 08019914 | `send_border_message` (см. §2) | + внутри классификация left/area/str и cJSON-упаковка | decompiled_all.c 14121-14191 | [V] |
| 08016d20 | `get_process_comm` (см. §1) | владелец состояния comm | — | [V] |
| ~08018xxx | `send_version_message` / `send_seach_border` / `send_follow_border` | генераторы сообщений версии/поиска/следования (имена в таблице; точные адреса не привязаны) | strings 0801c95f,0801c974,0801c98e | [U] |
| 08018758 | `cJSON_print/serialize` | 796 б, сериализация cJSON в строку (формат `%1.15g/%1.17g`, `u%04x`) | decompiled_all.c:12509; строки 080184f4,08018500,08018750 | [I] |
| 080180a0 | **cJSON_parse_value** | рекурсивный парсер cJSON: `{`,`[`,`"`,число,`false`,`null`; глубина <1000 | decompiled_all.c:12283+ (FUN_080180a0); строки 0801818c(false),08018184 | [V] |
| 08017f92 | `cJSON_parse_string` | разбор строкового литерала `"` | вызывается из FUN_080180a0 | [I] |
| 08012ed0 | `cJSON_parse_object_item` | разбор пары ключ:значение объекта | callgraph FUN_080180a0→FUN_08012ed0; FUN_08012e20→FUN_08012ed0 | [I] |
| 08012e20 | `cJSON_new_item` | создание нового cJSON-узла | callgraph → FUN_08012ed0 | [I] |
| 08012d4c | `cJSON_skip_whitespace` | пропуск пробелов при парсинге | callgraph FUN_080180a0→FUN_08012d4c | [I] |
| 08012d78 | `cJSON_add_item_to_object` | добавление поля в cJSON-объект (исп. в send_border_message) | decompiled_all.c в FUN_08019914 | [I] |
| 08012fd4 | `cJSON_render/finalize` | финализация cJSON перед отправкой | decompiled_all.c в FUN_08019914 | [I] |
| 08012e38 | `cJSON_delete` | освобождение cJSON-дерева | callgraph indeg; исп. после render | [I] |

## 5. Линк к chip1 — физ. порт (driver_mboard_port_snk_v2.c, тег `mport_drv`)

| Адрес | Имя-догадка | Назначение | Evidence | Дов |
|---|---|---|---|---|
| 08019a18 | **send_string_dpport_snk_v2** | отправка кадра ≤0x80 байт: спин-семафор(`FUN_0801b246/232`,50000 попыток), `memset`+`memcpy` в TX-буфер DAT_08019b38, запуск DMA/USART(`FUN_08013d96`,`FUN_08013f74`,`FUN_08013db6`); логи «send string length=%d too long», «@send len=0», «@wait send string overtime» | decompiled_all.c ~FUN_08019a18; строки 08019ab8,08019b2b,08019b3f; имя в таблице | [V] |
| 08013d96 | `usart_dma_tx_enable` | включение канала передачи (arg,1) | decompiled_all.c:7828 | [I] |
| 08013f74 | `usart_dma_set_len` | установка длины передачи (arg,1,len) | decompiled_all.c:8069 | [I] |
| 08013db6 | `usart_dma_start` | старт передачи | вызов в FUN_08019a18 | [I] |
| 08013f5c | `port_id_check` | проверка «это наш порт и len>4» (== DAT_08013f70) | decompiled_all.c ~FUN_08013f5c | [I] |
| 08013d7c | `queue_free_node` | освобождение узла (indeg 7, исп. в wave enqueue error) | callgraph | [I] |
| ~0801xxxx | **port_receive_callback** | RX-колбэк порта, разбор кадра от chip1 (CRC8/XOR-обрамление) | имя в таблице 0801c9a9; строка «dp receive» 08017147 | [U] |
| — | `frame CRC8 / XOR` | контрольная сумма кадра линка | НЕ локализовано в коде явно | [U] |

> ⚠️ **[U]-дыра линка:** конкретные функции RX-колбэка и подсчёта CRC8/XOR кадра к chip1 по коду пока не привязаны (MMIO USART2@0x40004800 в декомпиле не встречается литералом — доступ через DAT-хендлы). Формат кадра chip2↔chip1 требует отдельного захода (сверить с chip1-стороной USART2@115200).

## 6. Lift-сенсоры

| Адрес | Имя-догадка | Назначение | Evidence | Дов |
|---|---|---|---|---|
| 08013970 | **deal_lift_sensor** (см. §2) | 2 датчика подъёма, дебаунс, состояния left/right/both/none | строки 08013a7f… | [V] |
| 080192c8 | `get_lift_driver` (см. §1) | vtable lift-драйвера (3 метода: read L, read R, init) | decompiled_all.c ~FUN_080192c8 | [V] |

## 7. Логгер EasyLogger (elog*.c) + assert/print

| Адрес | Имя-догадка | Назначение | Evidence | Дов |
|---|---|---|---|---|
| 080141e8 | **elog_output** | главный лог: `(level, tag, file, line, fmt, …)` — сотни вызовов по всей прошивке | decompiled_all.c ~FUN_080141e8; сигнатура вызовов | [V] |
| 08011a1a | **rtt_print / low_assert** | низкоуровневый вывод `(0, str, …)` — исп. IEC60730-тестами и assert-ами; indegree=27 (макс) | forced_functions.c исп. `FUN_08011a1a(0,…,file,line)`; decompiled 12813 | [V] |
| 08014090 | `elog_assert_hook` | обработка assert `(%s) has assert failed at %s:%ld` | ref строки 0801413b; awk-attr easylogger | [V] |
| 080141e8-080148xx | `elog_*` набор | set_output_enabled/set_fmt/set_filter/find_lvl/find_tag/strcpy/cpyln (имена в таблице 0801cab2…) | strings 0801cab2-0801cb9b | [I] |
| 080145f8,080146b0,080147f8,08016ae4,08017b44 | `elog_* helpers` | форматирование/фильтры/старт (assert-строки elog.c) | awk-attr easylogger | [I] |

## 8. Самотест IEC60730 (functional safety, PreRun/RunTime)

| Адрес | Имя-догадка | Назначение | Evidence | Дов |
|---|---|---|---|---|
| 0801a5f0 | **iec60730_prerun_test** | пакет: CPU-тест(`FUN_08010470`), FLASH CRC32(`FUN_080138a4`), clock-тест(`FUN_0801a258`), ADC-тест, RAM-тест, PC-тест; лог «… IEC60730 Test …» | decompiled_all.c ~FUN_0801a5f0; строки 0801a52c-0801a94b | [V] |
| 0801a510 | **iec60730_test_fail** | halt-петля при провале: гасит SysTick (`_DAT_e000e010 &= ~2`), печатает «IEC60730 Test Fail», зависает | decompiled_all.c ~FUN_0801a510 | [V] |
| 08010470 | `cpu_register_test` | тест регистров CPU (PreRun), возвращает 1 при успехе | вызов в FUN_0801a5f0; строки CPU Test | [I] |
| 080138a4 | `crc32_compute` | подсчёт FLASH CRC32 (аппаратный CRC 0x40023000) | вызов `FUN_080138a4(base,0xbfff)` в FUN_0801a5f0; тег CHECHESUM | [V] |
| 0801a258 | `clock_test` | тест тактирования (IRC40K/HXTAL/PLL), switch по результату | вызов в FUN_0801a5f0; outdeg 19 (макс) | [I] |
| 0801ab88 | `flash_crc_prep` | подготовка/выбор блока для CRC-теста | вызов в FUN_0801a5f0 | [I] |
| 08019030/080191b0/080191c4/08019030 | `clock/pll switch helpers` | переключение IRC8M↔HXTAL, ожидание стабилизации | вызовы в FUN_0801a5f0 | [I] |

## 9. GD32F30x периферийный HAL (кластер 0801a…0801b, «std peripheral lib»)

Доступ к MMIO идёт через хендлы (`0x40000000`=TIMER1 и т.п.), не через литералы; функции — обёртки GD32 SPL.

| Адрес | Имя-догадка | Назначение | Evidence | Дов |
|---|---|---|---|---|
| 0801af7c | `timer_init` | базовая настройка таймера (`(0x40000000,&cfg)` в bdsensor_init) | decompiled_all.c в FUN_08012b6c | [I] |
| 0801ac58 | `timer_channel_config` | конфиг канала таймера (OC) | там же | [I] |
| 0801ae7a | `timer_channel_set_pulse` | установка сравнения/скважности (…,1,100) | там же | [I] |
| 0801ae2c | `timer_set_prescaler/period` | (…,1,0x70) | там же | [I] |
| 0801ae98 | `timer_channel_enable` | (…,1,0) вкл./выкл. канала | там же | [I] |
| 08019d34 / 08019bf4 | `timer_enable / adc_trigger_link` | запуск таймера-триггера ADC в калибровке | decompiled_all.c в FUN_08012b6c | [I] |
| 0801a258,0801a3e0,0801a16c,0801a5f0 | `rcu/clock HAL` | RCU/clock (высокий outdegree — конфиг тактов) | outdegree-chip2.tsv топ | [I] |
| 0801b180,0801b09c,0801b790… | `gpio/nvic/dma HAL` | обёртки периферии | outdegree/indegree | [U] |

## 10. FreeRTOS ядро (platform\freertos\*, кластер 0801b…0801c)

Все идентифицированы по assert-строкам `..\src\platform\freertos\{tasks,queue,port,heap_4}.c`.

| Адрес(а) | Модуль | Назначение | Evidence | Дов |
|---|---|---|---|---|
| 0801b458 / 0801b4c4 | port.c | вход/выход критической секции (vPortEnter/ExitCritical) — зовутся из тик-обработчика | forced_functions.c FUN_0801c818 исп. FUN_0801b458/0801b4c4; strings 0801b494 | [V] |
| 0801b246 / 0801b232 | port.c | semaphore/mutex take/give (спин в send_string) | decompiled_all.c в FUN_08019a18 | [I] |
| 0801b65c | tasks.c | `vTaskDelay` (задержки 100/50/0x32 мс в калибровке) | decompiled_all.c в FUN_08012b6c | [I] |
| 0801c818 | tasks.c | `xTaskIncrementTick` (тик-обработчик: декремент задержек, разблок задач, yield 0x10000000=PendSV) | forced_functions.c (полный листинг) | [V] |
| 0801c51c | tasks.c | обработка одной разблокированной задачи (в тике) | forced_functions.c вызов из FUN_0801c818 | [I] |
| 0801b3ce/0801b43e/0801b458 | list.c | операции со списками задач (uxListRemove/vListInsert) | forced_functions.c FUN_0801c818 | [I] |
| 0801bd28,0801bf78,0801bec0,0801c100,0801c1e8 | queue.c | xQueueSend/Receive/GenericCreate (assert queue.c) | strings 0801bcf4-0801c1d8 | [V] |
| 0801b51c…0801ba68,0801bcb0,0801c2d8-0801c76c | tasks.c | планировщик/создание задач/idle (assert tasks.c, «IDLE») | strings 0801b6c4-0801c8f8, 0801ba17 | [V] |
| 0801c618 | queue.c | `xQueueReceive` обёртка (в comm-таске) | decompiled_all.c в FUN_08013680 | [I] |
| 08018d24,08018828 | heap_4.c / tasks.c | pvPortMalloc/heap (assert heap_4.c) | strings 08018e03; awk-attr freertos | [V] |
| 08018c3c | port.c | trigger context switch helper | forced_functions.c вызов | [I] |

## 11. Аллокатор / память / libc-примитивы

| Адрес | Имя-догадка | Назначение | Evidence | Дов |
|---|---|---|---|---|
| 080195b8 | **malloc** | обёртка pvPortMalloc (`size=6` → tail-call); повсеместно | decompiled_all.c ~FUN_080195b8; indegree высок | [V] |
| 080195ac | **free** | освобождение (vPortFree) | decompiled_all.c ~FUN_080195ac | [V] |
| 08013950 | **zalloc/calloc** | malloc+zero (аргументы size[,align]) | callgraph FUN_08013950→FUN_080195b8; исп. в wave-init | [V] |
| 08010912 | `memcpy` | копирование (dst,src,len) — в send_string, enqueue | decompiled_all.c в FUN_08019a18/0801499e | [V] |
| 08010948 | `memset` | заполнение (dst,val?,len=0x80) | decompiled_all.c в FUN_08019a18 | [I] |
| 080109be | `memcmp/strncmp` | сравнение (в cJSON: "false"/"null"/маркеры) | decompiled_all.c в FUN_080180a0 | [V] |
| 080108a4 | `__udivmoddi4 / div64` | 64-битное деление (в wave_geom_calc) | callgraph FUN_080179f8→FUN_080108a4 | [I] |
| 0801499e | **list_enqueue** | push элемента в связный список (malloc узел 0xC + copy данных) | decompiled_all.c ~FUN_0801499e | [V] |
| 08013d30 | `list_clear/pop` | очистка/снятие головы списка | вызов из FUN_0801499e | [I] |
| 08019148 | `bitmap_set` | `bitmap[i>>6] |= 1<<(i&0x1f)` — регистрация канала/пина (0x602,0x603,0x609,0x60a,0x500,0x700 в bdsensor_init) | decompiled_all.c ~FUN_08019148 | [I] |

## 12. Рантайм-балласт / неразобранное (сгруппировано)

| Диапазон | Кол-во (прибл.) | Что это | Дов |
|---|---|---|---|
| 08000130–08001fff | ~72 | startup/CRT0, libc-init, целочисленное деление/сдвиги, `switchD`-таблицы (symbols: switchD@08001bb6) | [I] |
| 08010xxx–08011xxx | ~69 | libc-строки/форматирование (printf-ядро, hex-таблицы `0123456789abcdef`), memcpy/memset/cmp | [I] |
| 08012xxx–08013xxx | ~71 | cJSON + driver-getters + comm/lift (разобрано выше частично) | [V/I] |
| 08014xxx–08017xxx | ~90 | elog + wave DSP + rw_bdboard/process (разобрано выше частично) | [V/I] |
| 08018xxx–08019xxx | ~63 | cJSON-print + process_comm + port-driver + HAL-обёртки | [V/I] |
| 0801axxx–0801cxxx | ~93 | GD32 SPL (clock/timer/adc/gpio/dma) + FreeRTOS ядро + IEC60730 | [V/I] |

> Оставшиеся мелкие функции (2–50 б) — это в подавляющем большинстве геттеры-обёртки vtable, thunk-и и inline-хелперы SPL/FreeRTOS; они не несут прикладной логики косилки.

---

## Главные выводы / [U]-дыры

1. **[V] chip2 — это «сенсорный сопроцессор границы»**: калибрует базовый вольтаж коила (5× ADC, deflt 2048), гоняет волну через DSP (порог ±2500/±5000 → фичи area/str → классификация), и репортит на chip1 через cJSON-сообщения по порт-линку SNK v2. Собственной моторной логики тут нет — это плата коилов/датчиков.
2. **[V] Жёсткий safety-слой IEC60730**: при провале CPU/FLASH-CRC32/clock/ADC/RAM-теста прошивка **виснет** в halt-петле (`FUN_0801a510`), гася SysTick. Важно для любой модификации chip2-прошивки: FLASH CRC32 self-test (`FUN_080138a4`, аппаратный CRC 0x40023000) при заливке своего кода уронит плату в halt.
3. **[U] Формат кадра линка chip2↔chip1 не привязан к коду**: `send_string_dpport_snk_v2` (0x08019a18, ≤0x80 байт, DMA) найден и доказан, но RX-колбэк (`port_receive_callback`) и CRC8/XOR-обрамление кадра к адресам не привязаны (USART2 идёт через DAT-хендлы, не литералом). Это главная дыра для реверса протокола — нужен отдельный заход со сверкой USART2@115200 на стороне chip1.
