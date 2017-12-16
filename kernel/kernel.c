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
	filesystem_init(); //No printf as filesystem already introduces himself with label and version
    keyboard_init();
    printf("Keyboard Initialized\n");
    timer_init(100);
    printf("Timer Initialized\n");
    sti();
    printf("Interruptions now active\n");
    #ifndef TEST
        printf("Memory Available: %d KB\n", inf->mem_upper);
        moveCursor(0, 24);

        stat_t stat;
        if(file_stat("kernel/kernel.c", &stat) == -1)
            printf("Failed to find file\n");
        else
            printf("'%s' file size : %d bytes\n", stat.name, stat.size);

        int fd = file_open("makefile");
        printf("OPEN : %d\n", fd);
        printf("WRONG OPEN : %d\n", file_open("your_mom.enormous"));

        printf("EXIST : %d\n", file_exists("makefile"));
        printf("WRONG EXIST : %d\n", file_exists("your_mom.enormous"));

        printf("SEEK : %d\n", file_seek(fd, 96));
        printf("WRONG SEEK : %d\n", file_seek(fd, 322454245));

        file_close(fd);

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
