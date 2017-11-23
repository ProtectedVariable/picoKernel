/* Kernel main source file
 * Ibanez Thomas, Tournier Vincent
 */
#include "kernel.h"

#ifdef TEST
    #include "test.h"
#endif

int handleDeadKeysAccents(int stored, int typedChar) {
    int result = 0;
    if(stored == 94) {
        if(typedChar == 97) // a
            result = 131 ;
        else if(typedChar == 101) // e
            result = 136;
        else if(typedChar == 105) // i
            result = 140;
        else if(typedChar == 111) // o
            result = 147;
        else if(typedChar == 117) // u
            result = 150;
        else if(typedChar == 65) // A
            result = 182;
        else if(typedChar == 69) // E
            result = 210;
        else if(typedChar == 73) // I
            result = 215;
        else if(typedChar == 79) // O
            result = 226;
        else if(typedChar == 85) // U
            result = 234;
    }
    else if(stored == 96) {
        if(typedChar == 97) // a
            result = 133;
        else if(typedChar == 101) // e
            result = 138;
        else if(typedChar == 105) // i
            result = 141;
        else if(typedChar == 111) // o
            result = 149;
        else if(typedChar == 117) // u
            result = 151;
        else if(typedChar == 65) // A
            result = 183;
        else if(typedChar == 69) // E
            result = 212;
        else if(typedChar == 73) // I
            result = 222;
        else if(typedChar == 79) // O
            result = 227;
        else if(typedChar == 85) // U
            result = 235;
    }
    else if(stored == 249) {
        if(typedChar == 97) // a
            result = 132;
        else if(typedChar == 101) // e
            result = 137;
        else if(typedChar == 105) // i
            result = 139;
        else if(typedChar == 111) // o
            result = 148;
        else if(typedChar == 117) // u
            result = 129;
        else if(typedChar == 65) // A
            result = 142;
        else if(typedChar == 69) // E
            result = 211;
        else if(typedChar == 73) // I
            result = 216;
        else if(typedChar == 79) // O
            result = 153;
        else if(typedChar == 85) // U
            result = 154;
    }

    return result;
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
