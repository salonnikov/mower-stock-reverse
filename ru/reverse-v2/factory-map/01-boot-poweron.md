# 01 — ЗАГРУЗКА и POWER-ON (заводская chip1)

**Ветка реверса:** boot / power-on. Полный таймлайн от reset до idle (state 3), с упором на
защёлки питания и rail-enable. Источник: `chip1/decompiled_all.c` + резолв DAT_ по
`dist/gd32-mainboard-dump-v1.bin` (flash base 0x08000000, LE). Кросс-чек с живой картой:
`reference/FACTORY-GPIO-PERIPHERAL-MAP.md`.

---

## 0. ВАЖНО: реальный чип и адреса периферии

Резолв базовых указателей из дампа показал, что **это НЕ F4-раскладка из тех-задания.**
Чип — семейство **GD32F30x** (Cortex-M4, «GD32F303-класс»), F1-совместимая периферия:

| Периферия | База (факт из дампа) | Прим. |
|---|---|---|
| GPIOA | `0x40010800` | (в ТЗ ошибочно 0x40020000) |
| GPIOB | `0x40010c00` | |
| GPIOC | `0x40011000` | |
| GPIOD | `0x40011400` | |
| GPIOE | `0x40011800` | |
| AFIO  | `0x40010000` | |
| RCU   | `0x40021000` | (в ТЗ ошибочно 0x40023800) |
| DMA0/DMA1 | `0x40020000` / `0x40020400` | AHB |
| TIMER0 | `0x40012c00` | advanced |
| ADC0 | `0x40012400` | батарея |
| SPI0 | `0x40013000`, USART0 `0x40013800` | |
| APB1: TIMER1 `0x40000000`, TIMER2 `0x40000400` (мотор-PWM), TIMER3 `0x40000800` (тахо), USART1 `0x40004400`, USART2 `0x40004800` (BMS), UART3 `0x40004c00`, SPI1 `0x40003800`, I2C1 `0x40005800` | как в ТЗ |

**Регистры GPIO (F1-стиль, 4 бита/пин):** `CTL0=+0x00` (пины 0-7), `CTL1=+0x04` (пины 8-15),
`ISTAT=+0x08` (вход), `OCTL=+0x0c` (выход), `BOP=+0x10` (set-бит → HIGH), `BC=+0x14` (clear-бит → LOW).

**Регистры RCU:** `AHBEN=+0x14`, `APB2EN=+0x18`, `APB1EN=+0x1C`, `RSTSCK=+0x24` (флаги причины сброса).

### Как декодировать вызовы HAL (ключ ко всему таймлайну)

- `FUN_08010cd0(id)` / `FUN_0806c77c(id)` — **RCU clock enable**. Кодировка GD32 `REGIDX_BIT`:
  `reg = RCUбаза + (id>>6)`, бит `= id & 0x1f`, `*reg |= 1<<бит`.
  → `0x600`=AFIO, `0x602`=GPIOA, `0x603`=GPIOB, `0x604`=GPIOC, `0x605`=GPIOD, `0x606`=GPIOE
    (все на APB2EN off 0x18); `0x701`=TIMER2, `0x70e`=SPI1 (APB1EN off 0x1C); `0x506`=CRC (AHBEN).
- `FUN_0800ca10(base,mask)` → `*(base+0x14)=mask` = **BC** → пины в LOW.
- `FUN_0800ca14(base,mask)` → `*(base+0x10)=mask` = **BOP** → пины в HIGH.
- `FUN_0800ca22(base,p2,p3,mask)` → конфиг режима CTL0/CTL1 (4-битный нибл на пин).
  Нибл `= (CNF<<2)|MODE`. Наблюдаемые `p2`: `0x10`→нибл `3` (out-PP 50MHz);
  `0x14`→нибл `7` (out-OD 50MHz); `0x18`→нибл `0xb` (AF-PP 50MHz); `4`→нибл `4` (in-float).
- `FUN_08010a54(id)` → чтение бита `*(RCUбаза+(id>>6)) & (1<<bit)`; для `0x91x` читает **RCU_RSTSCK**
  (флаги причины сброса).
- `FUN_0800f5e0(irq)` → `NVIC_ICER` — **запрет** IRQ (используется при teardown бутлоадера).

---

## 1. КАРТА ФУНКЦИЙ (FUN → имя → действие)

