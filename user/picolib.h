#ifndef _PICO_LIB_H_
#define _PICO_LIB_H_

#include "../common/types.h"
#include "../common/picofs.h"

extern int file_open(char* filename);

extern void file_close(int fd);

extern int file_read(int fd, void* buf, uint count);

extern int file_stat(char* filename, stat_t* stat);

extern file_iterator_t file_iterator();

extern bool file_has_next(file_iterator_t* it);

extern void file_next(char* filename, file_iterator_t* it);

extern int exec(char* filename);

extern void exit();

extern void putc(char c);

extern void puts(char* str);

extern void printf(char* fmt, ...);

extern int getc();

extern int keypressed();

extern void srand(uint seed);

extern uint rand();

extern void sleep(uint ms);

extern uint get_ticks();

bool starts_with(char* pattern, char* string);

extern int strcmp(const char* a, const char* b);

char* trim(char* string);

#endif /* end of include guard: _PICO_LIB_H_ */
