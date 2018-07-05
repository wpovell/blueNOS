#include "drivers/uart.h"
#include "graphics/colors.h"
#include "graphics/font.h"
#include "util/stdio.h"
#include "util/stdlib.h"
#include <stdarg.h>

uint8_t print_screen = 1;
uint32_t font_x = 0;
uint32_t font_y = 0;

void putc(char c) {
  // TODO: Handle delete / special chars
// Don't print to screen if in bootloader
#ifndef __BOOTLOADER__
  if (print_screen) {
    draw_char(c, font_x, font_y, WHITE);
    if (c == '\n') {
      font_y++;
      font_x = 0;
    } else {
      font_x++;
    }
    if (font_y > N_ROWS) {
      // TODO: Scroll when out of space
    }
  }
#endif
  uart_putc(c);
}
char getc(void) { return uart_getc(); }

void puts(char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    putc((char)str[i]);
  }
}

void gets(char *buf, int buflen) {
  int i;
  char c;
  // Leave a spot for null char in buffer
  for (i = 0; (c = uart_getc()) != '\n' && buflen > 1; i++, buflen--) {
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
