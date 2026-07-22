/*
 * motor.c — A4963 + PWM motor control for mower-own (GD32F305).
 *
 * ===========================================================================
 * THE CRUX: what actually makes a wheel spin.
 * LIVE-VERIFIED on the factory firmware (bench capture, 2026-07-06): the
 * A4963s are configured ONCE at boot and stay armed; driving is PWM duty ONLY,
 * with NO per-drive SPI traffic or RUN edges. Wheels physically spun from PWM
 * duty alone during the capture.
 * ===========================================================================
 * A4963 is a sensorless BLDC driver. The measured factory model:
 *
 *   1. CONFIGURE the driver over SPI ONCE at boot: write CONFIG0..CONFIG5
 *      (regs 0..5) with the factory shadow values (dead-time, VDS, PWM period,
 *      gear/SMX, etc). reg6 (MASK) is NOT written (left at default).
 *
 *   2. ARM the driver ONCE at boot via reg7 (the RUN register). The factory
 *      writes reg7 = 0xEE0D (data field 0xE0D):
 *         RUN=1 (output stage ON), DIR=0 (forward), BRK=1, restart=1,
 *         demand-DC(serial)=0, stop-on-fail=1, CM=11 (ClosedLoopSpeed).
 *      => the driver is armed from init and STAYS armed. There are NO
 *         per-drive RUN edges. Serial speed-demand (reg7 DC[8:4]) stays 0.
 *
 *   3. SPEED is delivered by the PWM PIN alone, i.e. TIMER2 CHxCV duty — NOT
 *      by SPI. With RUN=1 & BRK=1 & duty=0 the wheel is braked/held
 *      (electrodynamic brake) — that IS the factory stop. A non-zero duty is
 *      the only thing that gates rotation (measured live, 2026-07-06).
 *
 *   4. DIRECTION is the DIR bit in reg7. To reverse, rewrite reg7 with DIR
 *      flipped, keeping RUN/BRK/CM the same. This is the ONLY reg7 rewrite
 *      that ever happens after init.
 *
 * So the whole life of the LEFT wheel is:
 *      a4963_write(MOTOR_LEFT, 0..5, cfg[..])   // once, at init
 *      a4963_write(MOTOR_LEFT, 7, 0xE0D)        // once, at init — armed
 *      pwm_set_duty(PWM_CH_LEFT, M)             // per command -> spins
 *      pwm_set_duty(PWM_CH_LEFT, 0)             // stop (braked/held)
 * and to reverse: rewrite reg7 with DIR set, then set the duty.
 *
 * All 3 A4963 config words (CONFIG0..5 + reg7) are VERIFIED (report §5,
 * Variant A): LEFT & RIGHT identical, BLADE differs. Each driver has its own
 * active-low CS on GPIOD (LEFT=PD5, RIGHT=PD4, BLADE=PD3); a single shared
 * gate-driver enable PB12 (HIGH) enables all three.
 *
 * SAFETY: the blade defaults OFF (CH1 duty 0). It spins ONLY when the host sets
 * blade_on in the mailbox. See motor_blade() and main.c.
 * ===========================================================================
 */
#include "motor.h"
#include "hal.h"

/* --- A4963 register addresses (drive-chain.md §3). --- */
enum {
    A4963_CONFIG0 = 0,
    A4963_CONFIG1 = 1,
    A4963_CONFIG2 = 2,
    A4963_CONFIG3 = 3,
    A4963_CONFIG4 = 4,
    A4963_CONFIG5 = 5,
    /* reg6 = MASK — intentionally NOT written (factory leaves default). */
    A4963_RUN     = 7
};

/* --- reg7 (RUN) bit layout (drive-chain.md §3/§4). --- */
#define A4963_RUN_RUN       (1U << 0)   /* output stage ON            */
#define A4963_RUN_DIR       (1U << 1)   /* direction                  */
#define A4963_RUN_BRK       (1U << 2)   /* brake                      */
#define A4963_RUN_RESTART   (1U << 3)   /* auto-restart               */
#define A4963_RUN_STOPFAIL  (1U << 9)   /* stop on fail               */
#define A4963_RUN_CM_CLSPD  (3U << 10)  /* CM=11 ClosedLoopSpeed      */
/* demand-DC serial = bits [8:4], kept 0 (speed goes via PWM pin). */

