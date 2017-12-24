/* Test source file
 * Ibanez Thomas, Tournier Vincent
 */
#ifdef TEST
#include "test.h"

static void testDisplay();
static void testUtil();
static void testExceptions();
static void testFileSystem();
static void pause();

void test() {
    testDisplay();
    testUtil();
    testFileSystem();
    testExceptions();
}

static void pause() {
    setColor(DARK_GREY);
    printf("\n\nPress key to launch next test\n");
    setColor(WHITE);
    getc();
}

static void testExceptions() {
    clearScreen();
    moveCursor(0, 0);
    printf("Now let's print 5 / 0 and kill our whole kernel (end of tests) => ");
    printf("%d\n", 5 / 0);
}

static void testFileSystem() {
    clearScreen();
    moveCursor(0, 0);
    int fdA = file_open("LargeFile");
    int fdB = file_open("VeryLargeFile");
    printf("Large File fd #%d, Max Size File fd #%d\n", fdA, fdB);
    printf("Does file \"splashscreen\" exists ? %d\n", file_exists("splashscreen"));
    printf("Does file \"yomomma.enormous\" exists ? %d\n", file_exists("yomomma.enormous"));

    stat_t stat;
    file_stat("LargeFile", &stat);
    printf("Stats of \"LargeFile\" -> name : %s, size %d\n", stat.name, stat.size);
    file_stat("VeryLargeFile", &stat);
    printf("Stats of \"VeryLargeFile\" -> name : %s, size %d\n", stat.name, stat.size);

    printf("Seek byte 1000 for \"VeryLargeFile\" -> %d\n", file_seek(fdB, 1000));

    pause();

    char bytes[10000];
    int count = 0;
    int total = 0;
    while((count = file_read(fdA, bytes, 10000)) > 0) {
        total += count;
        printf("Read %d bytes, total : %d\n", count, total);
        int test = 1;
        for(int i = 0 ; i < count ; i++){
            if(bytes[i] != 'a')
                test = 0;
        }
        if(!test)
            printf("Error in bytes\n");
    }
    file_close(fdA);

    pause();

    count = 0;
    total = 0;
    while((count = file_read(fdB, bytes, 10000)) > 0) {
        total += count;
        printf("Read %d bytes, total : %d\n", count, total);
        int test = 1;
        for(int i = 0 ; i < count ; i++){
            if(bytes[i] != 'b')
                test = 0;
        }
        if(!test)
            printf("Error in bytes\n");
    }
    file_close(fdB);
    pause();

    printf("Trying to read closed file : %d", file_read(fdA, bytes, 100));
    pause();
}


static void testDisplay() {
    clearScreen();
    moveCursor(0, 0);
    printf("Let's count in hex !\n");
    for (int i = 0; i <= 115; i++) {
        printf("%d = %x\t", i, i);
    }
    pause();
    printf("Good");
    clearScreen();
    moveCursor(0, 24);
    printf("Let's use colors !\n");
    setColor(BLACK);printString("Black");
    setColor(BLUE);printString("Blue");
    setColor(GREEN);printString("Green");
    setColor(CYAN);printString("Cyan");
    setColor(RED);printString("Red");
    setColor(MAGENTA);printString("Magenta");
    setColor(BROWN);printString("Brown");
    setColor(LIGHT_GREY);printString("LIGHT_GREY");
    setColor(DARK_GREY);printString("DARK_GREY");
    setColor(LIGHT_BLUE);printString("LIGHT_BLUE");
    setColor(LIGHT_GREEN);printString("LIGHT_GREEN");
    setColor(LIGHT_CYAN);printString("LIGHT_CYAN");
    setColor(LIGHT_RED);printString("LIGHT_RED");
    setColor(LIGHT_MAGENTA);printString("LIGHT_MAGENTA");
    setColor(YELLOW);printString("Yellow");
    setColor(WHITE);printString("White\n");
    sleep(2000);
    printf("Here's a loading bar\n");
    for (int i = 0; i < FRAMBUFFER_WIDTH; i++) {
        for (int j = 0; j <= i; j++) {
            printChar(0xDB);
        }
        printChar('\r');
        sleep(20);
    }
    setBackground(BLACK);printString("Black");
    setBackground(BLUE);printString("Blue");
    setBackground(GREEN);printString("Green");
    setBackground(CYAN);printString("Cyan");
    setBackground(RED);printString("Red");
    setBackground(MAGENTA);printString("Magenta");
    setBackground(BROWN);printString("Brown");
    setBackground(LIGHT_GREY);printString("LIGHT_GREY");
    setBackground(DARK_GREY);printString("DARK_GREY");
    setBackground(LIGHT_BLUE);printString("LIGHT_BLUE");
    setBackground(LIGHT_GREEN);printString("LIGHT_GREEN");
    setBackground(LIGHT_CYAN);printString("LIGHT_CYAN");
    setBackground(LIGHT_RED);printString("LIGHT_RED");
    setBackground(LIGHT_MAGENTA);printString("LIGHT_MAGENTA");
    setBackground(YELLOW);printString("Yellow");
    setBackground(WHITE);printString("White");
    setBackground(BLACK);printString("\n");
    pause();
}

static void testUtil() {
    clearScreen();
    moveCursor(0, 0);
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
    pause();
}
#endif
