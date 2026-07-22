# 06 — CHIP2: second controller = border sensor board (bdboard)

Sources: `reverse-v2/chip2/{decompiled_all.c(17424 lines,430 functions),strings.txt,symbols.txt,
functions_index.csv,memory_map.txt}`, dump `dist/gd32-mainboard-chip2-dump-v1.bin` (LE),
for role cross-check — `reverse-v2/chip1/strings.txt`.

**ONE-LINE CONCLUSION:** chip2 is the **controller of the border-detection sensor board**
(called *bd / bdboard / board sensor / mboard* in the sources). It digitizes the signal
of the perimeter wire (ADC+DMA), detects the border "wave" and the lift state
of the mower (lift), and over **UART** hands this data to the main chip1 as **cJSON strings**.
It is **not** connected to the motor/wheel power rails — only sensing and safety.

---

## (1) WHAT MCU + PERIPHERALS

**MCU: GD32F4xx (GD32F407/F405 class), the same chip family as chip1.**
Basis (`memory_map.txt`): a full set of F4 peripherals — ENET_MAC/DMA/PTP, USBFS (GLOBAL/HOST/
DEVICE/PWRCLK), EXMC, CAN0+CAN1, **three ADCs** (ADC0 @0x40012400, ADC1 @0x40012800, ADC2 @0x40013c00),
DAC @0x40007400, SPI0..2, USART0..2 + UART3..4, I2C0/I2C1, timers TIMER0..13, RCU/FMC/CRC, NVIC.
Flash region `08000000–0803ffff` (256 KB). Dump: SP=`0x20000988`, reset vector=`0x080001b4`
(FUN_080001b4). SRAM `20000000–20017fff` (96 KB).

**Software (per embedded source paths `..\src\...`):**
- RTOS: **FreeRTOS** (`platform\freertos\{tasks,queue,port,heap_4}.c`).
- Log: **EasyLogger v2.2.99** (`platform\easylogger\elog*.c`) — `elog init success`.
- Link serialization: **cJSON** (formatters `%1.15g`/`%1.17g`, Create/AddItem/Print/Delete).
- Safety: **IEC60730** self-test (CPU / FLASH-CRC32 / clock / **ADC** / RAM, PreRun+Run-Time).
- Application modules: `application\{rw_bdboard_init,process_deal_board,process_comm}.c`,
  `driver\driver_board_sensor\driver_bdsensor.c`, `driver\driver_port\driver_mboard_port_snk_v2.c`.

**What the peripherals engage:**
- **ADC + DMA** — the border-signal receiver. `FUN_08019d34` configures the ADC+DMA channel
  (config struct: 0x200, `base voltage`=0x800=**2048** by default, 0x2000; on error —
  string `base voltage =%d error, set default 2048`), sets NVIC priority for IRQ **0xB**.
  `FUN_08019e34`/`FUN_08019eb8` allocate sample buffers by field `+0x92` (**800** or **235**
  samples) — these are the "wave" capture windows.
- **UART + DMA** — the link with chip1 (see section 3). GPIO/AF is configured by `FUN_08019bf4`
  (two GPIO ports `DAT_08019cf0`/`DAT_08019cf4`, pin masks 0x60000/0xe0000/0x7000).
- **Timer** — "timer driver" (`FUN_08019340`), periodic launch of data collection.
- **GPIO lift** — "lift driver" (`FUN_080192c8`), inputs of the lift sensors (left/right).

---

## (2) FUNCTION MAP  FUN_ → name → action

