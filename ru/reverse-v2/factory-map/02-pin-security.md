# 02 — Кнопка питания, пинкод, разблокировка, связь с дисплеем (chip1, заводская)

Ветка «КНОПКА ПИТАНИЯ + ПИНКОД + разблокировка + дисплей». Реверс по
`reverse-v2/chip1/decompiled_all.c` (+ дизасм дампа `dist/gd32-mainboard-dump-v1.bin`
через `objcopy -I binary -O elf32-littlearm` для функций, которые Ghidra не разобрала).
Всё в flash `0x08000000`. Дата: 2026-07-08.

> **Главный вывод для расследования колёс:** весь тракт «кнопка → пин → разблокировка»
> — это чистый UART + cJSON + NVM. Он **не трогает ни одного силового GPIO/рейла, ни A4963,
> ни enable 20U**. См. раздел (5).

---

## (1) КАРТА ФУНКЦИЙ

### Кнопка питания / защёлки (src/user/key.c, power service)
| Адрес | Имя | Роль |
|---|---|---|
| `FUN_0800d034` | key_read | Читает кнопку: `FUN_0800cab2(port,0x400)` (маска бит10) с антидребезгом (>9 тиков). Возврат: 1=фронт-нажатие, 2=отпускание, 3=idle/удержание. |
| `FUN_0800cf50` | key_handler | По key_read: при state==3 (удержание) и счётчике>1 ставит флаг `|0x40`, лог **`key_press_power_on`** (`s_..._0800d01c`); при отпускании — `poweroff`/`key_press_down`. |
| `FUN_08055xxx` (power service) | — | Лог-теги `s_power_..._08055f78`, `button_driver_get_null`/`not_initialed` (@`0x08055f24`) — сервис кнопки/питания. |
| латч **PE12** OUT-PP HIGH | ГЛАВНАЯ защёлка питания | Ставится **бутлоадером**, app наследует. Само-удержание питания всей платы. |
| латч **PB0** OUT-PP HIGH | вторичная защёлка | Тоже bootloader-наследуемая. |

### Загрузка/точки входа
reset → `FUN_08019430` (C-runtime) → **main `FUN_08061714`** → `FUN_0807f010` (RCU+flash) →
FreeRTOS → task «initial» → **rw_init `FUN_080477c4`** (драйверы). PE12/PB0 уже HIGH к этому моменту.

### Связь с дисплеем (ESP32) — driver_dpport / UART
| Адрес | Имя (реверс) | Роль |
|---|---|---|
| `FUN_0804721c` | **dp_receive task** | RX-таск (имя задачи `"dp receive"`/строка `dpport_receive_task`@`0x0808864a`). Цикл: приём из очереди `FUN_08084...`, `malloc(0x100)`=`FUN_0807162c`, → `FUN_0803953c`. **(в decompiled_all.c ОТСУТСТВУЕТ — щель Ghidra 0x0804716c–0x08047470; разобрано дизасмом.)** |
| `FUN_0803953c` | dpport_deal_msg | Проверка длины (3<len<0x101). Если флаг=1 — **де-скрэмбл payload XOR `0x5b`** (байты 1..len-2). Затем → `FUN_0806e938`. |
| `FUN_0806e938` | dpport_frame_decode | Валидация кадра `&…#`, **CRC-8**, срез фрейминга → чистый payload. |
| `FUN_080241d0` / (двойник @~`0x08008378`) | crc8 | CRC-8, reflected poly **`0x8C`**, init 0. |
| `FUN_080470f0` | dpport_dispatch | **cJSON-парс** (`FUN_08023fe6`), берёт ключ `"cmd"` (`FUN_08023f80(json,&DAT_0804716c)`), диспатч по таблице 16 записей: `matcher@+0x60`, `handler@+0x64`, шаг 8, ключ = `cmd & 0xFF000000`. Затем `cJSON_Delete`. |
| `FUN_08009d7c` | dpport_frame_wrap (TX) | Формирует исходящий кадр: `buf[0]=0x26`, payload, `buf[len+1]=0x23`. |
| `FUN_08072e08` | dpport_reply | Собирает ответный cJSON `{<id>, "result": <bool>}` и шлёт дисплею (vtable+0x18). |

### Пароль / пинкод
| Адрес | Имя | Роль |
|---|---|---|
| `FUN_08044c80` (size 6676) | **userset service** = handler класса cmd `0x30xxxxxx` | Большой switch по полному значению `cmd`; сюда попадает ввод пина. |
| `FUN_08060d00` | load_user_password | Читает пароль из NVM в `DAT_08060d38` (`DAT_08060d34`=длина, 4 байта). Ошибка → лог `load_user_password_failed`. |
| `FUN_0807bf20` | set_password_error_count | Пишет счётчик ошибок пина в NVM «run_param», смещение `+0xb8`. |
| хранилище `DAT_08060d38` | user_password | Загруженный пароль. |
| vtable userset (`FUN_08050578`) | — | `+0x16c` get error-count, `+0x170` set count, `+0x17c` lock, `+0x258`(600) get stored pwd. |

