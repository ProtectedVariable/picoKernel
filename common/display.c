#include "display.h"

void printString(char* str) {
  for (int i = 0; str[i]; i++) {
    printChar(str[i]);
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
  memset((void*) (0xB8000 + (c2Dto1D(cursor.x, cursor.y, FRAMBUFFER_WIDTH) * 2)), c, 1);
  memset((void*) (0xB8000 + (c2Dto1D(cursor.x, cursor.y, FRAMBUFFER_WIDTH) * 2) + 1), color, 1);
  incrementCursor(FRAMBUFFER_WIDTH);
}

void setColor(int textcolor) {
  color &= 0xF0;
  color |= textcolor;
}

void setBackground(int backcolor) {
  color &= 0x0F;
  color |= backcolor << 4;
}

void clearScreen() {
  memset(FRAMBUFFER_START, 0, 0xFA0);
}
