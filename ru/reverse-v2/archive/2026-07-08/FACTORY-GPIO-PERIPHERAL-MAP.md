# Заводская chip1 — карта GPIO + периферии + дифф с нашей прошивкой (2026-07-07)

Источник GPIO: **ЖИВОЙ замер** заводской прошивки по SWD (idle, state=3) — это ТОЧНЕЕ статики
(декодированы регистры CTL0/CTL1/OCTL всех портов). Периферия: RCU EN-регистры (live) + реверс.
Контекст: ищем, что включает **коммутируемый драйв-рейл 20U** колёс (нож на живом рейле крутится,
колёса мертвы). См. [[a4963-run-default-por]].

## 1. GPIO-карта заводской (live, idle state=3)

Формат: `Pxx  роль` (OUT-PP=push-pull выход, =1/0 уровень; AF=alt-func; IN=вход; AIN=аналог; OD=open-drain).

| Пин | Завод (live) | Назначение (реверс) | Наша прошивка | Δ |
|---|---|---|---|---|
| PA0 | **OUT-PP HIGH** | BB-модуль UART/строб | OUT LOW | ≠ (мы LOW) |
| PA2 | AF | USART1 TX | не трогаем | |
| PA4 | **OUT-PP HIGH** | SPI0 NSS? / ??? | не трогаем | **≠ КАНДИДАТ** |
| PA5 | AF | SPI0 SCK | не трогаем | |
| PA6 | IN-float | SPI0 MISO | не трогаем | |
| PA7 | AF | SPI0 MOSI | не трогаем | |
| PA8 | IN-pull-UP | | IN-float | |
| PA9 | AF | USART0 TX | не трогаем | |
| PA13/14/15 | SWD/JTAG | отладка | не трогаем | |
| PB0 | OUT-PP HIGH | вторичная защёлка питания | OUT HIGH | = |
| PB1 | AIN | аналог (ток?) | не трогаем | |
| PB6 | OUT-PP LOW | ? | не трогаем | |
| PB9 | **OUT-PP HIGH** | строб/??? | не трогаем | **≠ КАНДИДАТ** |
| PB10/11 | AF-OD | I2C1 (IMU) SCL/SDA | не трогаем | |
| PB12 | OUT-PP HIGH | A4963 общий enable | OUT HIGH | = |
| PB13/14/15 | OUT-PP HIGH | SPI1 пины (SPI off в idle→GPIO) | AF (SPI on) | (SPI on/off) |
| PC1-5 | AIN | аналог: PC4/5=батарея V/I | PC4/5 AIN | = |
| PC6 | IN-pull-UP | TIMER2 CH0 (не исп.) | IN-float | |
| PC7/8/9 | AF | TIMER2 PWM CH1/2/3 (нож/прав/лев) | AF | = |
| PC10 | AF | UART3 TX | не трогаем | |
| PD3 | OUT-PP HIGH | нож CS | OUT HIGH | = |
| PD4 | OUT-PP HIGH | правый CS | OUT HIGH | = |
| PD5 | **IN-float** | левый CS (в idle не исп.!) | OUT HIGH | ≠ (мы жмём CS) |
| PD7 | OUT-PP HIGH | ? (в state5→LOW, 20U не гейтит) | (пробовали HIGH — нет) | тестили |
| PD8 | **OUT-PP HIGH** | USART2 TX (half-duplex GPIO/AF) | AF (USART2) | ≠ (мы AF) |
| PD10 | OUT-PP LOW | ? | не трогаем | |
| PD11 | OUT-PP **LOW** | (мы ошибочно HIGH) | OUT HIGH→LOW(fix) | ≠ (чиним) |
| PD12-15 | IN-pull-UP | | IN-float | |
| PE0 | OUT-PP HIGH | ? | (пробовали HIGH — нет) | тестили |
| PE1 | OUT-OD LOW | open-drain | не трогаем | |
| PE5 | OUT-OD LOW | open-drain | не трогаем | |
| PE7 | OUT-PP HIGH | aux-рейл | OUT HIGH | = |
| PE8 | IN-float | заряд-детект (мы pull-DN) | IN-pull-DN | (pull) |
| PE9 | OUT-PP **LOW** | (мы ошибочно HIGH) | OUT HIGH→LOW(fix) | ≠ (чиним) |
| PE10/11 | IN-float | кнопка (мы pull-UP) | IN-pull-UP | (pull) |
| PE12 | OUT-PP HIGH | ГЛАВНАЯ защёлка питания | OUT HIGH | = |

### ★ Итог диффа (что не совпадает с заводом)
- **Завод HIGH, мы не жмём (кандидаты на 20U-enable):** PA0*, **PA4**, **PB9**, PD7(тест.нет), PD8(UART), PE0(тест.нет). (* PA0/PD8 — вероятно comms.) → **PA4 и PB9 — свежие непробованные кандидаты.**
- **Мы жмём HIGH, завод LOW (могли ГАСИТЬ 20U):** PE9, PD11 — в тест-сборке уже поправлено на LOW (не успели проверить — косилка уснула). PD5 (мы CS HIGH, завод idle IN-float).
- Совпадает: PB12, PD3/4, PC7/8/9, PE7, PE12, PB0.