### FSM-сеттер
| Адрес | Имя | Роль |
|---|---|---|
| `FUN_0807976c` | **fsm_set_state** | `if(state<0xb) *(DAT_08079778+0x10)=state;` — пишет глоб. состояние робота. |
| `FUN_08049d14` | fsm_set_state_wrap | `FUN_0804b190(); FUN_0807976c(param);` — тонкая обёртка. |

Состояния: **3=idle/unlock, 4=departure, 5=cutting, 6/7=docking-варианты, 8=battery-low, 9=move/search, 10=docked/charge**.

---

## (2) ТАЙМЛАЙН «кнопка → включилось → пин → сверка → разблокировка (state 3)»

1. **Нажатие кнопки.** `FUN_0800d034` читает вход GPIO (маска `0x400`=бит10) с антидребезгом;
   `FUN_0800cf50` при удержании (state 3) ставит флаг `|0x40` и логирует `key_press_power_on`.
2. **Само-удержание питания.** Бутлоадер поднял **PE12 HIGH** (главная защёлка) + **PB0 HIGH**
   (вторичная) — плата держит питание сама. App это наследует, GPIO защёлок не переставляет.
3. **Boot app.** `FUN_08061714`(main) → RCU/flash → FreeRTOS → `FUN_080477c4`(rw_init):
   поднимаются драйверы, в т.ч. dpport (UART к дисплею) и userset. Пароль грузится
   `FUN_08060d00 → DAT_08060d38`.
4. **Дисплей (ESP32) просит пин.** Обмен по UART кадрами dpport (раздел 3). Пользователь
   вводит пин на дисплее.
5. **Дисплей шлёт пин chip1.** Кадр `0x26 | <cJSON {"cmd":0x30000009, "<pwd-key>":<pin>}> | CRC8 | 0x23`.
6. **Приём chip1:** `FUN_0804721c`(dp_receive) → `FUN_0803953c` (при флаге — XOR `0x5b`) →
   `FUN_0806e938` (проверка `&…#` + CRC-8) → `FUN_080470f0` (cJSON-парс, `cmd`, диспатч по
   `cmd&0xFF000000`=`0x30000000` → **`FUN_08044c80`**).
7. **Сверка пина** (`FUN_08044c80`, case `0x30000009`):
   - `iVar19 = cJSON_GetObjectItem(json, DAT_080450b4)` — введённый пин;
   - `iVar9 = vtable[+0x258]()` — сохранённый пароль;
   - **совпало** → `FUN_08072e08(id,0)` (ответ дисплею `result=true`); сброс счётчика ошибок;
     лог `compare_pwd_correct` (`s_..._080450e8`);
   - **не совпало** → инкремент счётчика (`vtable[+0x170]`); если `>9` → **lock**:
     `vtable[+0x17c](0,cnt)`, `FUN_08050448()[+0x14](0x20)` (событие 0x20), лог
     `compare_pwd_uncorrect ... overtime`; иначе лог `compare_pwd_uncorrect=%d`, ответ `result=false`.
8. **Разблокировка (state 3).** Пин-путь сам НЕ дёргает FSM — он лишь шлёт `result` дисплею и
   снимает security-флаг. Реальный переход в **state 3 (idle/unlock)** делает процесс-слой:
   `FUN_08069848` (process_find_bd), строка 77890 `FUN_08049d14(3)` — при неотрицательном
   сигнальном бите во время старта/поиска базы. Лог `Robot unlock success` (@`0x0806afc8`)
   печатает security-сервис (функция в щели Ghidra, xref через base+offset, не литпул).

> Т.е. пинкод-сверка — это **request/response** поверх JSON-протокола дисплея; смена
> состояния робота — отдельно, в app-FSM, по факту снятия защиты.

---

## (3) ПРОТОКОЛ dpport (кадр + таблица opcode)

