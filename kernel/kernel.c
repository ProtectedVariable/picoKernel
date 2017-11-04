#include "kernel.h"
#include <util.h>
#include "gdt.h"
#include <cursor.h>
#include <display.h>
#ifdef TEST
    #include "test.h"
#endif

void kernelEntry(multiboot_info_t* inf) {
    #ifndef TEST
        initDisplay();
        moveCursor(0, 0);
        printf("Display Initialized\n");
        gdt_init();
        printf("GDT Initialized\n");
        printf("Memory Available: %d KB\n", inf->mem_upper);
        moveCursor(0, 24);
        printChar('>');
    #else
        test();
    #endif
}
