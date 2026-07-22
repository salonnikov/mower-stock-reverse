# CHIP1 function catalog (GD32F305, main controller)

> Dump: `dist/gd32-mainboard-dump-v1.bin`, base `0x08000000`. Decompilation: `reverse-v2/chip1/`.
> Total functions in `functions_index.csv`: **1931**. Attributed with proof: **~460** (332 via `__FILE__` logger strings + ~130 boot/easyflash/sfud/lib via their own strings). The remaining ~1470 are HAL/peripheral/math/veneers without strings (see the tail).
>
> **Attribution method [V]:** logger `FUN_08048adc(level, tag, __FILE__, __LINE__, fmt, ...)` — the 3rd argument is the source path `..\src\...\module.c`. Any function that calls the logger is tagged with its module **reliably [V]**. The specific *role* within the module is taken from the log string embedded in the symbol name (e.g. `s__create_motor_spi_mutex_failed_...`) — this is [V] by the fact the string exists, but the interpretation of the purpose is usually [I].
> **Confidence:** [V]=string/offset/code proves it; [I]=plausible from string/module; [U]=unclear.
> ⚠️ This is a dump of **bootloader + application together**. The range ~`0x08000000..0x0800c000` is bootloader/OTA/EasyFlash; above that is the application.

---

## 1. Boot / OTA / IAP / CRC (bootloader)

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_08007e30 | `boot_main` / load_app | MAIN boot logic: BACK_TO_FACTORY, version check of MB/BB/DB/LB/BTL, `no need load app, jump to app` / `iap download success, load app`, `load_retry_cnt`, `jump failed` | strings `user_src_boot_c`, `no_need_load_app__jump_to_app`, `factory_mode` | [V] |
| FUN_080012d0 | jump_to_app | hand off control to the application | `jump_to_app`, `jump_faid_` | [V] |
| FUN_08001370 | ota_cmd_handler | OTA command handler: start/get info/progress/set ver/set mode/set mark/set baudrate, checksum checks | ~20 `ota_*` strings | [V] |
| FUN_0800214c | firmware_info/env | tables `*_SIZE/*_VER/*_BVER/*_CRC/*_BRF` for MB/BB/DB/LB/BTL, `firmware size error`, `no need to update`, `SetFirmInfo`, `start erase flash` | many `*_CRC` strings | [V] |
| FUN_080024dc | get_bootloader_ver | `Bootloader version %d`, `MB_BTL` | string | [V] |
| FUN_080025f0 | it_c / power-on hook | `ota watchdog Triggered`, `usb_config_finish`, `@charge power on`, `gd32f30x_it.c` | strings | [V] |
| FUN_08003054 | iap_flash_write | IAP write of firmware to flash: `MB/BB/DB/BTL/LB IAP start`, `flash write %d/%d`, `flash write finish/error`, `file open/read error` | strings | [V] |
| FUN_0800355c | usb_disk_handler | USB disk: `USB disk Ready`, `ready to format flash`, parsing of `SNK_MB.bin/SNK_BB.bin/...`, `env_config.json` | strings | [V] |
| FUN_080027e0 | usb_flash_info | `USB set flash info`, `USB shake hands`, `USB program failed`, `USB read env/log` | strings | [V] |
| FUN_08004404 | boot_mode_select | mode selection: `into ota mode` / `usb host` / `usb device`, `no lb board`, `lboard_en/cfg_rst` | strings | [V] |
| FUN_08006658 | crc_verify_BB | CRC check of bank BB: `app_size:%d,CRC_value...`, `CRC check error: app CRC 0x%08x`, `BB_ack_time_out` | strings | [V] |
| FUN_080068f4 | crc_verify_DB | CRC of bank DB | `DB_SIZE/DB_CRC` + `app_size` | [V] |
| FUN_08006ac0 | crc_verify_DBL | CRC of bank DBL | `DBL_BRF` + `CRC check error` | [V] |
| FUN_08006d3c | crc_verify_LB | CRC of bank LB, `LB_ack_time_out` | strings | [V] |
| FUN_08006fe8 | crc_verify_MB | CRC of bank MB (main firmware) | `MB_SIZE/MB_CRC` + `app_size` | [V] |
| FUN_080026a0 / 080026d4 / 08002714 | crc_end_LB/DB/BB | final check `*_end / *_btl / *_crc = 0x%08x` | strings | [V] |
| FUN_08009738 | set_env_versions | ENV write: `set MB/BB/DB/LB/BTL env VER/BVER/SIZE`, `Set env flag cfgstr`, `env file parse error` | strings | [V] |
| FUN_08008cd0 | checksum | `checksum error, sum_a:%x, sum_b:%x` | string | [V] |
| FUN_080045e6 / 08004714 | ota_check_file | `ota check file complete failed`, `download %d success` | strings | [V] |
| FUN_0800779c / 08007814 | ota_status | `ota st %d per %d` (progress) | strings | [I] |
| FUN_0800796c / 080079ec / 08007afc | *_lost | `BB_lost / DBL_lost / LB_lost` (bank loss during OTA) | strings | [I] |
| FUN_08003cf4 | save_log_to_usb | `log_yyyymmdd_hhmmss.html`, `save log to usb disk` | strings | [V] |
| FUN_0800bfb0 | ota_program | `ota program %d/%d`, `ota program failed` | strings | [V] |
| FUN_0800d170 | log/config_parser | parser of config fields: `type_param, pdt_ver, user_name, blade_info, multizone, Start_Point, rain_set, ult_cfg, led_cfg`, days of week | strings | [I] |
| FUN_08003e24 | load_env_vars | `load Environment variables`, `env file read error/too large` | strings | [V] |
| FUN_08001b4c | reset_power_on | `reset power on` | string | [I] |