/* reg7 (RUN) data field = 0xE0D: RUN=1, DIR=0 (fwd), BRK=1, CM=11 (closed-loop
 * speed), demand=0. Speed via PWM duty, not serial demand. (VERIFIED live:
 * factory reg7 = 0xEE0D at boot, then never rewritten while driving.)
 * a4963_write() builds the frame from data[11:0] only, so we pass the low 12
 * bits of the factory 0xEE0D word = 0x0E0D. DIR is bit1, within this field.
 * Same reg7 word for all three drivers (report). */
#define A4963_REG7_FWD   0x0E0DU                   /* VERIFIED (report §5) */

/*
 * CONFIG0..CONFIG5 words per motor — data[11:0] each (a4963_write masks to 12
 * bits and adds the reg address + write bit). All VERIFIED (report, Variant A):
 * LEFT & RIGHT identical; BLADE differs. Register-address bits [15:13] of the
 * factory words match the reg index, so the low 12 bits are the data field.
 */
static const unsigned s_a4963_cfg[3][6] = {
    /* MOTOR_LEFT  CONFIG0..5 (0x03e8,0x22df,0x4753,0x6721,0x8735,0xa736) FAST */
    { 0x3e8, 0x2df, 0x753, 0x721, 0x735, 0x736 },
    /* MOTOR_RIGHT CONFIG0..5 (identical to LEFT) FAST */
    { 0x3e8, 0x2df, 0x753, 0x721, 0x735, 0x736 },
    /* MOTOR_BLADE CONFIG0..5 (0x03e4,0x2a5f,0x478d,0x6723,0x8974,0xa933) */
    { 0x3e4, 0xa5f, 0x78d, 0x723, 0x974, 0x933 }
};

/* SLOW wheel config set (factory FUN_0805aee0 slow branch, selected when the
 * platform mode vtable returns 1 or 3): 0x01A8,0x2ADF,0x47D3,0x6721,0x8745,0xA736.
 * Differs from FAST in CONFIG0/1/2/4 (off-time, dead-time, VDS/current threshold,
 * bemf). AUDIT 2026-07-10: our fw always used FAST; if this board is SLOW the
 * wheel A4963s get a wrong VDS threshold and can false-trip -> no drive. Runtime
 * switchable via mailbox to A/B test without reflashing. */
static const unsigned s_a4963_cfg_slow[2][6] = {
    { 0x1a8, 0xadf, 0x7d3, 0x721, 0x745, 0x736 },   /* LEFT  slow */
    { 0x1a8, 0xadf, 0x7d3, 0x721, 0x745, 0x736 },   /* RIGHT slow */
};

/* Which wheel config is active (0=FAST, 1=SLOW). Blade always uses its own set. */
static int s_wheel_slow;

static const unsigned s_a4963_reg7_fwd[3] = {
    A4963_REG7_FWD,   /* LEFT  */
    A4963_REG7_FWD,   /* RIGHT */
    A4963_REG7_FWD    /* BLADE */
};

/* PWM channel for each motor. */
static const unsigned s_pwm_ch[3] = {
    PWM_CH_LEFT,   /* MOTOR_LEFT  -> CH3 */
    PWM_CH_RIGHT,  /* MOTOR_RIGHT -> CH2 */
    PWM_CH_BLADE   /* MOTOR_BLADE -> CH1 */
};

/* Current reg7 shadow per motor (the armed word incl. the DIR bit), so we only
 * re-send reg7 over SPI when the direction actually changes. */
static unsigned s_reg7_shadow[3];

/* Test A (bench, -DSPI_WRITE_VERIFY=1): mirror the factory write path exactly.
 * Default OFF = the previous blind double-send. */
#ifndef SPI_WRITE_VERIFY
#define SPI_WRITE_VERIFY 0
#endif

