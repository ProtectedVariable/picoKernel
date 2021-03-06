/* Keyboard header file
 * Ibanez Thomas, Tournier Vincent
 */
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "display.h"
#include "../common/types.h"
#include "x86.h"

#define KEYBOARD_BUFFER_SIZE 256
#define SCANCODES_TABLE_SIZE 256
#define KEYBOARD_STATUS_POINTER 0x64
#define KEYBOARD_DATA_POINTER 0x60

/**
 * Buffer to store keyboard inputs until they get treated by the kernel
 */
typedef struct keyboard_buffer_t {
	uint8_t buffer[KEYBOARD_BUFFER_SIZE];
	int writeIndex;
	int readIndex;
	int bufferCount;
	int capsCount;
	int capslocked;
} keyboard_buffer;

/**
 * Table de traduction des makecodes vers les caractères ASCII
 */
static const int scancodes[SCANCODES_TABLE_SIZE] = {
	-1, -1, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 39, 94, 8, 9, //0 à 15
	113, 119, 101, 114, 116, 122, 117, 105, 111, 112, 138, 249, 10, -1, 97, 115, //16 à 31
	100, 102, 103, 104, 106, 107, 108, 130, 133, 21, -1, 36, 121, 120, 99, 118, //32 à 47
	98, 110, 109, 44, 46, 45, -1, -1, -1, 32, -1, -1, -1, -1, -1, -1, //48 à 63
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //64 à 79
	-1, -1, -1, -1, -1, -1, 60, -1, -1, -1, -1, -1, -1, -1, -1, -1, //80 à 95
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //96 à 111
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //112 à 127
	-1, -1, 43, 34, 42, 135, 37, 38, 47, 40, 41, 61, 63, 96, 8, 9, //128 à 143
	81, 87, 69, 82, 84, 90, 85, 73, 79, 80, 129, 33, 10, -1, 65, 83, //144 à 159
	68, 70, 71, 72, 74, 75, 76, 148, 132, 248, -1, 156, 89, 88, 67, 86, //160 à 175
	66, 78, 77, 59, 58, 95, -1, -1, -1, 32, -1, -1, -1, -1, -1, -1, //176 à 191
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //192 à 207
	-1, -1, -1, -1, -1, -1, 62, -1, -1, -1, -1, -1, -1, -1, -1, -1, //208 à 223
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 //240 à 255
};

/**
 * Initialize the keyboard
 */
extern void keyboard_init();

/**
 * Keyboard interrupt handler
 */
extern void keyboard_handler();

/**
 * Gets the typed char from the keyboard
 * @return char code of the key pressed
 */
extern int getc();

/**
 * Check if any key is pressed
 * @return true if a key is pressed, false otherwise
 */
extern int keypressed();

/**
 * Converts a dead-key + letter into a combination
 * @param  stored    the stored dead key
 * @param  typedChar the typed letter
 * @return           combination if it exists of the two params, 0 otherwise
 */
extern int handleDeadKeysAccents(int stored, int typedChar);

#endif
