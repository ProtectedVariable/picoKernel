#include "display.h"

void initDisplay() {
  clearScreen();
  moveCursor(0, 0);
}

void printString(char* str) {
  for (int i = 0; str[i];) {
    printChar(str[i]);
    i++;
  }
}

void printChar(char c) {
  cursor_t cursor;
  getCursorPosition(&cursor);
  if(c == '\n') {
    moveCursor(0, cursor.y + 1);
    return;
  } else if(c == '\t') {
    incrementCursor(FRAMBUFFER_WIDTH);
    incrementCursor(FRAMBUFFER_WIDTH);
    incrementCursor(FRAMBUFFER_WIDTH);
    incrementCursor(FRAMBUFFER_WIDTH);
    return;
  } else if(c == '\r') {
    moveCursor(0, cursor.y);
    return;
  }
  void* address = (void*) (FRAMBUFFER_START + (c2Dto1D(cursor.x, cursor.y, FRAMBUFFER_WIDTH) * 2));
  memset(address, c, 1);
  memset(address + 1, color, 1);
  incrementCursor(FRAMBUFFER_WIDTH);
}

void setColor(int textcolor) {
  color &= 0xF0;
  color |= textcolor;
}

int getColor() {
  return color & 0x0F;
}

void setBackground(int backcolor) {
  color &= 0x0F;
  color |= backcolor << 4;
}

int getBackground() {
  return color & 0xF0 >> 4;
}

void clearScreen() {
  for (int i = 0; i < 0xFA0; i++) {
    if(i % 2 == 1) {
      memset(FRAMBUFFER_START + i, color, 1);
    } else {
      memset(FRAMBUFFER_START + i, 0, 1);
    }
  }

  void printf(char* format, ...) {
	  int argumentCount = 4;
	  for(int i = 0 ; format[i] ; i++) {
		  if(format[i] == '%') {
			  switch(format[i + 1]) {
			      case 'd':
				  	char image[12] = {0};
					itoa(image, format + argumentCount, 10);
				  	printString(image);
	  			  	argumentCount += 4;
					break;
				  case 's':
				  	printString(format + argumentCount);
					argumentCount += 4;
					break;
				  case 'c':
				  	printChar(*(format + argumentCount));
					argumentCount += 4;
					break;
				  case 'x':
				  	char image[12] = {0};
					itoa(image, format + argumentCount, 16);
				  	printString("0x");
				  	printString(image);
	  			  	argumentCount += 4;
					break;
				  case '%':
				  	printChar('%');
					break;
			  }
			  i++;
		  }
		  else
		  	printChar(format[i]);
	  }
  }
}
