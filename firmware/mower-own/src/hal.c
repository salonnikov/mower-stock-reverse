/*
 * hal.c — HAL implementation for mower-own (GD32F305).
 *
 * Full 3-motor drive path (2 wheels + blade) + power latch are filled with
 * VERIFIED values from the hardware-inventory report (2026-07-04). Battery ADC
 * and odometry ISRs remain TODO(hw).
 */
#include "hal.h"
#include "gd32_regs.h"

/* Short busy-wait; crude but startup-only. */
static void hal_delay_loop(volatile uint32_t n)
{
    while (n--) {
        __asm__ volatile("nop");
    }
}

void hal_short_delay(uint32_t n);
void hal_short_delay(uint32_t n)
{
    hal_delay_loop(n);
}

/* ===========================================================================
 * hal_power_latch — hold system power (MUST be done first).
 *
 * MAIN latch = PE12 HIGH (VERIFIED: the factory bootloader's very first GPIO
 * action at board init, FUN_08000f38, is PE12 output HIGH; both the factory
 * app and bootloader power off by holding PE12 LOW forever). The bootloader
 * has already latched PE12 HIGH before jumping to us — we re-assert it to be
 * self-sufficient. PB0 is the secondary latch the bootloader sets on key
 * confirm ("key_press_power_on"); keep asserting it too.
 * Standalone so main() can call it before anything slow.
 * ========================================================================= */
void hal_power_latch(void);
void hal_power_latch(void)
{
    RCU_APB2EN |= RCU_APB2EN_PBEN | RCU_APB2EN_PEEN;   /* GPIOB + GPIOE clocks */
    (void)RCU_APB2EN;                                  /* read-back barrier */
    /* Drive HIGH first (BOP), then configure as output, so each latch asserts
     * the instant the pin becomes an output. PE12 (MAIN) first. */
    GPIO_REG(PWR_MAIN_PORT, GPIO_BOP_OFF) = (1U << PWR_MAIN_PIN);
    GPIO_SET_CFG(PWR_MAIN_PORT, PWR_MAIN_PIN, GPIO_CFG_OUT_PP_50);
    GPIO_REG(PWR_MAIN_PORT, GPIO_BOP_OFF) = (1U << PWR_MAIN_PIN);

    GPIO_REG(PWR_LATCH_PORT, GPIO_BOP_OFF) = (1U << PWR_LATCH_PIN);
    GPIO_SET_CFG(PWR_LATCH_PORT, PWR_LATCH_PIN, GPIO_CFG_OUT_PP_50);
    GPIO_REG(PWR_LATCH_PORT, GPIO_BOP_OFF) = (1U << PWR_LATCH_PIN);
}

/* ===========================================================================
 * hal_clock_init — 8 MHz HXTAL -> 120 MHz SYSCLK (HCLK/APB2=120, APB1=60).
 * Exact ordered writes per the verified report.
 * ========================================================================= */
/* Bounded spin: wait until (expr) is true or the counter expires. Sets `ok`=0 on
 * timeout. ~1M iterations is well beyond any real stabilization time. */
#define CLK_TIMEOUT  1000000U
#define WAIT_BOUNDED(cond, ok)                                          \
    do {                                                                \
        uint32_t _t = CLK_TIMEOUT;                                      \
        while (!(cond)) {                                               \
            if (--_t == 0U) { (ok) = 0; break; }                       \
        }                                                               \
    } while (0)

