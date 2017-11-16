#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <display.h>
#include <types.h>
#include "x86.h"

#define KEYBOARD_BUFFER_SIZE 256
#define KEYBOARD_STATUS_POINTER 0x64
#define KEYBOARD_DATA_POINTER 0x60

typedef struct keyboard_buffer_t {
	char* buffer[KEYBOARD_BUFFER_SIZE];
	int writeIndex;
	int readIndex;
	int bufferCount;

} keyboard_buffer;

static int pression;
static keyboard_buffer keyboardBuffer;

extern void keyboard_init();
extern void keyboard_handler();
extern int getc();
extern int keypressed();

#endif
