/*
 * hal.h — minimal hardware-abstraction layer for mower-own (GD32F305).
 *
 * Bring-up + low-level driver primitives: clocks, GPIO, SPI (A4963), PWM (TIMER2).
 * All register sequences live in hal.c with clearly marked TODO(hw) where an
 * exact value is not yet known from the hardware-inventory report.
 */
#ifndef HAL_H
#define HAL_H

#include <stdint.h>

/* Logical motor indices (map to A4963 CS + PWM channel in hal.c / motor.c). */
enum {
    MOTOR_LEFT  = 0,   /* left wheel  — PWM TIMER2 CH3 */
    MOTOR_RIGHT = 1,   /* right wheel — PWM TIMER2 CH2 */
    MOTOR_BLADE = 2    /* cutting blade — PWM TIMER2 CH1 */
};

/* PWM channel selectors (TIMER2 channel number 1..3). */
enum {
    PWM_CH_BLADE = 1,  /* CH1CV */
    PWM_CH_RIGHT = 2,  /* CH2CV */
    PWM_CH_LEFT  = 3   /* CH3CV */
};

/* --- Power latch (PB0 HIGH) — MUST be called first, before anything slow --- */
void hal_power_latch(void);

/* --- Startup-only busy delay --- */
void hal_short_delay(uint32_t n);

/* --- Clock / core bring-up --- */
void hal_clock_init(void);

/* --- GPIO --- */
void hal_gpio_init(void);

/* --- Auxiliary power rails (PE7, PE9, PD11 HIGH) the factory holds up while
 * running. Must be raised during bring-up, BEFORE A4963 SPI config, so the
 * drivers are powered. See hal.c for the dump evidence + confidence note. --- */
void hal_aux_power_on(void);

/* --- Shared A4963 gate-driver enable (PB12 HIGH) — enables ALL 3 drivers --- */
void hal_a4963_enable(void);

/* --- Per-wheel RUN/COAST line (PD7=LEFT, PD4=RIGHT): run!=0 drives it LOW (run),
 * run==0 drives it HIGH (coast/brake). Blade index is a no-op. This is what makes
 * a wheel actually turn — the blade needs no such line (SPI+PWM only). --- */
void hal_motor_run(unsigned motor, int run);

/* --- SPI to A4963 drivers --- */
void spi_a4963_init(void);

/*
 * a4963_write — send one 16-bit A4963 frame to the driver selected by `motor`.
 * Frame format (verified, drive-chain.md §2):
 *   [15:13] = register address (0..7)
 *   [12]    = write bit (set by this function)
 *   [11:0]  = data
 * Returns 0 on success, -1 on bad args, -3 on SPI timeout.
 */
int a4963_write(unsigned motor, unsigned reg, unsigned data);

/*
 * a4963_read_reg — read one A4963 register back with a WR=0 READ frame (the
 * factory verify method, FUN_0808408c): frame = reg<<13, write bit CLEAR. The
 * A4963 shifts the addressed register's 12-bit content out on SDO[11:0]; that
 * content is written to *out for compare against the written shadow. Returns
 * 0 on success, -1 bad args, -3 on SPI timeout (*out = 0xFFF).
 */
int a4963_read_reg(unsigned motor, unsigned reg, uint16_t *out);

/* --- PWM (TIMER2) --- */
void pwm_init(void);

/*
 * pwm_set_duty — set the compare value (duty) for TIMER2 channel `ch`
 * (PWM_CH_LEFT / PWM_CH_RIGHT / PWM_CH_BLADE). `duty` is a raw compare value in
 * 0..TIMER2_PWM_PERIOD; it is clamped to the period.
 */
void pwm_set_duty(unsigned ch, unsigned duty);

#endif /* HAL_H */
