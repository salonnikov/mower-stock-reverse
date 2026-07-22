/*
 * motor.h — high-level motor control for mower-own (GD32F305).
 *
 * Sits on top of hal.c. Configures the 3 A4963 drivers + TIMER2 PWM, then turns
 * signed speed commands into (A4963 direction bit) + (PWM duty).
 */
#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>   /* uint16_t for the config-verify-word accessor */

/* Configure + ARM all 3 A4963 drivers ONCE (SPI config words + reg7 RUN=1,
 * the live-verified factory model) and start PWM at 0 (braked/held). */
void motor_init_all(void);

/*
 * motor_set_wheel — drive the wheels from signed speed commands.
 *   left_signed  : + = forward, - = reverse, 0 = stop (brake/hold).
 *   right_signed : same.
 * Magnitude range is |v| in 0..MOTOR_SPEED_MAX (see motor.c). Sign selects the
 * A4963 DIR bit; magnitude maps to PWM duty on the wheel's channel.
 */
void motor_set_wheel(int left_signed, int right_signed);

/* motor_blade — enable/disable the cutting blade. OFF (duty 0) by default;
 * spins only when on != 0. SAFETY-critical. */
void motor_blade(int on);

/* WRITE-PATH TEST: force BLADE reg7 RUN bit (run=0 must stop a spinning blade if
 * the SPI write reaches the chip). SWD-triggered via MAILBOX->pad0. */
void motor_blade_run(int run);

/* Runtime A/B (audit 2026-07-10): re-configure BOTH wheels with FAST (slow=0)
 * or SLOW (slow=1) A4963 config and re-arm — test fast-vs-slow in one power-cycle
 * without reflashing. Mailbox pad0: 0x50=FAST, 0x51=SLOW. */
void motor_wheels_reconfig(int slow);
int  motor_wheel_slow(void);

/* Safety: force all motors to stop (zero PWM => brake/hold with RUN=1,BRK=1). */
void motor_all_stop(void);

/*
 * Instrumentation (read-only). During motor_init_all(), motor.c reads back
 * CONFIG0..5 of each A4963 with WR=0 READ frames (the factory verify method)
 * and records whether the config LANDED. motor_verify() returns the per-motor
 * result word (index MOTOR_LEFT/RIGHT/BLADE):
 *   [13:8] landed mask — bit r = CONFIG reg r matched (0x3F00 = all six = OK)
 *   [7:0]  low 8 bits of the CONFIG0 read-back (raw evidence)
 * Used by main.c to expose the verify result in the telemetry mailbox.
 */
uint16_t motor_verify(unsigned motor);
/* Bitmask of motors whose verify SPI read TIMED OUT (bit0=left,1=right,2=blade).
 * Bit SET => SPI bus not clocking (software problem). Bit CLEAR + landed==0 =>
 * the frames completed but the driver drove SDO high (silent/unpowered = HW). */
uint8_t motor_verify_status(void);

/* Max magnitude accepted by motor_set_wheel (maps to full PWM). */
#define MOTOR_SPEED_MAX  0x1C20   /* == TIMER2_PWM_PERIOD */

#endif /* MOTOR_H */
