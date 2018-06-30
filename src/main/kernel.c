#include "drivers/uart.h"
#include "util/stdlib.h"

void kernel_main(void) {
  uart_init();
  uart_puts("Welcome to BluenOS!\n\r");
}
