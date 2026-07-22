/*
 * app.ld — linker script for mower-own STANDALONE firmware (GD32F305).
 *
 * This is OUR firmware; it REPLACES the factory application. It is NOT a hook.
 * The GD32 bootloader (0x08000000..0x08017FFF, we DO NOT touch it) runs first,
 * loads MSP from *(0x08018000) and jumps to *(0x08018004) — our Reset_Handler.
 *
 * Memory map:
 *   FLASH app : 0x08018000 .. 0x080FFFF8  (last word 0x080FFFFC reserved for
 *               the app CRC-32, written post-build by the packer; the image is
 *               padded to 0x08100000 with 0xFF, CRC computed over
 *               0x08018000..0x080FFFFC per tools/bench/app_crc.py).
 *   RAM       : 0x20000000 .. 0x20018000  (96 KB)
 *
 * The vector table (.isr_vector) MUST sit at 0x08018000 so the bootloader finds
 * a valid MSP (word0) and Reset entry (word1). Initial SP = 0x20018000 (top).
 */

ENTRY(Reset_Handler)

MEMORY
{
    /* Reserve the final word (0x080FFFFC) for the app CRC — length ends at 0x080FFFF8. */
    FLASH (rx)  : ORIGIN = 0x08018000, LENGTH = 0x080FFFF8 - 0x08018000
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
}

/* Top of stack = end of RAM. */
_estack = ORIGIN(RAM) + LENGTH(RAM);   /* 0x20018000 */

SECTIONS
{
    /* Vector table — first thing in flash, at 0x08018000. */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH

    /* Code + read-only data. */
    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    /* Initialized data: stored in flash (LMA), copied to RAM (VMA) by startup. */
    _sidata = LOADADDR(.data);
    .data :
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH

    /* Zero-initialized data. */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } > RAM

    /* Discard exception-unwind / metadata we don't need in a freestanding image. */
    /DISCARD/ : { *(.ARM.exidx*) *(.ARM.extab*) *(.comment) *(.ARM.attributes) }
}
