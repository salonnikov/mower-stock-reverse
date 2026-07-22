/*
 * main.c — mower-own STANDALONE firmware entry (GD32F305).
 *
 * This is OUR application, replacing the factory app at 0x08018000. The
 * bootloader jumps here. We bring up clocks/GPIO/SPI/PWM, configure the 3 A4963
 * drivers, then run a poll loop:
 *   - bump a heartbeat counter (observable over SWD, like the old rc-payload)
 *   - read the RC mailbox in SRAM (host writes it over SWD)
 *   - drive the wheels from it — with a safety gate.
 *
 * SAFETY: motors move ONLY when the mailbox magic is correct AND the command is
 * fresh (a host-incremented sequence counter differs from the last we acted on).
 * On wrong magic or a stale command, motors are forced to stop. On boot the
 * mailbox is RAM garbage => magic almost certainly wrong => motors stay off.
 */
#include <stdint.h>
#include <stddef.h>      /* offsetof for mailbox layout static-asserts */
#include "gd32_regs.h"   /* TIMER2_CHxCV for duty read-back telemetry */
#include "hal.h"
#include "motor.h"
#include "button.h"
#include "battery.h"
#include "bms.h"

/* ---------------------------------------------------------------------------
 * Mailbox — fixed SRAM address, shared with the Raspberry-Pi dashboard over SWD.
 * The Pi WRITES the control fields; the FIRMWARE WRITES the telemetry fields.
 * 0x20014000 is the slot proven free/usable by the rc-payload.
 * ------------------------------------------------------------------------- */
#define MAILBOX_ADDR   0x20014000U
#define MAILBOX_MAGIC  0x5243414DU   /* "MACR" — arm value                     */

/*
 * Mailbox layout (documented in README). Control fields = host writes; telemetry
 * fields = firmware writes. Signed 16-bit speeds: sign = direction, magnitude =
 * PWM duty (0..0x1C20). blade_on gates the blade (default OFF for safety).
 *
 *  CONTROL (host → firmware):
 *   +0x04 u32 magic       — host sets == 0x5243414D to arm
 *   +0x08 u32 seq         — host bumps on each fresh command (dead-man / activity)
 *   +0x0C i16 left_speed  — signed; + fwd / - rev; |v| = duty
 *   +0x0E i16 right_speed — signed
 *   +0x10 u8  blade_on    — 0 = OFF (safe default), !=0 = spin blade
 *   +0x11 u8  pad0
 *  TELEMETRY (firmware → host, written every loop):
 *   +0x00 u32 heartbeat   — firmware bumps every loop
 *   +0x14 u16 batt_mV     — pack voltage (mV)
 *   +0x16 u8  batt_pct    — state of charge (0..100)
 *   +0x17 u8  charge      — 1 = charger present (PE8)
 *   +0x18 u16 ch3_duty    — LEFT  actual PWM (TIMER2 CH3CV)
 *   +0x1A u16 ch2_duty    — RIGHT actual PWM (CH2CV)
 *   +0x1C u16 ch1_duty    — BLADE actual PWM (CH1CV)
 *   +0x1E u8  btn         — power button pressed 0/1
 *   +0x1F u8  fw_state    — 0=disarmed, 1=armed-idle, 2=driving
 *   +0x20 u16 left_vrfy   — A4963 LEFT  config-verify word (see below)
 *   +0x22 u16 right_vrfy  — A4963 RIGHT config-verify word
 *   +0x24 u16 blade_vrfy  — A4963 BLADE config-verify word
 *   +0x26 u8  spi_status  — verify SPI outcome (bit0=L,1=R,2=blade): SET=timeout
 *   +0x27 u8  pad3        — alignment (struct size 0x28)
 *
 * Config-verify word (motor.c: at init, CONFIG0..5 are read BACK with WR=0
 * frames — the factory verify method — and compared against the written values):
 *   [13:8] landed mask — bit r SET = CONFIG reg r read back == written value;
 *          0x3F00 = all six landed = driver alive and configured.
 *   [7:0]  low 8 bits of the CONFIG0 read-back (0xE8 wheels / 0xE4 blade when
 *          landed; 0xFF = driver silent, SDO all-high).
 * spi_status bit SET = that motor's verify SPI TIMED OUT (bus not clocking =
 * software); CLEAR + landed==0 = frames completed but driver silent (VBB/HW).
 */
