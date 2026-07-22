# UART: приём команд и диспетчер (chip1, GD32F305, Cortex-M4)

Бинарь: `/opt/mower/dist/gd32-mainboard-dump-v1.bin`
Инструмент: Ghidra-декомпилятор через radare2 `pdg` (проект `/opt/mower/tools/r2out/gd32.r2proj`, база 0x08000000).

Легенда: `✔` — подтверждено кодом/строкой; `ⓗ` — гипотеза, проверить на живом чипе (с доказательством: адрес/строка/вызов).

## Общая картина линка ✔

Три одинаковых JSON-линка на мейнборде (дисплей/ESP — dpport, граница — bdport, LED — ledport).
Кадр: `&`(0x26) + JSON + CRC-8 Dallas/Maxim(1B) + `#`(0x23).
Путь приёма (dpport): дефрейминг+CRC (`fcn.0806e938`) → `cJSON_Parse` (`fcn.08023fe6`) → `GetObjectItem("cmd")` (`fcn.08023f80`) → `category = valueint & 0xff000000` → линейный поиск в таблице `[category, handler]` → `blx handler(json_obj)`.

Хелперы cJSON (идентифицированы по коду/использованию):
- `fcn.08023fe6(char*)` = `cJSON_Parse` ✔ (берёт строку, `strlen` через `fcn.08018724`, аллоцирует и парсит).
- `fcn.08023f80(obj, "key")` = `cJSON_GetObjectItem` ✔ (возвращает item; `*(item+0x10)` = valuestring, `*(item+0x14)` = valueint).
- `fcn.08023e60(n)` = `cJSON_CreateNumber`, `fcn.08023dd4(obj,"key",item)` = `cJSON_AddItemToObject`, `fcn.08023ed8()` = `cJSON_CreateObject`, `fcn.08023f20/08023e48` = `cJSON_CreateFalse/True`, `fcn.08023f38` = `cJSON_Delete` — ⓗ по паттерну использования.
- `fcn.08048adc(level, fmt, file, ...)` = логгер (первый арг — уровень: 0 fatal/assert, 2 error, 3 warn). ⓗ

---

## fcn.080470f0 — RX-диспетчер порта дисплея (dpport) ✔

Строки рядом (`ps`): `"cmd"`, `"dpport drv"`, `..\src\platform\driver\driver_port\driver_dpport.c`, `"json get cmd failed"`, `"cjson parse failed"`.

```c
void fcn.080470f0(int param_1)

{
    int iVar1;
    int iVar2;
    int iVar3;
    code *pcVar4;

    if (param_1 != 0) {
        iVar1 = fcn.08023fe6();
        if (iVar1 != 0) {
            iVar2 = fcn.08023f80(iVar1,0x804716c);
            if (iVar2 == 0) {
                fcn.08048adc(3,0x80471c0,0x804718c,*0x8047188,0x348,0x8047174);
            }
            else {
                iVar3 = 0;
                do {
                    if (*(*(*0x8047170 + 4) + iVar3 * 8 + 0x60) == (*(iVar2 + 0x14) & 0xff000000)) {
                        pcVar4 = *(*(*0x8047170 + 4) + iVar3 * 8 + 100);
                        if (pcVar4 == NULL) {
                            halt_baddata();
                        }
                        (*pcVar4)(iVar1);
                        halt_baddata();
                    }
                    iVar3 = iVar3 + 1;
                } while (iVar3 < 0x10);
            }
            halt_baddata();
        }
        fcn.08048adc(3,0x80471c0,0x804718c,*0x8047188,0x34d,0x80471cc);
    }
    return;
}
```

Доказательства и разбор:
- `0x804716c = "cmd"` ✔ → `iVar2 = GetObjectItem(root, "cmd")`. Если 0 → лог `"json get cmd failed"` (0x8047174), строка 0x348.
- `iVar1 = cJSON_Parse(param_1)` ✔; если 0 → лог `"cjson parse failed"` (0x80471cc), строка 0x34d.
- `category = *(iVar2 + 0x14) & 0xff000000` ✔ — старший байт `valueint` = категория команды.
- Таблица: `*0x8047170` — указатель на объект драйвера; `[+4]` — база массива; шаг записи **8 байт**, поле `+0x60` = category, поле `+0x64`(100) = handler. Цикл до `0x10` = **16 слотов** ✔.
- Совпала категория → `(*handler)(iVar1)` — хендлеру передаётся распарсенный JSON-объект ✔ (не строка).
- ⓗ После вызова хендлера `halt_baddata()` — артефакт декомпилятора (плохой хвост), реально — return/выход из цикла.