## 2. EasyFlash / ENV / KV store

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_0800a6f0 / FUN_08047c8c | easyflash_init | `EasyFlash V%s is initialize success/fail`, `4.0.99` (two copies: boot and app) | strings | [V] |
| FUN_08008f24 / FUN_0802b3e0 | env_set_check | `ENV name length is more than`, `ENV size is too big` | strings | [V] |
| FUN_0800a864 / 0800aa84 / 0800ae40 / 0800af14 / FUN_080480b4 / 080484e4 / 08047f54 | env_get/write | `ENV isn't initialize OK`, `ef_write_env_total %d` — ENV getters/setters | strings | [V] |
| FUN_08010e4c / FUN_0806cdcc | env_crc_check | `EF40Error: The ENV (@0x%08X) CRC32 check failed` | string | [V] |
| FUN_080150d8 | ef_write_env | `ef_write_env %s %d bytes` | string | [V] |
| FUN_0800a7b4 / 0800a998 / 0800aa14 / 0800aaec / FUN_08047ea4 / 0804811c | flash_area_ops | `Write data to application entry OK/fault`, `Erase backup/user application`, `All sector header check failed` | strings | [V] |
| FUN_0800ad18 / 0800adc4 | erase_report | `erase failed at 0x%08x`, `address at 0x%08x` | strings | [V] |
| FUN_0804853c | update_bl | `update bl failed, err erase/write` | string | [I] |
| FUN_0805d66c | fal/cache_read | read from the flash abstraction with fallback via func-ptr (in-deg 30) | code `(**(code**)(param_1+4))(...)` | [I] |

## 3. SFUD — external SPI-flash driver

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_08053874 | sfud_probe | chip detect: `Read flash device JEDEC ID`, `Find a %s flash chip, Size is %ld` | strings | [V] |
| FUN_0806c81c / 0806c8bc | sfud_jedec/sfdp | `Can't read JEDEC basic flash`, `Uniform 4KB erase support`, `Read address bytes error` | strings | [V] |
| FUN_0806cf8c | sfud_sfdp_hdr | `Can't read SFDP header`, `This reversion V%d.%d SFDP` | strings | [V] |
| FUN_08063428 / 0807dc18 / 0807dac0 / 0807d908 | sfud_write/erase | `Flash address is out of bound`, `Flash write SPI communicate error` | strings | [V] |
| FUN_0807d58c / 0807dc44 | sfud_wr_status | `Can't enable write status`, `Write status register failed` | strings | [V] |
| FUN_0807d7dc | sfud_chip_erase | `Flash chip erase SPI communicate` | string | [V] |
| FUN_0807d888 | sfud_device_init | `%s flash device is initialize success/fail` | string | [V] |
| FUN_0806eb98 | sfud_reset | `Flash device reset failed` | string | [V] |
| FUN_08083d34 | sfud_wait_busy | `Flash wait busy has an error` | string | [V] |

