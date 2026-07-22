# 05 — BMS / умный пак по USART2 (chip1, заводская fw)

Полный разбор заводского протокола USART2 ↔ батарейный пак. Источники: `chip1/decompiled_all.c`,
`strings.txt`, `symbols.txt`, дамп `dist/gd32-mainboard-dump-v1.bin`. Все кадры извлечены из
распакованной `.data` (SL-Type2 декомпрессор `FUN_080197b4`, дескриптор src=0x0808DA24 → dst=0x20000070,
size=0x6D0) и **проверены по CRC-8/MAXIM живьём в этой сессии** (см. §2). Connect-кадр совпал с ранее
подтверждённым SWD-чтением `0x200005E0` — распаковка верна.

> ★ ГЛАВНЫЙ ВЫВОД (см. §5): отдельной команды «включить разряд / подать питание на мотор-тап»
> в протоколе **НЕТ**. Все команды паку — это телеметрия, link-wake и режим **заряда** (charge).
> Все строки со словом «discharger» — это решения РОБОТА (safety), не команды паку.

---

## 1. КАРТА ФУНКЦИЙ

### Транспорт / низкий уровень (driver_battery_snk_v*.c)
| Функция | Роль |
|---|---|
| `FUN_080213c0` / `FUN_08021468` | init USART2 (baud/CTL, RCU, GPIO, NVIC IRQ39) |
| `FUN_080211f0(dir)` / `FUN_080212d8(dir)` | half-duplex переключение PD8: 1=TX, 0=RX (v1/v2) |
| `FUN_0807e00c` / `FUN_0807e050` | **TX connect-кадра** (6 байт, шаблон BASE+4) [v1/v2] |
| `FUN_08074da0` / `FUN_08074de4` | **TX «CE-wake»-кадра** (12 байт, шаблон BASE+0x36) [v1/v2] |
| `FUN_08058de0` / `FUN_0805929c` | **цикл опроса телеметрии**: шлёт C3 (pack msg, +0x0A) и 0x53 (cell msg, +0x2E), парсит |
| `FUN_0802ab98` | **CRC-8/MAXIM** (poly refl 0x8C, init 0). Таблица @дамп 0x88B3C. Покрывает LEN-1 байт (opcode+args) |
| `FUN_0802440c` / `FUN_08024788` | **парсер ответа** на C1 (маркер 0xC1), пересчёт V/I/T, статус |
| `FUN_08020e54` / `FUN_08021020` | **connect**: 4 попытки TX C1, при таймауте → CE-wake, парс, сверка V vs OCV |
| `FUN_08083178(base,bit)` | опрос STAT-бита (7=TBE, 6=TC, 5=RBNE, 4=IDLE) |
| `FUN_08082e3c(base,byte)` | запись байта в DATA (base+0x04) |
| driver-init (v1 @≈0x080599xx, зовёт connect@69558→poll@69560) | malloc ctx (magic 0x55AA@ctx+0x11w), RCU/GPIO/USART/NVIC, connect, initial poll |

### Сервис (service_bms.c)
| Функция | Роль |
|---|---|
| `FUN_08056…` («create bms task failed» @0x08056c9c) | enable-хэндлер: валидит enable(0/1)→ctx+0xa5, создаёт bms-task (prio 0x200), зовёт vtable +0x1c/+0x20 |
| `FUN_08072948` | **тик bms-task / диспетчер команд**: по битам ctx+0xa6 вызывает vtable +0x30..+0x40 (into/exit charge, state) |
| «set bms enable charge current=%d» @0x080492c4, «set disenable…success» @0x08049323 | установка **зарядного** тока (charge, не discharge); тело — вне декомпил-подмножества |
| строки @0x08072a37 `send cmd into charge`, @0x08072a84 `send cmd exit charge` | лог диспетчера |

