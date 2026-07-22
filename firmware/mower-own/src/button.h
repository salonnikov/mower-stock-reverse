/*
 * button.h — power/KEY button + charge detect for mower-own (GD32F305).
 *
 * Button = PE10 AND PE11, active-low (pressed when the ANDed level == 0).
 * Charge detect = PE8, active-high. Long-press (~1.5 s held) → power off.
 */
#ifndef BUTTON_H
#define BUTTON_H

/* Poll the button once per main loop. Handles debounce + long-press power-off
 * internally (calls power-off and never returns if the long-press fires). */
void button_poll(void);

/* 1 if the charge line (PE8) is currently asserted (charger present). */
int button_charge_present(void);

/* 1 if the power button is currently pressed (raw, for telemetry/activity). */
int button_pressed_now(void);

/* Immediate power-off, factory sequence (dump 0x080714d8): motors off, PB12 +
 * aux rails (PE9/PD11/PE7) LOW, then hold the MAIN power latch PE12 LOW forever
 * (watchdog fed) until the rail collapses. No system reset. Never returns. */
void button_power_off(void);

#endif /* BUTTON_H */
