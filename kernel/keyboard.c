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

int keypressed() {
	return keyboardBuffer.bufferCount != 0;
}

int handleDeadKeysAccents(int stored, int typedChar) {
    int result = 0;
    if(stored == 94) {
        if(typedChar == 97) // a
            result = 131 ;
        else if(typedChar == 101) // e
            result = 136;
        else if(typedChar == 105) // i
            result = 140;
        else if(typedChar == 111) // o
            result = 147;
        else if(typedChar == 117) // u
            result = 150;
        else if(typedChar == 65) // A
            result = 182;
        else if(typedChar == 69) // E
            result = 210;
        else if(typedChar == 73) // I
            result = 215;
        else if(typedChar == 79) // O
            result = 226;
        else if(typedChar == 85) // U
            result = 234;
    }
    else if(stored == 96) {
        if(typedChar == 97) // a
            result = 133;
        else if(typedChar == 101) // e
            result = 138;
        else if(typedChar == 105) // i
            result = 141;
        else if(typedChar == 111) // o
            result = 149;
        else if(typedChar == 117) // u
            result = 151;
        else if(typedChar == 65) // A
            result = 183;
        else if(typedChar == 69) // E
            result = 212;
        else if(typedChar == 73) // I
            result = 222;
        else if(typedChar == 79) // O
            result = 227;
        else if(typedChar == 85) // U
            result = 235;
    }
    else if(stored == 249) {
        if(typedChar == 97) // a
            result = 132;
        else if(typedChar == 101) // e
            result = 137;
        else if(typedChar == 105) // i
            result = 139;
        else if(typedChar == 111) // o
            result = 148;
        else if(typedChar == 117) // u
            result = 129;
        else if(typedChar == 65) // A
            result = 142;
        else if(typedChar == 69) // E
            result = 211;
        else if(typedChar == 73) // I
            result = 216;
        else if(typedChar == 79) // O
            result = 153;
        else if(typedChar == 85) // U
            result = 154;
    }

    return result;
}