## 2. Периферия заводской (RCU EN, live)
- **APB2EN=0x527d:** AFIO, GPIOA-E, ADC0 (батарея), TIMER0 (advanced), USART0.
- **APB1EN=0x184e000f:** TIMER1, **TIMER2 (мотор-PWM)**, TIMER3, TIMER4 (тахо колёс), USART1, **USART2 (BMS-пак)**, UART3, **I2C1 (IMU ICM-42688)**, DAC, PMU.
- **AHBEN=0x57:** DMA0, DMA1?, SRAM, FMC, CRC (0x57=bits0,1,2,4,6).
- Наша прошивка включает: GPIOA-E, AFIO, ADC0, TIMER2, SPI1, USART2, DMA0. НЕ включаем: TIMER0/1/3/4, USART0/1/UART3, I2C1, DAC — они для тахо/IMU/дисплея, к драйв-рейлу вряд ли, но TIMER0 (advanced) стоит проверить (не он ли реальная PWM-опора?).

## 3. Код-флоу (init / пинкод / кошение)

### Boot/init GPIO (реверс, agent 2026-07-07) [V]
- **Bootloader FUN_08000f38** (наследуем, идёт до нашего app): PE12→HIGH (гл. защёлка), PB0→LOW, PA0→LOW, PB9→LOW, PB6→LOW, **PE0/PE1/PE5 → OUT-OD LOW**, PA4→LOW, PE7→HIGH, PB2 BOP-HIGH (strap), + AF-пины USART0/SPI0/UART3. GPIOD-клок бутлоадер НЕ включает.
- **App pre-scheduler FUN_0807f010: НЕ трогает GPIO** (только RCU+flash).
- **Platform-хелперы (из driver-init):** FUN_0802aaf0→**PE7 HIGH**; FUN_08026e20→**PE9 HIGH + PD11 HIGH** (пара enable, но живьём в idle/state5 = LOW ⇒ этот путь не активен в наблюдаемых состояниях); FUN_0805acec→SPI0 (PA4=NSS HIGH, PA5/7 AF, PA6 in).
- **rw_init/A4963:** SPI1 (PB12-15), TIMER2+TIMER3 (мотор-PWM), CS PD5/PD4/PD3, PD7 (right-motor enable/dir, FUN_0805f3d8 — в state5→LOW, крутится ⇒ не гейтит 20U).

### ★ Резолюция кандидатов (crux) [V]
**PA0=UART4 DE, PA4=SPI0 CS, PB9=UART DE, PD8=UART3 DE — ВСЕ comms, НЕ питание.** Сняты.
Non-comms enable-выходы завода: **PE0** (FUN_08063368 драйвит HIGH на «enable»-событии — топ-кандидат, но наш тест HIGH не помог), PE7/PE12 (совпадают с нами), PE9+PD11 (в наблюдаемых состояниях LOW).
PE1/PE5 (OD-LOW) — ставит бутлоадер, наследуем ⇒ у нас ТАКИЕ ЖЕ ⇒ не разница.

### ★★ ЕДИНСТВЕННОЕ реальное GPIO-расхождение (20U ON завод / OFF мы)
| Пин | Завод idle (20U ON) | Наша (20U OFF) |
|---|---|---|
| PE9 | OUT-**LOW** | OUT-**HIGH** (баг) |
| PD11 | OUT-**LOW** | OUT-**HIGH** (баг) |
| PE0 | OUT-**HIGH** (app) | OD-LOW (наследие bootloader) |
⇒ **Гипотеза: наш PE9/PD11=HIGH ГАСИТ 20U** (завод держит LOW всегда, даже при езде). Тест-сборка PE9/PD11→LOW ЗАЛИТА, но косилка уснула до проверки. Если не поможет — добавить PE0→HIGH (как завод).

### АЛГОРИТМ ВКЛЮЧЕНИЯ / power-on / пинкод (реверс, agent) [V]
Цепочка: reset-вектор → C-runtime `FUN_08019430` (init-array, копия .data) → **main `FUN_08061714`** → `FUN_0807f010` (pre-scheduler: только RCU+flash, GPIO НЕ трогает) → **`FUN_0806bbd8`** (старт FreeRTOS-планировщика, создаёт задачу **"initial"**) → тело задачи @0x0805bc28 → **`rw_init FUN_080477c4`** (создаёт драйверы: tilt→battery→BLADE→RIGHT→LEFT мотор, каждый по vtable).
- **Защёлка питания = PB0 HIGH** (единственный power-enable GPIO на power-on пути; питает ВСЮ плату/MCU, НЕ драйв-специфичный). Ставится и в бутлоадере (`FUN_08001b4c`), и в key-хендлере (`FUN_0800cf50`, «key_press_power_on» — это лог-строка, сам handler = скан кнопки). PE12 HIGH = главная защёлка (бутлоадер).
- **Пинкод: пароль хранится на chip1** (`FUN_08060d00` = load_user_password из NVM; счётчик ошибок `FUN_0807bf20`). Ввод приходит с дисплей-платы (ESP32) по UART; сравнение в security-хендлере (строки @0x0806af88+). Верный пин → **FSM-setter `FUN_0807976c(state)`** разблокирует в idle (state 3). **Никакого драйв/рейл-GPIO и команд питания на этом пути НЕТ** — только PB0-защёлка.

