# Function catalog — CHIP2 (GD32F30x, coil board / border-sensor board)

> **All facts below refer to CHIP2** (coil board, link to chip1 over USART2@115200).
> Sources: `reverse-v2/chip2/{functions_index.csv, symbols.txt, strings.txt, decompiled_all.c, forced_functions.c, memory_map.txt}`,
> `reverse-v2/analysis/{callgraph,indegree,outdegree}-chip2.tsv`.
> Dump: `dist/gd32-mainboard-chip2-dump-v1.bin`, base `0x08000000`.
> Total functions in index: **435** (all decomp_ok=1). Below, all large/significant clusters are analyzed + the runtime ballast is grouped (libc/HAL/FreeRTOS/elog).
> Confidence: **[V]** verified (code/string/offset citation), **[I]** inferred (plausible, NOT proven), **[U]** unknown.

## 0. Key to the chip2 "dictionary" (what the strings give)

The firmware is built with EasyLogger + FreeRTOS + cJSON, with debug `__FILE__` strings. Modules by paths in `strings.txt` (all `..\src\...`):

| Module (string path) | What it is | Confidence |
|---|---|---|
| `application\rw_bdboard_init.c` | board startup, driver registration, versions | [V] |
| `application\process_comm.c` | link to chip1: command reception, assembly of response messages (cJSON) | [V] |
| `application\process_deal_board.c` | coil wave processing, disturb-wave, base voltage | [V] |
| `platform\driver\driver_board_sensor\driver_bdsensor.c` | coil sensor driver (tag `hr_bd`) | [V] |
| `platform\driver\driver_port\driver_mboard_port_snk_v2.c` | physical port-link (tag `mport_drv`) SNK v2 | [V] |
| `platform\easylogger\elog*.c` | EasyLogger V2.2.99 logger | [V] |
| `platform\freertos\{tasks,queue,port,heap_4}.c` | FreeRTOS core | [V] |
| IEC60730 self-test strings (`0801a52c…`) | self-test per IEC60730 (CPU/FLASH-CRC/clock/ADC/RAM) | [V] |

**Function-name table** in `strings.txt` (`0801c910…`) — this is an embedded list of NAMES (a SEGGER/RTT-like table); it gives a dictionary of roles but is **not hard-bound to addresses**: `get_process_comm, comm_task_init, comm_task, deal_lift_sensor, send_border_message, send_version_message, send_seach_border_message, send_follow_border_message, port_receive_callback, init_board_task, follow_border_check_wave, start_collect_data, delay_and_start_next_collect_data, follow_check_subwave, init_task, log_init, init_all_driver, create_board_task, create_comm_task, bdsensor_init, send_string_dpport_snk_v2, elog_*`. Below, bindings where proven by code are marked [V]; where by meaning — [I].

---

## 1. Startup / init / driver registration