void hal_clock_init(void)
{
    int ok = 1;

    /* 1. Enable HXTAL, wait stable (bounded). */
    RCU_CTL |= RCU_CTL_HXTALEN;
    WAIT_BOUNDED((RCU_CTL & RCU_CTL_HXTALSTB) != 0U, ok);
    /* If HXTAL never came up, do NOT try to build the PLL from it. Bail out and
     * run on the reset HSI clock (~8 MHz): the firmware still boots and the
     * heartbeat/poll loop, SPI and PWM all work (just slower). Never hang. */
    if (!ok) {
        return;
    }

    /* 2. Enable PMU clock, set LDO high-drive select. */
    RCU_APB1EN |= RCU_APB1EN_PMUEN;
    PMU_CTL |= PMU_CTL_LDO_HI;

    /* 3. CFG0: PLL source = PREDV0, AHB/1, APB1/2, APB2/1, PLL x30. */
    RCU_CFG0 |= RCU_CFG0_OR1;
    RCU_CFG0 &= RCU_CFG0_AND;
    RCU_CFG0 |= RCU_CFG0_OR2;

    /* 4. CFG1: PREDV1/4, PLL1 x8, PREDV0/4, PREDV0SEL=PLL1. */
    RCU_CFG1 &= RCU_CFG1_AND;
    RCU_CFG1 |= RCU_CFG1_OR;

    /* 5. Enable PLL1, wait; then main PLL, wait (bounded). */
    RCU_CTL |= RCU_CTL_PLL1EN;
    WAIT_BOUNDED((RCU_CTL & RCU_CTL_PLL1STB) != 0U, ok);
    RCU_CTL |= RCU_CTL_PLLEN;
    WAIT_BOUNDED((RCU_CTL & RCU_CTL_PLLSTB) != 0U, ok);

    /* 6. PMU high-drive handshakes (required for 120 MHz) (bounded). */
    PMU_CTL |= PMU_CTL_HDEN;
    WAIT_BOUNDED((PMU_CS & PMU_CS_HDRF) != 0U, ok);
    PMU_CTL |= PMU_CTL_HDS;
    WAIT_BOUNDED((PMU_CS & PMU_CS_HDSRF) != 0U, ok);

    /* If any PLL/PMU step failed to stabilize, don't switch SYSCLK to a PLL that
     * isn't locked — stay on HSI. Safe fallback, no hang. */
    if (!ok) {
        return;
    }

    /* Flash wait-states: 120 MHz needs 2 WS. Set before switching SYSCLK. */
    FMC_WS = (FMC_WS & ~0x7U) | FMC_WS_WSCNT_2;

    /* 7. Switch system clock to PLL, wait until selected (bounded). */
    RCU_CFG0 &= ~RCU_CFG0_SCS_MASK;
    RCU_CFG0 |= RCU_CFG0_SCS_PLL;
    WAIT_BOUNDED(((RCU_CFG0 >> 2) & RCU_CFG0_SCS_MASK) == RCU_CFG0_SCS_PLL, ok);
    /* Whether or not the switch confirmed, we've done our best; fall through and
     * boot. (SCS not confirming is extremely unlikely once PLL is locked.) */
}

/* ===========================================================================
 * hal_gpio_init — peripheral clocks + pin configuration for the LEFT path.
 * ========================================================================= */
