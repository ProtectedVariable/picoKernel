#include "cursor.h"

static cursor_t cursor;

void moveCursor(int x, int y) {
    if(x > FRAMBUFFER_WIDTH - 1) {
        x = 0;
        y++;
    }
    else if(x < 0) {
        x = FRAMBUFFER_WIDTH - 1;
        if(y > 0)
            y--;
    }
    if(y > FRAMBUFFER_HEIGHT - 1) {
        scroll();
        x = 0;
        y = FRAMBUFFER_HEIGHT - 1;
    }
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

void incrementCursor() {
    moveCursor(cursor.x + 1, cursor.y);
}

void decrementCursor() {
    moveCursor(cursor.x - 1, cursor.y);
}
