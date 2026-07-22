/*
 * gd32_regs.h — GD32F305 peripheral register map for mower-own.
 *
 * Values here are VERIFIED from the factory dump (hardware-inventory report,
 * 2026-07-04) for the full 3-motor drive path (2 wheels + blade) + power latch.
 * Remaining unknowns (battery ADC, odometry ISRs) are marked TODO(hw).
 *
 * Convention: REGISTER(base) macros give a volatile uint32_t lvalue.
 */
#ifndef GD32_REGS_H
#define GD32_REGS_H

#include <stdint.h>

#define REG32(addr)   (*(volatile uint32_t *)(addr))
#define REG16(addr)   (*(volatile uint16_t *)(addr))

/* ---------------------------------------------------------------------------
 * RCU — Reset & Clock Unit (GD32F30x) @ 0x40021000  [VERIFIED]
 * ------------------------------------------------------------------------- */
#define RCU_BASE        0x40021000U
#define RCU_CTL         REG32(RCU_BASE + 0x00U)  /* oscillator control        */
#define RCU_CFG0        REG32(RCU_BASE + 0x04U)  /* clock config 0 (PLL, pre) */
#define RCU_INT         REG32(RCU_BASE + 0x08U)
#define RCU_APB2RST     REG32(RCU_BASE + 0x0CU)
#define RCU_APB1RST     REG32(RCU_BASE + 0x10U)
#define RCU_AHBEN       REG32(RCU_BASE + 0x14U)  /* AHB peripheral clock en   */
#define RCU_APB2EN      REG32(RCU_BASE + 0x18U)  /* APB2 clock en             */
#define RCU_APB1EN      REG32(RCU_BASE + 0x1CU)  /* APB1 clock en             */
#define RCU_BDCTL       REG32(RCU_BASE + 0x20U)
#define RCU_RSTSCK      REG32(RCU_BASE + 0x24U)
#define RCU_CFG1        REG32(RCU_BASE + 0x2CU)  /* PLL2/predivider config    */

/* RCU CTL bits (VERIFIED). */
#define RCU_CTL_HXTALEN   0x00010000U   /* HXTAL enable                        */
#define RCU_CTL_HXTALSTB  0x00020000U   /* HXTAL stable flag                   */
#define RCU_CTL_PLLEN     0x01000000U   /* main PLL enable                     */
#define RCU_CTL_PLLSTB    0x02000000U   /* main PLL stable flag                */
#define RCU_CTL_PLL1EN    0x04000000U   /* PLL1 enable                         */
#define RCU_CTL_PLL1STB   0x08000000U   /* PLL1 stable flag                    */

/* RCU APB2EN clock-enable bits (VERIFIED). */
#define RCU_APB2EN_AFEN     (1U << 0)   /* AFIO alternate-function I/O clock   */
#define RCU_APB2EN_PAEN     (1U << 2)   /* GPIOA (unused currently)             */
#define RCU_APB2EN_PBEN     (1U << 3)   /* GPIOB                               */
#define RCU_APB2EN_PCEN     (1U << 4)   /* GPIOC                               */
#define RCU_APB2EN_PDEN     (1U << 5)   /* GPIOD                               */
#define RCU_APB2EN_PEEN     (1U << 6)   /* GPIOE (buttons PE10/PE11, charge PE8)*/
#define RCU_APB2EN_ADC0EN   (1U << 9)   /* ADC0 (battery lines)                */

/* RCU APB1EN clock-enable bits (VERIFIED). */
#define RCU_APB1EN_TIMER2EN (1U << 1)   /* TIMER2 (wheel/blade PWM)            */
#define RCU_APB1EN_SPI1EN   (1U << 14)  /* SPI1 (A4963 bus)                    */
#define RCU_APB1EN_PMUEN    (1U << 28)  /* PMU (power management, for PLL seq) */

/* RCU AHBEN clock-enable bits. */
#define RCU_AHBEN_DMA0EN    (1U << 0)   /* DMA0 (reserved for future ADC DMA)  */

