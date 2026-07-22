/*
 * startup.c — C-based startup for mower-own (GD32F305, Cortex-M4).
 *
 * Provides:
 *   - the vector table in section .isr_vector (placed at 0x08018000 by app.ld),
 *     word0 = initial SP (_estack = 0x20018000), word1 = Reset_Handler.
 *   - Reset_Handler: copy .data (flash->RAM), zero .bss, SystemInit(), main().
 *   - Default_Handler for every unused vector (tight infinite loop).
 *
 * Only the core system exceptions + a generous IRQ fill are declared. The exact
 * GD32F305 IRQ ordering is not needed for the minimal control path (we poll, no
 * interrupts yet); every IRQ slot points at Default_Handler and can be given a
 * real name later. TODO(hw): wire real ISRs (TIMER3 input-capture odometry, etc).
 */
#include <stdint.h>

/* Symbols provided by the linker script. */
extern uint32_t _sidata;   /* .data load address in flash            */
extern uint32_t _sdata;    /* .data start in RAM                     */
extern uint32_t _edata;    /* .data end in RAM                       */
extern uint32_t _sbss;     /* .bss start in RAM                      */
extern uint32_t _ebss;     /* .bss end in RAM                        */
extern uint32_t _estack;   /* top of stack (0x20018000)              */

extern int  main(void);
void SystemInit(void);
void Reset_Handler(void);
void Default_Handler(void);

/*
 * Reset_Handler — first code the bootloader jumps to (via *(0x08018004)).
 * MSP is already loaded from *(0x08018000) by the bootloader, but we also set
 * it explicitly for robustness (harmless if identical).
 */
void Reset_Handler(void)
{
    /* M2: mask IRQs as the very first thing — before .data/.bss init — so a
     * bootloader-left-enabled IRQ can't fire during init while VTOR still points
     * at the bootloader's vector table. SystemInit() later disables NVIC/SysTick
     * and sets VTOR; IRQs stay masked (we use none). */
    __asm__ volatile("cpsid i" ::: "memory");

    /* Copy initialized data from flash to RAM. */
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* Zero the .bss segment. */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0U;
    }

    SystemInit();

    (void)main();

    /* main() should never return; if it does, trap. */
    for (;;) {
        /* halt */
    }
}

/*
 * SystemInit — quiesce interrupts and point the CPU at OUR vector table.
 *
 * M3: the bootloader may leave SysTick and/or NVIC IRQs enabled. If one fires
 * after we switch VTOR, it vectors into Default_Handler (an infinite loop) — the
 * board looks bricked. So before/around setting VTOR we: mask IRQs globally,
 * kill SysTick, and disable+clear every NVIC line. We configure no interrupts
 * ourselves (pure polling loop), so leaving them masked is correct.
 *
 * The real clock tree is brought up later in hal_clock_init().
 * TODO(hw): if enabling the FPU (hard-float), set CPACR CP10/CP11 here.
 */
void SystemInit(void)
{
    /* Cortex-M core peripheral registers. */
    volatile uint32_t *SCB_VTOR   = (volatile uint32_t *)0xE000ED08U;
    volatile uint32_t *SYSTICK_CTRL= (volatile uint32_t *)0xE000E010U;
    volatile uint32_t *NVIC_ICER  = (volatile uint32_t *)0xE000E180U; /* clr-enable */
    volatile uint32_t *NVIC_ICPR  = (volatile uint32_t *)0xE000E280U; /* clr-pending */

    /* 1. Mask all maskable interrupts globally. */
    __asm__ volatile("cpsid i" ::: "memory");

    /* 2. Stop SysTick (disable counter + its interrupt). */
    *SYSTICK_CTRL = 0U;

    /* 3. Disable and clear-pending every NVIC line. GD32F30x has < 84 external
     * IRQs => 3 ICER/ICPR words (0..95) cover all of them. */
    for (unsigned i = 0; i < 3U; i++) {
        NVIC_ICER[i] = 0xFFFFFFFFU;
        NVIC_ICPR[i] = 0xFFFFFFFFU;
    }

    /* 4. Point the CPU at OUR vector table at 0x08018000 (not the bootloader's
     * at 0x08000000). SCB->VTOR @ 0xE000ED08. */
    *SCB_VTOR = 0x08018000U;

    /* IRQs stay masked: we run a pure polling loop and enable none. If a future
     * revision adds ISRs, re-enable with __enable_irq() after configuring them. */
}

/* Default handler: park here so a fault/unexpected IRQ is observable over SWD. */
void Default_Handler(void)
{
    for (;;) {
        /* halt */
    }
}

/* Weak aliases: every named exception falls back to Default_Handler unless a
 * strong definition is linked in elsewhere. */
#define WEAK_ALIAS __attribute__((weak, alias("Default_Handler")))

void NMI_Handler(void)              WEAK_ALIAS;
void HardFault_Handler(void)        WEAK_ALIAS;
void MemManage_Handler(void)        WEAK_ALIAS;
void BusFault_Handler(void)         WEAK_ALIAS;
void UsageFault_Handler(void)       WEAK_ALIAS;
void SVC_Handler(void)              WEAK_ALIAS;
void DebugMon_Handler(void)         WEAK_ALIAS;
void PendSV_Handler(void)           WEAK_ALIAS;
void SysTick_Handler(void)          WEAK_ALIAS;

typedef void (*vector_t)(void);

/*
 * Vector table. Word0 = initial SP, word1 = Reset. Then the 14 core exception
 * slots, then a fill of external IRQ vectors all pointing at Default_Handler.
 * GD32F305 has ~68 external IRQs; we fill 84 slots (generous) with the default
 * so any spurious IRQ traps cleanly rather than running off the table.
 */
__attribute__((used, section(".isr_vector")))
const vector_t g_vectors[] = {
    (vector_t)(&_estack),   /*  0: Initial Stack Pointer (0x20018000)     */
    Reset_Handler,          /*  1: Reset                                  */
    NMI_Handler,            /*  2: NMI                                    */
    HardFault_Handler,      /*  3: HardFault                              */
    MemManage_Handler,      /*  4: MemManage                              */
    BusFault_Handler,       /*  5: BusFault                               */
    UsageFault_Handler,     /*  6: UsageFault                             */
    0, 0, 0, 0,             /*  7-10: reserved                            */
    SVC_Handler,            /* 11: SVCall                                 */
    DebugMon_Handler,       /* 12: Debug Monitor                          */
    0,                      /* 13: reserved                               */
    PendSV_Handler,         /* 14: PendSV                                 */
    SysTick_Handler,        /* 15: SysTick                                */

    /* 16..: external IRQs — all Default_Handler for now.
     * TODO(hw): name & implement the ones we use (e.g. TIMER3 capture for
     * odometry, USART for the coil link). */
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
    Default_Handler, Default_Handler, Default_Handler, Default_Handler,
};