## 4. RTOS (FreeRTOS) and low-level primitives

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_08083478 / 0808340c | port.c assert | `Error %s %d` — interrupt priority checks (port.c) | `__FILE__=freertos/port.c` | [V] |
| FUN_08083358 | stack_overflow_hook | `task %s is stack overflow` | string | [V] |
| FUN_08071620 | jump-table thunk | `(*(code*)*DAT)()` — indirect dispatch (in-deg **60**) | code | [I] |
| FUN_0801869c | memcpy | word-optimized copy (in-deg 58) | code | [V] |
| FUN_080004f8 | memcpy (copy) | same body (in-deg 27) | code | [V] |
| FUN_0806c77c | set_bit / event_set | `*(base+n>>6) |= 1<<(n&0x1f)` — set a bit in a bitmap; called by codes (0x701,0x600) at motor init (in-deg 42) | code | [I] |
| FUN_08001cc8 → FUN_08001cde | vprintf-wrapper | va_list gathering, formatter call (in-deg 42) | code | [I] |
| FUN_0800111c | tick_delta | `(DAT+4) - param` — tick/timer difference | code | [I] |
| FUN_08053558 | reg/gpio_config | bitwise configuration of an array by indices 0x28/0x48 (in-deg 32) | code | [U] |
| FreeRTOS tasks.c (16 functions) | scheduler | functions with `__FILE__=freertos/tasks.c` (create/delete/notify/delay) | `__FILE__` | [V] |
| FreeRTOS queue.c (7 functions) | queues/semaphores | `__FILE__=freertos/queue.c` | `__FILE__` | [V] |
| FreeRTOS heap_4.c (2), port.c (3) | heap/port | `__FILE__` | [V] |

## 5. Logging / debugging

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_08048adc | **elog_output (logger)** | central log: `(level, tag, __FILE__, __LINE__, fmt, ...)` — **in-degree 331, #1 by call count** | `__FILE__=elog.c`, `%s has assert failed at %s ld` | [V] |
| elog.c (10 functions total) | easylogger | formatting/output/flash-log (elog/elog_flash/elog_port/elog_utils) | `__FILE__` | [V] |
| FUN_08029890 + cm_backtrace (6 functions) | cm_backtrace | HardFault parsing: `on_fault`, register dump, `%s 0x%08x` stack | `__FILE__=cm_backtrace.c` | [V] |
| FUN_080003c6 | fault_reg_dump | prints `R0..R12, LR, PC, PSR, BFAR, CFSR, HFSR, DFSR, SCB_SHCSR` | register strings | [V] |
| FUN_0800cd4c | log_html_format | HTML coloring of the log: `Black/DarkGray/FireBrick/...`, `eventLog/batLog/cutLog/errorLog` | strings | [I] |

---

## 6. DRIVE / motors (A4963, left/right/blade) — `driver_motor`

⚠️ Key unsolved topic of the project. BLDC driver on A4963 over SPI. See also `forced_functions.c` (FUN_0805f620 and neighbors — commutation computation).

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_0805aee0 | **left_motor_init/drive** | left motor init: `malloc pri left drv data failed`, `robot_platform=%d error`, `lmdrv`, `A4963_init_failed`; out-deg 19. From motor-investigation — central in the drive chain | strings | [V]mod / [I]role |
| FUN_0805f300 | left_motor_set_dir | `set left motore direction failed`, `lmdrv` | string | [V] |
| FUN_0806fb54 | right_motor_set_dir | `set right motore direction failed`, `rmdrv` | string | [V] |
| FUN_08022a80 | blade_set_dir | `set blade direction failed`, `bmdrv` | string | [V] |
| FUN_08022dc8 | blade_set_brake | `@set blade brake failed`, `bmdrv` | string | [V] |
| FUN_0801f2f4 | a4963_spi_init | `@create motor_spi_mutex failed`, `a4963` (SPI mutex creation) | string `a4963_snk_v2.c` | [V] |
| FUN_0801d45c | a4963_op | function in `a4963_snk_v2.c` (by `__FILE__`), purpose [U] | `__FILE__` | [I] |
| FUN_0805f620 (forced) | a4963_commutation | large computation over param×0x1c20/speed — commutation/speed (in forced_functions.c) | code | [U] |

