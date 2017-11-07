/* All cursor-related functions
 * Ibanez Thomas, Vincent Tournier
 */
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

/**
 * Changes cursor position
 * @param x new x position
 * @param y new y position
 */
extern void moveCursor(int x, int y);

/**
 * Gets the cursor current position
 * @param buffer buffer to write the position to
 */
extern void getCursorPosition(cursor_t* buffer);

/**
 * Increments cursor position
 */
extern void incrementCursor();

#endif /* end of include guard: _CURSOR_H_ */
