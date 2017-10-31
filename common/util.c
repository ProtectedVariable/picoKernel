#include "util.h"

#include "display.h"

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

void itoa(char* buffer, int n, int radix) {
	int index = 0;
	char inverted[10];
	for(int modulator = radix ; modulator > 0 ; modulator *= radix) {
		inverted[index] = ((n % modulator) / (modulator / radix));
		inverted[index] = inverted[index] < 10 ? inverted[index] + 48 : inverted[index] + 87;
		index++;
	}
	if(radix == 10)
		inverted[index] = (n / 1000000000);
	else if(radix == 16)
	{
		inverted[index] = (n / 268435456);
		inverted[index] = inverted[index] < 10 ? inverted[index] + 48 : inverted[index] + 87;
	}

	int currentIndex = 0;
	if(n < 0)
		buffer[currentIndex++] = '-';

    int significantValue = 0;
	for(int i = 9 ; i >= 0 ; i--) {
		if(inverted[i] != '0' || significantValue) {
			significantValue = 1;
			buffer[currentIndex++] = inverted[i];
		}
	}
    buffer[currentIndex] = 0;
}
