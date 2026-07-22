# chip2 — инициализация и задачи (FreeRTOS)

Вывод `pdg` — доверяем. `⚑` = интерпретация (`ⓗ`). Имена `✔` из elog-тегов / строк.

## Цепочка старта
`reset 0x080001b4` → CRT (`fcn.080001b4` → `fcn.08000131`/`fcn.08000000`) → C-runtime → `init_task`/`main`.

`⚑ ⓗ` В прошивке есть строки задач/инициализаторов (elog-теги): `init_task`, `log_init`, `init_all_driver`, `create_board_task`, `create_comm_task`, `comm_task_init`, `comm_task`, `init_board_task`, `get_process_comm`, `bdsensor_init`. Плюс `..\src\platform\freertos\tasks.c` (`✔`) — подтверждает **FreeRTOS**. Часть теговых строк (`comm_task`, `create_*_task`, `init_task`, `bdsensor_init`) грузятся из nofunc-регионов (FreeRTOS передаёт entry как указатель в `xTaskCreate`, поэтому имя не всегда `ldr`-ится для лога) — привязать точный вход к каждому имени в этом заходе не удалось (`ⓗ`).

## `init_all_driver` — `fcn.0801715c`  (`✔` elog-тег)
`⚑` Поднимает драйверы через фабрики-геттеры + vtable-вызовы; при провале логирует `elog` уровня 2 (error) с номером строки `process`-файла.

**Доказательства (`✔`):**
- `fcn.08019340()` → объект, `(*obj[0])()` init, `(*obj[3])()` start (`ⓗ` драйвер №1).
- `fcn.08019240()` → драйвер №2 (init + проверка).
- `fcn.08019300()` → `(*obj[2])(1); (*obj[3])(1); (*obj[0])()` — драйвер с двумя enable-флагами (`ⓗ` вероятно АЦП/семплер).
- `fcn.080192c8()` → ещё один драйвер.
- Все ошибки → `fcn.080141e8(2, tag, file, ...)` = elog error.

```c
void fcn.0801715c(void)   // ✔ init_all_driver
{
    d = fcn.08019340(); if(!d) log_err(0xa7); else { if((*d[0])()) log_err(0xa4); (*d[3])(); }
    d = fcn.08019240(); if(!d) log_err(0xb0); else if((*d[0])()) log_err(0xad);
    d = fcn.08019300(); (*d[2])(1); (*d[3])(1); if((*d[0])()) log_err(0xbb);
    d = fcn.080192c8(); if((*d[0])()) log_err(0xc1);
    // ... далее ещё драйверы
}
```

## `get_process_comm` — `fcn.08016d20`  (`✔` elog-тег)
`⚑` Геттер синглтона process_comm (структура в RAM `0x20000070`, см. memory-map). Через него `send_border_message` и др. берут состояние катушек/lift для JSON.

## Реалтайм-конвейер (по данным 01–05)
`⚑ ⓗ` Периодический цикл (запуск от TIMER1 TRGO):
1. `start_collect_data` (`fcn.08019f84`) — DMA-набор ~сотен отсчётов АЦП обеих катушек. `✔`
2. `follow_border_check_wave` (`fcn.080164bc`) на каждую катушку → сегментация (`fcn.08015dcc`) → фичи (`fcn.08015c68`) → классификатор (`fcn.080159a0`) → код 0..8. `✔`
3. `follow_check_subwave` (`fcn.08016784`) → вторичные `sls`/`srs`. `✔`
4. `deal_lift_sensor` (`fcn.08013970`) → `lf`. `✔`
5. `send_border_message` (`0x080195c4`) → JSON в chip1 по USART2. `✔`
6. `delay_and_start_next_collect_data` (`fcn.08013bbc`) → следующий цикл. `✔`

`⚑ ⓗ` Разбиение по FreeRTOS-задачам (board_task семплинг/обработка vs comm_task отправка) — гипотеза из имён; точную привязку задач к функциям надо доснять по `xTaskCreate`.