---

## fcn.08020036 — регистрация хендлера в таблице ✔

Определена вручную (`af`), реальный пролог на 0x08020036 (push на +2), xref из `fcn.08020000`.

```c
ulong fcn.08020036(int16_t arg1)

{
    uint32_t uVar1;
    uint32_t in_r1;
    uint32_t uVar2;
    int iVar3;
    int iVar4;
    uint uVar5;

    uVar1 = arg1;
    uVar2 = in_r1 & uVar1;
    uVar5 = 0xffffffff;
    iVar4 = 0;
    iVar3 = *(*0x802009c + 4);
    do {
        if (*(iVar3 + iVar4 * 8 + 0x60) == uVar1) {
            *(iVar3 + iVar4 * 8 + 100) = uVar2;
            goto code_r0x08020096;
        }
        iVar4 = iVar4 + 1;
    } while (iVar4 < 0x10);
    iVar4 = 0;
    do {
        if (*(iVar3 + iVar4 * 8 + 0x60) == 0) {
            iVar3 = iVar3 + iVar4 * 8;
            *(iVar3 + 0x60) = uVar1;
            *(iVar3 + 100) = uVar2;
code_r0x08020096:
            uVar5 = 0;
            goto code_r0x0802008a;
        }
        iVar4 = iVar4 + 1;
    } while (iVar4 < 0x10);
    uVar2 = 0x235;
    fcn.08048adc(2,0x802010c,0x80200d8,*0x80200d4,0x235,0x80200a0,uVar1);
code_r0x0802008a:
    return CONCAT44(uVar2,uVar5);
}
```

Разбор ✔:
- Та же таблица, что читает dpport-диспетчер: шаг 8, `+0x60`=category, `+0x64`=handler, 16 слотов.
- Первый цикл: если category уже есть — **перезаписать handler** (`arg0`=category, `arg1`=handler-ptr).
- Второй цикл: найти пустой слот (category==0) и записать пару.
- Нет свободного слота → лог error (строка 0x235), возврат 0xffffffff. Успех → 0.
- ⓗ `uVar2 = in_r1 & arg1` — артефакт: реально handler берётся из r1 как есть, `&`-маскирование ложное (регистр не восстановлен).

---

## fcn.08063880 / fcn.0806396c — bdport / ledport диспетчеры ✔ (кратко)

Идентичны dpport по логике, но **другая раскладка таблицы**: шаг 8, `+0x50`=category, `+0x54`=handler, **5 слотов** (цикл `< 5`). Оба берут `GetObjectItem(root,"cmd")` (`ps` подтверждает строку `"cmd"` по 0x80638f8 и 0x80639e4) и сравнивают `valueint & 0xff000000`.

```c
// bdport 0x08063880 (ledport 0x0806396c — тождественна, свои указатели/строки)
iVar1 = fcn.08023fe6();               // cJSON_Parse
...
iVar2 = fcn.08023f80(iVar1,0x80638f8);// GetObjectItem "cmd"
iVar3 = 0;
do {
    if (*(*(*0x80638fc + 4) + iVar3 * 8 + 0x50) == (*(iVar2 + 0x14) & 0xff000000)) {
        pcVar4 = *(*(*0x80638fc + 4) + iVar3 * 8 + 0x54);
        if (pcVar4 == NULL) halt_baddata();
        (*pcVar4)(iVar1);
        halt_baddata();
    }
    iVar3 = iVar3 + 1;
} while (iVar3 < 5);
```

Вывод: три порта используют независимые таблицы хендлеров разного размера (dpport — 16, bd/led — по 5).

---

## fcn.080241d0 — CRC-8 Dallas/Maxim (poly 0x8C) ✔

```c
void fcn.080241d0(uint8_t *param_1,int param_2)

{
    uint32_t uVar1;
    uint8_t *puVar2;
    uint8_t uVar3;
    bool bVar4;

    uVar1 = 0;
    while (bVar4 = param_2 != 0, param_2 = param_2 + -1, bVar4) {
        puVar2 = param_1 + 1;
        uVar1 = uVar1 ^ *param_1;
        uVar3 = 0;
        do {
            if ((uVar1 & 1) == 0) {
                uVar1 = uVar1 >> 1;
            }
            else {
                uVar1 = uVar1 >> 1 ^ 0x8c;
            }
            uVar3 = uVar3 + 1;
            param_1 = puVar2;
        } while (uVar3 < 8);
    }
    return;
}
```