typedef struct {
    volatile uint32_t heartbeat;   /* +0x00 (fw) */
    volatile uint32_t magic;       /* +0x04 (host) */
    volatile uint32_t seq;         /* +0x08 (host) */
    volatile int16_t  left_speed;  /* +0x0C (host) */
    volatile int16_t  right_speed; /* +0x0E (host) */
    volatile uint8_t  blade_on;    /* +0x10 (host) */
    volatile uint8_t  pad0;        /* +0x11 */
    volatile uint16_t pad1;        /* +0x12 (align batt_mV to +0x14) */
    volatile uint16_t batt_mV;     /* +0x14 (fw) */
    volatile uint8_t  batt_pct;    /* +0x16 (fw) */
    volatile uint8_t  charge;      /* +0x17 (fw) */
    volatile uint16_t ch3_duty;    /* +0x18 (fw) LEFT  */
    volatile uint16_t ch2_duty;    /* +0x1A (fw) RIGHT */
    volatile uint16_t ch1_duty;    /* +0x1C (fw) BLADE */
    volatile uint8_t  btn;         /* +0x1E (fw) */
    volatile uint8_t  fw_state;    /* +0x1F (fw) */
    volatile uint16_t left_vrfy;   /* +0x20 (fw) A4963 LEFT  config-verify word */
    volatile uint16_t right_vrfy;  /* +0x22 (fw) A4963 RIGHT config-verify word */
    volatile uint16_t blade_vrfy;  /* +0x24 (fw) A4963 BLADE config-verify word */
    volatile uint8_t  spi_status;  /* +0x26 (fw) A4963 verify SPI outcome:
                                    * bit0=left,1=right,2=blade SET => SPI TIMED OUT
                                    * (bus not clocking = software). CLEAR + landed
                                    * ==0 => frames completed, driver silent (VBB/hw). */
    volatile uint8_t  pad3;        /* +0x27 (align struct to 0x28) */
} rc_mailbox_t;

/* firmware state (fw_state telemetry). */
enum { FW_DISARMED = 0, FW_ARMED_IDLE = 1, FW_DRIVING = 2 };

#define MAILBOX  ((rc_mailbox_t *)MAILBOX_ADDR)

/*
 * IWDG (free watchdog, FWDGT) feed. Disassembly of the bootloader shows it CAN
 * start the free watchdog (writes 0xCCCC to FWDGT_CTL 0x40003000) before jumping
 * to the app; whether the boot path actually starts it is not fully traced. The
 * factory app feeds its own watchdog — we replace that app, so WE must feed too.
 * Writing the reload key 0xAAAA is harmless if the watchdog is NOT running, and
 * prevents a reset-loop if it IS. Called early (before slow bring-up) and every
 * loop. FWDGT_CTL is write-only for the key; no read-modify needed.
 */
static inline void iwdg_feed(void) { FWDGT_CTL_REG = FWDGT_KEY_FEED; }

/*
 * Loop pacing: one main-loop iteration is paced to ~1 ms by a fixed nop-spin at
 * the TOP of the loop (so EVERY path — including the disarmed `continue` — is
 * paced, not just the drive path). ~25000 volatile iterations ≈ 1 ms at 120 MHz.
 * All loop-count thresholds below are therefore in ~ms. TODO(hw): replace with a
 * real SysTick tick for exact timing.
 */
#define LOOP_PACE_ITERS  25000U   /* ~1 ms at 120 MHz core */

/*
 * Staleness: the host bumps `seq` whenever it issues a fresh command. If `seq`
 * stops changing for STALE_LIMIT paced loops (~ms) we treat the command as stale
 * and stop the motors (dead-man). The host must keep bumping seq to keep the
 * wheels turning. This prevents a wedged host from leaving the mower running.
 */