void hal_gpio_init(void)
{
    /* ADC clock prescaler: ADCCLK = PCLK2 / 8 = 15 MHz (<= 40 MHz). Set BEFORE
     * enabling ADC0. */
    RCU_CFG0 = (RCU_CFG0 & ~RCU_CFG0_ADCPSC_MASK) | RCU_CFG0_ADCPSC_DIV8;

    /* Peripheral clock enables (VERIFIED). GPIOB already on from power latch.
     * ADC0 + GPIOC/GPIOB for battery sensing; DMA0 reserved for future ADC DMA. */
    RCU_APB2EN |= RCU_APB2EN_AFEN | RCU_APB2EN_PAEN
                | RCU_APB2EN_PBEN | RCU_APB2EN_PCEN
                | RCU_APB2EN_PDEN | RCU_APB2EN_PEEN
                | RCU_APB2EN_ADC0EN;
    RCU_APB1EN |= RCU_APB1EN_TIMER2EN | RCU_APB1EN_SPI1EN;
    RCU_AHBEN  |= RCU_AHBEN_DMA0EN;
    (void)RCU_APB2EN;

    /* --- SPI1 signal pins on GPIOB --- */
    /* PB13 = SCK (AF PP 50), PB15 = MOSI (AF PP 50), PB14 = MISO (input float). */
    GPIO_SET_CFG(GPIOB_BASE, SPI1_SCK_PIN,  GPIO_CFG_AF_PP_50);
    GPIO_SET_CFG(GPIOB_BASE, SPI1_MOSI_PIN, GPIO_CFG_AF_PP_50);
    GPIO_SET_CFG(GPIOB_BASE, SPI1_MISO_PIN, GPIO_CFG_IN_FLOAT);

    /* --- Shared A4963 enable: PB12 GP push-pull output, HIGH (enables all 3) --- */
    GPIO_REG(A4963_EN_PORT, GPIO_BOP_OFF) = (1U << A4963_EN_PIN);
    GPIO_SET_CFG(A4963_EN_PORT, A4963_EN_PIN, GPIO_CFG_OUT_PP_50);

    /* --- Per-motor CS on GPIOD (PD5/PD4/PD3): GP push-pull output, idle HIGH --- */
    GPIO_REG(A4963_CS_PORT, GPIO_BOP_OFF) =
          (1U << A4963_CS_PIN_LEFT)
        | (1U << A4963_CS_PIN_RIGHT)
        | (1U << A4963_CS_PIN_BLADE);
    GPIO_SET_CFG(A4963_CS_PORT, A4963_CS_PIN_LEFT,  GPIO_CFG_OUT_PP_50);
    GPIO_SET_CFG(A4963_CS_PORT, A4963_CS_PIN_RIGHT, GPIO_CFG_OUT_PP_50);
    GPIO_SET_CFG(A4963_CS_PORT, A4963_CS_PIN_BLADE, GPIO_CFG_OUT_PP_50);

    /* --- Per-wheel RUN/COAST line: PD7 (LEFT). PD4 (RIGHT) is already an output
     * above (shared with the right CS pin). Idle HIGH = coasted/braked (safe): a
     * wheel only runs when hal_motor_run() pulls its line LOW. --- */
    GPIO_REG(WHEEL_RUN_PORT, GPIO_BOP_OFF) = (1U << WHEEL_RUN_PIN_LEFT);
    GPIO_SET_CFG(WHEEL_RUN_PORT, WHEEL_RUN_PIN_LEFT, GPIO_CFG_OUT_PP_50);
    GPIO_REG(WHEEL_RUN_PORT, GPIO_BOP_OFF) = (1U << WHEEL_RUN_PIN_LEFT);

    /* --- Power button PE10 + PE11 (active-low), charge detect PE8 (active-high):
     * inputs with pull. In the GD32/STM32F1 GPIO model, input-with-pull selects
     * pull-UP when the ODR bit is 1, pull-DOWN when 0. Active-low buttons need
     * pull-UP → set PE10/PE11 ODR high (BOP). Leave PE8 ODR low = pull-DOWN
     * (correct for the active-high charge line). --- */
    GPIO_SET_CFG(BTN_PORT, BTN_PIN_A, GPIO_CFG_IN_PULL);
    GPIO_SET_CFG(BTN_PORT, BTN_PIN_B, GPIO_CFG_IN_PULL);
    GPIO_REG(BTN_PORT, GPIO_BOP_OFF) = BTN_MASK_A | BTN_MASK_B;   /* pull-UP */
    GPIO_SET_CFG(CHARGE_PORT, CHARGE_PIN, GPIO_CFG_IN_PULL);      /* ODR low = pull-DOWN */

    /* --- Battery ADC inputs: PC5 (ch15, pack voltage), PC4 (ch14, current) as
     * analog inputs. --- */
    GPIO_SET_CFG(BATT_VOLT_PORT, BATT_VOLT_PIN, GPIO_CFG_IN_ANALOG);
    GPIO_SET_CFG(BATT_CURR_PORT, BATT_CURR_PIN, GPIO_CFG_IN_ANALOG);

    /* TIMER2 PWM output pins PC7/PC8/PC9 (AF PP) are configured in pwm_init()
     * after the AFIO full remap. */
}