/* ADC clock prescaler in RCU_CFG0 (ADCPSC): ADCCLK must be <= 40 MHz. From
 * APB2=120 MHz, /8 → 15 MHz (safe). GD32F30x ADCPSC = CFG0[15:14] (+ [28] as the
 * high bit). /8 encoding = ADCPSC value 0b11 in [15:14] (bit28 = 0). */
#define RCU_CFG0_ADCPSC_MASK  (0x3U << 14)   /* [15:14]; bit28 stays 0 for /8   */
#define RCU_CFG0_ADCPSC_DIV8  (0x3U << 14)   /* PCLK2 / 8 → 15 MHz              */

/* PLL / system-clock config words (VERIFIED — 8 MHz HXTAL -> 120 MHz SYSCLK,
 * HCLK/APB2=120 MHz, APB1=60 MHz => APB1 timer clock x2 = 120 MHz). */
#define RCU_CFG0_OR1        0x00000400U
#define RCU_CFG0_AND        0x9FC3FFFFU
#define RCU_CFG0_OR2        0x20350000U   /* PLLSEL=PREDV0,AHB/1,APB1/2,APB2/1,PLL x30 */
#define RCU_CFG1_AND        0xBFFEF000U
#define RCU_CFG1_OR         0x00010633U   /* PREDV1/4, PLL1 x8, PREDV0/4, PREDV0SEL=PLL1 */
#define RCU_CFG0_SCS_MASK   0x00000003U   /* system clock switch bits          */
#define RCU_CFG0_SCS_PLL    0x00000002U   /* SCS = PLL                          */
#define RCU_CFG0_SCSS_PLL   0x00000008U   /* SCSS == PLL (bits [3:2] == 0b10)   */

/* PMU @ 0x40007000 (VERIFIED handshake in the PLL sequence). */
#define PMU_BASE        0x40007000U
#define PMU_CTL         REG32(PMU_BASE + 0x00U)
#define PMU_CS          REG32(PMU_BASE + 0x04U)
#define PMU_CTL_LDO_HI  0x0000C000U     /* LDO high-drive select bits          */
#define PMU_CTL_HDEN    0x00010000U     /* high-drive enable                   */
#define PMU_CS_HDRF     0x00010000U     /* high-drive ready flag               */
#define PMU_CTL_HDS     0x00020000U     /* high-drive switch                   */
#define PMU_CS_HDSRF    0x00020000U     /* high-drive switch ready flag (bit17)*/

/* FMC — flash controller @ 0x40022000 (2 wait states needed at 120 MHz). */
#define FMC_BASE        0x40022000U
#define FMC_WS          REG32(FMC_BASE + 0x00U)  /* wait-state register (WSCNT) */
#define FMC_WS_WSCNT_2  0x00000002U              /* 2 wait states               */

/* ---------------------------------------------------------------------------
 * SPI1 — A4963 motor-driver control @ 0x40003800 (APB1)  [VERIFIED]
 * Pins on GPIOB: PB12=CS(GPIO), PB13=SCK, PB14=MISO, PB15=MOSI.
 * ------------------------------------------------------------------------- */
#define SPI1_BASE       0x40003800U
#define A4963_SPI_BASE  SPI1_BASE

#define SPI_CTL0_OFF    0x00U   /* control 0                                    */
#define SPI_CTL1_OFF    0x04U   /* control 1                                    */
#define SPI_STAT_OFF    0x08U   /* status                                       */
#define SPI_DATA_OFF    0x0CU   /* data                                         */

#define SPI_REG(base, off)  REG32((base) + (off))

/* SPI status bits (VERIFIED). */
#define SPI_STAT_RBNE   (1U << 0)   /* receive buffer not empty                 */
#define SPI_STAT_TBE    (1U << 1)   /* transmit buffer empty                    */
#define SPI_STAT_TRANS  (1U << 7)   /* transfer in progress (busy)              */

/* SPI CTL0 for A4963 (VERIFIED): 0x0B2F = mode3 (CKPH=1,CKPL=1), master,
 * PSC=5 (/64), MSB-first, SW-NSS (SWNSS+SWNSSEN), FF16 (16-bit frame). */