**Структура ctx (BASE = 0x200005DC, v1):** [0]=ctx-ptr(malloc), +0x54=connect-статус(0/0xFF),
+0x55=RX-done-флаг(0xFF=ждём), +0x56=TX-len, +0x57=TX-idx, +0x58=TX-указатель на шаблон,
+0x5C=retry-cnt, +0x5D…=RX-буфер (0x40 байт), +0xA5=task-enable(0/1), +0xA6=битовая маска команд.

---

## 2. ТАБЛИЦА КОМАНД (host → pack) — все шаблоны из .data, CRC проверены

Заголовок запроса **`1C A1`**; ответ пака — **`3A A3`**. Формат: `1C A1 <LEN> <opcode> <args…> <CRC8>`,
где **LEN = число байт после LEN** (opcode+args+crc), **CRC8 = CRC-8/MAXIM по opcode+args**.

| Адрес (RAM) | Off | Кадр (hex) | Opcode | Назначение | Кто шлёт / когда |
|---|---|---|---|---|---|
| 0x200005E0 | +0x04 | `1C A1 03 C1 01 2E` | **C1** | connect / чтение телеметрии (V,I,T) | `FUN_0807e00c` — connect ×4 при init |
| 0x200005E6 | +0x0A | `1C A1 03 C3 01 BF` | **C3** | «pack message» (тип/ёмкость/статус) | `FUN_08058de0` — цикл опроса |
| 0x200005EC | +0x10 | `1C A1 03 B0 11 C1` | **B0** (arg 0x11) | команда режима (charge/state) | vtable-метод (не декомпил.) |
| 0x200005F2 | +0x16 | `1C A1 03 B1 00 C6` | **B1** (arg 0x00) | команда режима | vtable-метод |
| 0x200005F8 | +0x1C | `1C A1 03 B2 00 93` | **B2** (arg 0x00) | команда режима | vtable-метод |
| 0x200005FE | +0x22 | `1C A1 03 B3 55 B3` | **B3** (arg 0x55) | команда режима | vtable-метод |
| 0x20000604 | +0x28 | `1C A1 03 B4 0F 78` | **B4** (arg 0x0F) | команда режима | vtable-метод |
| 0x2000060A | +0x2E | `1C A1 05 53 00 02 1A 22` | **0x53 'S'** | «cell message» (напряжения банок) | `FUN_0805929c`/`FUN_08058de0` — цикл опроса |
| 0x20000612 | +0x36 | `1C A1 09 CE 55 55 55 55 55 55 55 6E` | **CE** | **link-wake / resync** (7×0x55) | `FUN_08074da0` — init, connect-retry, CRC-mismatch, poll-timeout |

Все 9 CRC сошлись (CRC-8/MAXIM по opcode+args). Диспетчер `FUN_08072948` (vtable @ (svc+4)+0x20):
- бит0 ctx+0xa6 → метод **+0x30 = «into charge»**
- бит1 → метод **+0x34 = «exit charge»**
- бит2 → метод **+0x38** (state), бит3 → **+0x3C** (state), бит4 → **+0x40** (state, param=0x10)

Пять vtable-методов почти наверняка соответствуют отправке B0…B4 (точное opcode↔метод не доказано —
тела методов вне декомпил-подмножества; но это единственный набор из 5 «командных» шаблонов в таблице).

### Ответ pack → host (парсеры FUN_0802440c / FUN_08024788)
Валидность: `buf[+0x5D]==0x3A && buf[+0x5E]==0xA3 && buf[+0x60]==0xC1(=-0x3F) && buf[+0x67]==CRC`.
Тянет: `+0x62`→байт статуса (@ctx+0x42), `+0x63/64`→u16 (@+0x48, ток), `+0x65/66`→u16 (@+0x46, напряжение).
Сверка: V<0x9C5, I<0x1068 → статус ctx+0x4C (0=ok/1/2). **Флага «разряд включён / FET открыт» в парсере НЕТ.**
Живой ответ завода (SWD): `1C 3A A3 08 C1 01 19 0E` — C1-эхо + телеметрия.