/*
 * Send one A4963 register. An A4963 WRITE frame does NOT echo the written data
 * on MISO — the factory verifies a write with a SEPARATE WR=0 read frame.
 *
 * SPI_WRITE_VERIFY (Test A): do EXACTLY what the factory FUN_0808408c does —
 * write, read the register back (WR=0), compare the low 12 bits, and retry up
 * to 50x until it lands. This is the real fix for "did the config reach the
 * wheel drivers": a blind write can silently fail to land, and if the wheel
 * A4963s default to coast/standby (unlike the blade), an unlanded config leaves
 * them dead. If the read path itself is broken/unpowered the loop exhausts and
 * the post-config verify mask (mailbox) shows 0 — which is itself the answer.
 *
 * Default (OFF): the previous behaviour — send the frame once, plus one extra
 * unconditional resend unless the bus timed out (-3).
 */
static void a4963_send(unsigned motor, unsigned reg, unsigned data)
{
#if SPI_WRITE_VERIFY
    for (unsigned attempt = 0; attempt < 50U; attempt++) {
        if (a4963_write(motor, reg, data) == -3) {
            return;   /* bus dead — stop hammering */
        }
        uint16_t rb = 0x0FFFU;
        if (a4963_read_reg(motor, reg, &rb) == 0
            && rb == (uint16_t)(data & 0x0FFFU)) {
            return;   /* landed */
        }
    }
#else
    if (a4963_write(motor, reg, data) != -3) {
        /* one extra unconditional resend unless the bus timed out */
        (void)a4963_write(motor, reg, data);
    }
#endif
}

static void a4963_verify_one(unsigned motor);   /* fwd (defined below) */

/* Pick the CONFIG0..5 word for a motor/reg, honouring the runtime FAST/SLOW
 * selection for the two wheels (blade always uses its own set). */
static unsigned a4963_cfg_word(unsigned motor, unsigned r)
{
    if (s_wheel_slow && (motor == MOTOR_LEFT || motor == MOTOR_RIGHT)) {
        return s_a4963_cfg_slow[motor][r];
    }
    return s_a4963_cfg[motor][r];
}

/* Configure one A4963: write CONFIG0..5 only (reg6/MASK skipped — matches the
 * factory FUN_0802a16c send order: reg0..5 then reg7, never reg6). */
static void a4963_config_one(unsigned motor)
{
    for (unsigned r = A4963_CONFIG0; r <= A4963_CONFIG5; r++) {
        a4963_send(motor, r, a4963_cfg_word(motor, r));
    }
}

/* Runtime A/B: re-configure BOTH wheels with the FAST (slow=0) or SLOW (slow=1)
 * config set and re-arm (reg7 RUN). SWD/mailbox-triggered so we can test the
 * audit fast-vs-slow hypothesis in one power-cycle without reflashing. Blade
 * untouched. Wheels are left braked (duty 0) afterwards; the host re-commands. */
void motor_wheels_reconfig(int slow)
{
    s_wheel_slow = slow ? 1 : 0;
    for (unsigned m = MOTOR_LEFT; m <= MOTOR_RIGHT; m++) {
        pwm_set_duty(s_pwm_ch[m], 0);
        a4963_config_one(m);
        s_reg7_shadow[m] = s_a4963_reg7_fwd[m];
        a4963_send(m, A4963_RUN, s_reg7_shadow[m]);
        a4963_verify_one(m);
    }
}

/* Which wheel config is currently active (for telemetry). */
int motor_wheel_slow(void) { return s_wheel_slow; }

/* ===========================================================================
 * Config read-back VERIFY (the factory method, replacing the old WR=1 "diag").
 *
 * After writing CONFIG0..5 we read each register back with a SEPARATE WR=0
 * READ frame (a4963_read_reg) and compare the low 12 bits against the written
 * shadow — exactly what the factory verify loop (FUN_0808408c) does. Pure
 * instrumentation: over SWD this shows whether the config LANDED in the driver
 * (vs. an unpowered/silent A4963 whose SDO floats all-high).
 *
 * Result word per motor (published in the mailbox, see main.c):
 *   [13:8] landed mask — bit r SET = CONFIG reg r read back == written value
 *          (0x3F00 = all six landed = config OK)
 *   [7:0]  low 8 bits of the CONFIG0 read-back (raw evidence: 0xE8 wheels /
 *          0xE4 blade when landed; 0xFF when the driver is silent)
 * s_verify_timeout: bit m SET = an SPI transaction TIMED OUT for that motor
 * (bus not clocking = software problem). Bit CLEAR + landed mask 0 = the
 * frames completed but the driver returned garbage (silent/unpowered = HW).
 * ========================================================================= */