/* ===========================================================================
 * hal_aux_power_on — raise the auxiliary POWER RAILS the factory app holds HIGH
 * during operation: PE7, PE9 (GPIOE) and PD11 (GPIOD).
 *
 * ROOT-CAUSE FIX (proven from the factory dump, 2026-07-05):
 *   The factory power-OFF routine (app FUN_080714d8 @0x080714d8) pulls THREE
 *   things LOW together, in this order, right before parking PE12 low:
 *       gpio_bit_reset(GPIOB, 0x1000)  — PB12 (A4963 shared gate-driver enable)
 *       gpio_bit_reset(GPIOE, 0x0200)  — PE9  (aux rail)
 *       gpio_bit_reset(GPIOD, 0x0800)  — PD11 (aux rail)
 *   i.e. PE9 and PD11 are ACTIVE-HIGH power-domain enables that sit HIGH the
 *   whole time the machine runs (they are grouped with the A4963 enable). They
 *   are raised HIGH by the factory's device power-on op:
 *       FUN_08026e20: gpio_bit_set(GPIOE,0x200); gpio_bit_set(GPIOD,0x800)
 *   PE7 is likewise driven HIGH at platform init by FUN_0802aaf0
 *       (gpio_init(GPIOE, OUT_PP, 0x80); gpio_bit_set(GPIOE,0x80)),
 *   and lowered on shutdown.
 *
 * Our firmware ALREADY lowers all three at power-off (button.c button_power_off)
 * but never RAISED them — a pure asymmetry. If (as the power-off grouping with
 * PB12 strongly suggests) any of these gates the A4963 supply, the drivers were
 * UNPOWERED: they cannot drive SDO (so every diagnostic read returns the
 * floating/all-faults word 0xFFFF) and they cannot commutate (so nothing spins).
 * Raising them here mirrors the factory operating state. Must run BEFORE we
 * configure the A4963s over SPI, so the drivers are powered when we talk to them.
 *
 * NOTE ON CONFIDENCE: that PE9/PD11/PE7 are factory-operated active-high rails is
 * PROVEN from the dump; that one of them specifically feeds the A4963 VBB/gate
 * supply is a HYPOTHESIS (the board net is not traced in the dump). This is the
 * single remaining discrepancy vs the factory after the SPI path was verified
 * byte-for-byte identical.
 * ========================================================================= */
