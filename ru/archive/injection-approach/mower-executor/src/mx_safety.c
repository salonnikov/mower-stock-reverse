#include "mx_safety.h"

void mx_safety_init(mx_safety_t *s)
{
    s->estop_latched = false;
}

void mx_safety_note_cmd(mx_safety_in_t *in, uint32_t now_ms)
{
    in->last_cmd_ms = now_ms;
}

void mx_safety_clear(mx_safety_t *s, const mx_safety_in_t *in)
{
    /* Снять латч можно только когда физические условия сняты. */
    if (!in->lift && !in->hit) {
        s->estop_latched = false;
    }
}

bool mx_safety_motion_allowed(mx_safety_t *s, const mx_safety_in_t *in)
{
    /* Защёлкиваем аварию по физическим триггерам. */
    if (in->lift || in->hit) {
        s->estop_latched = true;
    }
    if (s->estop_latched) {
        return false;
    }

    /* Failsafe: команда движения устарела -> стоп. */
    const uint32_t age = in->now_ms - in->last_cmd_ms; /* wrap-safe для uint32 */
    if (age > MX_CMD_TIMEOUT_MS) {
        return false;
    }
    return true;
}
