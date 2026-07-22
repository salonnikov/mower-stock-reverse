/*
 * mx_vendor.h — ABI родной прошивки chip1 (GD32F305), которую переиспользует
 * исполнитель. ЗДЕСЬ И ТОЛЬКО ЗДЕСЬ собраны все абсолютные адреса/офсеты.
 *
 * Легенда достоверности (см. каталог docs/fw):
 *   [OK] — подтверждено дизасмом+сырыми байтами (несколько независимых проверок);
 *   [H]  — гипотеза/приближение, ОБЯЗАТЕЛЬНО подтвердить на живом чипе (SWD)
 *          перед первой сборкой-под-заливку. Помечено MX_VENDOR_UNCONFIRMED.
 *
 * Тела функций вызываются по абсолютному адресу (исполнитель живёт в том же
 * образе, что и родной код). Thumb-указатели — с установленным битом 0.
 */
#ifndef MX_VENDOR_H
#define MX_VENDOR_H

#include <stdbool.h>
#include <stdint.h>

/* Пометка «адрес не подтверждён на живом чипе». */
#define MX_VENDOR_UNCONFIRMED 1

/* --- Приводы колёс [reverse-v2: сигнатура и адреса подтверждены форс-декомпилом] ---
 * ВАЖНО (исправлено против прежней трактовки): set_motor_speed берёт объект из
 * ГЛОБАЛА (не из аргумента!) и имеет сигнатуру (direction, |speed|):
 *   r0 = direction 0/1  (какое = вперёд — ⓗ, подтвердить на чипе)
 *   r1 = |скорость| 0..6141 (PWM-единицы). Внутри сам: выбирает передачу
 *        (CONFIG5 SMX через 0x0801f4dc), ставит направление (set_dir через
 *        0x0801f4c6) и пишет PWM в CH3CV TIMER2 (0x0807f696). A4963 уже в RUN
 *        с init — доп. включение не нужно. */
/* set_left_motor_speed(dir, |speed|), объект неявный (глобал 0x2000054c). */
#define MX_ADDR_LEFT_MOTOR_ROTATION  0x0805f621U /* [OK reverse-v2] thumb */
/* set_right_motor_speed — зеркало левого (gear-map), объект глобал 0x2000057c.
 * (Прежний 0x0806fce0 — ошибка: там 8-байтный геттер, не set_speed.) */
#define MX_ADDR_RIGHT_MOTOR_ROTATION 0x0806fe67U /* [OK reverse-v2] thumb */

/* Слоты объектов драйверов колёс (для проверки «драйвер поднят»; сами функции
 * читают эти же глобалы). lmdrv=*0x2000054c, rmdrv=*0x2000057c (шаг 0x30). [OK] */
#define MX_ADDR_LMDRV_SLOT 0x2000054cU /* [OK] */
#define MX_ADDR_RMDRV_SLOT 0x2000057cU /* [OK reverse-v2] */

/* Значения направления для r0 set_motor_speed. Какое = вперёд — ⓗ, уточнить
 * на чипе (стенд-тест bench-test-plan). */
#define MX_DIR_FORWARD 0 /* [H] подтвердить живьём */
#define MX_DIR_REVERSE 1 /* [H] */
#ifndef MX_SPEED_MAX
#define MX_SPEED_MAX   6141 /* верхний кламп |скорости| (PWM-ед., CONFIG5 SMX=3); ср. mx_mixer.h */
#endif

/* --- Главное состояние машины [OK] --- */
#define MX_ADDR_MAIN_STATE 0x200000bcU /* байт состояния 0..10 */

/* --- Одометрия [reverse-v2: ⚠ НЕ статический анкер!] ---
 * Внимание: 0x2000be28/be58 из прошлых live-findings — malloc-адреса из ОДНОГО
 * бута; в бинаре как литералы отсутствуют (0 вхождений против 9 у 0x2000054c).
 * НЕ надёжны: при другом порядке аллокации/версии прошивки сместятся, и чтение
 * даст мусор. Реальный механизм: тахо A4963 -> TIMER3 (0x40000800) input-capture
 * -> ISR (0x0805fbc0/0x0805fd45), счётчики в malloc-структуре колеса, достижимой
 * через движ-объект (*0x20000338 -> pri_data). Перед использованием ПЕРЕ-АНКЕРить
 * на живом чипе (или резолвить офсет тика в объекте колеса). */
#define MX_ADDR_ODOM_LEFT  0x2000be28U /* [H!] fragile — re-anchor per boot */
#define MX_ADDR_ODOM_RIGHT 0x2000be58U /* [H!] fragile */

/* --- Process-manager (точка инъекции) [OK: анкеры резолвнуты из бинаря] --- */
#define MX_ADDR_PMGR_SLOT   0x20000078U /* *slot -> обёртка сервиса */
#define MX_PMGR_TICK_INDEX  0U          /* индекс метода-tick в обёртке [H] */

/* --- Watchdog (IWDG/FWDGT) [OK: периферия; H: функция кормления] --- */
#define MX_ADDR_IWDG        0x40003000U
/* Родная feed-функция (fcn.0804bca0) — [H], либо кормить регистром напрямую. */
#define MX_ADDR_WDT_FEED    0x00000000U /* [H] TODO */

/* Тип указателя на родную set_motor_speed: (direction 0/1, |speed| 0..6141).
 * Объект НЕЯВНЫЙ (функция читает его из глобала). ABI: r0=dir, r1=speed_mag. */
typedef void (*mx_motor_rotation_fn)(int32_t dir, int32_t speed_mag);

/* Снимок телеметрии, отдаётся наружу. */
typedef struct {
    uint8_t  state;      /* MX_ADDR_MAIN_STATE */
    int32_t  odom_l;     /* тики левого */
    int32_t  odom_r;     /* тики правого */
    int16_t  coil_l;     /* [H] боковая магнитуда (или брать с USART2 chip2) */
    int16_t  coil_r;     /* [H] */
    int16_t  heading;    /* [H] IMU курс */
    uint16_t batt_mv;    /* [H] */
    int16_t  cur_l;      /* [H] ток левого мотора, мА */
    int16_t  cur_r;      /* [H] */
    uint8_t  flags;      /* bit0 lift, bit1 hit, bit2 rain, bit3 button [H] */
} mx_telemetry_t;

/* Команда привода: знаковые скорости колёс (уже отфильтрованы микшером). */
void mx_vendor_drive(int16_t left, int16_t right);

/* Заглушка ножа (вкл/выкл + обороты). */
void mx_vendor_blade(bool on, uint16_t rpm);

/* Немедленный стоп приводов. */
void mx_vendor_stop(void);

/* Собрать телеметрию из известных адресов/объектов. */
void mx_vendor_read_telemetry(mx_telemetry_t *t);

/* Покормить watchdog (обязателен, если гасим родные кормильцы). */
void mx_vendor_feed_watchdog(void);

/* Монотонное время в мс (из системного тика). */
uint32_t mx_vendor_now_ms(void);

#endif /* MX_VENDOR_H */