| Address | Name-guess | Purpose | Evidence | Conf |
|---|---|---|---|---|
| 08000130 | `startup_loop` | early start: spins `FUN_08000138`/`FUN_08000194` (.data copy/.bss clear or libc-init) | decompiled_all.c:2-31; callgraph 08000130→08000138,→08000194 | [I] |
| 08000138 | `data_bss_init` | continuation of early section copying | decompiled_all.c:33 | [I] |
| 08000194 | `init_array_step` | init step | decompiled_all.c | [I] |
| 0801715c | **init_all_driver** | sequentially gets and initializes drivers: timer(`FUN_08019340`), board_sensor(`FUN_08019240`), port(`FUN_08019300`), lift(`FUN_080192c8`), factory(`FUN_08019288`); logs "register/init … driver failed" | decompiled_all.c ~FUN_0801715c; strings 08017220-08017300 | [V] |
| 080175e8 | **init_board_task / rw_bdboard_init** | malloc pri-data, `elog`-init(`FUN_08017b44`), `init_all_driver`, log "hardwave version=%d, software version=%d" (=0xC353), launch process-init-board(cmd 0x17) and process-comm(cmd 0x14) | decompiled_all.c ~FUN_080175e8; strings 08017754,08017790,080177e0 | [V] |
| 08017b44 | `log_init / elog_start` | EasyLogger init ("elog init failed", "EasyLogger V%s initialize success") | ref strings 08017b9c, 080147a0; awk-attr → easylogger | [V] |
| 08019340 | `get_timer_driver` | lazily allocates timer-driver object (0x18 b) + vtable from DAT_08019374.. | decompiled_all.c ~FUN_08019340 | [V] |
| 08019240 | `get_board_sensor_driver` | bd-sensor-driver object (0x18 b) + 5-entry vtable | decompiled_all.c ~FUN_08019240 | [V] |
| 08019300 | `get_port_driver` | port-driver object, vtable DAT_08019330.. (via `FUN_080138dc`) | decompiled_all.c ~FUN_08019300 | [V] |
| 080192c8 | `get_lift_driver` | lift-driver object (0x10 b) + 3-entry vtable | decompiled_all.c ~FUN_080192c8 | [V] |
| 08019288 | `get_factory_driver` | factory-driver object (0x14 b) + 4-entry vtable | decompiled_all.c ~FUN_08019288 | [V] |
| 080138dc | `port_driver_alloc` | port-driver object allocator (called from get_port_driver) | callgraph FUN_08019300→FUN_080138dc | [I] |
| 08016ba8 | `get_process_init_board` | lazy singleton of the init-board struct (0x20 + 0xB8 b), fills vtable DAT_08016c28.. and defaults (0x92=800 samples) | decompiled_all.c ~FUN_08016ba8 | [V] |
| 08016d20 | `get_process_comm` | lazy singleton of the comm object (0x18 + 0x1C b), vtable DAT_08016da8.. | decompiled_all.c ~FUN_08016d20; name `get_process_comm` in table | [V] |
| 08016ab4 | `get_factory_driver_obj` | factory-driver getter (used in comm/board task) | callgraph FUN_08017884→FUN_08016ab4; ref "get factory driver failed" | [I] |
| 08016c98 | `get_wave_queue_obj` | getter of the wave queue object (`return *DAT_08016ca0`) | decompiled_all.c ~FUN_08016c98 | [I] |

## 2. Tasks (FreeRTOS tasks) and main loops

| Address | Name-guess | Purpose | Evidence | Conf |
|---|---|---|---|---|
| 08013680 | **comm_task_init_bd / process_comm task** | waits for bd-task readiness (flag +0xD), spins `FUN_0801c618` (queue-receive), processes; log "comm_task_init bd task wait failed" | decompiled_all.c ~FUN_08013680; strings 0801376f,08013810 | [V] |
| 08017884 | `wait_bd_ready / init_bd_service` | up to 0x50 attempts pulls factory-driver(`FUN_08016ab4`) vtable+4; readiness counters | decompiled_all.c ~FUN_08017884 | [I] |
| 08019914 | **send_border_message / follow_border_check_wave** | builds cJSON with wave data, classifies left/area/str, sends to chip1; log "left wave data error, area=%d, str=%d", "create cjson config data failed" | decompiled_all.c 14121-14191; strings 0801981c,080197cc | [V] |
| 08013970 | **deal_lift_sensor** | polls two lift sensors via vtable, debounce (threshold 10), states both/left/right/none lifted | decompiled_all.c ~FUN_08013970; strings 08013a7f,08013abc,08013ac8,08013ad8 | [V] |
| 08012b6c | **bdsensor_init / base-voltage calibration** | port config(`FUN_08019148` ×6), trigger-timer config(0x40000000=TIMER1), 5× ADC read → averaging → base voltage (deflt 2048); log tag `hr_bd` "base voltage=%d" | decompiled_all.c ~FUN_08012b6c; strings 08012c8b,08012ce0 | [V] |

## 3. ADC coils / DSP wave processing (process_deal_board.c + driver_bdsensor.c)