#define SPI_CTL0_A4963  0x0B2FU
#define SPI_CTL0_SPIEN  (1U << 6)   /* SPI enable                               */

/* ---------------------------------------------------------------------------
 * TIMER2 — wheel + blade PWM  @ 0x40000400   [VERIFIED]
 * AFIO full remap -> channels on PC6..PC9. CH3=LEFT(PC9), CH2=RIGHT(PC8).
 * ------------------------------------------------------------------------- */
#define TIMER2_BASE     0x40000400U
#define TIMER2_CTL0     REG32(TIMER2_BASE + 0x00U)  /* control 0 (CEN, ARSE)    */
#define TIMER2_CTL1     REG32(TIMER2_BASE + 0x04U)
#define TIMER2_SMCFG    REG32(TIMER2_BASE + 0x08U)
#define TIMER2_DMAINTEN REG32(TIMER2_BASE + 0x0CU)
#define TIMER2_INTF     REG32(TIMER2_BASE + 0x10U)
#define TIMER2_SWEVG    REG32(TIMER2_BASE + 0x14U)
#define TIMER2_CHCTL0   REG32(TIMER2_BASE + 0x18U)  /* CH0/CH1 mode (OCxM PWM)  */
#define TIMER2_CHCTL1   REG32(TIMER2_BASE + 0x1CU)  /* CH2/CH3 mode             */
#define TIMER2_CHCTL2   REG32(TIMER2_BASE + 0x20U)  /* channel enable (CHxEN)   */
#define TIMER2_CNT      REG32(TIMER2_BASE + 0x24U)  /* counter                  */
#define TIMER2_PSC      REG32(TIMER2_BASE + 0x28U)  /* prescaler                */
#define TIMER2_CAR      REG32(TIMER2_BASE + 0x2CU)  /* auto-reload (PWM period) */
#define TIMER2_CH0CV    REG32(TIMER2_BASE + 0x34U)  /* CH0 compare              */
#define TIMER2_CH1CV    REG32(TIMER2_BASE + 0x38U)  /* CH1 compare = BLADE      */
#define TIMER2_CH2CV    REG32(TIMER2_BASE + 0x3CU)  /* CH2 compare = RIGHT wheel*/
#define TIMER2_CH3CV    REG32(TIMER2_BASE + 0x40U)  /* CH3 compare = LEFT wheel */

#define TIMER2_PWM_PSC      9U        /* prescaler (VERIFIED)                    */
#define TIMER2_PWM_PERIOD   0x1C20U   /* CAR — 7200 (VERIFIED)                   */
#define TIMER2_CTL0_CEN     (1U << 0) /* counter enable                          */
#define TIMER2_CTL0_ARSE    (1U << 7) /* auto-reload shadow enable               */

/* CHCTL0 field values: CH0 in bits [7:0], CH1 (BLADE) in bits [15:8].
 *   OC1M[6:4]=0b110 (PWM0), OC1PE (preload) bit3. */
#define TIMER2_CH1_OCM_PWM0  (0x0060U << 8)   /* CH1 OC1M = PWM mode0 (BLADE)    */
#define TIMER2_CH1_OCPE      (0x0008U << 8)   /* CH1 output-compare preload      */

/* CHCTL1 field values (VERIFIED): OCxM = PWM mode0 (0b110), output mode.
 *   CH2 (RIGHT) in bits [7:0], CH3 (LEFT) in bits [15:8].
 *   OC3M[6:4]=0b110 (PWM0), OC3PE (preload) bit3. */
#define TIMER2_CH3_OCM_PWM0  (0x0060U << 8)   /* CH3 OC3M = PWM mode0 (LEFT)     */
#define TIMER2_CH3_OCPE      (0x0008U << 8)   /* CH3 output-compare preload      */
#define TIMER2_CH2_OCM_PWM0  (0x0060U)        /* CH2 OC2M = PWM mode0 (RIGHT)    */
#define TIMER2_CH2_OCPE      (0x0008U)        /* CH2 output-compare preload      */

