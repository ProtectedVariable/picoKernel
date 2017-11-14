#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <display.h>
#include <types.h>
#include "x86.h"

#define KEYBOARD_BUFFER_SIZE 256

typedef struct keyboard_buffer_t {
	char* buffer[KEYBOARD_BUFFER_SIZE];
	int writeIndex;
	int readIndex;
} keyboard_buffer;

static int pression;
static keyboard_buffer *keyboardBuffer;

extern void keyboard_init();
extern void keyboard_handler();
extern int getc();
extern int keypressed();

#endif