| Address | Name-guess | Purpose | Evidence | Conf |
|---|---|---|---|---|
| 08016a90 | `adc_read_raw` | `return *DAT_08016a98` — reading the last raw ADC sample | decompiled_all.c ~FUN_08016a90 | [V] |
| 0801a1f8 / thunk_FUN_0801a1f8 | `adc_start_convert` | start/wait for a single ADC conversion (in the calibration loop, waits for ==1) | decompiled_all.c in FUN_08012b6c | [I] |
| 08016fa0 | `wave_buffers_init` | allocation of wave buffers by field 0x92(=800)×2, setup of the processing struct (fields 0x78=0x49B4 etc.), 2 lists of 0x28×0x10 | decompiled_all.c ~FUN_08016fa0 | [I] |
| 080164bc | **enqueue_wave / disturb_wave** | alloc buffer param2×2, `FUN_0801baf8`(threshold), `FUN_08015dcc`(sample), `FUN_08014e88`(features), `FUN_0801499e`(enqueue), `FUN_080159a0`(classif.); logs "enqueue wave to disturb wave failed", "create wave queue error", "disturb find error size=%d" | decompiled_all.c ~FUN_080164bc; strings 080166d3,0801672c,08016888 | [V] |
| 0801baf8 | `wave_threshold_window` | windowed threshold on the wave: ±2500 (or ±5000 when param5==1) — detection of sign/amplitude of the magnetic field | decompiled_all.c ~FUN_0801baf8 | [I] |
| 08015dcc | `wave_sample_filter` | 9-arg DSP pass over the wave (short arrays, local accumulators) | decompiled_all.c ~FUN_08015dcc | [I] |
| 08014e88 | `wave_feature_extract` | 6-arg: extraction of wave features (area/strength) | decompiled_all.c ~FUN_08014e88 | [I] |
| 080159a0 | `wave_classify_area_str` | 704 b: final classification of area/str from features | decompiled_all.c ~FUN_080159a0 | [I] |
| 080179f8 | `wave_geom_calc` | 64-bit arithmetic over wave points (slope/distance), check `<0x96` | decompiled_all.c ~FUN_080179f8; callees FUN_080108a4(div64) | [I] |
| 08016fa0..08016xxx | `deal_board cluster` | the rest of the board-processing cluster (init_bd/disturb) — partially mapped | strings 08016xxx (init_bd/disturb) | [U] |

## 4. Classification of area / str + message assembly (process_comm.c)

| Address | Name-guess | Purpose | Evidence | Conf |
|---|---|---|---|---|
| 08019914 | `send_border_message` (see §2) | + classification of left/area/str and cJSON packing inside | decompiled_all.c 14121-14191 | [V] |
| 08016d20 | `get_process_comm` (see §1) | owner of comm state | — | [V] |
| ~08018xxx | `send_version_message` / `send_seach_border` / `send_follow_border` | generators of version/search/follow messages (names in the table; exact addresses not bound) | strings 0801c95f,0801c974,0801c98e | [U] |
| 08018758 | `cJSON_print/serialize` | 796 b, cJSON serialization into a string (format `%1.15g/%1.17g`, `u%04x`) | decompiled_all.c:12509; strings 080184f4,08018500,08018750 | [I] |
| 080180a0 | **cJSON_parse_value** | recursive cJSON parser: `{`,`[`,`"`,number,`false`,`null`; depth <1000 | decompiled_all.c:12283+ (FUN_080180a0); strings 0801818c(false),08018184 | [V] |
| 08017f92 | `cJSON_parse_string` | parsing of a string literal `"` | called from FUN_080180a0 | [I] |
| 08012ed0 | `cJSON_parse_object_item` | parsing of a key:value pair of an object | callgraph FUN_080180a0→FUN_08012ed0; FUN_08012e20→FUN_08012ed0 | [I] |
| 08012e20 | `cJSON_new_item` | creation of a new cJSON node | callgraph → FUN_08012ed0 | [I] |
| 08012d4c | `cJSON_skip_whitespace` | skipping whitespace during parsing | callgraph FUN_080180a0→FUN_08012d4c | [I] |
| 08012d78 | `cJSON_add_item_to_object` | adding a field to a cJSON object (used in send_border_message) | decompiled_all.c in FUN_08019914 | [I] |
| 08012fd4 | `cJSON_render/finalize` | finalizing cJSON before sending | decompiled_all.c in FUN_08019914 | [I] |
| 08012e38 | `cJSON_delete` | freeing the cJSON tree | callgraph indeg; used after render | [I] |

## 5. Link to chip1 — physical port (driver_mboard_port_snk_v2.c, tag `mport_drv`)

