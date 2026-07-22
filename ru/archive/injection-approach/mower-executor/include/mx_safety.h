/*
 * mx_safety.h — защитная логика внешнего RC-режима.
 *
 * Инварианты (проверяются тестами):
 *   1. Failsafe по таймауту: если корректная команда движения не приходила
 *      дольше MX_CMD_TIMEOUT_MS — движение запрещено (стоп).
 *   2. E-stop латч: подъём (lift) или удар (hit) защёлкивают стоп; снятие —
 *      только явной командой clear И при снятом физическом условии.
 *   3. Пока стоп активен, разрешённая скорость = 0 независимо от команды.
 *
 * Чистая логика, без обращения к железу; решение принимает вызывающий слой.
 */
#ifndef MX_SAFETY_H
#define MX_SAFETY_H

#include <stdbool.h>
#include <stdint.h>

#define MX_CMD_TIMEOUT_MS 500U /* нет команды дольше -> стоп */

typedef struct {
    bool     lift;      /* сработал концевик подъёма */
    bool     hit;       /* сработал контактный бампер */
    uint32_t now_ms;    /* монотонное время, мс */
    uint32_t last_cmd_ms; /* время последней валидной команды движения */
} mx_safety_in_t;

typedef struct {
    bool estop_latched; /* защёлкнутый аварийный стоп */
} mx_safety_t;

void mx_safety_init(mx_safety_t *s);

/* Зафиксировать приход валидной команды движения (обновляет таймер failsafe). */
void mx_safety_note_cmd(mx_safety_in_t *in, uint32_t now_ms);

/* Явный сброс e-stop (сработает только если lift/hit сняты). */
void mx_safety_clear(mx_safety_t *s, const mx_safety_in_t *in);

/*
 * Вернуть true, если движение разрешено. Обновляет латч по lift/hit.
 * При false вызывающий обязан выдать нулевую скорость.
 */
bool mx_safety_motion_allowed(mx_safety_t *s, const mx_safety_in_t *in);

#endif /* MX_SAFETY_H */