### АЛГОРИТМ КОШЕНИЯ: departure → cutting (states 3→4→5) [V]
- **3→4 (departure):** idle-handler `FUN_080270a8` опрашивает док/границу/погоду/safety; при OK → `FUN_0807976c(4)` («auto departure delay reached» / «on schedule start work»), перед этим вызов border-service.
- **4→5 (cutting):** `FUN_08054274` → `FUN_0807976c(5)` по флагу+счётчику.
- **Побочные эффекты переходов — СОФТОВЫЕ сервис-вызовы** (border-service, movebase wheel-speed-queue, blade-service), **НЕ прямые драйв-питание-GPIO.** Отдельного «включить драйв-рейл» шага при departure/cutting НЕТ.
- **Команда паку по USART2 при cutting — НЕТ** (BMS-сервис шлёт только into/exit-charge при доке; «enable discharge» команды нет; пак разряжает автономно).

### ★ АСИММЕТРИЯ НОЖ vs КОЛЁСА (была ключевой гипотезой) [V]
Все 3 мотора структурно одинаковы (A4963 на общей SPI1, config reg0-5+reg7=0xEE0D). Но у КОЛЁС есть per-motor GPIO, которого у ножа НЕТ:
- **ЛЕВОЕ: PD7** (FUN_0805f3d8) — LOW=run, HIGH=coast/brake. **ПРАВОЕ: PD4** (FUN_0806fc2c) — LOW=run, HIGH=coast. Нож: только SPI+PWM, без этой линии.
- ⚠️ **НО:** живой тест (наша fw драйвит PD7/PD4 LOW при езде — точная копия заводского state5) колёса **НЕ закрутил.** ⇒ PD7/PD4 (вероятно direction/coast) — НЕ корень; колёса не запитаны по иной причине (см. вывод ниже).

## 4. ГЛАВНЫЙ ВЫВОД РАССЛЕДОВАНИЯ (2026-07-07)
**«Колёса не едут» ≠ прошивочный GPIO/SPI/арм/пак/периф-enable — ВСЁ это исключено:**
- Нож КРУТИТСЯ нашей прошивкой (даже без SPI-конфига) ⇒ VBB есть, A4963-стек (SPI-necessary=нет, auto-run с EEPROM RUN=1), PWM (TIMER2), CS — рабочие.
- Наша fw СОВПАЛА с заводом по: GPIO-состоянию езды (PD7/PD4 LOW+PWM), enable-периферии (RCU почти идентичен). Колёса всё равно мертвы.
⇒ **Колёсный драйв-рейл 20U ВЫКЛючен под нашей fw, и его enable — НЕ firmware-GPIO и НЕ простой periph-enable.** Нож на отдельном (живом) рейле.
**~~TIMER0-лид ЗАКРЫТ оффлайн (2026-07-07):~~** TIMER0 = отдельный interrupt-таймер задержки
(FUN_08009e60: timer_init period 11999, БЕЗ каналов/CCHP/AF); мотор-драйвер (FUN_0805aee0) трогает
только TIMER2 (PWM ch3) + TIMER3 (тахо ch1), TIMER0 нигде. Подробно: `reports/SESSION-2026-07-07-TIMER0-analysis.md`.
**Все firmware-авеню исчерпаны. Остаётся ЖЕЛЕЗО:** (A) прозвон гейта high-side ключа 20U (Q10/Q17/Q14)
завод-на-ходу vs наш; (B) discharge-FET силового мотор-тапа в **паке** (устойчивый BMS/USART2-протокол?).

## 5. ИСКЛЮЧЕНО (не возвращаться)
GPIO-дробовик по всем пинам; «нет VBB глобально» (нож крутится); пак-опрос/handshake (не включает VBB); SPI-арм/конфиг колёс (A4963 auto-run, SPI не нужен); PD7/PD4 run-line; PE9/PD11 (мы зря HIGH, завод LOW — поправлено, не корень); PE0/PA0/PA4/PB9/PD8 (comms/dpport); PE1/PE5 (I2C IMU, bootloader); база (ехала без базы); periph-enable (только TIMER3/4/I2C1 лишние — тахо/IMU).

## Уже исключено
-  кандидаты **PA4 HIGH, PB9 HIGH** (завод их держит, мы нет) — возможно, один включает 20U. не сработало.
-  fix PE9/PD11→LOW - не сработало.
-  **TIMER0-конфиг** (был топ-лид) — оффлайн-разбор: TIMER0 = таймер-задержка, не gate-drive;
   мотор-драйвер использует TIMER2+TIMER3, не TIMER0. `reports/SESSION-2026-07-07-TIMER0-analysis.md`.
