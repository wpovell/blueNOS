#include "drivers/mbox/framebuf.h"
#include "drivers/mbox/info.h"
#include "drivers/uart.h"
#include "graphics/colors.h"
#include "util/debug.h"
#include "util/stdio.h"
#include "xmodem/client.h"
#include <stdint.h>

uint32_t width = 1024;
uint32_t height = 768;
void kernel_main(void) {
  uart_init();
  // initiate_transfer();
  // framebuf_init(width, height);
  // for (int i = 0; i < 10000; i++) {
  //   buffer[i] = BLUE;
  // }
  // KASSERT(buffer_size = sizeof(pixel_t) * width * height);
  // printf("addr: %x, size: %d\n", buffer, buffer_size);
  printf("Welcome to BluenOS!\n");
  printf("%d\n", getc());
  mbox_print_info();
}
