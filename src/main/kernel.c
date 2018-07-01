#include "drivers/uart.h"
#include "main/atags.h"
#include "util/stdio.h"
#include <stdint.h>
void kernel_main(void) {
  uart_init();
  puts("Welcome to BluenOS!\n\r");
}
