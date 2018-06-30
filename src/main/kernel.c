#include "drivers/uart.h"

void kernel_main(void) {
  // char buf[1024];
  uart_init();
  uart_puts("Welcome to BluenOS!\n");
  while (1) {
    // uart_puts(">> ");
    // uart_gets(buf, sizeof(buf));
    uart_printf("%d\n", uart_getc());
  }
}
