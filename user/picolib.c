#include "picolib.h"
#include "../common/syscall_nb.h"
#include "../common/util.h"
#include "../kernel/x86.h"

extern int syscall(uint32_t nb, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);
extern void exitpoint();
static uint a = 9;
#define B 19234567326109
#define C 564897312567

int file_open(char* filename) {
    return syscall(SYSCALL_FILE_OPEN, (uint32_t)filename, 0, 0, 0);
}

void file_close(int fd) {
    syscall(SYSCALL_FILE_CLOSE, fd, 0, 0, 0);
}

int file_read(int fd, void* buf, uint count) {
    return syscall(SYSCALL_FILE_READ, fd, (uint32_t)buf, count, 0);
}

int file_stat(char* filename, stat_t* stat) {
    return syscall(SYSCALL_FILE_STAT, (uint32_t)filename, (uint32_t)stat, 0, 0);
}

file_iterator_t file_iterator() {
    file_iterator_t iterator;
    syscall(SYSCALL_FILE_ITERATOR, (uint32_t)&iterator, 0, 0, 0);
    return iterator;
}

bool file_has_next(file_iterator_t* it) {
    return it->nextInode != -1;
}

void file_next(char* filename, file_iterator_t* it) {
    syscall(SYSCALL_FILE_NEXT, (uint32_t)filename, (uint32_t)it, 0, 0);
}
int exec(char* filename) {
    return syscall(SYSCALL_EXEC, (uint32_t)filename, 0, 0, 0);
}

void exit() {
    exitpoint();
}

void putc(char c) {
    syscall(SYSCALL_PUTS, (uint32_t)&c, 0, 0, 0);
}

void puts(char* str) {
    syscall(SYSCALL_PUTS, (uint32_t)str, 0, 0, 0);
}


void printf(char* format, ...) {
    char image[12];
    uint32_t* arg = ((uint32_t*)&format);
    arg++;
    for(int i = 0 ; format[i] ; i++) {
        if(format[i] == '%') {
            switch(format[i + 1]) {
                case 'd':
                itoa(image, *((int*)arg), 10);
                puts(image);
                break;
                case 's':
                puts(*(char**)arg);
                break;
                case 'c':
                putc(*(char*)arg);
                break;
                case 'x':
                itoa(image, *((int*)arg), 16);
                puts("0x");
                puts(image);
                break;
                case '%':
                putc('%');
                break;
            }
            arg++;
            i++;
        }
        else
        putc(format[i]);
    }
}

int getc() {
    return syscall(SYSCALL_GETC, 0, 0, 0, 0);
}

int keypressed() {
    return syscall(SYSCALL_KEYPRESSED, 0, 0, 0, 0);
}

void sleep(uint ms) {
    syscall(SYSCALL_SLEEP, ms, 0, 0, 0);
}

uint get_ticks() {
    return syscall(SYSCALL_GET_TICKS, 0, 0, 0, 0);
}

void srand(uint seed) {
    a = seed;
}

uint rand() {
    a = a * B + C;
    return a;
}

bool starts_with(char* pattern, char* string) {
    while(*pattern != '\0') {
        if(*string == '\0' || *pattern++ != *string++) {
            return false;
        }
    }
    return true;
}

char* trim(char* string) {
    while (*string == ' ') {
        string++;
    }
    char *begin = string;
    char *first_space = NULL;
    while (*string != '\0') {
        if(*string == ' ') {
            if(first_space == NULL) {
                first_space = string;
            }
        }else{
            first_space = NULL;
        }
        string++;
    }
    if(first_space != NULL) {
        *first_space = '\0';
    }
    return begin;
}
