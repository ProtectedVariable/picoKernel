#include "keyboard.h"

void keyboard_init() {
	pression = 0;
	keyboardBuffer.readIndex = 0;
	keyboardBuffer.writeIndex = 0;
	for(int i = 0 ; i < KEYBOARD_BUFFER_SIZE ; i++)
		keyboardBuffer.buffer[i] = 0;
}

void bufferWrite(char toWrite) {
	if(keyboardBuffer.bufferCount >= KEYBOARD_BUFFER_SIZE) {
		//TODO Show error
		return;
	}
	keyboardBuffer.buffer[keyboardBuffer.writeIndex] = toWrite;
	keyboardBuffer.writeIndex++;
	keyboardBuffer.bufferCount++;
	keyboardBuffer.writeIndex %= KEYBOARD_BUFFER_SIZE;
}

char bufferRead() {
	if(keyboardBuffer.readIndex == keyboardBuffer.writeIndex)
		return 0;

	char read = keyboardBuffer.buffer[keyboardBuffer.readIndex];
	keyboardBuffer.readIndex++;
	keyboardBuffer.bufferCount--;
	keyboardBuffer.readIndex %= KEYBOARD_BUFFER_SIZE;
	return read;
}

void keyboard_handler() {
	if(inb(KEYBOARD_STATUS_POINTER) & 1) {
		uint8_t scanCode = inb(KEYBOARD_DATA_POINTER);
		if(scanCode >> 7) {
			printf("%d\n", scanCode);

			//Key released
		} else {
			//convert to ascii
			printf("%d\n", scanCode);
			//bufferWrite();
		}
	}
}

int getc() {
	while(keyboardBuffer.bufferCount == 0) {}
	return bufferRead();
}

// Non-blocking call. Return 1 if a key is pressed
int keypressed() {
	return pression;
}
