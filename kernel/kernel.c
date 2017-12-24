/* Kernel main source file
 * Ibanez Thomas, Tournier Vincent
 */
#include "kernel.h"

#ifdef TEST
    #include "test.h"
#endif

void keyboardRoutine() {
    int stored = 0;
    int typed = 0;
    while(1) {
        typed = getc();
        if(typed == 81)
        {
            setColor(LIGHT_GREY);
            printf("\n\nSystem is now shutting down...\n");
            sleep(2000);
            clearScreen();
            halt();
        }
        else if(typed == 8) {
            decrementCursor();
            printChar('\0');
            decrementCursor();
        }
        else if(typed == '\n') {
            printChar(typed);
            printChar('>');
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
}

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
    printf("Memory available: %d KB\n", inf->mem_upper);
    #ifndef TEST
        printf("Files available:\n");

        file_iterator_t iterator = file_iterator();
        char nameBuffer[FILENAME_MAXSIZE];
        while(file_has_next(&iterator))
        {
            file_next(nameBuffer, &iterator);
            printf("\t %s\n", nameBuffer);
        }


        printf("Launching picoKernel...\n");
        sleep(3000);
        clearScreen();
        int fd = file_open("splashscreen");
        if(fd < 0) {
            printf("Y NO SPLASHSCREEN ????????????????\n");
        } else {
            char splashBuf = 0;
            //printf("%d\n", file_read(fd, &splashBuf, 1));
            while(file_read(fd, &splashBuf, 1) > 0)
                printChar(splashBuf);
            file_close(fd);
        }
        moveCursor(0, 24);
        printChar('>');
        keyboardRoutine();
    #else
        test();
    #endif
}