> Observation [I]: in motor-investigation the A4963 reads as `0xFFFF` (no SPI response). The chain `FUN_0805aee0 → a4963 init → set dir → RUN` is present, but there is no physical motion — probably the RUN bit/enable is not set (see the separate investigation, not solved).

## 7. IMU / MEMS (TDK ICM-42688 + snk v13) — `driver_mems`

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_0802a3d0 | mems_snk13_init | `read mems id/set page/opr/acc cfg/gry cfg/axis map/power mode failed`, `mems13` — snk v13 sensor init | strings | [V] |
| FUN_0804e80c | mems_power_mode | `power model %d error`, `read mems opr mode`, `read mems stable value failed`, `mems13` | strings | [V] |
| FUN_0804bbbc | icm42688_fifo | `get data from fifo read/set reg failed`, `42688lib/42688drvhl` | strings | [V] |
| FUN_0807d65c | icm42688_init | `failed to initialize Icm426xx`, `42688lib` | string | [V] |
| FUN_0805b16c | imu_fusion_task | `imu_fuse`, `configure/setup inv device failed`, `init inv agm algo failed`, `create fuse task failed`, `tdk42688` — sensor-fusion task | strings | [V] |
| FUN_08083dac | tilt_sensor_init | `ERROR: robot tilt sensor initial...` | string | [I] |

## 8. Battery — `driver_battery` (snk v1/v2) and `service_bms`

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_0802440c / FUN_08024788 | battery_connect v1/v2 | `battery calculte flag is not true`, `battery connect failed/error, crc value` (two driver revisions) | strings | [V] |
| FUN_08020e54 / FUN_08021020 | battery_ocv v1/v2 | `bat voltage %dmV, ocv %dmV, diff...` — OCV calibration | strings | [V] |
| FUN_08058de0 / 0805929c / 08059ae0 | battery_drv_priv | `error data %d`, `malloc dat driver private data failed` | strings | [V] |
| FUN_08022fe4 | bms_battery_health | `sony/samsang/eve battery charge times`, `battery cell max/min` — wear accounting | strings | [V] |
| FUN_08024b04 | bms_model_select | `bms model %d error` | string | [V] |
| FUN_080279ac | bms_cell_protect | `cell over discharge/charge, voltage=%dmV` | strings | [V] |
| FUN_080500dc | bms_service_create | `create bms service failed` | string | [V] |
| FUN_08056980 | bms_service_init | `bms service already initialed`, `bms_data_pri` | string | [V] |
| FUN_08072948 | bms_send_charge | `send cmd into charge` | string | [V] |
| FUN_0806d800 | bms_change_health | `battery change times %d, health` | string | [V] |
| FUN_080231e0 / 08038c48 / 08059758 / 0805a050 / 08070bb8 / 08052368 | bms_* | connect drv null / used voltage point null / type not define / dock value / data_pri | strings | [V]mod |

## 9. Odometry / speed / motion — `service_movement`, `movebase`, `deal_movement`