---

## 3. ТАЙМЛАЙН BMS

```
POWER-ON / platform-init
        │
        ▼
driver_battery init (FUN_080599xx)
   ├─ malloc ctx (0xA8), magic 0x55AA
   ├─ RCU(USART2/GPIOD/AFIO), GPIO PD8/PD9 remap, USART2 19200 8N1, NVIC IRQ39
   ├─ FUN_08074da0()               ← CE-wake (1C A1 09 CE 55×7 6E)   [«разбудить» линк]
   ├─ FUN_08020e54()  connect ×4:
   │     TX C1 → ждать RX 25×2мс → при таймауте CE-wake+retry → парс, сверка V/OCV
   │     успех → ctx+0x54=0 ; провал → 0xFF + «battery connect failed»
   └─ FUN_08058de0()  initial poll: TX C3 (pack msg) → TX 0x53 (cell msg) → парс тип/ёмкость
        │
        ▼
bms-service enable (FUN_08056…, enable=1 → ctx+0xa5=1) → создаётся bms-task (prio 0x200)
        │
        ▼
bms-task цикл (FUN_08072948 + poll):
   ├─ периодически опрос телеметрии (C3 + 0x53), обновление V/I/T/банок  ← keep-alive-трафик
   ├─ при потере кадра/CRC-mismatch → CE-wake + retry
   └─ по битам ctx+0xa6 (ставит FSM заряда/departure):
         into charge  (dock) → метод +0x30 (шлёт B-кадр)
         exit charge  (уход) → метод +0x34 (шлёт B-кадр)
         state cmds          → методы +0x38/+0x3C/+0x40

ДОК/ЗАРЯД (deal_charge / check_recharge):
   is_reach_charger_station → «send cmd into charge» → пак принимает зарядный ток
   «set bms enable charge current=%d» → задать ток заряда
DEPARTURE/COSИТЬ:
   «send cmd exit charge» → выйти из режима заряда
   решение ЕХАТЬ/КОСИТЬ — на стороне РОБОТА (deal_safety), паку команд разряда НЕ шлётся
```

---

## 4. КЛЮЧЕВОЙ КОД (аннотировано)

### TX-примитив connect (FUN_0807e00c)
```c
FUN_080211f0(1);                       // PD8 → TX (half-duplex)
iVar3 = *DAT_0807e048;                 // ctx = *BASE
*(u8*)(iVar3+0x56)=6;                   // LEN окна = 6 байт
*(u8*)(iVar3+0x57)=0;                   // idx
*(int**)(iVar3+0x58)=piVar1+1;          // шаблон = BASE+4 = 1C A1 03 C1 01 2E
do {} while(!FUN_08083178(usart,7));    // ждать TBE
FUN_08082e3c(usart, first_byte);        // 1-й байт; остальные — в IRQ по TBE
```

### CE-wake / resync (FUN_08074da0) — шаблон BASE+0x36
```c
FUN_080211f0(1);
*(u8*)(ctx+0x56)=0xc;                    // LEN окна = 12
*(int*)(ctx+0x58)=(int)BASE+0x36;        // 1C A1 09 CE 55 55 55 55 55 55 55 6E
... TBE, отправка ...
```
Вызывается при init, connect-retry (`FUN_08020e54` LAB_08020eb8), CRC-mismatch (`FUN_0802440c` @34371),
poll-timeout (`FUN_08058de0` @68770/68830). ⇒ это **link-level «пробуждение/ресинк»**, не силовая команда.

### CRC-8/MAXIM (FUN_0802ab98)
```c
uint crc=0;
for (i=0; i<len; i++) crc = table[(byte)(buf[i]^crc)];   // table @ dump 0x88B3C
```