void hal_aux_power_on(void);
void hal_aux_power_on(void)
{
    /* GPIOE clock is up from hal_power_latch (PEEN); GPIOD from hal_gpio_init
     * (PDEN). Drive HIGH first (BOP), then make each an output, so the rail
     * asserts the instant the pin becomes an output.
     *
     * Only the PROVEN factory aux rails PE7/PE9/PD11 are driven. The earlier
     * shotgun candidates (PA0/PD7/PD8/PD10/PE0) were DISPROVEN on hardware
     * (2026-07-06 live capture: A4963s are armed at boot and driven by PWM duty
     * alone; PD8 is USART2-TX) and have been removed. */
    GPIO_REG(GPIOE_BASE, GPIO_BOP_OFF) = (1U << PWR_AUX_E7_PIN)
                                       | (1U << PWR_AUX_E9_PIN);
    GPIO_SET_CFG(GPIOE_BASE, PWR_AUX_E7_PIN, GPIO_CFG_OUT_PP_50);
    GPIO_SET_CFG(GPIOE_BASE, PWR_AUX_E9_PIN, GPIO_CFG_OUT_PP_50);
    GPIO_REG(GPIOE_BASE, GPIO_BOP_OFF) = (1U << PWR_AUX_E7_PIN)
                                       | (1U << PWR_AUX_E9_PIN);

    GPIO_REG(GPIOD_BASE, GPIO_BOP_OFF) = (1U << PWR_AUX_D11_PIN);
    GPIO_SET_CFG(GPIOD_BASE, PWR_AUX_D11_PIN, GPIO_CFG_OUT_PP_50);
    GPIO_REG(GPIOD_BASE, GPIO_BOP_OFF) = (1U << PWR_AUX_D11_PIN);

    /* EXPERIMENTAL (2026-07-07): a live GPIO diff (factory idle vs ours) shows the
     * factory drives PD7 and PE0 HIGH the whole time it runs, and our firmware
     * does not. Both were independently flagged as a relay / load-switch-with-
     * power-good — the top remaining candidates for the motor-VBB enable (the
     * A4963s read silent for us = unpowered). Drive both HIGH here, BEFORE A4963
     * config, as a clean test. Remove if disproven. */
    /* TEST (2026-07-07): live factory GPIO capture (idle AND state-5 driving)
     * shows the factory keeps PE9 and PD11 LOW the whole time — even while the
     * wheels spin. Our firmware wrongly drove them HIGH ("aux rails"). If our
     * PE9/PD11 HIGH disables the switched 20U drive rail (active-low), that's why
     * our wheels are dead while the blade (separate always-on rail) spins. Force
     * PE9 and PD11 LOW (overriding the HIGH above) to match the factory. */
    GPIO_REG(GPIOE_BASE, GPIO_BC_OFF) = (1U << PWR_AUX_E9_PIN);   /* PE9 LOW */
    GPIO_REG(GPIOD_BASE, GPIO_BC_OFF) = (1U << PWR_AUX_D11_PIN);  /* PD11 LOW */

    /* MEASURED factory driving-state GPIO (live diff factory-vs-ours 2026-07-09,
     * both captured WHILE DRIVING). Three OUTPUT pins the factory drives and we
     * did NOT match. NB: PD10 is driven LOW by the factory — the old 2026-07-06
     * "shotgun" drove it HIGH (wrong state) and also drove PD8 (breaking BMS), so
     * its DISPROOF doesn't apply to these exact states. Replicate byte-for-byte. */
    GPIO_REG(GPIOA_BASE, GPIO_BOP_OFF) = (1U << 0);              /* PA0  HIGH  */
    GPIO_SET_CFG(GPIOA_BASE, 0U, GPIO_CFG_OUT_PP_50);
    GPIO_REG(GPIOA_BASE, GPIO_BOP_OFF) = (1U << 0);
    GPIO_REG(GPIOE_BASE, GPIO_BOP_OFF) = (1U << 0);              /* PE0  HIGH (OUT-PP, override boot OD-LOW) */
    GPIO_SET_CFG(GPIOE_BASE, 0U, GPIO_CFG_OUT_PP_50);
    GPIO_REG(GPIOE_BASE, GPIO_BOP_OFF) = (1U << 0);
    GPIO_REG(GPIOD_BASE, GPIO_BC_OFF)  = (1U << 10);             /* PD10 LOW   */
    GPIO_SET_CFG(GPIOD_BASE, 10U, GPIO_CFG_OUT_PP_50);
    GPIO_REG(GPIOD_BASE, GPIO_BC_OFF)  = (1U << 10);

    /* Match remaining factory driving-state INPUT pins pin-for-pin (measured diff
     * 2026-07-09). Inputs cannot gate a rail, but replicate for a clean full match.
     * pull-UP: PA8,PC6,PD12-15,PE2,PE3 ; analog: PB1,PC1,PC2,PC3 ; float: PB2.
     * (PE8 charge / PE10-11 buttons kept at OUR pulls — functional, intentional.) */
    GPIO_SET_CFG(GPIOA_BASE, 8U, GPIO_CFG_IN_PULL);  GPIO_REG(GPIOA_BASE, GPIO_BOP_OFF) = (1U << 8);
    GPIO_SET_CFG(GPIOC_BASE, 6U, GPIO_CFG_IN_PULL);  GPIO_REG(GPIOC_BASE, GPIO_BOP_OFF) = (1U << 6);
    for (unsigned p = 12U; p <= 15U; p++) {
        GPIO_SET_CFG(GPIOD_BASE, p, GPIO_CFG_IN_PULL);
        GPIO_REG(GPIOD_BASE, GPIO_BOP_OFF) = (1U << p);          /* pull-UP */
    }
    GPIO_SET_CFG(GPIOE_BASE, 2U, GPIO_CFG_IN_PULL);  GPIO_REG(GPIOE_BASE, GPIO_BOP_OFF) = (1U << 2);
    GPIO_SET_CFG(GPIOE_BASE, 3U, GPIO_CFG_IN_PULL);  GPIO_REG(GPIOE_BASE, GPIO_BOP_OFF) = (1U << 3);
    GPIO_SET_CFG(GPIOB_BASE, 1U, GPIO_CFG_IN_ANALOG);
    GPIO_SET_CFG(GPIOC_BASE, 1U, GPIO_CFG_IN_ANALOG);
    GPIO_SET_CFG(GPIOC_BASE, 2U, GPIO_CFG_IN_ANALOG);
    GPIO_SET_CFG(GPIOC_BASE, 3U, GPIO_CFG_IN_ANALOG);
    GPIO_SET_CFG(GPIOB_BASE, 2U, GPIO_CFG_IN_FLOAT);
}

