#include "test.h"
#include <display.h>
#include <util.h>

static void testDisplay();
static void testUtil();

void test() {
    testDisplay();
    testUtil();
}

static void testDisplay() {
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
    setBackground(BLACK);printString("Black");
    setBackground(BLUE);printString("Blue");
    setBackground(GREEN);printString("Green");
    setBackground(CYAN);printString("Cyan");
    setBackground(RED);printString("Red");
    setBackground(MAGENTA);printString("Magenta");
    setBackground(BROWN);printString("Brown");
    setBackground(LIGHT_GRAY);printString("LIGHT_GRAY");
    setBackground(DARK_GRAY);printString("DARK_GRAY");
    setBackground(LIGHT_BLUE);printString("LIGHT_BLUE");
    setBackground(LIGHT_GREEN);printString("LIGHT_GREEN");
    setBackground(LIGHT_CYAN);printString("LIGHT_CYAN");
    setBackground(LIGHT_RED);printString("LIGHT_RED");
    setBackground(LIGHT_MAGENTA);printString("LIGHT_MAGENTA");
    setBackground(YELLOW);printString("Yellow");
    setBackground(WHITE);printString("White\n");
    setBackground(BLACK);
}

static void testUtil() {
    printf("Testing Util functions\n");
    printf("STRCMP Test:");
    if(!(strcmp("", "") == 0)) {
        printf("FAIL_0");
    } else if(!(strncmp("aba", "abc", 2) == 0)) {
        printf("FAIL_1");
    } else if(!(strcmp("The quick brown fox jumps over the lazy dog", "The quick brown fox jumps over the lazy dog") == 0)) {
        printf("FAIL_3");
    } else if(strcmp("A", "B") > 0) {
        printf("FAIL_4");
    } else if(strcmp("C", "B") < 0) {
        printf("FAIL_5");
    } else {
        printf("OK\n");
    }

    printf("ITOA Test:");
    char buffer[9];
    if(strcmp(itoa(buffer, 0, 10), "0") != 0) {
        printf("FAIL_0");
    } else if(strcmp(itoa(buffer, 543, 10), "543") != 0) {
        printf("FAIL_1");
    } else if(strcmp(itoa(buffer, 54332, 16), "d43c") != 0) {
        printf("FAIL_2");
    } else if(strcmp(itoa(buffer, -532, 10), "-532") != 0) {
        printf("FAIL_3 %s", buffer);
    } else {
        printf("OK\n");
    }
}
