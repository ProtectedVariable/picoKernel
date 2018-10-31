ENTRY(entrypoint)            /* the entry point */

SECTIONS {
    . = 0x100000;            /* all sections located above 1MB */

    .multiboot ALIGN(4):     /* multiboot header first (needs to reside within the first 8KB) */
    {
        *(.multiboot)
    }

    .stack ALIGN(4):         /* kernel stack */
    {
        *(.stack)
    }

    .text ALIGN(4K) :        /* code */
    {
        *(.text*)            
    }

    .rodata ALIGN(4K) :      /* read-only data */
    {
        *(.rodata*)          
    }

    .data ALIGN(4K) :        /* initialized data */
    {
        *(.data*)
    }

    .bss ALIGN(4K) :         /* unitialized data */
    {
        *(COMMON)
        *(.bss*)
    }
}