### Бутлоадер (0x08000xxx)

| FUN | Имя | Что делает с железом |
|---|---|---|
| `FUN_08008960` | `boot_sys_init` | Верхняя раскрутка: LED/лог → `FUN_08000f38` (GPIO+защёлки) → `FUN_08001b4c` (втор. защёлка) → задержка → CRC-clock. Порядок в §2. |
| `FUN_08000f38` | `boot_gpio_init` | **Главный power-on GPIO бутлоадера.** Вкл. клоки GPIOA-E+AFIO, конфиг всех пинов, **поднимает PE12 (главная защёлка) и PE7 (aux-рейл)**. Разбор в §3. |
| `FUN_08001b4c` | `boot_power_latch` | Читает причину сброса (`FUN_0800c8e0`); если не «1» → **PB0=HIGH (вторичная защёлка)** через BOP GPIOB, ставит RAM-флаг `0x40` @ `0x20000018`, лог `reset_power_on`. |
| `FUN_0800c8e0` | `reset_cause` | Читает RCU_RSTSCK (`0x91a..0x91f`), возвращает код источника сброса (por/ext/sw/wdg). |
| `FUN_080012d0` | `jump_to_app` | Проверяет валидность вектора приложения (`*app & mask == 0x20000000`), лог `jump_to_app`, вызывает teardown `FUN_08000e48`, ставит MSP, прыгает на app. |
| `FUN_08000e48` | `boot_deinit` | Перед прыжком: запрет IRQ (0x43,0x1c,0x19,0x25,0x26,0x34), reset TIMER0/DMA0/DMA1, deinit SPI0/USART1/UART3, GPIOB; `SYST_CSR &= ~3` (стоп SysTick). Питание НЕ трогает (защёлки остаются). |
| `FUN_08000f06` | `boot_rcu_x2` | Вкл. клоки `0x500`,`0x501` (AHBEN DMA0/DMA1). |
| `FUN_08001bb8` | `boot_early` | Клоки `0x71b`,`0x71c` (APB1 UART3/UART4), UART-инит `FUN_0800fd60`. |

### Приложение (0x08061xxx / main)

| FUN | Имя | Что делает с железом |
|---|---|---|
| `FUN_08019430` | `Reset_Handler` (app C-runtime) | Копирует .data, чистит .bss, вызывает `main` через `thunk_FUN_08061714`. |
| `FUN_08061714` | `main` | Тело: `FUN_0807f010(); FUN_0806bbd8();` — и всё. |
| `FUN_0807f010` | `pre_scheduler_init` | **Вкл. клоки GPIOE (`0x606`) и GPIOB (`0x603`)** (порты защёлок PE12/PE7/PB0), затем flash/CRC-проверка (`FUN_0802abbc`), при несовпадении — `FUN_0807ef68`. GPIO-пины НЕ конфигурит. |
| `FUN_0806bbd8` | `rtos_start` | FreeRTOS: создаёт task **«initial»** (`FUN_080849c0`, стек 0x1000, prio 0xf, entry `0x0805bc29`) и **«IDLE»**, настраивает SysTick/PendSV (`SHPR3 |= 0xc0000000`), стартует планировщик (`FUN_08018194`). Не возвращается. |
| `FUN_080477c4` | `rw_init` | **Bring-up драйверов** (в task «initial»). Определяет тип платы, создаёт объекты-драйверы по порядку **tilt → battery → BLADE → RIGHT → LEFT**, вызывает их init/enable-методы. Разбор в §4. |
| `FUN_0807976c` | `fsm_set_state` | `if(state<0xb) *(0x200000ac+0x10)=state`. Записывает FSM-состояние (`0x200000bc`). **3 = idle**. |
| `FUN_08063368` | `enable_event` | App-уровневое «событие enable» (объектная модель, флаг+инкремент счётчика). Ассоциировано с PE0 в живой карте; не прямой GPIO-драйв. |

---

## 2. ТАЙМЛАЙН (reset → idle) по шагам

### Стадия A — БУТЛОАДЕР (сразу после reset, до app)

