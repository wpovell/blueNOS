#include "drivers/mbox/framebuf.h"
#include "graphics/util.h"
#include "util/stdio.h"
void set_bg(pixel_t color) {
  // QUESTION: This overflowed when using buffer_size rather than h/w, don't
  //           understand why.
  for (uint32_t i = 0; i < buffer_width * buffer_height; i++) {
    buffer[i] = color;
  }
}
