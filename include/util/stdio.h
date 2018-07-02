#pragma once
#include <stdint.h>

extern uint8_t print_screen;

void putc(char c);
char getc(void);
void puts(char *str);
void gets(char *buf, int buflen);
void printf(const char *fmt, ...);