1. **Reset** → стартап бутлоадера → `FUN_08008960` (`boot_sys_init`):
   1. `FUN_0800f68c(0x500)` — ранняя инициализация (лог/LED-канал).
   2. `FUN_080025a8()` — систем-инит.
   3. `FUN_08001bb8()` — клоки UART3/UART4 (APB1 `0x71b/0x71c`), UART-инит для лога.
   4. **`FUN_08000f38()` (`boot_gpio_init`) — КЛЮЧЕВОЙ ШАГ ПИТАНИЯ:**
      - RCU: `FUN_08010cd0(0x602/0x606/0x603/0x604/0x605/0x600)` → вкл. клоки **GPIOA, GPIOE, GPIOB, GPIOC, GPIOD, AFIO**.
      - Конфиг пинов (полный разбор §3). Существенное:
        - **PE12 → OUT-PP 50MHz, затем BOP HIGH ⇒ ГЛАВНАЯ ЗАЩЁЛКА ПИТАНИЯ поднята.**
        - **PE7 → OUT-PP 50MHz, затем BOP HIGH ⇒ AUX-РЕЙЛ поднят.**
        - PB0 → OUT-PP, BC LOW (пока LOW; поднимет шаг 5).
        - PE0/PE1/PE5 → OUT-OD, LOW.
        - Comms-пины: PA9/PA10 (USART0), PC10/PC11 (UART3), PA5/6/7+PA4 (SPI0), PA0/PB9/PB6 (OUT low, UART-DE).
        - Входы: PE10/PE11, PC0/PC1, PA3, PE8.
   5. **`FUN_08001b4c()` (`boot_power_latch`):** читает причину сброса; если ≠1 →
      **BOP GPIOB бит0 ⇒ PB0=HIGH (ВТОРИЧНАЯ ЗАЩЁЛКА)**, RAM-флаг `0x40`, лог `reset_power_on`.
   6. `FUN_08002614(100)` — задержка ~100 (стабилизация питания/защёлок).
   7. `FUN_08001e5c()`, `FUN_08000f06()` (клоки DMA0/DMA1), `FUN_08003fc0()`.
   8. `FUN_08010cd0(0x506)` — клок CRC.
2. Бутлоадер проверяет/выбирает образ и вызывает **`FUN_080012d0` (`jump_to_app`)**:
   - валидирует вектор app, лог `jump_to_app`, → **`FUN_08000e48` (`boot_deinit`)**: гасит IRQ,
     reset TIMER0/DMA/SPI0/USART1/UART3/GPIOB, стоп SysTick. **Защёлки PE12/PE7/PB0 остаются подняты.**
   - ставит MSP из вектора app, прыгает на Reset app.

### Стадия B — ПРИЛОЖЕНИЕ

3. **App Reset** `FUN_08019430`: копирует .data, чистит .bss → `main` `FUN_08061714`.
4. **`FUN_0807f010` (`pre_scheduler_init`):**
   - `FUN_0806c77c(0x606)` + `FUN_0806c77c(0x603)` → повторно вкл. клоки **GPIOE и GPIOB**
     (порты защёлок; app держит их питание). Другие RCU/GPIO тут не трогает.
   - flash/CRC self-check (`FUN_0802abbc`), при несовпадении `FUN_0807ef68`.
5. **`FUN_0806bbd8` (`rtos_start`):** создаёт task «initial» и «IDLE», конфиг SysTick+PendSV,
   старт планировщика. Управление уходит в task «initial» (entry `0x0805bc28`).
6. **Task «initial»** → сервис-конфиг (`FUN_0805bb4c`: run_param/OTA-флаги) → вызывает
   **`FUN_080477c4` (`rw_init`)** — bring-up драйверов (§4): tilt → battery → BLADE → RIGHT → LEFT.
7. После успешного bring-up FSM переводится в рабочий цикл; **idle = state 3** (`fsm_set_state(3)`),
   система ждёт команды (unlock/departure).

---

## 3. КЛЮЧЕВОЙ КОД — `FUN_08000f38` (boot_gpio_init), аннотировано