/* ===========================================================================
 * hal_a4963_enable — drive PB12 HIGH: the shared gate-driver enable for ALL 3
 * A4963s. Re-asserts HIGH right at motor init. There is NO per-motor enable.
 * ========================================================================= */
void hal_a4963_enable(void);
void hal_a4963_enable(void)
{
    GPIO_REG(A4963_EN_PORT, GPIO_BOP_OFF) = (1U << A4963_EN_PIN);
}

/* ===========================================================================
 * hal_motor_run — per-WHEEL RUN/COAST line (the fix for "wheels dead, blade
 * spins"). The factory drives PD7 (LEFT) / PD4 (RIGHT) LOW to run and HIGH to
 * coast/brake; the blade has no such line. run!=0 => LOW (run), run==0 => HIGH
 * (coast). Blade index is a no-op. ========================================= */
void hal_motor_run(unsigned motor, int run);
void hal_motor_run(unsigned motor, int run)
{
    uint32_t mask;
    switch (motor) {
    case MOTOR_LEFT:  mask = (1U << WHEEL_RUN_PIN_LEFT);  break;   /* PD7 */
    case MOTOR_RIGHT: mask = (1U << WHEEL_RUN_PIN_RIGHT); break;   /* PD4 */
    default:          return;   /* MOTOR_BLADE has no run/coast line */
    }
    if (run) {
        GPIO_REG(WHEEL_RUN_PORT, GPIO_BC_OFF)  = mask;   /* LOW  = run   */
    } else {
        GPIO_REG(WHEEL_RUN_PORT, GPIO_BOP_OFF) = mask;   /* HIGH = coast */
    }
}

/* Map a logical motor index to its CS pin bit mask on GPIOD. */
static uint32_t a4963_cs_mask(unsigned motor)
{
    switch (motor) {
    case MOTOR_LEFT:  return (1U << A4963_CS_PIN_LEFT);
    case MOTOR_RIGHT: return (1U << A4963_CS_PIN_RIGHT);
    case MOTOR_BLADE: return (1U << A4963_CS_PIN_BLADE);
    default:          return 0U;
    }
}

/* ===========================================================================
 * SPI1 to A4963.
 * ========================================================================= */
void spi_a4963_init(void)
{
    /* CTL0 = 0x0B2F (mode3, master, /64, MSB-first, SW-NSS, 16-bit), then enable. */
    SPI_REG(A4963_SPI_BASE, SPI_CTL0_OFF) = SPI_CTL0_A4963;
    SPI_REG(A4963_SPI_BASE, SPI_CTL0_OFF) |= SPI_CTL0_SPIEN;
}

/* Bounded SPI wait: many core cycles per SPI bit even at 120 MHz, so a large
 * cap is safe and never hangs if the peripheral is dead. */
#define SPI_TIMEOUT  1000000U

/*
 * Transfer one 16-bit word over SPI1 (raw; CS handled by caller). Returns 0 on
 * success and writes the readback to *out; returns -1 on a TBE/RBNE timeout so
 * a stuck SPI can't wedge init.
 */
static int spi_xfer16(uint16_t word, uint16_t *out)
{
    uint32_t t = SPI_TIMEOUT;
    while ((SPI_REG(A4963_SPI_BASE, SPI_STAT_OFF) & SPI_STAT_TBE) == 0U) {
        if (--t == 0U) { return -1; }
    }
    SPI_REG(A4963_SPI_BASE, SPI_DATA_OFF) = word;

    t = SPI_TIMEOUT;
    while ((SPI_REG(A4963_SPI_BASE, SPI_STAT_OFF) & SPI_STAT_RBNE) == 0U) {
        if (--t == 0U) { return -1; }
    }
    *out = (uint16_t)SPI_REG(A4963_SPI_BASE, SPI_DATA_OFF);
    return 0;
}