> There is no explicit `odometry` module; odometry/tacho is computed in movebase (wheel perimeter/reduction) and in the high-speed loop.

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_08055f98 | movement_init | `movebase_srv is null`, `wheel_perimeter %d, redution ratio`, `station_type %d has no large`, `robot_platform %d` | strings | [V] |
| FUN_080515ec | movebase_create | `malloc movebase service/pri data failed`, `movebase` | strings | [V] |
| FUN_080517e8 | movement_create | `malloc movement srv/pri failed`, `create control mutex failed` | strings | [V] |
| FUN_08020164 | movebase_turn | `turn dir param err`, `movebase` | string | [V] |
| FUN_08020520 | movement_angle | `model error, angle could not change to tick`, `movement` | string | [V] |
| FUN_08078ee8 | set_movement_model | `change speed model error`, `set movement model input param error` | strings | [V] |
| FUN_0802a80c | pro_move_area | `get config service point is null, use default 500m2`, `pro_move` | strings | [V] |
| FUN_0805bfd0 | pro_move_deps | dependency check: hit/lift/rain/slope/bms/border service null | strings | [V] |
| FUN_08078300 / 08078390 / 0807cb68 | pro_move_setters | enable auto_poweroff / check charging current / station type | strings | [V] |

## 10. Fast control loop — `service_hs_loop`

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_0805115c | hs_loop_create | `create highspeed loop service failed` | string | [V] |
| FUN_08058504 | hs_loop_task | `create highspeed loop task failed`, `highspeed loop service already init` | strings | [V] |

## 11. Blade / mowing — `service_blade`, `process_cutting`

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_08022938 | blade_force | `force blade foreward/backward, speed=%d` | strings | [V] |
| FUN_0804ff60 | blade_service_create | `create blade service failed` | string | [V] |
| FUN_0807b624 | blade_set_info | `input info point is null` | string | [V]mod |
| FUN_08030128 | cutting_step (out-deg 33) | mowing step, `cutting` — large node of the cutting logic | `__FILE__=process_cutting.c` | [V]mod |
| FUN_0802bd4c / 0802c05c / 0802c69c / 0802f97c / 0802fa0c / 080718a0 | cutting_substeps | handling of cutting situations (`cutting`) | `__FILE__` | [V]mod |
| FUN_0802c424 | cutting_get_dir | `undeal get dir in situation hit` | string | [I] |
| FUN_0802fa90 | cutting_exhibition | `exhibition mode, disable blade` | string | [V] |
| FUN_08077e10 | cutting_departure | `step departure station` | string | [I] |

## 12. Border / bdport — `service_border`, `driver_port(bdport)`, `service_port`

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_0805034c | border_service_create | `create border service failed` (in-deg 20) | string | [V] |
| FUN_0804d4c8 | border_service | function of service_border.c, [U] role | `__FILE__` | [I] |
| FUN_0802abf4 | bdport_drv_alloc | `malloc bdport driver failed`, `bdport drv` | string | [V] |
| FUN_08063880 | bdport_drv | `bdport drv` (driver_bdport) | string | [V]mod |
| FUN_0801fb80 | deal_message | `message/new_message/deal_mes/memcpy failed` — port message assembly | `__FILE__=deal_message.c` | [V] |
| FUN_0803953c | port_recv_parse | `receive message len is over max`, `rec_mes/dealed_mes/deal_mes` | strings | [V] |
| FUN_08039e8c | port_pack | `message length too short/over max`, `add crc and packed err` | strings | [V] |
| FUN_08021bb4 | bdport_cb | `bdport callback message is null` | string | [V] |
| FUN_08022064 | bdboard_disconnect | `bdboard disconnect` | string | [V] |
| FUN_08056cbc / 080764fc / 08060714 | bdboard_versions | load/set bdboard hardware/software version | strings | [V]mod |
| FUN_0804d0f0 / 0804fe38 | bdport_srv | bdport service | strings | [V]mod |