```c
void boot_gpio_init(void) {
  // --- RCU: клоки всех портов + AFIO ---
  rcu_en(0x602); // GPIOA
  rcu_en(0x606); // GPIOE
  rcu_en(0x603); // GPIOB
  rcu_en(0x604); // GPIOC
  rcu_en(0x605); // GPIOD
  rcu_en(0x600); // AFIO

  // === GPIOE (0x40011800) ===
  cfg(E, 0x10,4, 0x1000);  // PE12 = OUT-PP 50MHz
  BOP(E, 0x1000);          // *** PE12 = HIGH  → ГЛАВНАЯ ЗАЩЁЛКА ПИТАНИЯ ***
  cfg(E, 4,4, 0x400);      // PE10 = IN-float
  cfg(E, 4,4, 0x800);      // PE11 = IN-float

  // === GPIOC (0x40011000) ===
  cfg(C, 4,4, 0x1);        // PC0 = IN-float
  cfg(C, 4,4, 0x2);        // PC1 = IN-float

  // === GPIOB (0x40010c00) ===
  cfg(B, 0x10,4, 0x1);  BC(B, 0x1);      // PB0 = OUT-PP, LOW (втор.защёлку поднимет FUN_08001b4c)
  // (далее опять GPIOB после блока GPIOA)

  // === GPIOA (0x40010800) ===
  cfg(A, 0x10,4, 0x1);  BC(A, 0x1);      // PA0 = OUT-PP LOW (UART DE)

  cfg(B, 0x10,4, 0x200); BC(B, 0x200);   // PB9 = OUT-PP LOW (UART DE)
  cfg(B, 0x10,4, 0x40);  BC(B, 0x40);    // PB6 = OUT-PP LOW

  cfg(A, 0x18,4, 0x200);   // PA9  = AF-PP  → USART0 TX
  cfg(A, 4,4, 0x400);      // PA10 = IN     → USART0 RX
  cfg(A, 4,4, 0x8);        // PA3  = IN-float

  cfg(C, 0x18,4, 0x400);   // PC10 = AF-PP  → UART3 TX
  cfg(C, 4,4, 0x800);      // PC11 = IN     → UART3 RX

  cfg(E, 0x14,4, 0x1);  BC(E, 0x1);      // PE0 = OUT-OD LOW
  cfg(E, 0x14,4, 0x2);  BC(E, 0x2);      // PE1 = OUT-OD LOW
  cfg(E, 0x14,4, 0x20); BC(E, 0x20);     // PE5 = OUT-OD LOW

  cfg(A, 0x10,4, 0x10); BC(A, 0x10);     // PA4 = OUT-PP LOW → SPI0 CS
  cfg(A, 0x18,4, 0x20);    // PA5 = AF-PP → SPI0 SCK
  cfg(A, 0x18,4, 0x80);    // PA7 = AF-PP → SPI0 MOSI
  cfg(A, 4,4,   0x40);     // PA6 = IN    → SPI0 MISO

  cfg(E, 0x10,4, 0x80);  BOP(E, 0x80);   // *** PE7 = OUT-PP HIGH → AUX-РЕЙЛ ***
  cfg(E, 4,4,   0x100);    // PE8 = IN-float
  // ...AFIO/misc хвост
}
```

`FUN_08001b4c` (вторичная защёлка):
```c
void boot_power_latch(void) {
  if (reset_cause() != 1) {          // FUN_0800c8e0: RCU_RSTSCK
    BOP(GPIOB, 0x1);                 // *** PB0 = HIGH  → ВТОРИЧНАЯ ЗАЩЁЛКА ***
    *(u32*)0x20000018 |= 0x40;       // RAM-флаг «power-on»
    log("reset_power_on");
  }
}
```

---

## 4. `rw_init` (FUN_080477c4) — bring-up драйверов (объектная модель)

Драйверы созданы как объекты с vtable; фабрики выбираются по **типу платы** (`iVar4`, из
`FUN_08050578()`), затем вызываются методы. Порядок и лог-строки подтверждают последовательность:

| # | Драйвер | Фабрика (board=0/`0x50dc`) | init-error лог | Методы после init |
|---|---|---|---|---|
| 1 | **tilt-сенсор** | `FUN_0806e5a4` | `ERROR robot_tilt_sensor_initial` (0x300) | `[8](cfg)`, `[0]()` |
| 2 | **battery** | `FUN_0806e1f0`/`FUN_0806df14` | `ERROR robot_battery_initial_fail` (0x32d) | `[4](1)`,`[5](1)`,`[0]()` |
| 3 | **BLADE (нож)** | `FUN_0806e0b4` | `blade_motor_driver_init_failed` (0x353) | `[0]()`,`+0x78 hook`,`[0x13]()` |
| 4 | **RIGHT (правый)** | `FUN_0806e79c` | `right_motor_driver_init_failed` (0x362) | `[0]()`,`+0x80 hook`,`[0x13]()`, если board==4: `[0x15](0)` |
| 5 | **LEFT (левый)** | `FUN_0806e4e4` | `left_motor_driver_init_failed` (0x371) | `[0]()`,`+0x80 hook`,`[0x13]()`, если board==4: `[0x15](0)` |

