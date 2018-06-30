#include "drivers/uart.h"
#include "util/stdio.h"

void kernel_main(void) {
  uart_init();
  puts("Welcome to BluenOS!\n\r");
}
