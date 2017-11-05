#ifndef _UTIL_H_
#define _UTIL_H_
#include "types.h"

#ifndef FRAMBUFFER_WIDTH
#define FRAMBUFFER_WIDTH 80
#endif

#ifndef FRAMBUFFER_HEIGHT
#define FRAMBUFFER_HEIGHT 25
#endif

#ifndef FRAMBUFFER_START
#define FRAMBUFFER_START ((char*) 0xB8000)
#endif

/**
 * Sets a memory zone to a certain value
 * @param dst   pointer to the beginning of the memory zone
 * @param value value to set each byte to
 * @param count number of bytes to set
 */
void* memset(void* dst, int value, uint count);

/**
 * Copy a memory zone to another memory zone
 * @param dst   zone to copy to
 * @param src   zone to copy from
 * @param count number of bytes to copy
 */
void* memcpy(void* dst, void* src, uint count);

/**
 * Compares to strings n first characters
 * @param  p first string
 * @param  q second string
 * @param  n number of characters to compare
 * @return   -1 if p < q, 1 if p > q, 0 if p = q
 */
int strncmp(const char *p, const char *q, uint n);

/**
 * Converts a 2d coordinate to a 1d coordinate
 * @param  x    x-coordinate
 * @param  y    y-coordinate
 * @param  xmax maximum x value
 * @return      1D coordinate created from (x,y)
 */
int c2Dto1D(int x, int y, int xmax);

/**
 * Integer to String conversion
 * @param buffer buffer to write the number to
 * @param n      number to parse
 * @param radix  base to convert to
 */
void itoa(char* buffer, int n, int radix);

#endif /* end of include guard: _UTIL_H_ */
