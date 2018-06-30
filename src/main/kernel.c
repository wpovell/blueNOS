#include "drivers/hrand.h"
#include "drivers/time.h"
#include "drivers/uart.h"
#include "util/debug.h"
#include "util/stdlib.h"

#define N 10000
void kernel_main(void) {
  uart_init();
  uart_puts("Welcome to BluenOS!\n\r");
}
