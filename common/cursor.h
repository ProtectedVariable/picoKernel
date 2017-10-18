#ifndef _CURSOR_H_
#define _CURSOR_H_
#include "util.h"
#include "../kernel/pio.h"

typedef struct cursor_t {
  int x;
  int y;
} cursor_t;

static cursor_t cursor;

void moveCursor(int x, int y);
void getCursorPosition(cursor_t* buffer);
void incrementCursor(int maxX);

#endif /* end of include guard: _CURSOR_H_ */