/* CHCTL2 channel-enable bits (VERIFIED): CHxEN at bit 4*x. */
#define TIMER2_CH1EN         (1U << 4)   /* CH1 output enable (BLADE)            */
#define TIMER2_CH2EN         (1U << 8)   /* CH2 output enable (RIGHT)            */
#define TIMER2_CH3EN         (1U << 12)  /* CH3 output enable (LEFT)             */

/* ---------------------------------------------------------------------------
 * AFIO — alternate function I/O @ 0x40010000  [VERIFIED]
 * ------------------------------------------------------------------------- */
#define AFIO_BASE       0x40010000U
#define AFIO_PCF0       REG32(AFIO_BASE + 0x04U)   /* port config factor 0       */
/* TIMER2 full remap = TIMER2_REMAP[1:0] (PCF0[11:10]) = 0b11 -> CH0..CH3 on
 * PC6..PC9. ONLY these two bits — the old 0x001A0C00 value also blasted bits
 * 17/19/20 (USART/other remaps we don't use); write via read-modify-write. */
#define AFIO_PCF0_TIMER2_FULLREMAP  0x00000C00U    /* TIMER2 full remap PC6..PC9 */

/* ---------------------------------------------------------------------------
 * GPIO — ports B/C/D/E, STM32F1-style CTL0/CTL1/ISTAT/OCTL/BOP/BC  [bases VERIFIED]
 * ------------------------------------------------------------------------- */
#define GPIOA_BASE      0x40010800U
#define GPIOB_BASE      0x40010C00U
#define GPIOC_BASE      0x40011000U
#define GPIOD_BASE      0x40011400U
#define GPIOE_BASE      0x40011800U

#define GPIO_CTL0_OFF   0x00U   /* port control low  (pins 0..7)  */
#define GPIO_CTL1_OFF   0x04U   /* port control high (pins 8..15) */
#define GPIO_ISTAT_OFF  0x08U   /* input status (IDR)             */
#define GPIO_OCTL_OFF   0x0CU   /* output control (ODR)           */
#define GPIO_BOP_OFF    0x10U   /* bit set/reset (BSRR)           */
#define GPIO_BC_OFF     0x14U   /* bit clear (BRR)                */

#define GPIO_REG(base, off)  REG32((base) + (off))

/* GPIO pin config nibbles (STM32F1/GD32F1 CTL model, 4 bits per pin):
 *   MODE[1:0]: 00=input, 01=out 10MHz, 10=out 2MHz, 11=out 50MHz
 *   CTL [1:0]: (output) 00=GP push-pull, 10=AF push-pull
 *              (input)  01=floating
 * Composed value per pin = (CTL<<2)|MODE. */
#define GPIO_CFG_OUT_PP_50    0x3U   /* GP push-pull, 50 MHz   (0b0011) */
#define GPIO_CFG_AF_PP_50     0xBU   /* AF push-pull, 50 MHz   (0b1011) */
#define GPIO_CFG_IN_FLOAT     0x4U   /* input floating         (0b0100) */
#define GPIO_CFG_IN_PULL      0x8U   /* input with pull        (0b1000) */
#define GPIO_CFG_IN_ANALOG    0x0U   /* analog input           (0b0000) */

/* Helper: set the 4-bit config of pin `p` (0..15) in a CTL0/CTL1 pair. */
#define GPIO_SET_CFG(port, p, cfg)                                          \
    do {                                                                    \
        volatile uint32_t *ctl = (volatile uint32_t *)((port) +            \
            (((p) < 8U) ? GPIO_CTL0_OFF : GPIO_CTL1_OFF));                  \
        uint32_t sh = (((p) & 7U) * 4U);                                    \
        *ctl = (*ctl & ~(0xFU << sh)) | ((uint32_t)(cfg) << sh);           \
    } while (0)

/* ---------------------------------------------------------------------------
 * Named pins (VERIFIED).
 * ------------------------------------------------------------------------- */
