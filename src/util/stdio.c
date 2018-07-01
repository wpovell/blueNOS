#include "util/stdio.h"
#include "util/stdlib.h"
#include <stdarg.h>

// Set stdio functions to use uart as output
#include "drivers/uart.h"
#define putc uart_putc
#define getc uart_getc

void puts(char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    putc((char)str[i]);
  }
}

void gets(char *buf, int buflen) {
  int i;
  char c;
  // Leave a spot for null char in buffer
  for (i = 0; (c = uart_getc()) != '\r' && buflen > 1; i++, buflen--) {
    putc(c);
    buf[i] = c;
  }

  putc('\n');
  if (c == '\n') {
    buf[i] = '\0';
  } else {
    buf[buflen - 1] = '\0';
  }
}

void printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  for (; *fmt != '\0'; fmt++) {
    if (*fmt == '%') {
      switch (*(++fmt)) {
      case '%':
        putc('%');
        break;
      case 'd':
        puts(itoa(va_arg(args, int), 10));
        break;
      case 'x':
        puts(itoa(va_arg(args, int), 16));
        break;
      case 's':
        puts(va_arg(args, char *));
        break;
      }
    } else if (*fmt == '\n') {
      putc(*fmt);
      putc('\r');
    } else {
      putc(*fmt);
    }
  }

  va_end(args);
}