| Address | Name-guess | Purpose | Evidence | Conf |
|---|---|---|---|---|
| 08019a18 | **send_string_dpport_snk_v2** | sending a frame ≤0x80 bytes: spin-semaphore(`FUN_0801b246/232`,50000 attempts), `memset`+`memcpy` into TX buffer DAT_08019b38, launch DMA/USART(`FUN_08013d96`,`FUN_08013f74`,`FUN_08013db6`); logs "send string length=%d too long", "@send len=0", "@wait send string overtime" | decompiled_all.c ~FUN_08019a18; strings 08019ab8,08019b2b,08019b3f; name in table | [V] |
| 08013d96 | `usart_dma_tx_enable` | enabling the transmit channel (arg,1) | decompiled_all.c:7828 | [I] |
| 08013f74 | `usart_dma_set_len` | setting the transmit length (arg,1,len) | decompiled_all.c:8069 | [I] |
| 08013db6 | `usart_dma_start` | start transmit | call in FUN_08019a18 | [I] |
| 08013f5c | `port_id_check` | check "this is our port and len>4" (== DAT_08013f70) | decompiled_all.c ~FUN_08013f5c | [I] |
| 08013d7c | `queue_free_node` | freeing a node (indeg 7, used in wave enqueue error) | callgraph | [I] |
| ~0801xxxx | **port_receive_callback** | port RX callback, parsing of a frame from chip1 (CRC8/XOR framing) | name in table 0801c9a9; string "dp receive" 08017147 | [U] |
| — | `frame CRC8 / XOR` | frame checksum of the link | NOT explicitly localized in code | [U] |

> ⚠️ **[U] link hole:** the concrete functions of the RX callback and of the frame CRC8/XOR computation to chip1 are not yet bound to code (the MMIO USART2@0x40004800 does not appear literally in the decompile — access is via DAT handles). The chip2↔chip1 frame format requires a separate pass (cross-check against the chip1-side USART2@115200).

## 6. Lift sensors

| Address | Name-guess | Purpose | Evidence | Conf |
|---|---|---|---|---|
| 08013970 | **deal_lift_sensor** (see §2) | 2 lift sensors, debounce, states left/right/both/none | strings 08013a7f… | [V] |
| 080192c8 | `get_lift_driver` (see §1) | lift-driver vtable (3 methods: read L, read R, init) | decompiled_all.c ~FUN_080192c8 | [V] |

## 7. EasyLogger logger (elog*.c) + assert/print

| Address | Name-guess | Purpose | Evidence | Conf |
|---|---|---|---|---|
| 080141e8 | **elog_output** | main log: `(level, tag, file, line, fmt, …)` — hundreds of calls throughout the firmware | decompiled_all.c ~FUN_080141e8; call signature | [V] |
| 08011a1a | **rtt_print / low_assert** | low-level output `(0, str, …)` — used by IEC60730 tests and asserts; indegree=27 (max) | forced_functions.c uses `FUN_08011a1a(0,…,file,line)`; decompiled 12813 | [V] |
| 08014090 | `elog_assert_hook` | assert handling `(%s) has assert failed at %s:%ld` | ref strings 0801413b; awk-attr easylogger | [V] |
| 080141e8-080148xx | `elog_*` set | set_output_enabled/set_fmt/set_filter/find_lvl/find_tag/strcpy/cpyln (names in table 0801cab2…) | strings 0801cab2-0801cb9b | [I] |
| 080145f8,080146b0,080147f8,08016ae4,08017b44 | `elog_* helpers` | formatting/filters/start (assert strings elog.c) | awk-attr easylogger | [I] |

## 8. IEC60730 self-test (functional safety, PreRun/RunTime)

| Address | Name-guess | Purpose | Evidence | Conf |
|---|---|---|---|---|
| 0801a5f0 | **iec60730_prerun_test** | suite: CPU-test(`FUN_08010470`), FLASH CRC32(`FUN_080138a4`), clock-test(`FUN_0801a258`), ADC-test, RAM-test, PC-test; log "… IEC60730 Test …" | decompiled_all.c ~FUN_0801a5f0; strings 0801a52c-0801a94b | [V] |
| 0801a510 | **iec60730_test_fail** | halt loop on failure: kills SysTick (`_DAT_e000e010 &= ~2`), prints "IEC60730 Test Fail", hangs | decompiled_all.c ~FUN_0801a510 | [V] |
| 08010470 | `cpu_register_test` | CPU register test (PreRun), returns 1 on success | call in FUN_0801a5f0; strings CPU Test | [I] |
| 080138a4 | `crc32_compute` | FLASH CRC32 computation (hardware CRC 0x40023000) | call `FUN_080138a4(base,0xbfff)` in FUN_0801a5f0; tag CHECHESUM | [V] |
| 0801a258 | `clock_test` | clocking test (IRC40K/HXTAL/PLL), switch on result | call in FUN_0801a5f0; outdeg 19 (max) | [I] |
| 0801ab88 | `flash_crc_prep` | preparation/selection of block for the CRC test | call in FUN_0801a5f0 | [I] |
| 08019030/080191b0/080191c4/08019030 | `clock/pll switch helpers` | switching IRC8M↔HXTAL, waiting for stabilization | calls in FUN_0801a5f0 | [I] |

