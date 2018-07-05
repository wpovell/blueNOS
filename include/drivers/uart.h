#pragma once

void uart_init(void);
char uart_getc_timeout(int timeout);
char uart_getc(void);
void uart_putc(char c);
