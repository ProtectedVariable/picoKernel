/* All cursor-related functions
 * Thomas Ibanez, Vincent Tournier
 */
#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "util.h"
#include "pio.h"
#include "display.h"

#define CURSOR_CMD 0x3d4
#define CURSOR_DATA 0x3d5

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

/**
 * Decrements cursor position until it reaches (0,0)
 * Then only decrements x position
 */
extern void decrementCursor();

#endif /* end of include guard: _CURSOR_H_ */