## 13. Display board / dpport — `service_display`, `driver_port(dpport)`

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_08050d7c | display_service | `display` — large node of the display service | `__FILE__=service_display.c` | [V]mod |
| FUN_0803b85c / 0803baa0 / 0803bc44 / 0803bd50 / 0803c3dc / 0803c4e8 / 0803c5ec | display_msgs | building of messages to the display (`display`) | `__FILE__` | [V]mod |
| FUN_0803c2ec | display_pause | `pause` | string | [I] |
| FUN_08072134 / 08072230 / 08072304 / 08072400 | display_result | `result` (replies to the display) | strings | [I] |
| FUN_08029648 | dpport_data | `dpport_data_pri` | string | [V] |
| FUN_0807344c / 08072c94 / 08072d5c / 08078a1c | dpport_srv/drv | dpport service/driver | strings | [V]mod |
| FUN_08058808 | dpport_queue | `create dpport receive quequ failed` | string | [V] |
| FUN_080749bc / 08074390 / 08072ad8 | dpport_send | `dpport send str point is null`, `send string/data is null` | strings | [V] |
| FUN_080470f0 | dpport_json | `json get cmd failed` | string | [V] |

## 14. LED board / ledport — `service_led`, `driver_port(ledport)`

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_080512dc | led_service | `srv led` | string | [V] |
| FUN_08072fd4 | led_config_json | `create cjson config data failed` (led) | string | [V]mod |
| FUN_0802b564 | ledport_alloc | `malloc ledport driver failed` | string | [V] |
| FUN_0806396c | ledport_drv | `ledport drv` | string | [V] |
| FUN_0805e438 | ledport_cb | `ledport callback message is null` | string | [V] |
| FUN_080513d8 / 080731a8 | lport_srv | ledport service | strings | [V]mod |

## 15. Command dispatcher / user settings — `service_command`, `service_user_set`

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_08050448 | command_service_create | `create command service failed` | string | [V] |
| FUN_08052118 | command_data | `cmd_data_pri` | string | [V] |
| FUN_08076a5c | set_command | `set command error %d` | string | [V] |
| FUN_08044c80 | **user_set_dispatch** | HUGE parsing of commands from the display: schedule/pwd/time/name/sn/rain/multizone/led/ultrasonic/ota flag/wifi/blt state/manual set/reset — hundreds of `set_*`/`get_*` strings (out-deg 32) | strings | [V] |
| FUN_08052d34 | user_service_create | `create user service failed`, `user_data_pri` | string | [V] |
| FUN_08073a28 / 08073b38 / 08073f10 / 08080d44 / 08072e08 | user_json_builders | `create cjson config data failed`, `map_sn` — building of JSON replies | strings | [V]mod |
| FUN_08080ae4 / 080810e0 / 080819a4 / 080827b8 | user_get_config | led/rain/schedule/ultrasonic config point null | strings | [V] |

## 16. Config — `service_config`

31 functions. Core: load/save of all robot parameters from ENV/JSON.

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_08050578 | config_service_create | `create config service failed` (in-deg 37) | string | [V] |
| FUN_08054cc0 | config_load_all | startup load: exflash driver, `cfg_ver`, sn, ledboard enable, hit mode (torque/float), exhibition mode, all robot shape/type parameters, versions (out-deg 33) | many strings | [V] |
| FUN_08076b2c | config_apply_json | applying the product configuration: `pwd/rain/fslip/ult/led/gps/sch/zone/hit/lift/mems/border/...` enable flags, `perimeter/ratio/wheel_dis/blade_diameter/bmotor_polar` | strings | [V] |
| FUN_0808051c | config_bootloader_update | `reload/load mboard bootloader update flag`, `BTL_SIZE/CRC`, `update bootloader version success/failed` | strings | [V] |
| FUN_0802829c | config_external | `config update flag set`, `set external config failed`, `get config json string failed` | strings | [V] |
| FUN_08060228 / 0806031c / 0806038c / 08060404..08060d60 | config_getters (load_*) | loading of individual fields: run_param, mboard/bdboard/dpboard/ledboard hw/sw version, ota date/flag, shape/type param, user password/language/name, test server | strings | [V] |
| FUN_080607d0 / 08060924 / 0806f39c / 08077af0 / 08077c88 / 08078be8..0807d3ec | config_setters (set_*) | saving of fields: software/hardware version, run parameter, motor polar, blade diameter, robot type/platform/available, password, wheel distance, etc. | strings | [V] |
| FUN_0805bb4c | config_init_running | `initial running info failed`, `run_param` | string | [V] |

## 17. Application state machine — `process_*`