static volatile uint16_t s_verify[3];
static volatile uint8_t  s_verify_timeout;

/* Diagnostic (read over SWD by symbol g_a4963_rb): FULL 16-bit read-back of all 8
 * A4963 registers per motor [0]=LEFT [1]=RIGHT [2]=BLADE, sampled once at init.
 * [15:13]=FF/POR/SE status (=1 on a live, SELECTED chip). Reading 0x0000 = the
 * chip drove nothing back (CS not selecting or SDO not reaching MISO) = our SPI
 * comms don't actually reach it. Real non-zero status = comms work. This settles
 * whether the wheel A4963s are ever configured/armed by us. */
volatile uint16_t g_a4963_rb[3][8];

static void a4963_verify_one(unsigned motor)
{
    unsigned landed  = 0;
    uint16_t reg0_rb = 0x0FFFU;

    for (unsigned r = A4963_CONFIG0; r <= A4963_CONFIG5; r++) {
        uint16_t rb = 0x0FFFU;
        int rc = a4963_read_reg(motor, r, &rb);
        if (rc == -3) {
            s_verify_timeout |= (uint8_t)(1U << motor);
        }
        if (r == A4963_CONFIG0) {
            reg0_rb = rb;
        }
        if (rc == 0 && (rb & 0x0FFFU) == (uint16_t)(a4963_cfg_word(motor, r) & 0x0FFFU)) {
            landed |= (1U << r);
        }
    }

    s_verify[motor] = (uint16_t)(((landed & 0x3FU) << 8) | (reg0_rb & 0xFFU));
}

uint16_t motor_verify(unsigned motor)
{
    return (motor < 3U) ? s_verify[motor] : 0U;
}

uint8_t motor_verify_status(void)
{
    return s_verify_timeout;
}

/*
 * a4963_set_dir — rewrite reg7 with the requested DIR bit, keeping every other
 * factory bit (RUN/BRK/restart/stop-fail/CM) exactly as armed at init. Only
 * re-sends over SPI when the direction actually changes — the factory never
 * touches reg7 during same-direction driving (measured live, 2026-07-06).
 */
static void a4963_set_dir(unsigned motor, int reverse)
{
    unsigned reg7 = s_a4963_reg7_fwd[motor];
    if (reverse) { reg7 |= A4963_RUN_DIR; }

    if (reg7 != s_reg7_shadow[motor]) {
        s_reg7_shadow[motor] = reg7;
        a4963_send(motor, A4963_RUN, reg7);
    }
}

void motor_init_all(void)
{
    /* PWM first, all duties zero => motors held (safe) before anything drives. */
    pwm_init();

    /* Shared gate-driver enable (PB12 HIGH) — enables ALL 3 A4963s; must be up
     * before A4963 config. There is no per-motor enable GPIO. */
    hal_a4963_enable();
    hal_short_delay(2000);

    /* Configure + ARM each A4963 ONCE (the live-verified factory model):
     * CONFIG0..5, then reg7 = 0xE0D (RUN=1, BRK=1, DIR=fwd, CM=11). The driver
     * stays armed from here on; driving is PWM duty only, and the only future
     * reg7 rewrite is a DIR flip. Then read CONFIG0..5 back (WR=0 frames) and
     * record whether the config LANDED. Small settle between drivers (shared
     * SPI bus). */
    /* DIAGNOSTIC (2026-07-07): skip ALL A4963 SPI config/arm to test whether the
     * motors run purely on their EEPROM power-up defaults. If the BLADE still
     * spins with this =1, our SPI config is irrelevant/not landing (and the
     * wheels, whose EEPROM default must be RUN=0, stay dead because our arm never
     * reaches them). Set back to 0 for normal operation. */
    static const int A4963_SKIP_CONFIG = 0;
    s_verify_timeout = 0;
    for (unsigned m = 0; m < 3; m++) {
        s_reg7_shadow[m] = s_a4963_reg7_fwd[m];
        if (!A4963_SKIP_CONFIG) {
            a4963_config_one(m);
            a4963_send(m, A4963_RUN, s_reg7_shadow[m]);
        }
        a4963_verify_one(m);
        /* Full 16-bit read-back of all 8 regs for SWD diagnosis (see g_a4963_rb). */
        for (unsigned r = 0; r < 8U; r++) {
            uint16_t v = 0xFFFFU;
            (void)a4963_read_reg(m, r, &v);
            g_a4963_rb[m][r] = v;
        }
        hal_short_delay(2000);
    }

    /* Everything stopped after init (all duties 0; drivers armed + braked). */
    motor_all_stop();
}

