/*
 * button.c — power/KEY button + charge detect (GD32F305), mirrors the FACTORY
 * app power-off (0x080714d8, reached from process_power_off @0x08069c94).
 *
 * Button = PE10 (mask 0x400) AND PE11 (mask 0x800), ACTIVE-LOW. VERIFIED from
 * the dump: bootloader key scan FUN_0800d034 @0x0800d034 reads
 * gpio_input_bit_get(GPIOE,0x400) & gpio_input_bit_get(GPIOE,0x800); pressed
 * when the AND == 0; it also has the same seen-released boot latch (returns 3
 * for a still-held boot press until the key is first seen released) and a
 * 10-sample debounce before "power off" fires.
 * Charge detect = PE8 (mask 0x100), ACTIVE-HIGH.
 *
 * Long-press power-off (~1.5 s held) → FACTORY sequence (see button_power_off):
 * motors off, aux rails low, then hold PE12 (MAIN latch) LOW forever, feeding
 * the watchdog, until the power rail collapses. NO system reset — resetting
 * re-runs the bootloader which re-latches PE12 HIGH, so the old
 * "PB0-LOW + SYSRESETREQ" implementation just rebooted into our firmware and
 * the mower never turned off.
 *
 * Timing note: we have no calibrated tick yet, so the thresholds are expressed
 * in main-loop iterations. The main loop paces at a crude nop-spin; these counts
 * are APPROXIMATE and easy to tune on the bench once a real 10 ms tick exists.
 */
#include "button.h"
#include "gd32_regs.h"

/* Debounce: consecutive "pressed" samples before we start the hold timer. */
#define BTN_DEBOUNCE_SAMPLES   10U

/* Long-press hold threshold, in main-loop iterations. The main loop is now paced
 * to ~1 ms/iter (LOOP_PACE_ITERS in main.c), so ~1500 iters ≈ 1.5 s. Tune on the
 * bench. TODO: replace with a real ms tick for an exact 1.5 s. */
#define BTN_LONGPRESS_LOOPS    1500U

/* Boot-release latch: consecutive RELEASED samples required, once, before the
 * hold counter may ever accumulate. The user powers ON by HOLDING the key, so at
 * boot the key is pressed; we must NOT treat that still-held boot press as a
 * long-press. Only after the key has been seen released do we arm power-off. */
#define BTN_RELEASE_SAMPLES    3U

/* Persisted across calls; .bss is zero-initialized by startup. */
static uint32_t s_debounce;      /* consecutive pressed samples, capped */
static uint32_t s_hold;          /* iterations held after debounce passed */
static uint32_t s_released_ctr;  /* consecutive released samples seen */
static int      s_seen_released; /* 1 once released >= BTN_RELEASE_SAMPLES */

/* Read the raw button level: 1 = released (both high), 0 = pressed. */
static int button_pressed(void)
{
    uint32_t istat = GPIO_REG(BTN_PORT, GPIO_ISTAT_OFF);
    /* level = level(PE10) AND level(PE11); pressed when AND == 0. */
    int level_a = (istat & BTN_MASK_A) ? 1 : 0;
    int level_b = (istat & BTN_MASK_B) ? 1 : 0;
    return ((level_a & level_b) == 0) ? 1 : 0;
}

int button_charge_present(void)
{
    return (GPIO_REG(CHARGE_PORT, GPIO_ISTAT_OFF) & CHARGE_MASK) ? 1 : 0;
}

int button_pressed_now(void)
{
    return button_pressed();
}

/* Startup-only busy delay, from hal.c. */
extern void hal_short_delay(uint32_t n);

/* ~100 ms of nop at 120 MHz between releasing the aux rails and the PE12-LOW
 * loop (the factory inserts a watchdog-fed busy delay here, 0x8064630). */
#define POWEROFF_SETTLE_ITERS  4000000U