/* MAIN power latch = PE12 (VERIFIED from the factory dump, 2026-07-05):
 *   - bootloader board-init (FUN_08000f38 @0x08000f38) drives PE12 (0x1000)
 *     output HIGH as its VERY FIRST GPIO action = hold system power;
 *   - factory APP manual power-off (0x080714d8, called in a loop from
 *     process_power_off @0x08069c94) ends in `for(;;){ gpio_bit_reset(GPIOE,
 *     0x1000); }` — PE12 held LOW forever until the rail collapses;
 *   - bootloader power-off (FUN_08004250 @0x08004250, "poweroff"/"Unity
 *     Failed" path) is the same infinite PE12-LOW loop.
 * PB0 is a SECONDARY latch: bootloader key-confirm ("key_press_power_on",
 * FUN_0800cf50 @0x0800cf50) sets PB0 HIGH; the app power-off never touches it.
 * PB0-LOW alone does NOT cut power. */
#define PWR_MAIN_PORT    GPIOE_BASE
#define PWR_MAIN_PIN     12U            /* PE12 = MAIN power latch (hold HIGH)  */
#define PWR_LATCH_PORT   GPIOB_BASE
#define PWR_LATCH_PIN    0U             /* PB0 = secondary latch, drive HIGH    */

/* Peripheral power rails released by the factory power-off (0x080714d8), in
 * factory order, before the PE12-LOW loop: PB12 (A4963 enable, already named
 * A4963_EN_*), then PE9, PD11, PE7 (display/sensor/aux rails; bootloader init
 * drives PE7 HIGH). All cleared via plain BC writes, no re-config — exactly
 * what the factory gpio_bit_reset does. */
#define PWR_AUX_E9_PIN   9U             /* PE9  — aux rail, LOW at power-off    */
#define PWR_AUX_D11_PIN  11U            /* PD11 — aux rail, LOW at power-off    */
#define PWR_AUX_E7_PIN   7U             /* PE7  — aux rail, LOW at power-off    */

/* (The former shotgun candidates PA0/PD7/PD8/PD10/PE0 were DISPROVEN on
 * hardware — 2026-07-06 live capture: the A4963s are armed once at boot and
 * driven by PWM duty alone; PD8 is USART2-TX — and have been removed.) */

/* Shared A4963 gate-driver ENABLE — PB12, drive HIGH (enables ALL 3 drivers).
 * NOT an SPI pin; SPI1 uses software-NSS. */
#define A4963_EN_PORT    GPIOB_BASE
#define A4963_EN_PIN     12U            /* PB12 = shared enable, drive HIGH     */

/* SPI1 signal pins on GPIOB. */
#define SPI1_SCK_PIN     13U            /* PB13 = SCK  (AF PP 50)               */
#define SPI1_MISO_PIN    14U            /* PB14 = MISO (input floating)         */
#define SPI1_MOSI_PIN    15U            /* PB15 = MOSI (AF PP 50)               */

/* Per-motor active-low CS on GPIOD (software-NSS). Idle HIGH; LOW around xfer. */
#define A4963_CS_PORT    GPIOD_BASE
#define A4963_CS_PIN_LEFT   5U          /* PD5 = LEFT  CS (mask 0x20)           */
#define A4963_CS_PIN_RIGHT  4U          /* PD4 = RIGHT CS (mask 0x10)           */
#define A4963_CS_PIN_BLADE  3U          /* PD3 = BLADE CS (mask 0x08)           */

/* Per-WHEEL RUN/COAST line on GPIOD — the crux of "wheels dead, blade spins".
 * VERIFIED (live factory state-5 capture + driver_left/right_motor reverse):
 *   PD7 = LEFT wheel, PD4 = RIGHT wheel; LOW = RUN, HIGH = COAST/BRAKE.
 * The blade has NO such line (SPI + PWM only) — which is why the blade spins for
 * us but the wheels, left in COAST (line HIGH), do not. Drive LOW to run.
 * NOTE: PD4 is shared with the right CS pin above; CS is irrelevant here (the
 * A4963s run from their EEPROM defaults + PWM), so PD4 is used as the run line. */
#define WHEEL_RUN_PORT       GPIOD_BASE
#define WHEEL_RUN_PIN_LEFT   7U          /* PD7 (mask 0x80) */
#define WHEEL_RUN_PIN_RIGHT  4U          /* PD4 (mask 0x10) */

