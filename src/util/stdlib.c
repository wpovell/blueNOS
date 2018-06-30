#include "util/stdlib.h"
#include <stdarg.h>

void memcpy(void *dest, const void *src, int bytes) {
  char *d = dest;
  const char *s = src;
  while (bytes--) {
    *d++ = *s++;
  }
}

void bzero(void *dest, int bytes) {
  char *d = dest;
  while (bytes--) {
    *d++ = 0;
  }
}

char *itoa(int num, int base) {
  static char intbuf[32];
  uint32_t j = 0, isneg = 0, i;

  if (num == 0) {
    intbuf[0] = '0';
    intbuf[1] = '\0';
    return intbuf;
  }

  if (base == 10 && num < 0) {
    isneg = 1;
    num = -num;
  }

  i = (uint32_t)num;

  while (i != 0) {
    uint32_t div = i / base;
    uint32_t mod = i % base;
    intbuf[j++] = (mod) < 10 ? '0' + (mod) : 'a' + (mod)-10;
    i = div;
  }

  if (isneg)
    intbuf[j++] = '-';

  if (base == 16) {
    intbuf[j++] = 'x';
    intbuf[j++] = '0';
  } else if (base == 8) {
    intbuf[j++] = '0';
  } else if (base == 2) {
    intbuf[j++] = 'b';
    intbuf[j++] = '0';
  }

  intbuf[j] = '\0';
  j--;
  i = 0;
  while (i < j) {
    isneg = intbuf[i];
    intbuf[i] = intbuf[j];
    intbuf[j] = isneg;
    i++;
    j--;
  }

  return intbuf;
}

int atoi(char *num) {
  int res = 0, power = 0, digit, i;
  char *start = num;

  // Find the end
  while (*num >= '0' && *num <= '9') {
    num++;
  }

  num--;

  while (num != start) {
    digit = *num - '0';
    for (i = 0; i < power; i++) {
      digit *= 10;
    }
    res += digit;
    power++;
    num--;
  }

  return res;
}