| FUN_ | name (by strings) | action |
|---|---|---|
| FUN_080001b4 | Reset_Handler | entry point (reset vector) |
| FUN_080175e8 | **init_task / rw_bdboard_init** | main init: malloc, calls init_all_driver, logs `hardwave version=%d, software version=%d`, creates processes init_board (id 0x17) and comm (id 0x14); on error — infinite loop with `init error=%d` |
| FUN_0801715c | **init_all_driver** | registers+initializes drivers: timer(FUN_08019340) → board_sensor(FUN_08019240) → port(FUN_08019300) → lift(FUN_080192c8) → factory(FUN_08019288); each = object with vtable {init,…,start}; on failure — `init … driver failed` |
| FUN_08019240 | get board_sensor driver | border ADC-driver object |
| FUN_08019300 | get port(mport) driver | UART-link object |
| FUN_080192c8 | get lift driver | lift-sensor object |
| FUN_08019340 | get timer driver | collection-timer object |
| FUN_08019d34 | **bdsensor_init (ADC+DMA)** | ADC+DMA config, base voltage 2048, NVIC IRQ 0xB |
| FUN_08019e34/eb8 | alloc wave buffers | sample buffers (800 / 235) for wave capture |
| FUN_08019bf4 | **mport GPIO/AF init** | AF setup of UART TX/RX pins + DMA |
| FUN_08019a18 | **send_string_dpport_snk_v2** | TX ≤128 B over DMA-UART; mutex (FUN_0801b246 take/FUN_0801b232 give, obj+6); memset 0x80, memcpy(FUN_08010912), DMA: FUN_08013d96 en-channel → FUN_08013f74 set-count → FUN_08013db6 start; errors `send len=0`/`wait send string overtime`/`send string length=%d too long` |
| FUN_08019cc? (FUN_080199cc) | port TX wrapper | forms and emits the string via the port vtable |
| ~lines 14060–14232 | **build_border_message (cJSON)** | assembles JSON: left/right `wave` (area+direction cVar1∈{0,1}), lift-state, versions → serializes (FUN_08012fd4) → sends; errors `create cjson config data failed`, `left wave data error, area=%d, str=%d` |
| ~lines 7500+ | **deal_lift_sensor** | reads lift sensors → `left lifted`/`right lifted`/`both lifted`/`lift none` |
| FUN_08012e20 | cJSON_CreateObject | — |
| FUN_08012d78 | cJSON_AddItemToObject | add field |
| FUN_08010e52 / FUN_08010e74 | cJSON_CreateNumber (int/bool) | border numbers |
| FUN_08012fd4 / FUN_08012e38 | cJSON_PrintUnformatted / Delete | serialize/free |
| FUN_080141e8 | **elog_output (log wrapper)** | (indegree 18) all `..\src\...` logs |
| FUN_08011a1a | log/printf core | (indegree 27) |
| ~lines 15100+ | **IEC60730_selftest** | CPU/FLASH-CRC32/clock/`adc test`/RAM tests; on failure `====== IEC60730 Test Fail ======` |

Process/function names are listed in a string table in the dump @`0801c910`+: `get_process_comm`,
`comm_task_init`, `comm_task`, `deal_lift_sensor`, `send_border_message`, `send_version_message`,
`send_seach_border_message`, `send_follow_border_message`, `port_receive_callback`,
`init_board_task`, `follow_border_check_wave`, `start_collect_data`,
`delay_and_start_next_collect_data`, `follow_check_subwave`, `bdsensor_init`,
`send_string_dpport_snk_v2`.

---

## (3) LINK WITH CHIP1  (bus / format / data)

**Bus:** asynchronous **UART** (half-duplex-like, DMA on TX; chip2 side —
`driver_mboard_port_snk_v2.c` = "mport"). The exact USART/pins are abstracted behind the
driver object in the decompilation (bases come in as a parameter); for the physical layer see the GPIO-init `FUN_08019bf4` +
`reference/FACTORY-GPIO-PERIPHERAL-MAP.md`.

**Frame format:** **cJSON string** (text), frame ≤ **128 bytes** (hard limit in TX
`FUN_08019a18`, on overrun — `send string length=%d too long`). Both sides use cJSON
(on chip1 also `create cjson config data failed`).

**Master = chip1.** On the chip1 side this is the **bdport** subsystem (`service_bdport.c` /
`driver_bdport.c` / `deal_message.c`, tags `bdport srv`/`bdport drv`/`dpport drv`). chip1 is the
initiator, chip2 is the slave sensor.

