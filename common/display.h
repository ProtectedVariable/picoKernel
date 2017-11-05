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

/**
 * Initialize the display and puts the cursor to position 0,0 (upper left)
 */
extern void initDisplay();

/**
 * Prints a given string to the screen
 * @param str the string to print
 */
extern void printString(char* str);

/**
 * Prints a given character to the screen
 * @param c the character to print
 */
extern void printChar(char c);

/**
 * Sets the text color
 * @param textcolor the new text color (0-15)
 */
extern void setColor(int textcolor);

/**
 * Gets the current text color
 * @return the current text color
 */
extern int getColor();

/**
 * Sets the background color
 * @param backcolor the new background color
 */
extern void setBackground(int backcolor);

/**
 * Gets the current background color
 * @return the current background color
 */
extern int getBackground();

/**
 * Clears the screen, the screen will be set to the current background color
 */
extern void clearScreen();

/**
 * Prints the string with it's arguments
 * Available formats are: %d (decimal), %x (hex), %s(string) and %c(char)
 * @param format  Format of the string
 * @param VARARGS arguments to format into the string
 */
extern void printf(char* format, ...);

/**
 * Scrolls the screen, the new line will be set to the current background color
 */
extern void scroll();

/**
 * Clears a zone, the zone will be set to the current background color
 * @param x    x-coordinate of the beginning of the zone
 * @param y    y-coordinate of the beginning of the zone
 * @param size size of the zone (in characters)
 */
extern void clearZone(int x, int y, int size);
#endif /* end of include guard: _DISPLAY_H_ */
