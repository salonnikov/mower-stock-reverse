#include "mx_mixer.h"

void mx_mixer_init(mx_mixer_t *m)
{
    m->out_l = 0;
    m->out_r = 0;
}

int16_t mx_mixer_clamp(int16_t v)
{
    int32_t x = (int32_t)v;
    if (x > MX_SPEED_MAX) {
        x = MX_SPEED_MAX;
    } else if (x < -MX_SPEED_MAX) {
        x = -MX_SPEED_MAX;
    }
    if (x > -MX_SPEED_DEADBAND && x < MX_SPEED_DEADBAND) {
        x = 0;
    }
    return (int16_t)x;
}

/* Ограничить прирост от prev к target не более чем на MX_SLEW_MAX_STEP. */
static int16_t slew(int16_t prev, int16_t target)
{
    int32_t d = (int32_t)target - (int32_t)prev;
    if (d > MX_SLEW_MAX_STEP) {
        d = MX_SLEW_MAX_STEP;
    } else if (d < -MX_SLEW_MAX_STEP) {
        d = -MX_SLEW_MAX_STEP;
    }
    return (int16_t)((int32_t)prev + d);
}

void mx_mixer_step(mx_mixer_t *m, int16_t cmd_l, int16_t cmd_r,
                   int16_t *out_l, int16_t *out_r)
{
    const int16_t tl = mx_mixer_clamp(cmd_l);
    const int16_t tr = mx_mixer_clamp(cmd_r);
    m->out_l = slew(m->out_l, tl);
    m->out_r = slew(m->out_r, tr);
    *out_l = m->out_l;
    *out_r = m->out_r;
}
