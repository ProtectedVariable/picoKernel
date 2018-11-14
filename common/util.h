/* Utility functions header file
 * Ibanez Thomas, Tournier Vincent
 */
#ifndef _UTIL_H_
#define _UTIL_H_
#include "../common/types.h"

/**
 * Sets a memory zone to a certain value
 * @param dst   pointer to the beginning of the memory zone
 * @param value value to set each byte to
 * @param count number of bytes to set
 */
extern void* memset(void* dst, int value, uint count);

/**
 * Copy a memory zone to another memory zone
 * @param dst   zone to copy to
 * @param src   zone to copy from
 * @param count number of bytes to copy
 */
extern void* memcpy(void* dst, void* src, uint count);

/**
 * Compares two string's n first characters
 * @param  p first string
 * @param  q second string
 * @param  n maximum number of characters to compare
 * @return   -1 if p < q, 1 if p > q, 0 if p = q
 */
extern int strncmp(const char *p, const char *q, uint n);

/**
 * Compares two string
 * @param  p first string
 * @param  q second string
 * @return   -1 if p < q, 1 if p > q, 0 if p = q
 */
extern int strcmp(const char *p, const char *q);

/**
 * Converts a 2d coordinate to a 1d coordinate
 * @param  x    x-coordinate
 * @param  y    y-coordinate
 * @param  xmax maximum x value
 * @return      1D coordinate created from (x,y)
 */
extern int c2Dto1D(int x, int y, int xmax);

/**
 * Integer to String conversion
 * @param buffer buffer to write the number to
 * @param n      number to parse
 * @param radix  base to convert to
 */
extern char* itoa(char* buffer, int n, int radix);

extern int atoi(const char* str);

extern int strlen(const char* str);

#endif /* end of include guard: _UTIL_H_ */