## 9. GD32F30x peripheral HAL (cluster 0801a…0801b, "std peripheral lib")

Access to MMIO goes through handles (`0x40000000`=TIMER1 etc.), not via literals; the functions are GD32 SPL wrappers.

| Address | Name-guess | Purpose | Evidence | Conf |
|---|---|---|---|---|
| 0801af7c | `timer_init` | basic timer setup (`(0x40000000,&cfg)` in bdsensor_init) | decompiled_all.c in FUN_08012b6c | [I] |
| 0801ac58 | `timer_channel_config` | timer channel config (OC) | ibid. | [I] |
| 0801ae7a | `timer_channel_set_pulse` | setting the compare/duty (…,1,100) | ibid. | [I] |
| 0801ae2c | `timer_set_prescaler/period` | (…,1,0x70) | ibid. | [I] |
| 0801ae98 | `timer_channel_enable` | (…,1,0) enable/disable of the channel | ibid. | [I] |
| 08019d34 / 08019bf4 | `timer_enable / adc_trigger_link` | launch of the ADC trigger timer in calibration | decompiled_all.c in FUN_08012b6c | [I] |
| 0801a258,0801a3e0,0801a16c,0801a5f0 | `rcu/clock HAL` | RCU/clock (high outdegree — clock config) | outdegree-chip2.tsv top | [I] |
| 0801b180,0801b09c,0801b790… | `gpio/nvic/dma HAL` | peripheral wrappers | outdegree/indegree | [U] |

## 10. FreeRTOS core (platform\freertos\*, cluster 0801b…0801c)

All identified by the assert strings `..\src\platform\freertos\{tasks,queue,port,heap_4}.c`.

| Address(es) | Module | Purpose | Evidence | Conf |
|---|---|---|---|---|
| 0801b458 / 0801b4c4 | port.c | enter/exit critical section (vPortEnter/ExitCritical) — called from the tick handler | forced_functions.c FUN_0801c818 uses FUN_0801b458/0801b4c4; strings 0801b494 | [V] |
| 0801b246 / 0801b232 | port.c | semaphore/mutex take/give (spin in send_string) | decompiled_all.c in FUN_08019a18 | [I] |
| 0801b65c | tasks.c | `vTaskDelay` (delays 100/50/0x32 ms in calibration) | decompiled_all.c in FUN_08012b6c | [I] |
| 0801c818 | tasks.c | `xTaskIncrementTick` (tick handler: decrement delays, unblock tasks, yield 0x10000000=PendSV) | forced_functions.c (full listing) | [V] |
| 0801c51c | tasks.c | processing of one unblocked task (in the tick) | forced_functions.c call from FUN_0801c818 | [I] |
| 0801b3ce/0801b43e/0801b458 | list.c | task list operations (uxListRemove/vListInsert) | forced_functions.c FUN_0801c818 | [I] |
| 0801bd28,0801bf78,0801bec0,0801c100,0801c1e8 | queue.c | xQueueSend/Receive/GenericCreate (assert queue.c) | strings 0801bcf4-0801c1d8 | [V] |
| 0801b51c…0801ba68,0801bcb0,0801c2d8-0801c76c | tasks.c | scheduler/task creation/idle (assert tasks.c, "IDLE") | strings 0801b6c4-0801c8f8, 0801ba17 | [V] |
| 0801c618 | queue.c | `xQueueReceive` wrapper (in the comm task) | decompiled_all.c in FUN_08013680 | [I] |
| 08018d24,08018828 | heap_4.c / tasks.c | pvPortMalloc/heap (assert heap_4.c) | strings 08018e03; awk-attr freertos | [V] |
| 08018c3c | port.c | trigger context switch helper | forced_functions.c call | [I] |