/*
 * Turn a signed speed into (direction bit) + (PWM duty) for one wheel. RUN is
 * NEVER touched here — the driver was armed once at init (factory model).
 */
static void drive_wheel(unsigned motor, int signed_speed)
{
    int reverse = 0;
    int mag = signed_speed;
    if (mag < 0) {
        reverse = 1;
        mag = -mag;
    }
    if (mag > MOTOR_SPEED_MAX) {
        mag = MOTOR_SPEED_MAX;
    }

    if (mag == 0) {
        /* Stop: zero duty AND coast/brake the wheel (its run line HIGH). The
         * blade index is a no-op in hal_motor_run. */
        pwm_set_duty(s_pwm_ch[motor], 0);
        hal_motor_run(motor, 0);
        return;
    }

    /* Direction first (reg7 rewrite only if DIR changed), then RELEASE the wheel's
     * run/coast line (LOW = run — the factory's per-wheel enable, without which
     * the wheel stays braked no matter the PWM), then the duty. */
    a4963_set_dir(motor, reverse);
    hal_motor_run(motor, 1);
    pwm_set_duty(s_pwm_ch[motor], (unsigned)mag);
}

void motor_set_wheel(int left_signed, int right_signed)
{
    drive_wheel(MOTOR_LEFT, left_signed);
    drive_wheel(MOTOR_RIGHT, right_signed);
}

/* Blade run duty when enabled. The blade A4963 uses the same PWM-duty speed path
 * as the wheels (CH1). A moderate default; the host controls on/off via the
 * mailbox blade_on flag. TODO(hw): tune to the factory blade_sp if a specific
 * cutting RPM is required. */
#define MOTOR_BLADE_DUTY  0x1000U   /* ~57% of 0x1C20 */

/* WRITE-PATH TEST (SWD-triggered): rewrite the BLADE reg7 with RUN=run. If our
 * SPI WRITE actually reaches the chip, writing RUN=0 while the blade spins must
 * STOP it (output stage off) even with PWM>0. If the blade keeps spinning, our
 * A4963 write never reaches the chip (blade runs on POR default) — which would
 * mean the wheel A4963s are never armed either. This settles whether writes land. */
void motor_blade_run(int run)
{
    unsigned reg7 = run ? A4963_REG7_FWD : (A4963_REG7_FWD & ~A4963_RUN_RUN);
    s_reg7_shadow[MOTOR_BLADE] = reg7;
    a4963_send(MOTOR_BLADE, A4963_RUN, reg7);
}

void motor_blade(int on)
{
    /* Same armed model as the wheels: the blade A4963 got reg7 (RUN=1, BRK=1)
     * once at init and stays armed; on/off is purely PWM duty on CH1, and
     * duty=0 = braked/held (the factory stop). SAFETY: main.c only calls this
     * with blade_on set; the duty stays 0 until then. */
    pwm_set_duty(PWM_CH_BLADE, on ? MOTOR_BLADE_DUTY : 0U);
}

void motor_all_stop(void)
{
    /* Zero PWM on all channels. The drivers stay armed (RUN=1, BRK=1 from
     * init) => active electrodynamic brake/hold with zero demand — the factory
     * stop. reg7 is NOT touched. */
    pwm_set_duty(PWM_CH_LEFT, 0);
    pwm_set_duty(PWM_CH_RIGHT, 0);
    pwm_set_duty(PWM_CH_BLADE, 0);
    /* Coast/brake both wheels (run lines HIGH). */
    hal_motor_run(MOTOR_LEFT, 0);
    hal_motor_run(MOTOR_RIGHT, 0);
}
