#include "drivers/time.h"
#include "drivers/uart.h"
#include "util/stdlib.h"

void kernel_main(void) {
  uart_init();
  uart_puts("Welcome to BluenOS!\n\r");
  for (int i = 0; i < 100; i++) {
    uart_printf("%d\r\n", i);
    wait_sec(1);
  }
}
