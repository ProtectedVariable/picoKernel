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

void memset(void* dst, int value, uint count);
void memcpy(void* dst, void* src, uint count);
int strncmp(const char *p, const char *q, uint n);
int c2Dto1D(int x, int y, int xmax);
void itoa(char* buffer, int i, int radix);

#endif /* end of include guard: _UTIL_H_ */