**Directions and content (by chip1 strings):**
- **chip1 → chip2 (commands):** `bdboard start search border` (+`becauseof %d`),
  `bdboard reset by flag=%d`, version request (`set bdboard version`), follow-border.
- **chip2 → chip1 (data/responses):**
  - version on connect: chip1 reads `bdboard hardware version=%d, software version=%d`
    (chip2 logs its own `hardwave version=%d, software version=%d` in init_task);
  - border data: `bdboard find border`; JSON with left/right `wave` (area + direction sign),
    lift state;
  - timeouts/connectivity: chip1 records `bdboard send overtime`, `bdport service receive overtime`,
    `bdboard disconnect`, `reconnect bdport`.

In sum: **chip1 (bdport, master) ⇄ UART/cJSON ⇄ chip2 (mport, sensor-slave).** chip2 continuously
digitizes the perimeter wire, finds the border "wave", and on command/periodically sends chip1 the
border+lift detection result; chip1 builds "search/follow border" navigation on top of this.

---

## (4) OPERATION TIMELINE

1. **Reset** → `FUN_080001b4` → clock/RCU, **IEC60730 PreRun** (CPU, FLASH-CRC32, clock, ADC, RAM);
   on failure — `IEC60730 Test Fail` / hang.
2. **FreeRTOS** start → task `initial` (@`0801884f`) → **`init_task`/`rw_bdboard_init`**
   (`FUN_080175e8`): malloc pri-data, log versions.
3. **`init_all_driver`** (`FUN_0801715c`): timer → **board_sensor(ADC+DMA)** → **port(UART+DMA)** →
   **lift(GPIO)** → factory. `bdsensor_init` (`FUN_08019d34`): base voltage=2048, IRQ 0xB.
4. Processes **init_board (0x17)** and **comm (0x14)** are created.
5. **Work loop:**
   - `comm_task`: handshake with chip1 over UART (hand over version), receive commands
     (`port_receive_callback`), dispatch (`start search border` / `follow border` / `reset`).
   - `board_task`: `start_collect_data` → ADC+DMA accumulates a sample window (800/235) → `check_wave` /
     `follow_border_check_wave` / `follow_check_subwave` detects the border → `deal_lift_sensor`
     reads lift → **build_border_message (cJSON)** → **`send_string_dpport_snk_v2`** to chip1;
   - `delay_and_start_next_collect_data` → next iteration.
   - In the background: IEC60730 Run-Time (FLASH-CRC/RAM) periodically.

---

## (5) RELATION TO MOTOR POWER / CURRENT

**No direct involvement.** chip2 controls neither the 20U wheel rail, nor the A4963 drivers, nor the BMS —
its strings/peripherals contain neither `a4963`, nor `motor`, nor `bms`, nor power GPIO. It reads the **ADC
of the border signal** (not the motor current shunts) and the **GPIO of the lift sensors**.

Indirect safety factors (worth keeping in mind for "the wheels"):
- **Lift sensor**: chip2 reports `left/right/both lifted`. A lifted mower is a standard stop
  of blade/motion on the chip1 side. If lift is read as "lifted", chip1 may block the
  drive. Verify that lift=none during our tests.
- **FSM dependency on the bdport link**: chip1 logs `bdboard disconnect`/`reconnect bdport`/
  `send overtime`. If chip1 requires a live/valid bdboard before allowing motion, a dead
  UART link with chip2 could keep the FSM out of the driving state. BUT: under our fw the blade spins ⇒ the FSM
  reaches the working state ⇒ this is **not** the main wheel blocker. Leave it as a secondary
  check; priority stays on the 20U rail hardware / BMS discharge (see MEMORY).

**Verdict:** chip2 does not directly affect "why the wheels don't move"; it is a border+lift sensor.
Its only levers over the drive are the lift signal and the health of the bdport link, both secondary.