✔ Классический bit-by-bit CRC-8/MAXIM: init=0x00, reflected, poly reflected = 0x8C, 8 итераций на байт. Возвращает CRC в r0 (тип void — артефакт; вызывающие берут r0). Длина `param_2`, буфер `param_1`.

---

## fcn.0801fb80 — add_crc_and_packed (TX-фреймер) ✔

```c
uint fcn.0801fb80(int param_1,int *param_2,uchar *param_3,int *param_4,int param_5)

{
    ...
    iVar5 = *param_2;                              // длина полезной нагрузки
    iVar3 = fcn.0801869c(param_3 + 1,param_1,iVar5); // memcpy(out+1, payload, len)
    if (iVar3 == 0) {
        fcn.08048adc(2,0x801fce0, ...);            // "... failed"
        uVar4 = 0xffffffff;
    }
    else {
        if (param_5 == 1) {                        // режим с CRC
            uVar2 = fcn.080241d0(param_1,iVar5);   // CRC-8 по payload
            *param_3 = 0x26;                       // '&'
            param_3[iVar5 + 2] = 0x23;             // '#'
            param_3[iVar5 + 1] = uVar2;            // CRC перед '#'
            iVar5 = iVar5 + 3;
        }
        else {                                     // режим без CRC
            *param_3 = 0x26;                       // '&'
            param_3[iVar5 + 1] = 0x23;             // '#'
            iVar5 = iVar5 + 2;
        }
        uVar4 = 0;
        *param_4 = iVar5;                          // итоговая длина кадра
    }
    return uVar4;
}
```

Разбор ✔:
- Собирает кадр `& <payload> [CRC] #`. `param_1`=payload, `*param_2`=len, `param_3`=выходной буфер, `*param_4`=итоговая длина, `param_5`=флаг CRC.
- `fcn.0801869c` = `memcpy` ✔ (копирует payload со смещением 1, оставляя место под `&`).
- `param_5==1`: добавляет CRC-8 (`fcn.080241d0`) байтом перед `#`, длина +3. Иначе только `&`/`#`, +2.
- Байты рамки: `0x26='&'`, `0x23='#'` ✔ — совпадает с описанием кадра.

---

## fcn.0806e938 — deal_receive_message (RX-дефреймер + проверка CRC) ✔

```c
uint fcn.0806e938(char *param_1,uint32_t *param_2,int param_3,int *param_4,int param_5)

{
    ...
    uVar6 = *param_2;                    // длина принятого буфера
    if (uVar6 < 4) { ...error 0x93...; return 0xffffffff; }

    if ((*param_1 == '&') && (param_1[uVar6 - 1] == '#')) {   // быстрый путь: рамка целиком
        if (param_5 == 1) {                                  // с CRC
            *param_4 = uVar6 - 3;
            uVar3 = fcn.080241d0(param_1 + 1);               // CRC по payload
            if (param_1[uVar6 - 2] != uVar3) return 0xffffffff; // сверка CRC
            iVar4 = fcn.0801869c(param_3,param_1 + 1,*param_4);  // memcpy payload
            if (iVar4 != 0) return 0;
            uVar5 = 0xa0;
        }
        else {                                               // без CRC
            *param_4 = uVar6 - 2;
            iVar4 = fcn.0801869c(param_3,param_1 + 1,uVar6 - 2);
            if (iVar4 != 0) return 0;
            uVar5 = 0xa8;
        }
    }
    else {                                // медленный путь: искать '&' ... '#' внутри буфера
        uVar3 = 0;
        if (param_5 == 1) {
            for (; (uVar3 < uVar6 && (param_1[uVar3] != '&')); uVar3 = uVar3 + 1) { }  // найти '&'
            uVar7 = uVar3;
            if (uVar6 - 2 <= uVar3) return 0xffffffff;
            do { uVar1 = uVar7; uVar7 = uVar1 + 1; if (uVar6 <= uVar7) break; }
            while (param_1[uVar7] != '#');                    // найти '#'
            if (uVar6 <= uVar7) return 0xffffffff;
            if (uVar1 + 2 <= uVar3 + 3) return 0xffffffff;
            *param_4 = (uVar7 - uVar3) + -2;
            uVar6 = fcn.080241d0(param_1 + uVar3 + 1);        // CRC по найденному payload
            if (param_1[uVar1] != uVar6) return 0xffffffff;   // сверка CRC
            iVar4 = fcn.0801869c(param_3,param_1 + uVar3 + 1,*param_4);
            if (iVar4 != 0) return 0;
            uVar5 = 0xcf;
        }
        else { /* то же без CRC, uVar5 = 0xf0 */ }
    }
    fcn.08048adc(2,0x806eb7c, ... ,uVar5, ...);  // лог ошибки с кодом стадии
    return 0xffffffff;
}
```

