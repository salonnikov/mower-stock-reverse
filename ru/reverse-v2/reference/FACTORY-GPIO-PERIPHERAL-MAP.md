# Заводская chip1 — карта GPIO + периферии + дифф с нашей прошивкой

**Свежая версия 2026-07-08** (старая → `reverse-v2/archive/2026-07-08/`). Чистый актуальный снимок:
только то, что подтверждено, и честный статус «пробовали / не пробовали».

Источник GPIO: **живой замер** заводской прошивки по SWD (idle state=3 + один снимок state5-езды) —
декодированы CTL0/CTL1/OCTL всех портов. Периферия: RCU EN-регистры (live) + реверс.
Контекст: ищем, что включает **коммутируемый силовой рейл 20U** колёс (нож на живом рейле крутится
и нашей fw, колёса мертвы). Полный статус расследования: `reports/WHEELS-STATE-2026-07-08.md`.

## 1. GPIO-карта заводской (live, idle state=3)

OUT-PP=push-pull выход (=1/0 уровень); AF=alt-func; IN=вход; AIN=аналог; OD=open-drain.

| Пин | Завод (live) | Назначение (реверс) | Наша fw | Совпадает? |
|---|---|---|---|---|
| PA0 | OUT-PP HIGH | UART DE (comms) | OUT LOW | не крит (comms) |
| PA2 | AF | USART1 TX | — | |
| PA4 | OUT-PP HIGH | SPI0 CS (comms) | — | не крит (comms) |
| PA5/6/7 | AF/IN/AF | SPI0 SCK/MISO/MOSI | — | |
| PA9 | AF | USART0 TX | — | |
| PA13/14/15 | SWD | отладка | — | |
| PB0 | OUT-PP HIGH | вторичная защёлка питания | OUT HIGH | = |
| PB1 | AIN | аналог (ток?) | — | |
| PB9 | OUT-PP HIGH | UART DE (comms) | — | не крит (comms) |
| PB10/11 | AF-OD | I2C1 (IMU) SCL/SDA | — | |
| PB12 | OUT-PP HIGH | A4963 общий enable | OUT HIGH | = |
| PB13/14/15 | AF(SPI1) | SPI1 к A4963 | AF | = |
| PC4/5 | AIN | батарея V/I | AIN | = |
| PC7/8/9 | AF | TIMER2 PWM CH1/2/3 (нож/прав/лев) | AF | = |
| PC10 | AF | UART3 TX | — | |
| PD3 | OUT-PP HIGH | нож CS | OUT HIGH | = |
| PD4 | OUT-PP HIGH | правый CS **+ правый run/coast** | OUT HIGH | = |
| PD5 | IN-float (idle) | левый CS | OUT HIGH | ≠ (не крит) |
| PD7 | OUT-PP HIGH→**LOW при езде** | левый run/coast (LOW=run) | реализовано LOW при езде | = (не помогло) |
| PD8 | OUT-PP HIGH | USART2 TX (BMS, half-duplex) | AF | (тот же линк) |
| PD9 | — | USART2 RX (BMS) | AF | |
| PE7 | OUT-PP HIGH | aux-рейл | OUT HIGH | = |
| PE9 | OUT-PP **LOW** | (роль неясна) | LOW (поправлено) | = (не помогло) |
| PD11 | OUT-PP **LOW** | (роль неясна) | LOW (поправлено) | = (не помогло) |
| PE0 | OUT-PP HIGH (app) | enable-событие (FUN_08063368) | пробовали HIGH | ≠→тест не помог |
| PE1/PE5 | OUT-OD LOW | (ставит бутлоадер, наследуем) | = | = |
| PE12 | OUT-PP HIGH | ГЛАВНАЯ защёлка питания | OUT HIGH | = |

## 2. Периферия заводской (RCU EN, live)
- **APB2EN=0x527d:** AFIO, GPIOA-E, ADC0 (батарея), TIMER0 (advanced), USART0.
- **APB1EN=0x184e000f:** TIMER1, **TIMER2 (мотор-PWM)**, **TIMER3 (тахо колёс)**, TIMER4, USART1,
  **USART2 (BMS-пак)**, UART3, **I2C1 (IMU ICM-42688)**, DAC, PMU.
