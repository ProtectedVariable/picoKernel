#include "kernel.h"
#include <util.h>
#include "gdt.h"
#include <cursor.h>
#include <display.h>

void test() {

}

void kernelEntry(multiboot_info_t* inf) {
    gdt_init();
    initDisplay();
    //splashScreen();
    #ifndef TEST
        moveCursor(0, 24);
        printChar('>');
    #else
        test();
    #endif
}
/*
void splashScreen() {
    splashP(22, 7);
    splashK(28, 4);
    splashE(34, 6);
    splashR(40, 6);
    splashN(44, 6);
    splashE(50, 6);
    splashL(56, 4);
}

void splashR(int bx, int by) {
    for (int i = by; i <= by + 6; i++) {
        moveCursor(bx, i);
        printChar(0xDB);
        printChar(0xDB);
        if(i < 8) {
            printChar(0xDB);
        }
    }
}

void splashL(int bx, int by) {
    for (int i = by; i <= by+8; i++) {
        moveCursor(bx, i);
        printChar(0xDB);
        printChar(0xDB);
    }
}

void splashN(int bx, int by) {
    for (int i = by; i <= by+6; i++) {
        moveCursor(bx, i);
        printChar(0xDB);
        printChar(0xDB);
        printChar(i > 6 ? ' ' : 0xDB);
        printChar(0xDB);
        printChar(0xDB);
    }
}

void splashK(int bx, int by) {
    for (int i = by; i <= by+8; i++) {
        moveCursor(bx, i);
        printChar(0xDB);
        printChar(0xDB);
        if(i < 6 || i >= 10) {
            printChar(' ');
        }
        printChar(0xDB);
        printChar(0xDB);
    }
}

void splashP(int bx, int by) {
    for (int i = by; i <= by+7; i++) {
        moveCursor(bx, i);
        printChar(0xDB);
        printChar(0xDB);
        printChar((i == by || i == by + 5) ? 0xDB : ' ');
        if(i <= by + 5) {
            printChar(0xDB);
            printChar(0xDB);
        }
    }
}

void splashE(int bx, int by) {
    for (int i = by; i <= by + 6; i++) {
        moveCursor(bx, i);
        printChar(i == 6 || i == 12 ? ' ' : 0xDB);
        printChar(0xDB);
        printChar(i == 7 || i == 8 || i == 10 || i == 11 ? ' ' : 0xDB);
        printChar(i == 10 ? ' ' : 0xDB);
        printChar(i == 6 || i == 12 || i == 10 ? ' ' : 0xDB);
    }
}
*/
