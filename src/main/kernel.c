#include "drivers/mbox/framebuf.h"
#include "drivers/mbox/info.h"
#include "drivers/uart.h"
#include "util/debug.h"
#include "util/stdio.h"
#include <stdint.h>

uint32_t width = 1024;
uint32_t height = 768;
void kernel_main(void) {
  uart_init();
  framebuf_init(width, height);
  KASSERT(buffer_size = sizeof(pixel_t) * width * height);
  printf("addr: %x, size: %d\n", buffer, buffer_size);
  printf("Welcome to BluenOS!\n");
  mbox_print_info();
}