## 11. Allocator / memory / libc primitives

| Address | Name-guess | Purpose | Evidence | Conf |
|---|---|---|---|---|
| 080195b8 | **malloc** | pvPortMalloc wrapper (`size=6` → tail-call); everywhere | decompiled_all.c ~FUN_080195b8; indegree high | [V] |
| 080195ac | **free** | freeing (vPortFree) | decompiled_all.c ~FUN_080195ac | [V] |
| 08013950 | **zalloc/calloc** | malloc+zero (arguments size[,align]) | callgraph FUN_08013950→FUN_080195b8; used in wave-init | [V] |
| 08010912 | `memcpy` | copy (dst,src,len) — in send_string, enqueue | decompiled_all.c in FUN_08019a18/0801499e | [V] |
| 08010948 | `memset` | fill (dst,val?,len=0x80) | decompiled_all.c in FUN_08019a18 | [I] |
| 080109be | `memcmp/strncmp` | comparison (in cJSON: "false"/"null"/markers) | decompiled_all.c in FUN_080180a0 | [V] |
| 080108a4 | `__udivmoddi4 / div64` | 64-bit division (in wave_geom_calc) | callgraph FUN_080179f8→FUN_080108a4 | [I] |
| 0801499e | **list_enqueue** | push of an element onto a linked list (malloc node 0xC + copy data) | decompiled_all.c ~FUN_0801499e | [V] |
| 08013d30 | `list_clear/pop` | clear/pop the list head | call from FUN_0801499e | [I] |
| 08019148 | `bitmap_set` | `bitmap[i>>6] |= 1<<(i&0x1f)` — channel/pin registration (0x602,0x603,0x609,0x60a,0x500,0x700 in bdsensor_init) | decompiled_all.c ~FUN_08019148 | [I] |

## 12. Runtime ballast / unanalyzed (grouped)

| Range | Count (approx.) | What it is | Conf |
|---|---|---|---|
| 08000130–08001fff | ~72 | startup/CRT0, libc-init, integer division/shifts, `switchD` tables (symbols: switchD@08001bb6) | [I] |
| 08010xxx–08011xxx | ~69 | libc strings/formatting (printf core, hex tables `0123456789abcdef`), memcpy/memset/cmp | [I] |
| 08012xxx–08013xxx | ~71 | cJSON + driver-getters + comm/lift (partially analyzed above) | [V/I] |
| 08014xxx–08017xxx | ~90 | elog + wave DSP + rw_bdboard/process (partially analyzed above) | [V/I] |
| 08018xxx–08019xxx | ~63 | cJSON-print + process_comm + port-driver + HAL wrappers | [V/I] |
| 0801axxx–0801cxxx | ~93 | GD32 SPL (clock/timer/adc/gpio/dma) + FreeRTOS core + IEC60730 | [V/I] |

> The remaining small functions (2–50 b) are, in the overwhelming majority, vtable getter-wrappers, thunks and inline SPL/FreeRTOS helpers; they carry no application logic of the mower.

---

## Main conclusions / [U] holes

1. **[V] chip2 is a "border sensor coprocessor"**: it calibrates the coil base voltage (5× ADC, deflt 2048), runs the wave through DSP (threshold ±2500/±5000 → area/str features → classification), and reports to chip1 via cJSON messages over the SNK v2 port-link. It has no motor logic of its own — it is the coil/sensor board.
2. **[V] Hard IEC60730 safety layer**: on failure of the CPU/FLASH-CRC32/clock/ADC/RAM test the firmware **hangs** in a halt loop (`FUN_0801a510`), killing SysTick. Important for any modification of chip2 firmware: the FLASH CRC32 self-test (`FUN_080138a4`, hardware CRC 0x40023000) will drop the board into halt when your own code is flashed.
3. **[U] The chip2↔chip1 link frame format is not bound to code**: `send_string_dpport_snk_v2` (0x08019a18, ≤0x80 bytes, DMA) is found and proven, but the RX callback (`port_receive_callback`) and the frame CRC8/XOR framing are not bound to addresses (USART2 goes via DAT handles, not a literal). This is the main hole for reversing the protocol — a separate pass is needed, cross-checking USART2@115200 on the chip1 side.