int a4963_write(unsigned motor, unsigned reg, unsigned data)
{
    if (motor > 2U || reg > 7U) {
        return -1;
    }

    /* Frame: [15:13]=reg, [12]=write(0x1000), [11:0]=data. */
    uint16_t frame = (uint16_t)(((reg & 0x7U) << 13)
                                | 0x1000U            /* write bit */
                                | (data & 0x0FFFU));

    /* Select this motor's active-low CS on GPIOD (PD5/PD4/PD3). */
    uint32_t cs_pin = a4963_cs_mask(motor);

    /* Assert CS low, transfer, deassert high. Always deassert CS even on timeout
     * so we don't leave a driver selected. */
    uint16_t rb = 0;
    GPIO_REG(A4963_CS_PORT, GPIO_BC_OFF)  = cs_pin;
    int xfer = spi_xfer16(frame, &rb);
    GPIO_REG(A4963_CS_PORT, GPIO_BOP_OFF) = cs_pin;

    if (xfer != 0) {
        return -3;   /* SPI timeout — bus dead; caller stops retrying/init */
    }

    /*
     * NOTE: do NOT compare `rb` against `data`. Verified against the factory SPI
     * routine (FUN_0808408c @0x0808408c): an A4963 WRITE frame does NOT echo the
     * written value on MISO — MISO carries the 16-bit diagnostic/fault word. The
     * factory verifies a write by issuing a SEPARATE READ frame and comparing,
     * retrying up to 50x. A single-frame "rb == data" test (the old code here)
     * can never pass, so it made every write report a bogus failure. We report
     * success once the frame has clocked out; `rb` is available for future fault
     * decoding if needed.
     */
    (void)rb;
    return 0;
}

/* ===========================================================================
 * a4963_read_reg — read one A4963 register back with a WR=0 READ frame.
 *
 * THE FACTORY VERIFY METHOD (FUN_0808408c @0x0808408c): after a write, the
 * factory issues a SEPARATE read frame — [15:13]=reg, [12]=WR=0, [11:0]
 * don't-care — and compares the low 12 bits of the SDO word against what it
 * wrote (retrying up to 50x). During a WR=0 frame the A4963 shifts the
 * addressed register's 12-bit content out on SDO[11:0] (SDO[15:13] carry the
 * FF/POR/SE status bits). Our previous instrumentation used a WR=1 frame
 * instead (diagnostic word on SDO) — not the factory method, and it read
 * 0xFFFF; replaced by this read-back.
 *
 * Writes the 12-bit register content to *out (0xFFF on SPI timeout).
 * Returns 0 on success, -1 on bad args, -3 on SPI timeout.
 * ========================================================================= */
int a4963_read_reg(unsigned motor, unsigned reg, uint16_t *out)
{
    if (motor > 2U || reg > 7U) {
        if (out) { *out = 0x0FFFU; }
        return -1;
    }

    /* WR=0 read frame: [15:13]=reg, [12]=0 (NO 0x1000 write bit), [11:0]=0. */
    uint16_t frame = (uint16_t)((reg & 0x7U) << 13);

    uint32_t cs_pin = a4963_cs_mask(motor);
    uint16_t rb = 0;

    /* BYTE-EXACT factory read (FUN_0806caac @0x0806caac): ONE full-duplex frame.
     * assert CS -> wait TBE -> write reg<<13 -> wait RBNE -> read SDO -> deassert.
     * The A4963 shifts the addressed register out on SDO during the SAME frame
     * (single frame, no pre-drain, no 2nd frame). The prior 2-frame/pre-drain
     * variant diverged from the factory and still read 0xFFFF; this matches the
     * factory verify path that provably works at boot. spi_xfer16 already drains
     * RBNE by reading DATA each transfer, so no separate pre-drain is needed. */
    GPIO_REG(A4963_CS_PORT, GPIO_BC_OFF)  = cs_pin;   /* CS assert   */
    int xfer = spi_xfer16(frame, &rb);                 /* single frame */
    GPIO_REG(A4963_CS_PORT, GPIO_BOP_OFF) = cs_pin;   /* CS deassert */

    if (xfer != 0) {
        if (out) { *out = 0xFFFFU; }   /* bus dead — all-ones = invalid */
        return -3;
    }

    /* Return the FULL 16-bit SDO word (diagnostic): [15:13]=FF/POR/SE status bits
     * (=1 on a live, selected chip), [11:0]=register content. 0x0000 here means the
     * chip drove NOTHING back (CS not selecting / SDO not reaching MISO). Callers
     * that compare config mask the low 12 bits themselves. */
    if (out) { *out = rb; }
    return 0;
}

