/* Kernel main source file
 * Ibanez Thomas, Tournier Vincent
 */
#include "kernel.h"

#ifdef TEST
    #include "test.h"
#endif

void kernelEntry(multiboot_info_t* inf) {
    initDisplay();
    printf("Display Initialized\n");
    gdt_init();
    printf("GDT Initialized\n");
    idt_init();
    printf("IDT Initialized\n");
    pic_init();
    printf("PIC Initialized\n");
	filesystem_init();
	printf("picoFS Initialized");
    keyboard_init();
    printf("Keyboard Initialized\n");
    timer_init(100);
    printf("Timer Initialized\n");
    sti();
    printf("Interruptions now active\n");
    #ifndef TEST
        printf("Memory Available: %d KB\n", inf->mem_upper);
        moveCursor(0, 24);
        printChar('>');

        int stored = 0;
        int typed = 0;
        while(1) {
            typed = getc();
            if(typed == 81)
            {
                setColor(LIGHT_GREY);
                printf("\n\nSystem is now shutting down...\n");
                halt();
            }
            else if(typed == 8) {
                decrementCursor();
                printChar('\0');
                decrementCursor();
            }
            else if(stored == 0 && (typed == 94 || typed == 96 || typed == 249)) {
                stored = typed;
                typed = getc();
                int combined = handleDeadKeysAccents(stored, typed);
                if(combined == 0) {
                    printChar(stored);
                    printChar(typed);
                }
                else
                    printChar(combined);
                stored = 0;
            }
            else
                printChar(typed);
        }
    #else
        test();
    #endif
}
