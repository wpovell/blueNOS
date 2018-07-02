#include "drivers/mbox/framebuf.h"
#include "graphics/util.h"
#include "util/stdio.h"
void set_bg(pixel_t color) {
  printf("%d\n", buffer_size);
  for (uint32_t i = 0; i < buffer_size; i++) {
    buffer[i] = color;
  }
}
