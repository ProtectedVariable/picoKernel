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

        //THIS PART NEEDS TO BE TAKEN OFF AFTER THOMAS HAS ACKNOWLEDGED HOW FUCKING CASH IT IS
        printf("Available files :\n");

        file_iterator_t iterator = file_iterator();
        while(file_has_next(&iterator)) {
            char filename[FILENAME_MAXSIZE];
            file_next(filename, &iterator);
            printf("- %s\n", filename);
        }
        if(iterator.state == FINISHED)
            printf("----------\n");

        int fd = file_open(".gitignore");
        printf("OPEN .gitignore : OPENED TO FD #%d\n", fd);
        printf("EXIST makefile : %d\n", file_exists("makefile"));
        printf("EXIST yourmom.enormous : %d\n", file_exists("yourmom.enormous"));
        printf("SEEK .gitignore, 23 bytes : %d\n", file_seek(fd, 23));

        stat_t stat;
        if(file_stat(".gitignore", &stat) == -1)
            printf("Failed to find file\n");
        else
            printf("Stat for '%s' file size : %d bytes. CONTENT : \n", stat.name, stat.size);

        char buffer = 0;
        while(file_read(fd, &buffer, 1) > 0)
            printChar(buffer);
        printf("\n");

        file_close(fd);
        //END OF FUCKING CASH PART

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
