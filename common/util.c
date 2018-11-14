/* Utility functions source file
* Ibanez Thomas, Tournier Vincent
*/
#include "util.h"

void* memset(void* dst, int value, uint count) {
    if(count <= 0) return NULL;
    char* ptr = (char*) dst;
    uint p = 0;
    do {
        ptr[p] = value;
        p++;
    } while(p < count);
    return dst;
}

void* memcpy(void* dst, void* src, uint count) {
    if(count <= 0) return NULL;
    char* ptrd = (char*) dst;
    char* ptrs = (char*) src;
    uint i = 0;
    do {
        ptrd[i] = ptrs[i];
        i++;
    } while(i < count);
    return dst;
}

int strncmp(const char* p, const char* q, uint n) {
    if(n <= 0) return NULL;
    uint i = 0;
    do {
        if(p[i] > q[i]) {
            return 1;
        } else if(p[i] < q[i]) {
            return -1;
        }
        i++;
    } while(p[i] != 0 && q[i] != 0 && i < n);
    return 0;
}

extern int strcmp(const char *p, const char *q) {
    uint i = 0;
    do {
        if(p[i] > q[i]) {
            return 1;
        } else if(p[i] < q[i]) {
            return -1;
        }
        i++;
    } while(p[i] != 0 && q[i] != 0);
    return 0;
}

int c2Dto1D(int x, int y, int xmax) {
    return y * xmax + x;
}

char* itoa(char* buffer, int n, int radix) {
    int index = 0;
    char inverted[10];
    int neg = 0;
    if(n < 0) {
        n = n * -1;
        neg = 1;
    }
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
    if(neg == 1) {
        buffer[currentIndex++] = '-';
    }

    int significantValue = 0;
    for(int i = 9 ; i >= 0 ; i--) {
        if(inverted[i] != '0' || significantValue || i == 0) {
            significantValue = 1;
            buffer[currentIndex++] = inverted[i];
        }
    }
    buffer[currentIndex] = 0;
    return buffer;
}

int atoi(const char* str) {
	int result = 0;
	unsigned int digit;
	bool negative;

	if (*str == '-') {
		negative = true;
		str += 1;
	} else {
		negative = false;
		if (*str == '+') {
			str += 1;
		}
	}
	do {
		digit = *str - '0';
		if (digit > 9) {
			break;
		}
		result = (10 * result) + digit;
	} while (str += 1);

	if (negative) {
		return -result;
	}
	return result;
}

int strlen(const char* string) {
	int i = 0;
	const char* str = string;
	while (*str) {
		i++;
		str++;
	}
	return i;
}