/*
 * button_power_off — the EXACT factory power-off (app routine 0x080714d8 in the
 * dump), which is what physically cuts power:
 *
 *   1. TIMER2 CH1/CH3/CH2 compare = 0        (all motor PWM off)
 *   2. gpio_bit_reset(GPIOB, 0x1000) — PB12  (A4963 shared gate-driver enable)
 *   3. gpio_bit_reset(GPIOE, 0x0200) — PE9   (aux rail)
 *   4. gpio_bit_reset(GPIOD, 0x0800) — PD11  (aux rail)
 *   5. gpio_bit_reset(GPIOE, 0x0080) — PE7   (aux rail, bootloader had it HIGH)
 *   6. watchdog-fed busy delay
 *   7. for(;;){ gpio_bit_reset(GPIOE, 0x1000); feed FWDGT; }   — PE12 = MAIN
 *      power latch held LOW until the rail collapses. NO reset, ever.
 *
 * We additionally release our secondary PB0 latch (the factory bootloader owns
 * that pin; releasing it is harmless and leaves nothing holding power).
 * If an external supply (charger/debugger) keeps the rail up, we stay parked
 * here with motors off and the watchdog fed — same as the factory. */
void button_power_off(void)
{
    /* 1. All motor PWM off (mirrors timer_channel_output_pulse_value_config
     * calls in the factory shutdown; also provably safe if PWM never ran). */
    TIMER2_CH1CV = 0;   /* BLADE */
    TIMER2_CH3CV = 0;   /* LEFT  */
    TIMER2_CH2CV = 0;   /* RIGHT */

    /* 2. Gate-driver enable off: PB12 LOW. */
    GPIO_REG(A4963_EN_PORT, GPIO_BC_OFF) = (1U << A4963_EN_PIN);

    /* 3-5. Aux power rails LOW, factory order: PE9, PD11, PE7. Plain BC writes
     * with no re-config — exactly what the factory gpio_bit_reset does. */
    GPIO_REG(GPIOE_BASE, GPIO_BC_OFF) = (1U << PWR_AUX_E9_PIN);
    GPIO_REG(GPIOD_BASE, GPIO_BC_OFF) = (1U << PWR_AUX_D11_PIN);
    GPIO_REG(GPIOE_BASE, GPIO_BC_OFF) = (1U << PWR_AUX_E7_PIN);

    /* Extra: release our secondary PB0 latch too. */
    GPIO_REG(PWR_LATCH_PORT, GPIO_BC_OFF) = (1U << PWR_LATCH_PIN);

    /* 6. Short settle delay, watchdog fed on both sides. */
    FWDGT_CTL_REG = FWDGT_KEY_FEED;
    hal_short_delay(POWEROFF_SETTLE_ITERS);

    /* 7. MAIN latch: make sure PE12 is a GP push-pull output (the bootloader
     * configured it that way at board init; re-assert to be self-sufficient),
     * then hold it LOW forever, feeding the free watchdog, until power dies. */
    GPIO_SET_CFG(PWR_MAIN_PORT, PWR_MAIN_PIN, GPIO_CFG_OUT_PP_50);
    for (;;) {
        GPIO_REG(PWR_MAIN_PORT, GPIO_BC_OFF) = (1U << PWR_MAIN_PIN);
        FWDGT_CTL_REG = FWDGT_KEY_FEED;
    }
}

void button_poll(void)
{
    if (button_pressed()) {
        /* Reset the released-run counter on any press. */
        s_released_ctr = 0;

        /* Boot-release latch: until the key has been seen RELEASED at least once,
         * do NOT accumulate the hold counter or power off. This prevents the
         * still-held power-on press from being read as a long-press. */
        if (!s_seen_released) {
            return;
        }

        if (s_debounce < BTN_DEBOUNCE_SAMPLES) {
            s_debounce++;
        } else {
            /* Debounced-pressed: accumulate hold time. */
            if (s_hold < BTN_LONGPRESS_LOOPS) {
                s_hold++;
            }
            if (s_hold >= BTN_LONGPRESS_LOOPS) {
                button_power_off();   /* never returns */
            }
        }
    } else {
        /* Released: reset debounce + hold. (Short press = ignored for now.) */
        s_debounce = 0;
        s_hold = 0;

        /* Count consecutive released samples; once enough, arm power-off. */
        if (!s_seen_released) {
            if (s_released_ctr < BTN_RELEASE_SAMPLES) {
                s_released_ctr++;
            }
            if (s_released_ctr >= BTN_RELEASE_SAMPLES) {
                s_seen_released = 1;
            }
        }
    }
}
