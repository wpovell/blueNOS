#include "drivers/mbox/framebuf.h"
#include "graphics/font.h"
#include "graphics/font_bitmap.h"
#include "util/debug.h"
#include "util/stdio.h"

int is_printable(char c) {
  if (c == '\n' || c == ' ' || c == '\t') {
    return 1;
  }

  for (int i = 0; i < 8; i++) {
    if (font8x8_basic[(int)c][i]) {
      return 1;
    }
  }

  return 0;
}

void draw_char(char c, uint32_t x, uint32_t y, pixel_t color) {
  int size = PADDING + FONT_SIZE;
  KASSERT(x < buffer_width && y < buffer_height);
  uint32_t startX = x * size;
  uint32_t startY = y * size;
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