### Диспетчер команд bms-task (FUN_08072948)
```c
iVar6=*(svc+4);
if (*(char*)(iVar6+0xa5)==1) {           // task enabled
  m=*(byte*)(iVar6+0xa6);                 // битовая маска запросов
  if (m&1)  { (*vtbl[+0x30])(); log("send cmd into charge"); }  // → B-кадр
  if (m&2)  { (*vtbl[+0x34])(); log("send cmd exit charge"); }  // → B-кадр
  if (m&4)  { (*vtbl[+0x38])(); m&=~4; }                        // state
  if (m&8)  { (*vtbl[+0x3c])(); m&=~8; }                        // state
  if (m&16) { (*vtbl[+0x40])(); *param=0x10; m&=~16; }          // state
}
```
Биты +0xa6 ставит верхний уровень (FSM заряда/дока). НЕТ ветки «enable discharge/motor».

---

## 5. ★ ЕСТЬ ЛИ КОМАНДА ВКЛЮЧЕНИЯ СИЛОВОГО РАЗРЯДА (мотор-тап колёс)? — **НЕТ**

**Прямой ответ: в заводском протоколе USART2↔пак НЕТ команды «включить разряд / подать питание на
мотор-тап».** Доказательства:

1. **Полная таблица команд декодирована и CRC-верна** (§2): C1, C3, 0x53, CE, B0–B4. Это исчерпывающий
   набор шаблонов запросов в `.data` (соседние адреса 0x200005E0…0x20000612, между ними мусора нет).
   Семантика: телеметрия (C1/C3/0x53), link-wake (CE), режим **заряда**/state (into/exit charge + 3 state).
2. **Парсер ответа не содержит флага разрядного FET** — только V/I/T/статус (§4). Пак не рапортует
   «разряд вкл/выкл», хост его и не запрашивает.
3. **Все строки «discharger» — решения РОБОТА, не команды паку**:
   `battery singal protect, can not discharger`, `battery health, can not discharger`,
   `cutting but bat health can not discharger` → это `deal_safety`/FSM, возвращающие «нельзя ехать»
   (робот сам не даёт привод), а не кадр в USART2. `enable auto reconnect battery discharge temp` /
   `recover discharge temp` — тепловая авто-реконнект-логика (по температуре), тоже хост-сайд.
4. **«set bms enable charge current» = ЗАРЯДНЫЙ ток** (для приёма заряда от базы), не разряд.
5. **Живой тест (2026-07-06/07)**: полный C1-handshake установлен (connected=1, пак отвечает
   телеметрией), но VBB колёс НЕ поднялся. ⇒ опрос C1 разряд не включает.

### Однако — что завод шлёт, а наша mower-own НЕ шлёт (единственная незакрытая щель)
Живой тест v2 слал **только C1** (connect + ре-опрос). Завод дополнительно и постоянно шлёт:
- **CE-wake** `1C A1 09 CE 55×7 6E` — при init и на каждой ошибке связи;
- **C3** (pack msg) и **0x53** (cell msg) — циклический keep-alive-опрос из bms-task.

Если пак держит разрядный FET по **сторожевому таймеру связи** и роняет его, когда хост «замолкает»,
то недостающее звено — не спец-команда, а **устойчивый keep-alive именно этими кадрами** (CE + C3 + 0x53),
которого C1-only тест не обеспечил. Это единственная гипотеза «пак гейтит колёса», не закрытая ботом-железом.

### Рекомендация для mower-own
Реализовать полную последовательность как у завода и замерить VBB колёс:
1. `CE-wake` (12 байт) → 2. `connect C1` ×4 (при таймауте повтор CE) → 3. периодический keep-alive
`C3` + `0x53` (плюс `CE` при CRC-fail) с коротким периодом. Все байты и CRC — в §2 (готовы к хардкоду).
Если VBB не поднимется и при полном keep-alive — гипотеза «пак гейтит колёса» закрывается окончательно,
и остаётся только аппаратный гейт ключа 20U (Q10/Q17/Q14).

---
_Составлено реверс-агентом 2026-07-08. Кадры распакованы из .data и проверены по CRC-8/MAXIM в контейнере._