Each `process_*.c` is a state of the top-level FSM. The manager (`process_manager`) dispatches.

| group | functions | purpose | conf |
|---|---|---|---|
| process_manager | FUN_08024328, 08026ea8, 0802775c, 080286ec, 0804ef44, 08065440, 080716b4 | state dispatcher; `receive display board message error`, `get area, use default 500m2` | [V]mod |
| process_charging | FUN_08025c90, 08025fd4, 080265dc, 080267f8, 080656cc, 0807ff04 | charging: `bat full finish`, `changing current to low`, `normal/temp-protect/trickle charging step`, `battery temperature high`, `tricle charge overtime` | [V] |
| process_cutting | (see §11) | mowing | [V] |
| process_docking_smooth | FUN_0803cb94, 08041a14, 08041d7c, 080436f8..08044488, 0805e02c, 08067e78, 08068594 | docking: `follow border unnormal situation`, `dock step to follow border`, `wl dock connect success`, `brake overtime`, `receive station line err` | [V] |
| process_departure_smooth | FUN_08032e20, 0803482c, 08035010, 08036fe8, 080372b8..0803b64c, 0805dca0, 0805e194, 080679f8, 0808084c | leaving the station: `check far from station`, `departure rain and on border`, `record map len/heading/x`, `start blade lift → error`, `Robot manual power off`, `border length too short` | [V] |
| process_find_bd | FUN_0804b0b8, 08053224, 08069848 | border search (`find bd`) | [V]mod |
| process_wait | FUN_080270a8, 08027e1c, 080287a4, 080391e4, 080542bc, 08054438 | waiting: `ready to departure interrupt`, `check station power cut`, `dock command and in station` | [V] |
| process_control | FUN_0804dce4 | control state | [V]mod |
| process_security | FUN_0807202c | security state | [V]mod |
| process_ram | FUN_0805b5f8 | `led service point is null` (service checks) | [V]mod |
| deal_safety | FUN_08027c48, 08039824, 0804f6a4, 0804f758 | safety: `can not receive display/sensor/led board message error` — link watchdogs | [V] |
| deal_movement | (see §9) | motion glue | [V] |

## 18. Other services

| module | functions | purpose | conf |
|---|---|---|---|
| service_multizone (multizone) | FUN_08024e14, 08026b44, 0802924c, 08051ac0, 0806489c, 0807631c, 08076850, 08079064, 08079118, 080791e0, 08082460 | mowing zones: `start point percent error`, `multizone`, `multizone_ex`, `set multizone parameter input zone` | [V] |
| service_workmap / map | FUN_080202f8, 080203ac, 08024114, 08024200, 080256e4, 08027ff8, 08028970, 08029f04, 08052ed8, 08053754, 0805825c, 0805fef4, 08061878, 080619c8, 08061ba8, 0806d0d4, 0806d5ac, 08080ca0 | map building: `line pnt`, `calc brd obb`, `link fitted line point`, `self intersection pnt`, `map flash data`, `map version change`, `rebuild line remerge`, `record map pnt alloc` | [V] |
| service_time (schedule/RTC) | FUN_08020678, 08020878, 0802a2fc, 0804fce0, 080529f0, 0805594c, 08064d80, 0806f79c, 08070890, 08076950, 0807c434, 0807d330 | auto-schedule: `auto schedule work day`, `initial auto schedule success`, `avg work minutes`, `rtc driver not create`, `set calendar`, `input set schedule day value error` | [V] |
| service_rain (rain) | FUN_08051d24, 08052664, 08064ca8, 0806c3c0, 080711b8 | rain sensor: `create rain service failed`, `rain %s, rain delay %d minutes`, `rain_set`, `rain_data_pri` | [V] |
| service_slope (slope) | FUN_08028c70, 0802b868, 08051f2c | tilt: `mems pitch value error`, `avg pitch error`, `create slope service failed` (in-deg 25) | [V] |
| service_ultrasonic (sonar) | FUN_08025908, 08052c08, 08071374, 08074cc8 | ultrasonic sensor: `ultrasonic sensitivity %d error`, `ult srv`, `ultrasonic_data_pri` | [V] |
| service_hit / impact (torque_hit) | FUN_0805104c, 080525a4, 080710c8, 0807fa9c, 0807fcf8, + torque_hit/state_check: 4 functions | collision: `create hit service failed`, `hit_data_pri`, `unknown move state`, `torque hit checked/overtime`, `collision state check init failed`, `save hit env failed` | [V] |
| service_lift (lift) | FUN_08051508 | `create lift service failed` | [V] |
| service_slip (wheel slip) | FUN_08051e44 | `create slip service failed` | [V] |
| service_stop | FUN_080528fc, 0807eb7c | `press stop long cb is null` (STOP button) | [V] |
| service_power (power) | FUN_08051c04, 08055ce4, 08079924 | `power`, `left motor driver get null`, `set power model err` | [V] |
| service_connect | (1 function) | link service | [V]mod |
| rw_init (start/initialization) | FUN_0802a708, 080477c4, 08060dfc, 08060f24, 080615a0, 08072f00, 08074e28, 08083e34 | robot data initialization: `robot stored data initial failed`, `main board type unknown`, `motherboard flash init`, `ultrasonic driver init overtime`, `windows watch dog Reset occurred`, `highspeed loop service is null`, `display port service connect fail` | [V] |

