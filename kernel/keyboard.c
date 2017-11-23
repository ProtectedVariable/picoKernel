/* Keyboard source file
 * Ibanez Thomas, Tournier Vincent
 */
#include "keyboard.h"

static keyboard_buffer keyboardBuffer;

void keyboard_init() {
	keyboardBuffer.readIndex = 0;
	keyboardBuffer.writeIndex = 0;
	keyboardBuffer.capslocked = 0;
	keyboardBuffer.capsCount = 0;
	for(int i = 0 ; i < KEYBOARD_BUFFER_SIZE ; i++)
		keyboardBuffer.buffer[i] = 0;
}

void bufferWrite(uint8_t toWrite) {
	if(keyboardBuffer.bufferCount >= KEYBOARD_BUFFER_SIZE) {
		setColor(RED);
		printf("KEYBOARD BUFFER IS FULL\n");
		setColor(WHITE);
		return;
	}
	keyboardBuffer.buffer[keyboardBuffer.writeIndex] = toWrite;
	keyboardBuffer.writeIndex++;
	keyboardBuffer.bufferCount++;
	keyboardBuffer.writeIndex %= KEYBOARD_BUFFER_SIZE;
}

uint8_t bufferRead() {
	if(keyboardBuffer.readIndex == keyboardBuffer.writeIndex)
		return 0;

	uint8_t read = keyboardBuffer.buffer[keyboardBuffer.readIndex];
	keyboardBuffer.readIndex++;
	keyboardBuffer.bufferCount--;
	keyboardBuffer.readIndex %= KEYBOARD_BUFFER_SIZE;
	return read;
}

void keyboard_handler() {
	if(inb(KEYBOARD_STATUS_POINTER) & 1) {
		bufferWrite(inb(KEYBOARD_DATA_POINTER));
	}
}

int getc() {
	int charGet = 0;
	uint8_t scanCode = 0;
	while(!charGet) {
		while(keyboardBuffer.bufferCount == 0) {}
		scanCode = bufferRead();

		if(scanCode == 58) {
			if(keyboardBuffer.capslocked)
				keyboardBuffer.capsCount--;
			else
				keyboardBuffer.capsCount++;
			keyboardBuffer.capslocked = !keyboardBuffer.capslocked;
		}
		else if(scanCode == 42 || scanCode == 54)
			keyboardBuffer.capsCount++;
		else if(scanCode == 170 || scanCode == 182)
			keyboardBuffer.capsCount--;
		else if(scanCode >> 7 || scancodes[scanCode] == -1)
			continue;
		else
			charGet = 1;
	}
	if(keyboardBuffer.capsCount != 0)
		return scancodes[scanCode + (SCANCODES_TABLE_SIZE / 2)];
	else
		return scancodes[scanCode];
}

// Non-blocking call. Return 1 if a key is pressed
int keypressed() {
	return keyboardBuffer.bufferCount != 0;
}
