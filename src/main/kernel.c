#include "drivers/mbox/info.h"
#include "drivers/uart.h"
#include "util/stdio.h"
#include <stdint.h>
void kernel_main(void) {
  uart_init();
  printf("Welcome to BluenOS!\n");
  mbox_print_info();
}
