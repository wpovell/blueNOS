#include "drivers/mbox/framebuf.h"
#include "drivers/mbox/info.h"
#include "drivers/uart.h"
#include "graphics/colors.h"
#include "graphics/font.h"
#include "graphics/util.h"
#include "util/debug.h"
#include "util/stdio.h"
#include "xmodem/client.h"
#include <stdint.h>

uint32_t width = 1024;
uint32_t height = 768;
void kernel_main(void) {
  framebuf_init(width, height);
  printf("addr: %x, size: %d\n", buffer, buffer_size);
  printf("Welcome to BluenOS!\n");
  mbox_print_info();
  while (1) {
    printf("%d\n", getc());
  }
}
