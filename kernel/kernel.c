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
        while(1) {
            int typed = getc();
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
            else if(stored == 0 && (typed == 94 || typed == 96 || typed == 249))
                stored = typed;
            else {
                if(stored == 94) {
                    if(typed == 97) // a
                        typed = 131 ;
                    else if(typed == 101) // e
                        typed = 136;
                    else if(typed == 105) // i
                        typed = 140;
                    else if(typed == 111) // o
                        typed = 147;
                    else if(typed == 117) // u
                        typed = 150;
                    else if(typed == 65) // A
                        typed = 182;
                    else if(typed == 69) // E
                        typed = 210;
                    else if(typed == 73) // I
                        typed = 215;
                    else if(typed == 79) // O
                        typed = 226;
                    else if(typed == 85) // U
                        typed = 234;
                    else
                        printChar(stored);
                    stored = 0;
                }
                else if(stored == 96) {
                    if(typed == 97) // a
                        typed = 133;
                    else if(typed == 101) // e
                        typed = 138;
                    else if(typed == 105) // i
                        typed = 141;
                    else if(typed == 111) // o
                        typed = 149;
                    else if(typed == 117) // u
                        typed = 151;
                    else if(typed == 65) // A
                        typed = 183;
                    else if(typed == 69) // E
                        typed = 212;
                    else if(typed == 73) // I
                        typed = 222;
                    else if(typed == 79) // O
                        typed = 227;
                    else if(typed == 85) // U
                        typed = 235;
                    else
                        printChar(stored);
                    stored = 0;
                }
                else if(stored == 249) {
                    if(typed == 97) // a
                        typed = 132;
                    else if(typed == 101) // e
                        typed = 137;
                    else if(typed == 105) // i
                        typed = 139;
                    else if(typed == 111) // o
                        typed = 148;
                    else if(typed == 117) // u
                        typed = 129;
                    else if(typed == 65) // A
                        typed = 142;
                    else if(typed == 69) // E
                        typed = 211;
                    else if(typed == 73) // I
                        typed = 216;
                    else if(typed == 79) // O
                        typed = 153;
                    else if(typed == 85) // U
                        typed = 154;
                    else
                        printChar(stored);
                    stored = 0;
                }
                printChar(typed);
            }
        }
    #else
        test();
    #endif
}