Разбор ✔:
- `param_1`=входной буфер, `*param_2`=его длина, `param_3`=выход payload, `*param_4`=длина payload, `param_5`=флаг CRC.
- Минимум 4 байта, иначе ошибка (стадия 0x93).
- Быстрый путь: буфер начинается с `&` и кончается `#`. С CRC (`param_5==1`): предпоследний байт — CRC, сверяется с `fcn.080241d0` по payload; успех → return 0.
- Медленный путь: сканирует `&`...`#` внутри мусора/склейки (half-open recovery), затем та же сверка CRC.
- Коды стадий (0xa0/0xa8/0xcf/0xf0) уходят в лог как диагностика при неудаче.

---

## fcn.08081244 — строитель STATUS-репорта ✔ (входная точка из задания 0x08081300 — середина этой функции)

Функция ссылается как **данные** из `fcn.080810e0` (т.е. лежит в таблице как handler-указатель). Читает большую структуру `param_1`, сравнивает с кэшем состояния (`*(iVar2+4)`), и только изменённые поля добавляет в JSON. Итоговый кадр несёт `"cmd"` = **0x50000000** (прочитано `pxw @ 0x8081650` = `0x50000000`) ✔ — это TX-репорт, а не значение 0x33000020.

```c
uint fcn.08081244(int *param_1)

{
    ...
    iVar2 = *0x808164c;
    if (param_1 == NULL) { ...error... ; uVar4 = 0xffffffff; }
    else {
        iVar3 = *(*0x808164c + 4);
        if ( /* хоть одно поле изменилось относительно кэша */ ) {
            iVar3 = fcn.08023ed8();                 // cJSON_CreateObject
            ...
            uVar4 = fcn.08023e60(*0x8081650);       // number(0x50000000)
            fcn.08023dd4(iVar3,0x8081658,uVar4);    // Add "cmd"
            if (*(*(iVar2+4)+0x28) != *param_1) {   // state
                *(*(iVar2+4)+0x28) = *param_1;
                uVar4 = fcn.08018eaa(*param_1);
                uVar4 = fcn.08023e60(uVar4);
                fcn.08023dd4(iVar3,0x808165c,uVar4);// Add "state"
                if (*(*(iVar2+4)+0x28) == 7) {      // state==7 → добавить "error"
                    ...
                    fcn.08023dd4(iVar3,0x80816ec,uVar4); // "error" = param_1[9]
                }
            }
            ... (аналогично по всем полям ниже) ...
            iVar5 = fcn.080240d4(iVar3);            // cJSON_PrintUnformatted
            fcn.08023f38(iVar3);                    // cJSON_Delete
            if (iVar5 != 0) {
                (**(*(*(iVar2+4)+0x10)+0x18))(iVar5); // отправить строку в порт (vtable+0x18)
                fcn.080240e0(iVar5);                  // free
            }
        }
        uVar4 = 0;
    }
    return uVar4;
}
```

Смещения структуры `param_1` → JSON-поле → кэш `*(iVar2+4)+off` (имена строк подтверждены `ps`) ✔:

| Поле JSON (строка) | Чтение из param_1 | Кэш-offset | Примечание |
|---|---|---|---|
| `cmd` (0x8081658) | const 0x50000000 (@0x8081650) | — | всегда |
| `state` (0x808165c) | `*param_1` (off 0) | +0x28 | |
| `error` (0x80816ec) | `param_1[9]` (off 0x24) | — | только если state==7 |
| `bat_lv` (0x80816f4) | `*(param_1+0x19)` байт | +0x41 | |
| `bat_per` (0x80816fc) | `*(param_1+0x22)` байт | +0x4a | |
| `rain_delay` (0x8081704) | `param_1[8]` (off 0x20) | +0x48 | |
| `rain_state` (0x8081710) | `param_1[1]` (off 4) | +0x2c | |
| `stop_state` (0x808171c) | `param_1[2]` (off 8) | +0x30 | |
| `border_state` (0x8081728) | `param_1[6]` (off 0x18) | +0x40 | через `fcn.08018e88` |
| `station` (0x8081738) | `*(param_1+0x1a)` байт | +0x42 | bool → CreateTrue/False |
| `bat_ctime` (0x8081740) | `param_1[0xb]` (off 0x2c) | +0x54 | |
| `bat_dtime` (0x808174c) | `param_1[0xc]` (off 0x30) | +0x58 | |
| `on_minutes` (0x8081758) | `param_1[0xd]` (off 0x34) | +0x5c | |
| `bat_health` (0x8081764) | `*(param_1+0x1b)` байт | +0x43 | |
| `work_area` (0x8081770) | `param_1[0x12]` (off 0x48) | +0x70 | |
| `total_minutes` (0x808177c) | `param_1[0xe]` (off 0x38) | +0x60 | ниже вложены cut/current/cur |
| `cut_area` (0x808178c) | `param_1[0xf]` (off 0x3c) | +0x64 | |
| `current_area` (0x8081798) | `param_1[0x10]` (off 0x40) | +0x68 | значение * 10.0 |
| `cur_minutes` (0x80817a8) | `param_1[0x11]` (off 0x44) | +0x6c | |
| `bat_max_temp` (0x80817b4) | `param_1[7]` (off 0x1c) int16 | +0x44 | пропуск если ==100; `fcn.08018e88` |
| `bat_min_temp` (0x8081800) | `*(param_1+0x1e)` int16 | +0x46 | пропуск если ==100; `fcn.08018e88` |

Также кэш обновляется без выдачи в JSON для полей `param_1[3]`(+0x34), `param_1[4]`(+0x38), `param_1[5]`(+0x3c), `*(param_1+0x1b)`(+0x43) — ⓗ внутренние счётчики/дубли.

Отправка: `(**(*(*(iVar2+4)+0x10)+0x18))(json_str)` — вызов через vtable драйвера порта (метод по смещению +0x18 = «send») ✔. `fcn.080240d4` = cJSON_PrintUnformatted, `fcn.080240e0` = free ⓗ.

Замечание ✔: репорт **инкрементальный** — поля добавляются только при отличии от кэша, поэтому реальный STATUS-кадр обычно содержит лишь изменившиеся ключи, а не весь список.

---

## fcn.08076b2c — service_config (обработчик конфиг-кадров) ✔ (структура + ключи)

Строки рядом: `..\src\framework\service_config\service_config.c`, `"config"`. Функция огромная (3788 байт, ~1155 строк дизасма) — привожу структуру диспетча и полный список ключей.

Структура диспетча ✔ (голова pdg):
```c
ulong fcn.08076b2c(uint param_1, ...)
{
    iVar3 = fcn.08023f80(param_1,0x8076ee4);   // GetObjectItem "config"/подобъект
    if (iVar3 != 0 && fcn.0807c9a0(*(iVar3+0x10)) != 0) { ...error "config sn=%s failed"... }
    iVar3 = fcn.08023f80(param_1,0x8076f3c);   // след. ключ
    if (iVar3 != 0) { if (*(iVar3+0x14) < 30000) {error} else {...} }  // проверка диапазона
    ...
    iVar4 = fcn.08023f80(param_1,0x8076f9c...); // очередной ключ
    if (iVar4 != 0) { *(cfg+off) = *(iVar4+0x14); iVar8 = 0; }  // взять valueint, применить
    ...  // длинная цепочка одинаковых блоков "GetObjectItem(key) → если есть, применить valueint/valuestring"
}
```
Паттерн ✔: линейная последовательность `GetObjectItem(param_1, "<key>")`; если item != 0 — берётся `*(item+0x14)` (valueint) или `*(item+0x10)` (valuestring) и пишется в структуру конфигурации `*0x8076f9c`(+offset); часть ключей валидируется (диапазон/enum) с логом `config <...>=%d value error/failed`. Это НЕ таблица хендлеров, а «плоский» разбор известных ключей.

Полный список ключей конфигурации (из строковых ссылок функции), ~110 шт. ✔:

`sn`, `pdt_ver`, `type`, `type_name`, `available`, `platform`, `region`, `type_param`, `mb_hv`,
`pwd_en`, `rain_en`, `fslip_en`, `ult_en`, `led_en`, `gps_en`, `sch_en`, `zone_en`, `com_en`, `hit_en`, `lift_en`, `mems_en`, `border_en`, `mtrack_en`, `wlch_en`, `auto_off_en`, `blade_check_en`, `speaker_en`, `smooth_en`, `rtt_en`, `qdock_en`, `area_limit_en`, `map_update_en`, `rb_en_mag`, `lboard_en`,
`perimeter`, `ratio`, `wheel_dis`, `blade_diameter`, `bmotor_polar`, `cmotor_polar`, `blade_sp`, `blade_max_sp`, `station`, `shape_param`,
`ota`, `robot_name`, `area`, `current_area`, `map_flag`, `bat_times`, `charge_min`, `work_min`, `ota_failed_count`, `current_time`, `current_cut_area`,
`run_param`, `exhib_mode`, `exhib_idle_sec`, `exhib_work_sec`, `exhibition_cfg`,
`pwd_rst`, `test_serve_add`, `border_offset`.

(Строки-«мусор» `,f\b\bspeaker_en`, `h\x01`, ` \x02` — артефакты выравнивания в таблице строк, не ключи.)

Валидируемые ключи (есть отдельный лог ошибки диапазона/enum) ✔: `sn`, `pdt_ver` (product version < min), `region`, `mb_hv`, `hit_en`, `lift_en`, `mems_en`, `border_en`, `mtrack_en`, `wlch_en`, `auto_off_en`, `blade_check_en`, `speaker_en`, `smooth_en`, `rtt_en`, `qdock_en`, `area_limit_en`, `map_update_en`, `ota`, `robot_name`, `map_flag`, `test_serve_add`, `pwd_rst` (сброс пароля, лог `reset pwd success/failed`).

---

## Итоговая таблица: cmd → хендлер

Механика ✔: `category = valueint("cmd") & 0xff000000`. Хендлеры кладутся в таблицу порта через `fcn.08020036(category, handler)`. Конкретные пары регистрируются в стороннем коде инициализации (не в этих функциях), поэтому большинство соответствий — ⓗ, кроме подтверждённых кодом значений.

| cmd (category) | Направление | Хендлер / смысл | Статус |
|---|---|---|---|
| `0x50000000` | MB → дисплей (TX) | STATUS-репорт, строит `fcn.08081244` (поля state/bat/area/…) | ✔ (const @0x8081650) |
| `0x33000020` | дисплей → MB (RX) | «запрос STATUS» → триггерит построение репорта 0x50000000 | ⓗ (из задания; в дизасме не найдена явная регистрация) |
| категория `& 0xff000000` | RX любой порт | линейный поиск по таблице, `blx handler(json_obj)` | ✔ (fcn.080470f0/08063880/0806396c) |
| config-кадр | RX | `fcn.08076b2c` разбирает ~110 ключей в структуру конфига | ✔ |

Таблицы хендлеров ✔:
- dpport: `*0x8047170`→[+4]=база, шаг 8, `+0x60`=category / `+0x64`=handler, **16 слотов**.
- bdport: `*0x80638fc`→[+4], шаг 8, `+0x50`/`+0x54`, **5 слотов**.
- ledport: `*0x80639e8`→[+4], шаг 8, `+0x50`/`+0x54`, **5 слотов**.
- Реестр: `fcn.08020036` пишет в таблицу dpport (`*0x802009c`→[+4], `+0x60`/`+0x64`, 16 слотов).

## Открытые вопросы (ⓗ)

- Значение 0x33000020: в декомпилированных функциях не встречено как константа — нужно найти место регистрации хендлера этой категории (grep по `izz`/`/x 20000033` little-endian) и подтвердить, что его хендлер вызывает `fcn.08081244`. ⓗ
- Точная семантика `param_5` (флаг CRC) у фреймера/дефреймера: какие порты передают 1, какие 0 — определяется вызывающими `fcn.0801fb80`/`fcn.0806e938`. ⓗ
- Поля STATUS `param_1[3]/[4]/[5]` обновляют кэш, но не эмитятся в JSON — назначение неясно (ⓗ внутренние счётчики).
- Идентификация vtable-метода порта `+0x18` как «send» — по контексту (передаётся готовая JSON-строка). ⓗ
- cJSON-функции (`fcn.08023dd4/e60/ed8/f20/e48/f38/240d4/240e0`) сопоставлены по паттерну использования, не по сигнатурам библиотеки. ⓗ
