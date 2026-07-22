/*
 * mx_mixer.h — преобразование RC-команд в целевые скорости колёс.
 *
 * Единицы совпадают с родным приводом (left_motor_rotation): знаковая скорость,
 * |v| в диапазоне 0..MX_SPEED_MAX (6141). Микшер:
 *   - кламп по модулю до MX_SPEED_MAX;
 *   - дедбенд около нуля (гасит дрожание джойстика);
 *   - слю-рейт лимит (ограничение прироста за тик — бережёт механику/драйвер).
 * Чистая функция состояния, детерминирована, тестируема на хосте.
 */
#ifndef MX_MIXER_H
#define MX_MIXER_H

#include <stdint.h>

#define MX_SPEED_MAX      6141   /* потолок модуля скорости (ед. привода) */
#define MX_SPEED_DEADBAND 100    /* |cmd| ниже -> 0 */
#define MX_SLEW_MAX_STEP  400    /* макс. |Δскорость| за один тик */

typedef struct {
    int16_t out_l; /* последняя выданная скорость левого */
    int16_t out_r; /* последняя выданная скорость правого */
} mx_mixer_t;

void mx_mixer_init(mx_mixer_t *m);

/* Один шаг: (cmd_l,cmd_r) -> сглаженные/клампнутые (out_l,out_r). */
void mx_mixer_step(mx_mixer_t *m, int16_t cmd_l, int16_t cmd_r,
                   int16_t *out_l, int16_t *out_r);

/* Кламп одиночного значения скорости в [-MAX, +MAX] с дедбендом. */
int16_t mx_mixer_clamp(int16_t v);

#endif /* MX_MIXER_H */
