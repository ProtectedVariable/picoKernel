#ifndef _CURSOR_H_
#define _CURSOR_H_
#include "util.h"
#include "../kernel/pio.h"
#include "display.h"

#ifndef CURSOR_CMD
#define CURSOR_CMD 0x3d4
#endif

#ifndef CURSOR_DATA
#define CURSOR_DATA 0x3d5
#endif

typedef struct cursor_t {
  int x;
  int y;
} cursor_t;

void moveCursor(int x, int y);
void getCursorPosition(cursor_t* buffer);
void incrementCursor();

#endif /* end of include guard: _CURSOR_H_ */
