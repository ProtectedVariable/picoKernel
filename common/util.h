#ifndef _UTIL_H_
#define _UTIL_H_

#include "types.h"

void memset(void* dst, int value, uint count);
void memcpy(void* dst, void* src, uint count);
int strncmp(const char *p, const char *q, uint n);

#endif /* end of include guard: _UTIL_H_ */