/* ===========================================================================
 * PWM (TIMER2) — AFIO full remap (PC6..PC9) + CH1 (BLADE)/CH2 (RIGHT)/CH3 (LEFT).
 * ========================================================================= */
void pwm_init(void)
{
    /* AFIO full remap: TIMER2 channels on PC6/PC7/PC8/PC9. Read-modify-write of
     * ONLY the TIMER2_REMAP field (PCF0[11:10] = 0b11); leave every other remap
     * bit alone (the old blast-write also set stray bits 17/19/20). */
    AFIO_PCF0 = (AFIO_PCF0 & ~AFIO_PCF0_TIMER2_FULLREMAP) | AFIO_PCF0_TIMER2_FULLREMAP;

    /* PWM pins as AF push-pull. CH1=BLADE PC7, CH2=RIGHT PC8, CH3=LEFT PC9. */
    GPIO_SET_CFG(GPIOC_BASE, 9U, GPIO_CFG_AF_PP_50);   /* PC9 = CH3 LEFT  */
    GPIO_SET_CFG(GPIOC_BASE, 8U, GPIO_CFG_AF_PP_50);   /* PC8 = CH2 RIGHT */
    GPIO_SET_CFG(GPIOC_BASE, 7U, GPIO_CFG_AF_PP_50);   /* PC7 = CH1 BLADE */

    /* Timebase. */
    TIMER2_PSC = TIMER2_PWM_PSC;           /* 9 */
    TIMER2_CAR = TIMER2_PWM_PERIOD;        /* 0x1C20 = 7200 */

    /* Channel output mode = PWM mode0 + preload for CH1/CH2/CH3. */
    TIMER2_CHCTL0 = TIMER2_CH1_OCM_PWM0 | TIMER2_CH1_OCPE;   /* CH1 (blade) */
    TIMER2_CHCTL1 = TIMER2_CH3_OCM_PWM0 | TIMER2_CH3_OCPE
                  | TIMER2_CH2_OCM_PWM0 | TIMER2_CH2_OCPE;    /* CH3 + CH2   */

    /* Enable channel outputs (CH1 blade + CH2 right + CH3 left). */
    TIMER2_CHCTL2 |= TIMER2_CH1EN | TIMER2_CH2EN | TIMER2_CH3EN;

    /* Start with zero duty on all (motors held; blade OFF for safety). */
    TIMER2_CH1CV = 0;
    TIMER2_CH2CV = 0;
    TIMER2_CH3CV = 0;

    /* Auto-reload shadow + counter enable. */
    TIMER2_CTL0 |= TIMER2_CTL0_ARSE | TIMER2_CTL0_CEN;
}

void pwm_set_duty(unsigned ch, unsigned duty)
{
    if (duty > TIMER2_PWM_PERIOD) {
        duty = TIMER2_PWM_PERIOD;   /* saturate to 100% */
    }
    switch (ch) {
    case PWM_CH_BLADE: TIMER2_CH1CV = duty; break;
    case PWM_CH_RIGHT: TIMER2_CH2CV = duty; break;
    case PWM_CH_LEFT:  TIMER2_CH3CV = duty; break;
    default: break;
    }
}
