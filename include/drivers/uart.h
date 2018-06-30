#pragma once

void uart_init(void);
char uart_getc(void);
void uart_putc(char c);
void uart_puts(char *str);
void uart_gets(char *buf, int buflen);
void uart_printf(const char *fmt, ...);
