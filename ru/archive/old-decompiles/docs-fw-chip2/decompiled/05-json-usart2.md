# chip2 → chip1: сборка и отправка JSON (border-сообщения, USART2)

Собрано из **дизассемблера** (доверяем) — `pdg` на этой функции частично ломается на VFP/coprocessor-инструкциях, поэтому JSON-часть восстановлена по чистому `pd`. `⚑` = интерпретация (`ⓗ`).

## `send_border_message` — вход `0x080195c4`  (`✔`)

`⚑` Строит cJSON-объект и шлёт его в chip1 по USART2 **голым, без рамки и без CRC**. Границей служит длина DMA-передачи.

**Доказательства (`✔`, из `pd @ 0x080195c4`):**
- `fcn.08012e20()` → `cJSON_CreateObject` (`ⓗ` по сигнатуре: возвращает объект, ветка `beq → "create cjson config data failed"`).
- Ключи добавляются через `fcn.08012d78(obj, key, <double в d0/s0>)` = **`cJSON_AddNumberToObject`** (`ⓗ` по сигнатуре):
  - `"cmd"` = **16** — `vldr` из пула `0x080197bc`, байты `00 00 80 41` = float `16.0` (`✔`).
  - `"ls"`  — `adr r1, "ls"` @ `0x08019672`, значение из `fcn.08010e52` (int→double, `__aeabi_i2d`) (`✔`).
  - `"rs"`  — `adr r1, "rs"` @ `0x080196ec` (`✔`).
  - `"lf"`  — `adr r1, "lf"` @ `0x08019702`; источник `[0x0801984c] = 0x20000070` = объект process_comm (`✔`).
  - `"sls"` — `adr r1, "sls"` @ `0x08019738` (`✔`).
  - `"srs"` — `adr r1, "srs"` @ `0x0801976e`, строка на `0x8019858` (`✔`).
- Отправка: `fcn.08012fd4(obj)` → сериализация в строку (`cJSON_PrintUnformatted`, `ⓗ`), затем `fcn.08016c98()`/vtable `+0x10` — драйвер вывода (USART2), `fcn.08012e38(obj)` = `cJSON_Delete` (`ⓗ`).
- Тег логгера при ошибке: `send_border_message`, файл `..\src\application\process_comm.c` (`✔`).

`⚑ ⓗ` Значения `ls`/`rs` — знаковые магнитуды основных катушек (из `follow_border_check_wave`), `sls`/`srs` — вторичные из `follow_check_subwave`, `lf` — уровень/флаг подъёма (lift) из `deal_lift_sensor`. Знак числа = сторона провода. Всё это — интерпретация из контекста; **в коде подтверждены только ключи, тип (число), cmd=16 и источники-регистры**.

```c
// Восстановлено из дизасма 0x080195c4..0x080197b8 (process_comm.c)
void send_border_message(void)   // ✔ вход 0x080195c4
{
    cJSON *o = cJSON_CreateObject();          // fcn.08012e20
    if (!o) { log("create cjson config data failed", ...); return; }

    cJSON_AddNumberToObject(o, "cmd", 16.0);  // ✔ float 16.0 @0x080197bc

    // ls: левая знаковая магнитуда (int → double), состояние катушки читается
    int ls = get_coil_state_L();              // fcn.08016ba8 ... ldrb [r0,1]
    cJSON_AddNumberToObject(o, "ls", (double)ls);   // ✔

    int rs = get_coil_state_R();
    cJSON_AddNumberToObject(o, "rs", (double)rs);   // ✔

    int lf = *(int*)0x20000070->lift;         // ✔ источник 0x20000070
    cJSON_AddNumberToObject(o, "lf", (double)lf);   // ✔

    cJSON_AddNumberToObject(o, "sls", (double)sls); // ✔ вторичная левая
    cJSON_AddNumberToObject(o, "srs", (double)srs); // ✔ вторичная правая

    char *s = cJSON_PrintUnformatted(o);      // fcn.08012fd4
    usart2_send(s);                           // fcn.08016c98 -> vtable[+0x10]
    cJSON_Delete(o);                          // fcn.08012e38
}
```

Итоговый кадр (`✔` по ключам/типам):
```json
{"cmd":16,"ls":<num>,"rs":<num>,"lf":<num>,"sls":<num>,"srs":<num>}
```

## Соседние отправители (тот же регион `fcn.08019914`)
- `send_seach_border_message` — тег @ `0x080199aa`; строит «search»-кадр (`ⓗ`, вероятно `{"cmd":26}`-семейство).
- `send_follow_border_message` — тег @ `0x080198a4`.
- `send_version_message` — тег @ `0x08019bbe`; version-кадр (`hv`/`sv`, `ⓗ`).

`⚑ ⓗ` Приём от chip1: в этом регионе **нет** `cJSON_Parse`/`GetObjectItem` — линк односторонний (датчик→мозг). Требует финального подтверждения обходом всех xref RX-IRQ USART2 (в этом заходе не делалось исчерпывающе).
