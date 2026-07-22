/*
 * battery.c — ADC0 battery sensing (GD32F305), mower-own.
 *
 * Path (VERIFIED): ADC0 ch15 (PC5) = pack voltage, ch14 (PC4) = current.
 * Software-triggered single conversions, polled EOC (no DMA). Sample time 239.5.
 *
 * Voltage scale (VERIFIED): pack_mV = raw * 5.4277. The factory sums 2 samples
 * and multiplies by 2.7138548; a single sample → 2 * 2.7138548 = 5.4277.
 * We use fixed-point (raw * 54277 / 10000) to avoid floating point.
 *
 * Percent: TODO(batt) approximate 5-cell Li-ion curve, linear 15000..21000 mV
 * → 0..100 %. Calibrate on the bench.
 */
#include "battery.h"
#include "gd32_regs.h"

/* Cached results (.bss zero-init by startup). */
static uint16_t s_mv;
static uint8_t  s_pct;

/* Bounded ADC EOC wait. */
#define ADC_EOC_TIMEOUT  1000000U

/* Percent-curve endpoints (5-cell Li-ion). TODO(batt): calibrate on bench. */
#define BATT_MV_EMPTY  15000U   /* 5 x 3.0 V = 0 %   */
#define BATT_MV_FULL   21000U   /* 5 x 4.2 V = 100 % */

static void adc_short_delay(volatile uint32_t n)
{
    while (n--) { __asm__ volatile("nop"); }
}

void battery_init(void)
{
    /* Sample time 239.5 cycles for ch14 and ch15. SAMPT0 covers ch10..17:
     * 3 bits/ch, ch14 at bits [14:12], ch15 at bits [17:15]. */
    ADC0_SAMPT0 = (ADC0_SAMPT0
                   & ~((0x7U << (3 * (BATT_CURR_CH - 10)))
                     | (0x7U << (3 * (BATT_VOLT_CH - 10)))))
                | (ADC_SAMPT_239_5 << (3 * (BATT_CURR_CH - 10)))
                | (ADC_SAMPT_239_5 << (3 * (BATT_VOLT_CH - 10)));

    /* Regular trigger source = software (SWRCST), external-trigger enabled. */
    ADC0_CTL1 |= ADC0_CTL1_ETSRC_SW | ADC0_CTL1_ETERC;

    /* Power on the ADC, let it stabilize. */
    ADC0_CTL1 |= ADC0_CTL1_ADCON;
    adc_short_delay(10000);

    /* Calibrate: reset calibration, then start calibration, wait for it to clear. */
    ADC0_CTL1 |= ADC0_CTL1_RSTCLB;
    { uint32_t t = ADC_EOC_TIMEOUT; while ((ADC0_CTL1 & ADC0_CTL1_RSTCLB) && --t) {} }
    ADC0_CTL1 |= ADC0_CTL1_CLB;
    { uint32_t t = ADC_EOC_TIMEOUT; while ((ADC0_CTL1 & ADC0_CTL1_CLB) && --t) {} }
}

/* Convert one regular channel, single-shot, polled EOC. Returns 12-bit raw
 * (0 on timeout). */
static uint16_t adc_read_channel(unsigned ch)
{
    /* RSQ0 length = 1 conversion (RL[3:0] = 0 → 1 conversion). */
    ADC0_RSQ0 = 0;
    /* First (and only) conversion = `ch` in RSQ2 bits [4:0]. */
    ADC0_RSQ2 = (ch & 0x1FU);

    /* Clear EOC, software-start the regular conversion. */
    ADC0_STAT = 0;
    ADC0_CTL1 |= ADC0_CTL1_SWRCST;

    uint32_t t = ADC_EOC_TIMEOUT;
    while ((ADC0_STAT & ADC0_STAT_EOC) == 0U) {
        if (--t == 0U) { return 0; }   /* ADC dead → 0, never wedge */
    }
    return (uint16_t)(ADC0_RDATA & 0x0FFFU);
}

void battery_sample(void)
{
    uint16_t raw_v = adc_read_channel(BATT_VOLT_CH);
    (void)adc_read_channel(BATT_CURR_CH);   /* current read for future use */

    /* pack_mV = raw * 5.4277 (fixed-point raw*54277/10000). */
    uint32_t mv = ((uint32_t)raw_v * 54277U) / 10000U;
    if (mv > 0xFFFFU) { mv = 0xFFFFU; }
    s_mv = (uint16_t)mv;

    /* Percent: linear map, clamped. TODO(batt): real curve. */
    uint32_t pct;
    if (mv <= BATT_MV_EMPTY) {
        pct = 0;
    } else if (mv >= BATT_MV_FULL) {
        pct = 100;
    } else {
        pct = ((mv - BATT_MV_EMPTY) * 100U) / (BATT_MV_FULL - BATT_MV_EMPTY);
    }
    s_pct = (uint8_t)pct;
}

uint16_t battery_mv(void)  { return s_mv; }
uint8_t  battery_pct(void) { return s_pct; }
