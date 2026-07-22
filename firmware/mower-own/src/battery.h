/*
 * battery.h — battery voltage/percent sensing via ADC0 (GD32F305), mower-own.
 *
 * Analog path: ADC0 ch15 (PC5) = pack voltage, ch14 (PC4) = current.
 * pack_mV = raw * 5.4277 (factory used (2 samples) * 2.7138548).
 */
#ifndef BATTERY_H
#define BATTERY_H

#include <stdint.h>

/* Configure ADC0 for battery sensing. Call once after clocks/GPIO. */
void battery_init(void);

/* Sample the pack and update cached mV / percent. Bounded (polled EOC). */
void battery_sample(void);

/* Last computed pack voltage in millivolts. */
uint16_t battery_mv(void);

/* Last computed state-of-charge percent (0..100). */
uint8_t battery_pct(void);

#endif /* BATTERY_H */