/* Power/KEY button + charge detect on GPIOE (VERIFIED). Button = PE10 AND PE11,
 * active-low (pressed when either level is 0 — factory ANDs the two levels).
 * Charge detect = PE8, active-HIGH. */
#define BTN_PORT         GPIOE_BASE
#define BTN_PIN_A        10U            /* PE10 (mask 0x400) — power button       */
#define BTN_PIN_B        11U            /* PE11 (mask 0x800) — power button       */
#define BTN_MASK_A       (1U << BTN_PIN_A)
#define BTN_MASK_B       (1U << BTN_PIN_B)
#define CHARGE_PORT      GPIOE_BASE
#define CHARGE_PIN       8U             /* PE8 (mask 0x100) — charge present      */
#define CHARGE_MASK      (1U << CHARGE_PIN)

/* FWDGT (free watchdog, IWDG) — fed with the 0xAAAA key. The factory power-off
 * PE12-LOW loop feeds it every iteration (0x804bca0); we do the same. */
#define FWDGT_CTL_REG    REG32(0x40003000U)
#define FWDGT_KEY_FEED   0x0000AAAAU

/* SCB core registers. NOTE: SYSRESETREQ must NOT be used for power-off — a
 * reset re-runs the bootloader, which re-asserts PE12 HIGH (and our app then
 * re-latched PB0), so the mower reboots instead of turning off. That was the
 * bug that made the power button "do nothing". The factory app power-off never
 * resets: it parks in the PE12-LOW loop until the rail dies. */
#define SCB_AIRCR        REG32(0xE000ED0CU)
#define SCB_AIRCR_RESETREQ  0x05FA0004U   /* VECTKEY 0x05FA | SYSRESETREQ bit2   */

/* ---------------------------------------------------------------------------
 * ADC0 @ 0x40012400 — battery analog sensing (VERIFIED path).
 * ch15 (PC5) = pack voltage, ch14 (PC4) = current. Software-triggered scan,
 * polled EOC (no DMA needed).
 * ------------------------------------------------------------------------- */
#define ADC0_BASE       0x40012400U
#define ADC0_STAT       REG32(ADC0_BASE + 0x00U)  /* status (EOC bit1)          */
#define ADC0_CTL0       REG32(ADC0_BASE + 0x04U)  /* control 0 (SM scan mode)   */
#define ADC0_CTL1       REG32(ADC0_BASE + 0x08U)  /* control 1 (ADCON,SWRCST..) */
#define ADC0_SAMPT0     REG32(ADC0_BASE + 0x0CU)  /* sample time ch10..17       */
#define ADC0_SAMPT1     REG32(ADC0_BASE + 0x10U)  /* sample time ch0..9         */
#define ADC0_RSQ0       REG32(ADC0_BASE + 0x2CU)  /* regular sequence 0 (len)   */
#define ADC0_RSQ1       REG32(ADC0_BASE + 0x30U)  /* regular sequence 1         */
#define ADC0_RSQ2       REG32(ADC0_BASE + 0x34U)  /* regular sequence 2 (1st)   */
#define ADC0_RDATA      REG32(ADC0_BASE + 0x4CU)  /* regular data               */

#define ADC0_STAT_EOC     (1U << 1)    /* end of conversion                     */
#define ADC0_CTL0_SM      (1U << 8)    /* scan mode                             */
#define ADC0_CTL1_ADCON   (1U << 0)    /* ADC on                                */
#define ADC0_CTL1_CTN     (1U << 1)    /* continuous (unused; single-shot)      */
#define ADC0_CTL1_RSTCLB  (1U << 3)    /* reset calibration                     */
#define ADC0_CTL1_CLB     (1U << 2)    /* start calibration                     */
#define ADC0_CTL1_SWRCST  (1U << 22)   /* software start regular conversion     */
#define ADC0_CTL1_ETERC   (1U << 20)   /* external trigger enable (SWSTART src) */
#define ADC0_CTL1_ETSRC_SW (7U << 17)  /* regular trigger source = SWRCST       */

