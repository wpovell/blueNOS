#include "drivers/mbox/framebuf.h"
#include "graphics/font.h"
#include "graphics/font_bitmap.h"
#include "util/debug.h"
#include "util/stdio.h"

void draw_char(char c, uint32_t x, uint32_t y, pixel_t color) {
  KASSERT(x < buffer_width && y < buffer_height);
  uint32_t startX = x * 8;
  uint32_t startY = y * 8;
  char *bitmap = font8x8_basic[(int)c];
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      int fill = (bitmap[y] >> x) & 1;
      if (fill) {
        buffer[(startY + y) * buffer_width + (startX + x)] = color;
      }
    }
  }
}

// void draw_char(char c, uint32_t x, uint32_t y) {
//   draw_char(c, x, y, (pixel_t){0xFF, 0xFF, 0xFF, 0xFF});
// }
