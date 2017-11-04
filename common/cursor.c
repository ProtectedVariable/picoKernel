#include "cursor.h"

static cursor_t cursor;

void moveCursor(int x, int y) {
  cursor.x = x;
  cursor.y = y;
  int pos = c2Dto1D(x, y, FRAMBUFFER_WIDTH);
  outb(CURSOR_CMD, 0xF);
  outb(CURSOR_DATA, pos & 0xFF);
  outb(CURSOR_CMD, 0xE);
  outb(CURSOR_DATA, (pos & 0xFF00) >> 8);
}

void getCursorPosition(cursor_t* buffer) {
  buffer->x = cursor.x;
  buffer->y = cursor.y;
}

void incrementCursor(int maxX) {
  if(cursor.x == maxX - 1) {
    if(cursor.y + 1 >= FRAMBUFFER_HEIGHT) {
        scroll();
        moveCursor(0, cursor.y);
    } else {
        moveCursor(0, cursor.y + 1);
    }
  } else {
    moveCursor(cursor.x + 1, cursor.y);
  }
}