### Кадр (байты между chip1 ↔ дисплей)
```
+------+------------------+--------+------+
| 0x26 |  payload[N]      | CRC8   | 0x23 |
| '&'  |  (ASCII cJSON)   | 1 байт | '#'  |
+------+------------------+--------+------+
общая длина = N+3,  максимум 0x100 (256) байт
```
- **Header** `0x26` ('&'), **Trailer** `0x23` ('#').
- **CRC8** = `FUN_080241d0`: reflected poly **`0x8C`**, init 0, считается по `payload` (без хедера/CRC/трейлера), лежит одним байтом ПЕРЕД `#`.
- **Опц. скрэмблинг**: при флаге режима payload XOR `0x5b` (байты 1..N) — де-скрэмбл в `FUN_0803953c`. Есть и режим без CRC/скрэмбла (param_5≠1 в `FUN_0806e938`): тогда payload = len-2, CRC не проверяется.
- `FUN_0806e938` также умеет искать `&…#` внутри «грязного» буфера (скан старт/стоп).
- **Payload = cJSON-строка**, обязательное поле `"cmd"` (u32). Старший байт `cmd` (`&0xFF000000`) выбирает handler в таблице из **16 записей** (`matcher@+0x60`, `handler@+0x64`, шаг 8). Класс `0x30xxxxxx` → `FUN_08044c80` (userset). Прочие классы (0x10/0x20/0x40…) → другие сервисы (bd/battery/map/query); их регистрация — статический init-массив, рантайм-указатель `_DAT_08047170`.

### Команды дисплея класса 0x30 (userset, `FUN_08044c80`)
| cmd | Смысл | Действие / лог-строка |
|---|---|---|
| `0x30000005` | (query/reset-ветка) | `LAB_080459cc` |
| `0x30000006` | **into manual set** | `*(struct+4)=0`, лог `into_manual_set` |
| `0x30000007` | **quit manual set** | `*(struct+4)=0xff`, лог `quit_manual_set` |
| `0x30000009` | **compare_pwd (ввод пина)** | сверка пина, ответ `result`, счётчик/lock |
| `0x30000010` | **set_pwd** (новый пароль) | `vtable[+0x25c](pwd)`, `set_pwd_success/failed` |
| `0x30000011` | set (u16 param, vtable+0x10) | — |
| `0x30000012`–`0x30000014` | **set_start_time** | час старта расписания, `set_start_time_success hour=%d` |
| `0x30000015` | **set_work_time** | минуты работы, `set_work_time %d minutes success` |
| `0x30000016`–`0x3000001d` | настройки | различные set-команды |
| `0x3000001f` | **set_pwd_old** (смена пароля со старым) | `compare_pwd_old_success` / `set_pwd_old_failed because input old password error` |
| `0x30000020`–`0x30000023` | **reset_pwd** | сброс пароля, `reset_pwd_success/failed` |
| default | — | `undefine command=%d` |

### Команды chip1 → дисплей (TX)
- Ответ на команду: `FUN_08072e08` → cJSON `{<id>, "result": true|false}` (`s_result_08072e8c`), обёртка `FUN_08009d7c`.
- Телеметрия/состояние: периодические send-и (класс отдельного сервиса), напр. `send cmd into charge`, `send cmd exit charge`, `error send battery state cmd` (@`0x08072a37`/`0x08072a84`).

---

## (4) КЛЮЧЕВОЙ КОД (аннотировано)

### Декод кадра + CRC — `FUN_0806e938`
```c
uVar6 = *param_2;                       // len
if (uVar6 < 4) return -1;               // слишком короткий
if (*param_1=='&' && param_1[uVar6-1]=='#') {   // 0x26 ... 0x23
  if (param_5 == 1) {                   // режим с CRC
    *param_4 = uVar6 - 3;               // длина payload
    uVar3 = FUN_080241d0(param_1+1);    // CRC8(payload), poly 0x8C
    if ((byte)param_1[uVar6-2] != uVar3) return -1;   // байт перед '#'
    memcpy(param_3, param_1+1, *param_4);             // чистый payload
    return 0;
  } else { *param_4=uVar6-2; memcpy(param_3,param_1+1,uVar6-2); return 0; }
}
// иначе: скан '&' ... '#' внутри буфера (тот же CRC-механизм)
```

### Диспатч по opcode — `FUN_080470f0`
```c
iVar1 = cJSON_Parse(payload);                   // FUN_08023fe6
iVar2 = cJSON_GetObjectItem(iVar1, "cmd");      // FUN_08023f80(&DAT_0804716c)
iVar4 = *(_DAT_08047170 + 4);                   // база таблицы handler'ов
for (i=0; i<0x10; i++)                           // 16 записей
  if (*(iVar4 + i*8 + 0x60) == (cmd_val & 0xFF000000)) {  // matcher
     h = *(iVar4 + i*8 + 0x64);                  // handler
     if (h) h(iVar1);                            // вызвать с json
     break;
  }
cJSON_Delete(iVar1);                             // FUN_08023f38
```

