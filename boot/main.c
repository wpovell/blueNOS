#include "drivers/uart.h"
#include "util/stdio.h"
#include "xmodem/client.h"
#include <stdint.h>

void kernel_main(void) {
  uart_init();
  xmodem_init();

  uint8_t *load_point = (uint8_t *)0x80000;
  while (!xmodem_read()) {
    for (int i = 0; i < PACKET_SIZE; i++) {
      *load_point = xmodem_buffer[i];
      load_point++;
    }
  }

  __asm__ volatile("br %0" ::"r"(0x80000));
}
