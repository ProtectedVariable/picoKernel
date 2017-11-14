#include "keyboard.h"

void keyboard_init() {
	pression = 0;
	keyboardBuffer->readIndex = 0;
	keyboardBuffer->writeIndex = 0;
	for(int i = 0 ; i < KEYBOARD_BUFFER_SIZE ; i++)
		keyboardBuffer->buffer[i] = 0;
}

void bufferWrite(char toWrite) {
	keyboardBuffer->buffer[keyboardBuffer->writeIndex] = toWrite;
	keyboardBuffer->writeIndex++;
	keyboardBuffer->writeIndex %= KEYBOARD_BUFFER_SIZE;
}

char bufferRead() {
	if(keyboardBuffer->readIndex == keyboardBuffer.writeIndex)
		return 0;

	char read = keyboardBuffer->buffer[keyboardBuffer->readIndex];
	keyboardBuffer->readIndex++;
	keyboardBuffer->readIndex %= KEYBOARD_BUFFER_SIZE;
	return read;
}

void keyboard_handler() {

}

int getc() {

}

// Non-blocking call. Return 1 if a key is pressed
int keypressed() {
	return pression;
}