#define STALE_LIMIT  200U   /* ~200 ms with no new seq before we force-stop */

/*
 * Auto-sleep (power-save): if there is NO activity for AUTOSLEEP_LIMIT paced
 * (~1 ms) loops, power off (same as a long-press: the factory shutdown —
 * motors off, aux rails low, PE12 main latch held LOW until the rail dies).
 * "Activity" = a fresh mailbox seq bump (a command), OR the power button pressed,
 * OR charging (PE8 high). We NEVER sleep while charging or while armed+driving.
 * 600000 loops ≈ 10 minutes at ~1 ms/loop. Mirrors the factory auto-off.
 */
#define AUTOSLEEP_LIMIT  600000U   /* ~10 min at ~1 ms/loop */

/* Battery sampling cadence: every BATT_SAMPLE_PERIOD loops (~1 s). ADC scan is
 * cheap but no need to run it every ms. */
#define BATT_SAMPLE_PERIOD  1000U

/* Compile-time check that the mailbox telemetry offsets match the spec. */
_Static_assert(sizeof(rc_mailbox_t) == 0x28, "mailbox must be 40 bytes");
_Static_assert(offsetof(rc_mailbox_t, left_vrfy)  == 0x20, "left_vrfy @ +0x20");
_Static_assert(offsetof(rc_mailbox_t, right_vrfy) == 0x22, "right_vrfy @ +0x22");
_Static_assert(offsetof(rc_mailbox_t, blade_vrfy) == 0x24, "blade_vrfy @ +0x24");
_Static_assert(offsetof(rc_mailbox_t, spi_status) == 0x26, "spi_status @ +0x26");