### Сверка пина — `FUN_08044c80`, case `0x30000009`
```c
iVar19 = cJSON_GetObjectItem(json, DAT_080450b4);   // введённый пин
if (iVar19) {
  iVar9 = vtable[+0x258]();                          // сохранённый пароль (=600)
  if (*(iVar19+0x14) == iVar9) {                     // СОВПАЛО
     FUN_08072e08(id, 0);                            // reply result=true
     if (vtable[+0x16c]() == 0) log("compare_pwd_correct");
     ...
  } else {                                           // НЕ совпало
     FUN_08072e08(id, -1);                           // reply result=false
     cnt = (vtable[+0x16c]() + 1) & 0xff;
     if (cnt > 9) { vtable[+0x17c](0,cnt);           // LOCK
                    FUN_08050448()[+0x14](0x20);      // событие блокировки
                    log("compare_pwd_uncorrect overtime"); }
     else { vtable[+0x170](cnt); log("compare_pwd_uncorrect=%d"); }
  }
} else { FUN_08072e08(id,-1); log("compare_pwd_err"); }
```

### FSM-сеттер — `FUN_0807976c`
```c
void FUN_0807976c(uint state){ if(state<0xb) *(uint*)(DAT_08079778+0x10)=state; }
```

### Все места вызова FUN_0807976c / FUN_08049d14 и какие state ставят
| Функция | state | Условие |
|---|---|---|
| `FUN_08025be0` | 10 / 9 | dock-check ок → 10; иначе 9 |
| `FUN_08025c16` | 10 / param | dock-check |
| `FUN_080270a8` (scheduler) | **4** (×3) / 8 | 4 = departure (авто-отправка/расписание/ворота, safety ок); 8 = battery-low |
| `FUN_0802bce4` / `FUN_0803cb54` / `FUN_08049cfc` / `FUN_08083d9c` | 9 | безусловный move/error-fallback |
| `FUN_0802bd10` / `FUN_08032dd8` / `FUN_0803cb78` | param | passthrough |
| `FUN_080391e4` | 8 / 6 / 7 | dock-команды (idle/border) |
| `FUN_08054274` | **5** | cutting: счётчик @+0x42 < 0x40 |
| `FUN_08063c8c` | 9 | таймаут зоны |
| `FUN_08069848` (find_bd) | 10 / 10 / **3** / 7 / 7 | 3(unlock) @77890 = сигнальный бит не отрицателен; 10 = battery<3 / сигнал отрицателен; 7 = на границе |

> **state 4 (departure) / 5 (cutting)** — «поехали косить» — ставит ТОЛЬКО планировщик/процесс-слой
> (`FUN_080270a8`, `FUN_08054274`), **не** пин/дисплей-путь. Дисплей может лишь через
> userset-команды влиять на manual-set/расписание/пароль.

---

## (5) СВЯЗЬ С ПИТАНИЕМ КОЛЁС

- **Пин/дисплей/разблокировка НЕ трогают силовые рейлы.** Весь тракт `FUN_0804721c → FUN_0803953c
  → FUN_0806e938 → FUN_080470f0 → FUN_08044c80 → FUN_08072e08` — это UART-приём, CRC, cJSON, NVM
  и отправка ответа. Нет ни одной записи в GPIO OCTL/BOP, нет обращений к A4963/SPI1, нет enable 20U.
- **Защёлки питания PE12 (главная) + PB0 (вторичная)** — питают ВСЮ плату, ставятся бутлоадером,
  не драйв-специфичны. Кнопка (`FUN_0800d034`, вход маска `0x400`) только детектирует нажатие/
  удержание для power-on/off, латч сама не трогает (уже держится).
- **state 3 (unlock) ≠ включение колёсного рейла.** Разблокировка переводит FSM в idle; движение
  (state 4/5) инициирует планировщик. Ни один из этих переходов в разобранном коде не поднимает
  коммутируемый рейл 20U.
- Совпадает с общим статусом (`reports/WHEELS-STATE-2026-07-08.md`): enable рейла колёс — **не
  firmware-GPIO и не в ветке кнопка/пин/дисплей**. Эта ветка чиста от питания колёс.
- Побочно замечено (для power-трекинга, НЕ трогать): `PE0 HIGH` = «enable-событие» `FUN_08063368`;
  `PE7 HIGH` = aux-рейл; USART2/BMS keep-alive (`bms-usart2-spec`) — единственный силовой
  дельта-кандидат, но он вне этой ветки.

---
### Замечания по методу
- Функции `FUN_0804721c` (dp_receive) и security-сервис (`Robot unlock success`, `security_password_right`)
  ОТСУТСТВУЮТ в `decompiled_all.c` — Ghidra их не выделила (щели в function_index). Разобраны
  дизасмом дампа (`objcopy -O elf32-littlearm` + `objdump --disassembler-options=force-thumb`).
- Строка `dpport_receive_task`@`0x0808864a` — это ИМЯ задачи (rodata), а не адрес функции;
  реальная RX-функция = `FUN_0804721c` (указатель из `DAT_08058a3c=0x0804721d`).
