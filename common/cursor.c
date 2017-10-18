#include "cursor.h"

void moveCursor(int x, int y) {
  cursor.x = x;
  cursor.y = y;
  int pos = c2Dto1D(x, y, FRAMBUFFER_WIDTH);
}

void getCursorPosition(cursor_t* buffer) {
  buffer->x = cursor.x;
  buffer->y = cursor.y;
}

void incrementCursor(int maxX) {
  if(cursor.x == maxX) {
    cursor.x = 0;
    cursor.y++;
  } else {
    cursor.x++;
  }
}