- **AHBEN=0x57:** DMA0, DMA1, SRAM, FMC, CRC.
- Наша fw включает: GPIOA-E, AFIO, ADC0, **TIMER2**, SPI1, USART2, DMA0.
  Не включаем: TIMER0/1/3/4, USART0/1/UART3, I2C1, DAC.
  Лишние у завода на ходу = **TIMER3/TIMER4 (тахо) + I2C1 (IMU)** — сенсоры, не силовой путь.

## 3. Ключевые точки входа кода (реверс, подтверждены)
- reset → C-runtime `FUN_08019430` → **main `FUN_08061714`** → pre-scheduler `FUN_0807f010`
  (RCU+flash, GPIO не трогает; RCU-enable через `FUN_0806c77c(id)`) → FreeRTOS `FUN_0806bbd8`
  (task «initial») → тело @0x0805bc28 → **rw_init `FUN_080477c4`** (создаёт драйверы: tilt→battery→BLADE→RIGHT→LEFT).
- Защёлки питания: **PE12 HIGH** (главная, бутлоадер) + **PB0 HIGH** (вторичная). Питают всю плату, не драйв-специфичны.
- FSM-setter **`FUN_0807976c(state)`**: 3=idle/unlock, 4=departure, 5=cutting, 9=move?, 10=docked/charge.
- Мотор-драйверы: левый `FUN_0805aee0`, правый `FUN_0806fc2c`; A4963 SPI-write `FUN_0808408c`
  (write+readback, WR-бит `word|0x1000`), трансфер `FUN_080840d8`. Модули: `driver_left/right/blade_motor_snk_v2.c`, `a4963_snk_v2.c`.

## 4. ГЛАВНЫЙ ВЫВОД (актуальный)
**«Колёса не едут» ≠ прошивочный GPIO/SPI/арм/пак/периф-enable — всё это проверено и исключено.**
- Нож КРУТИТСЯ нашей fw (даже без SPI-конфига) ⇒ VBB ножа есть, A4963 auto-run (RUN=1 в EEPROM),
  PWM (TIMER2), CS — рабочие. Для езды нужно ТОЛЬКО питание+ШИМ.
- Наша fw СОВПАЛА с заводом по состоянию езды (PD7/PD4 LOW + TIMER2 PWM) и периф-enable. Колёса всё равно мертвы.
⇒ **Колёсный силовой рейл 20U ВЫКЛючен под нашей fw, и его enable — не firmware-GPIO и не periph-enable.**
Нож — на отдельном (живом) рейле. Остаётся: (A) аппаратный гейт high-side ключа 20U (Q10/Q17/Q14);
(B) discharge-FET силового мотор-тапа в паке (BMS); (C) незакрытый модуль `movebase_snk_v10.c` (ветка езды).

## 5. Что ПРОБОВАЛИ и НЕ сработало (не переоткрывать)
- GPIO-дробовик по всем пинам; прямое совпадение с заводским state5 (PD7/PD4 LOW + PWM).
- Кандидаты **PA4/PB9 HIGH** — оказались UART/SPI comms (не питание); тест не помог.
- **PE9/PD11 → LOW** (были ошибочно HIGH; завод держит LOW) — поправлено, не помогло.
- **PE0 → HIGH** (как завод) — не помог.
- **PD7/PD4 run/coast LOW при езде** (точная копия state5) — не помог.
- Пак/BMS по USART2: линк установлен (connected=1, ответ 3A A3), connect-байты `1C A1 03 C1 01 2E` —
  но опрос/handshake **VBB колёс не включает** (в текущем виде). См. `reports/WHEELS-STATE-2026-07-08.md`.
- **TIMER0-конфиг** (был топ-лид) — оффлайн-разбор: TIMER0 = таймер-задержка, не gate-drive;
  мотор-драйвер использует TIMER2+TIMER3, не TIMER0. Разбор в архиве `archive/2026-07-08/SESSION-2026-07-07-TIMER0-analysis.md`.
- База: косилка ехала на заводской и БЕЗ базы ⇒ не база.

## 6. Что НЕ пробовали (открытые направления)
- **`movebase_snk_v10.c`** — сервис движения колёс: не разобран, может дёргать что-то при старте езды. ★ первым.
- **BMS discharge-enable** — есть ли команда «включить силовой разряд» паку и когда (парсер телеметрии её не ставит;
  не проверены «set charge current» / into-exit-charge как побочный enable). ★.
- Аппаратный гейт ключа 20U (Q10/Q17/Q14) — прозвон затвора завод-на-ходу vs наш (по решению пользователя — низкий приоритет).