int main(void)
{
    /* --- CRITICAL: hold system power FIRST (PE12 HIGH = main latch, PB0 HIGH =
     * secondary). Without this, the mower powers OFF as soon as the button is
     * released. Must precede anything slow. */
    hal_power_latch();

    /* Feed the free watchdog immediately: if the bootloader started it, our
     * bring-up below must not outlast its timeout. Harmless if it isn't running. */
    iwdg_feed();

    /* --- H2: clear the mailbox control fields at boot so stale/retained SRAM
     * (after `reset run` or a warm power cycle) can NOT drive the motors. A prior
     * test could have left a valid magic + nonzero speeds here; wipe them now,
     * before bring-up, so nothing moves until the host RE-ARMS from scratch.
     * heartbeat is left intact (only a liveness counter). */
    MAILBOX->magic       = 0U;
    MAILBOX->seq         = 0U;
    MAILBOX->left_speed  = 0;
    MAILBOX->right_speed = 0;
    MAILBOX->blade_on    = 0U;

    /* --- Bring-up --- */
    hal_clock_init();
    hal_gpio_init();
    /* Raise the auxiliary power rails (PE7/PE9/PD11) the factory holds HIGH while
     * running — the drive-power domain that is grouped with the A4963 enable in
     * the factory power-off. Must precede A4963 SPI config so the drivers are
     * powered when we configure them. See hal.c hal_aux_power_on() for the dump
     * evidence: without this the A4963s can read back 0xFFFF (unpowered, SDO not
     * driven) and never commutate. */
    hal_aux_power_on();

    /* Bring up the pack link and poll it a few times BEFORE configuring the
     * A4963s: the pack gates the motor discharge rail (A4963 VBB) on being
     * polled, so this is what actually powers the drivers. Space the initial
     * polls so the pack has time to close its discharge FET before we talk SPI
     * to the (now powered) drivers. Kept alive afterwards by bms_tick() in the
     * loop. See bms.c for the reverse-engineered protocol + evidence. */
    bms_init();
    for (int i = 0; i < 4; i++) {
        bms_poll();
        hal_short_delay(400000);   /* ~few ms settle between polls */
    }

    spi_a4963_init();
    /* pwm_init() (incl. AFIO TIMER2 remap) + shared A4963 enable are done inside
     * motor_init_all(). */
    motor_init_all();
    battery_init();

    /* Start provably stopped. */
    motor_all_stop();

    /* Feed again after bring-up, before entering the paced loop. */
    iwdg_feed();

    /* Dead-man: require a FRESH seq transition (host bump) while armed before we
     * ever drive. We seed last_seq from the just-zeroed mailbox (0) and set
     * `seen_fresh` false: the first drive happens only after the host, with magic
     * armed, writes a seq different from this baseline. Until then motors stay
     * stopped no matter what other fields contain. */
    uint32_t last_seq    = MAILBOX->seq;   /* 0, just cleared */
    uint32_t stale_ctr   = 0;
    int      seen_fresh  = 0;              /* have we observed a fresh seq yet? */
    uint32_t idle_ctr    = 0;              /* loops since last activity (auto-sleep) */
    uint32_t last_act_seq= MAILBOX->seq;   /* seq value at last observed activity */
    uint32_t batt_ctr    = 0;

    for (;;) {
        /* Feed the free watchdog every iteration, before the ~1 ms pacing spin,
         * so a running IWDG never fires while we're alive. */
        iwdg_feed();

        /* --- Loop pacing at the TOP: EVERY iteration (including the disarmed
         * `continue` paths below) is paced to ~1 ms. This is CRITICAL: the
         * button long-press timing counts loop iterations, so an unpaced disarmed
         * path would make sub-microsecond loops and trip the ~1.5 s long-press in
         * <100 ms — powering the mower off at boot. Pace first, always. --- */
        for (volatile uint32_t d = 0; d < LOOP_PACE_ITERS; d++) {
            __asm__ volatile("nop");
        }

        /* Heartbeat: proves OUR firmware is running (mdw 0x20014000 over SWD). */
        MAILBOX->heartbeat++;

        /* --- Serviced EVERY loop, before any drive gate can `continue`, so the
         * button (power-off), sensors and telemetry are updated regardless of
         * motor/arming state. All bounded/non-blocking. --- */
        button_poll();                               /* long-press → power off */
        int charging = button_charge_present();
        int btn_now  = button_pressed_now();

        /* Battery sampled periodically (~1 s). */
        if (++batt_ctr >= BATT_SAMPLE_PERIOD) {
            batt_ctr = 0;
            battery_sample();
        }

        /* Keep the pack polled so it never drops the motor discharge rail (A4963
         * VBB). Runs every loop before any drive gate; actually transmits every
         * BMS_POLL_PERIOD iterations. Not counted as "activity" for auto-sleep. */
        bms_tick();

        /* --- Telemetry (firmware → host), every loop. Actual PWM duties are read
         * back so the dashboard sees exactly what the outputs are doing. --- */
        MAILBOX->batt_mV  = battery_mv();
        MAILBOX->batt_pct = battery_pct();
        MAILBOX->charge   = (uint8_t)charging;
        MAILBOX->ch3_duty = (uint16_t)TIMER2_CH3CV;
        MAILBOX->ch2_duty = (uint16_t)TIMER2_CH2CV;
        MAILBOX->ch1_duty = (uint16_t)TIMER2_CH1CV;
        MAILBOX->btn      = (uint8_t)btn_now;

        /* A4963 config-verify result (sampled ONCE in motor_init_all via the
         * factory WR=0 read-back method; static afterwards, no per-loop SPI).
         * Published every loop so the host always sees whether the config
         * LANDED in each driver: 0x3Fxx = all six CONFIG regs matched. */
        MAILBOX->left_vrfy  = motor_verify(MOTOR_LEFT);
        MAILBOX->right_vrfy = motor_verify(MOTOR_RIGHT);
        MAILBOX->blade_vrfy = motor_verify(MOTOR_BLADE);
        /* SPI outcome of the verify reads: timeout (bus dead = software) vs
         * completed-but-silent (driver unpowered = hardware). */
        MAILBOX->spi_status = motor_verify_status();
        /* Reuse the trailing pad byte (+0x27) to surface the BMS poll counter
         * over SWD/dashboard — confirms the pack is being kept alive. */
        MAILBOX->pad3 = bms_status();

        /* --- Auto-sleep activity tracking. Activity = fresh seq bump, button
         * pressed, or charging. No activity for AUTOSLEEP_LIMIT loops (~10 min)
         * → power off. Never sleep while charging or while armed+driving. --- */
        uint32_t seq_now = MAILBOX->seq;
        int armed_now = (MAILBOX->magic == MAILBOX_MAGIC);
        int driving_now = armed_now && seen_fresh && (stale_ctr < STALE_LIMIT);
        if (seq_now != last_act_seq || btn_now || charging) {
            last_act_seq = seq_now;
            idle_ctr = 0;
        } else if (!charging && !driving_now) {
            if (idle_ctr < AUTOSLEEP_LIMIT) {
                idle_ctr++;
            }
            /* BENCH (2026-07-07): auto-sleep disabled so the mower does not power
             * itself off between manual SWD tests. Set to 1 to restore. */
            static const int BENCH_NO_AUTOSLEEP = 1;
            if (!BENCH_NO_AUTOSLEEP && idle_ctr >= AUTOSLEEP_LIMIT) {
                motor_all_stop();
                button_power_off();                  /* never returns */
            }
        } else {
            idle_ctr = 0;                            /* charging/driving: stay awake */
        }

        /* Safety gate 1: magic must be armed. Disarming resets the fresh-seq
         * requirement, so re-arming again needs a new seq bump before driving. */
        if (MAILBOX->magic != MAILBOX_MAGIC) {
            motor_all_stop();
            last_seq   = MAILBOX->seq;
            stale_ctr  = 0;
            seen_fresh = 0;
            MAILBOX->fw_state = FW_DISARMED;
            continue;
        }

        /* Safety gate 2: command freshness (dead-man). A change in seq is a fresh
         * command; the FIRST such change while armed unlocks driving. */
        uint32_t seq = MAILBOX->seq;
        if (seq != last_seq) {
            last_seq   = seq;
            stale_ctr  = 0;
            seen_fresh = 1;
        } else {
            if (stale_ctr < STALE_LIMIT) {
                stale_ctr++;
            }
        }

        /* Gate 3: never drive until we've seen at least one fresh seq bump while
         * armed (blocks stale-SRAM drive on the very first loops), and stop if
         * the command goes stale. */
        if (!seen_fresh || stale_ctr >= STALE_LIMIT) {
            motor_all_stop();
            MAILBOX->fw_state = FW_ARMED_IDLE;
            continue;
        }

        /* Armed + fresh: drive the wheels from the mailbox. */
        int left  = (int)MAILBOX->left_speed;
        int right = (int)MAILBOX->right_speed;
        motor_set_wheel(left, right);

        /* Blade: OFF by default; spins ONLY when the host sets blade_on. */
        motor_blade((int)MAILBOX->blade_on);

        /* WRITE-PATH TEST (SWD): pad0=0xA5 -> SPI-write blade RUN=0 (must STOP a
         * spinning blade if the write reaches the chip); pad0=0x5A -> RUN=1. */
        if (MAILBOX->pad0 == 0xA5U)      { MAILBOX->pad0 = 0U; motor_blade_run(0); }
        else if (MAILBOX->pad0 == 0x5AU) { MAILBOX->pad0 = 0U; motor_blade_run(1); }
        else if (MAILBOX->pad0 == 0xB0U) { MAILBOX->pad0 = 0U; bms_send_bcmds(); }
        /* AUDIT 2026-07-10 fast/slow A/B: 0x50=FAST wheels, 0x51=SLOW wheels
         * (re-config A4963 + re-arm). Test both in one power-cycle, no reflash. */
        else if (MAILBOX->pad0 == 0x50U) { MAILBOX->pad0 = 0U; motor_wheels_reconfig(0); }
        else if (MAILBOX->pad0 == 0x51U) { MAILBOX->pad0 = 0U; motor_wheels_reconfig(1); }

        MAILBOX->fw_state = FW_DRIVING;
        /* (Pacing happens at the top of the loop — see LOOP_PACE_ITERS.) */
    }

    /* not reached */
    return 0;
}