- Метод `[0]()` = аппаратный init драйвера; `[2]()` — later start; `[0x13]()` — регистрация/enable в фреймворке.
- Мотор-драйверы (`driver_{left,right,blade}_motor_snk_v2.c`) внутри дергают A4963 по SPI1
  (`FUN_0808408c` write+readback) и CS/run-coast GPIO. Детальный разбор мотор-пути — в ветке
  motor/A4963 (см. `MEMORY: a4963-driver-RESOLVED`, `a4963-RUN-default-por`). Здесь важно: **rw_init
  НЕ поднимает никакого дополнительного силового rail-GPIO** сверх того, что уже сделал бутлоадер.
- **Различие нож vs колёса в init:** нож (BLADE) успешно крутится и нашей fw ⇒ его VBB присутствует;
  колёса (RIGHT/LEFT) конструируются тем же кодом, но их силовой рейл 20U остаётся выключен — это
  подтверждает, что enable рейла колёс лежит ВНЕ этого firmware-пути.

---

## 5. СВЯЗЬ С ПИТАНИЕМ КОЛЁС — рейлы / enable / latch (отдельно)

**Что firmware реально включает на пути power-on (полный список драйвов «в питание»):**

| GPIO | Где ставится | Уровень | Роль | Питает |
|---|---|---|---|---|
| **PE12** | бутлоадер `FUN_08000f38` (1-й) | HIGH (BOP) | **главная защёлка питания** | всю плату (self-hold после кнопки/базы) |
| **PE7**  | бутлоадер `FUN_08000f38` | HIGH (BOP) | **aux-рейл** | вспомогательное питание (логика/сенсоры) |
| **PB0**  | бутлоадер `FUN_08001b4c` (по причине сброса) | HIGH (BOP) | **вторичная защёлка** | плата (дублирует hold) |

- App (`FUN_0807f010`) лишь **повторно включает клоки GPIOE/GPIOB**, чтобы удерживать драйв этих
  пинов; новых силовых GPIO не поднимает. Бутлоадерный `boot_deinit` перед прыжком защёлки НЕ трогает.
- **PE0/PE1/PE5** ставятся в OUT-OD **LOW** (бутлоадер) — это не силовые рейлы (open-drain, низкий).
- **GPIOD** в бутлоадере не конфигурируется вообще; клок GPIOD (`0x605`) включается, но пины
  (CS/run-coast моторов PD3/4/5/7) настраивает уже мотор-драйвер в `rw_init`.

**ГЛАВНЫЙ ВЫВОД ПО КОЛЁСАМ (согласуется с `WHEELS-STATE-2026-07-08`):**
На всём пути reset → idle firmware поднимает ровно ТРИ «в питание» линии — **PE12 (главная защёлка),
PE7 (aux), PB0 (вторичная защёлка)**. Все они общеплатные, не драйв-специфичные, и наша fw их
воспроизводит. **Отдельного firmware-GPIO / periph-enable для силового рейла 20U колёс на
boot-пути НЕТ.** Нож сидит на живом рейле (VBB есть → крутится), колёсный рейл 20U коммутируется
аппаратно (high-side ключ Q10/Q17/Q14) или гейтится discharge-FET пака по BMS — вне этого таймлайна.
Следующее направление = железо/BMS, не boot-firmware.

---

## Приложение — резолв DAT_ (из дампа)

```
RCU base          DAT_08010ce8/DAT_0806c794 = 0x40021000
GPIO input base   FUN_08010a54 → RCU (RSTSCK @ +0x24)
FUN_08000f38 порты: E=0x40011800 C=0x40011000 B=0x40010c00 A=0x40010800
FUN_08001b4c latch: DAT_08001b90=0x40010c00 (GPIOB), flag @ 0x20000018 |= 0x40
FSM state var:    0x200000ac + 0x10 = 0x200000bc  (3=idle)
initial task entry: DAT_0806bc30 = 0x0805bc29 (thumb → 0x0805bc28)
```
