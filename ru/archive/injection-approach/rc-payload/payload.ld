/*
 * payload.ld — RC-исполнитель живёт в свободном bank1 флеша приложения chip1.
 * Своего reset/startup НЕТ: вызывается родным кодом через hook. Никакой
 * ненулевой .data/.bss (нечем инициализировать). mx_veneer — строго по ORIGIN
 * (туда указывает hook-BL из hs-loop).
 */
ENTRY(mx_veneer)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08090000, LENGTH = 0x8000   /* свободно 0x08090000..0x080ffff8 */
}

SECTIONS
{
    .text 0x08090000 :
    {
        KEEP(*(.text.entry))     /* mx_veneer первым, по ORIGIN */
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    /* Страховка: секций с данными быть не должно (нет стартапа). */
    .data : { *(.data*) } > FLASH
    .bss  (NOLOAD) : { *(.bss*) *(COMMON) } > FLASH

    /DISCARD/ : { *(.ARM.exidx*) *(.comment) *(.ARM.attributes) }
}