/* Sample time = 239.5 cycles (code 0b111) per channel field (3 bits/ch). */
#define ADC_SAMPT_239_5   0x7U

/* Battery ADC channels + pins (VERIFIED). */
#define BATT_VOLT_CH     15U           /* pack voltage = ADC ch15 = PC5         */
#define BATT_CURR_CH     14U           /* current      = ADC ch14 = PC4         */
#define BATT_VOLT_PORT   GPIOC_BASE
#define BATT_VOLT_PIN    5U            /* PC5 analog                            */
#define BATT_CURR_PORT   GPIOC_BASE
#define BATT_CURR_PIN    4U            /* PC4 analog                            */

/* ---------------------------------------------------------------------------
 * USART2 — smart-battery (BMS/pack) link @ 0x40004800 (APB1)  [VERIFIED]
 *
 * 19200 8N1, PD8=TX / PD9=RX (AFIO USART2 full remap). WHY THIS MATTERS: the
 * smart pack keeps its high-current DISCHARGE FET — the rail that powers the
 * A4963 gate drivers (motor VBB) — closed only while it is being polled by the
 * host over this link. Our firmware previously never talked to the pack, so the
 * pack treated the host as absent and opened the FET → A4963 unpowered → wheels
 * dead (confirmed on hardware 2026-07-06). Polling the pack keeps VBB alive.
 * Frame format + connect bytes recovered in reverse-v2/reference (BMS spec).
 * ------------------------------------------------------------------------- */
#define USART2_BASE      0x40004800U
#define USART_STAT_OFF   0x00U   /* status                      */
#define USART_DATA_OFF   0x04U   /* data                        */
#define USART_BAUD_OFF   0x08U   /* baud-rate divisor           */
#define USART_CTL0_OFF   0x0CU   /* control 0 (UEN/TEN/REN/WL/PM)*/
#define USART_CTL1_OFF   0x10U   /* control 1 (STB stop bits)   */
#define USART_CTL2_OFF   0x14U   /* control 2 (flow/DMA/HDEN)   */
#define USART_REG(base, off)  REG32((base) + (off))

/* USART STAT flags (GD32F30x). */
#define USART_STAT_RBNE  (1U << 5)   /* read data buffer not empty            */
#define USART_STAT_TC    (1U << 6)   /* transmission complete                 */
#define USART_STAT_TBE   (1U << 7)   /* transmit data buffer empty            */

/* USART CTL0 bits. */
#define USART_CTL0_REN   (1U << 2)   /* receiver enable                       */
#define USART_CTL0_TEN   (1U << 3)   /* transmitter enable                    */
#define USART_CTL0_UEN   (1U << 13)  /* USART enable (8N1: WL=0, PCEN=0)       */

/* USART CTL2 bits. */
#define USART_CTL2_HDEN  (1U << 3)   /* HDSEL: half-duplex (single-wire) mode  */

/* BAUD divisor for 19200 @ PCLK1 = APB1 = 60 MHz: BRR = round(60e6 / 19200) =
 * 3125 = 0x0C35 (GD32 USART_BAUD holds fck/baud directly). */
#define USART2_BAUD_19200  0x0C35U

/* RCU APB1EN clock-enable bit for USART2. */
#define RCU_APB1EN_USART2EN (1U << 18)

/* AFIO USART2 full remap = PCF0[5:4] = 0b11 -> PD8=TX, PD9=RX. RMW only these
 * two bits (leave the TIMER2 remap and everything else). */
#define AFIO_PCF0_USART2_FULLREMAP  0x00000030U

/* USART2 pins on GPIOD (full remap). PD10..PD12 (CK/CTS/RTS) are unused and left
 * as-is — PD11 stays our GP aux rail (its nibble is GP output, not AF). */
#define BMS_TX_PORT   GPIOD_BASE
#define BMS_TX_PIN    8U    /* PD8 = USART2 TX (AF push-pull)   */
#define BMS_RX_PORT   GPIOD_BASE
#define BMS_RX_PIN    9U    /* PD9 = USART2 RX (floating input) */

#endif /* GD32_REGS_H */
