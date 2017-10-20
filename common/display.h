#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include "types.h"
#include "cursor.h"

#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GRAY 7
#define DARK_GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define YELLOW 14
#define WHITE 15

static uint8_t color = 0x0F;

void initDisplay();
void printString(char* str);
void printChar(char c);
void setColor(int textcolor);
int getColor();
void setBackground(int backcolor);
int getBackground();
void clearScreen();

#endif /* end of include guard: _DISPLAY_H_ */
