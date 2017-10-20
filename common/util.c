#include "util.h"

void memset(void* dst, int value, uint count) {
  char* ptr = (char*) dst;
  uint p = 0;
  do {
    ptr[p] = value;
    p++;
  } while(p < count);
}

void memcpy(void* dst, void* src, uint count) {
  char* ptrd = (char*) dst;
  char* ptrs = (char*) src;
  uint i = 0;
  do {
    ptrd[i] = ptrs[i];
    i++;
  } while(i < count);
}

int strncmp(const char* p, const char* q, uint n) {
  int i = 0;
  do {
    if(p[i] > q[i]) {
      return 1;
    } else if(p[i] < q[i]) {
      return -1;
    }
    i++;
  } while(p[i] != 0 && q[i] != 0 && i <= n);
  return 0;
}

int c2Dto1D(int x, int y, int xmax) {
  return y * xmax + x;
}
