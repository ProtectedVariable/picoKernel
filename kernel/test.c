#include "test.h"
#include <display.h>

void test() {
    testDisplay();
}

void testDisplay() {
    initDisplay();
    printf("Let's count in hex !\n");
    for (int i = 0; i < 170; i++) {
        printf("%d = %x  ", i, i);
    }
    for (int i = 0; i < (1 << 30); i++) {}
    printf("Good");
    clearScreen();
    moveCursor(0, 23);
    printf("Let's use colors !\n");
    setColor(BLACK);printString("Black");
    setColor(BLUE);printString("Blue");
    setColor(GREEN);printString("Green");
    setColor(CYAN);printString("Cyan");
    setColor(RED);printString("Red");
    setColor(MAGENTA);printString("Magenta");
    setColor(BROWN);printString("Brown");
    setColor(LIGHT_GRAY);printString("LIGHT_GRAY");
    setColor(DARK_GRAY);printString("DARK_GRAY");
    setColor(LIGHT_BLUE);printString("LIGHT_BLUE");
    setColor(LIGHT_GREEN);printString("LIGHT_GREEN");
    setColor(LIGHT_CYAN);printString("LIGHT_CYAN");
    setColor(LIGHT_RED);printString("LIGHT_RED");
    setColor(LIGHT_MAGENTA);printString("LIGHT_MAGENTA");
    setColor(YELLOW);printString("Yellow");
    setColor(WHITE);printString("White\n");
    for (int i = 0; i < (1 << 20); i++) {}
    printf("Here's a loading bar\n");
    for (int i = 0; i < FRAMBUFFER_WIDTH; i++) {
        for (int j = 0; j <= i; j++) {
            printChar(0xDB);
        }
        printChar('\r');
        for (int i = 0; i < (1 << 23); i++) {}
    }
}