---

## 19. cJSON / formatting utilities

| addr | name-guess | purpose | evidence | conf |
|---|---|---|---|---|
| FUN_08010558 / FUN_08065260 | json_utf_escape | `u%04x` — unicode escaping in cJSON | string | [I] |
| FUN_080106c8 / 08063b64 / 08065370 | json_literal | `false` (+ `-1.15g/-1.17g`) — printing of cJSON literals/numbers | strings | [I] |
| FUN_08005c48 / FUN_0801eba4 | hex_tables | `0123456789ABCDEF/abcdef` — hex conversion | strings | [V] |
| FUN_0800c7b0 | iso8601_time | `%04d-%02d-%02dT%02d:%02d:%02d` | string | [V] |
| FUN_080047d8 / FUN_0801de94 | segger_rtt | `Terminal/SEGGER` — RTT debug | strings | [I] |
| FUN_0804c8b0 / FUN_0806f65c / FUN_0806fa08 | misc | `obb_pnt %g %g`, `MyMower`, `revise close loop failed parameter` | strings | [U] |
| FUN_08063c8c | area_limit | `area_limit_overtime_error` | string | [I] |

---

## 20. NOT attributed

- **~1470 functions without string clues** — they neither call the logger nor contain text. These are predominantly:
  - GD32 HAL peripherals (RCU/GPIO/USART/SPI/I2C/TIMER/ADC/DMA/RTC/FMC register wrappers),
  - math/float runtime (`__aeabi_*`, divisions, trigonometry for map geometry/PID),
  - small struct getters/setters, thunk veneers, jump-table targets.
  - Identifiable only from the callgraph (`analysis/*-chipN.tsv`) and manual code reading — purpose **[U]**.
- **~130 functions with strings but not in the lists above** — distributed across §1–§19 (boot/easyflash/sfud/cjson/lib). Full raw dump «function→strings»: temporarily in `scratchpad/fstr_all.tsv` (do not commit).
- **High in-degree but [U] role:** FUN_08053558 (in-deg 32, bitwise register configuration), FUN_08023ed8/08023f38/08023dd4/08023e60/080240e0 (cluster 0x08023xxx, in-deg 20–32 — probably a shared allocator/service queue, [U]), FUN_0801da86 (in-deg 30, [U]).

### Caveats
- Module [V] means «the function belongs to this .c» (by `__FILE__`). The exact *role* within the module is [I] unless confirmed by a unique string.
- Some modules have 2 copies of the code (boot vs app: easyflash, hex tables) — these are not duplicate errors, but separate binary regions.
- `driver_motor` by `__FILE__` yields only 6 functions, but the real A4963 commutation logic is broader (`forced_functions.c`, cluster 0x0805f6xx–0x0806fxxx) — there are almost no strings there, so [U].
