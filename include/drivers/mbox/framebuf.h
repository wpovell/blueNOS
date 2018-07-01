#pragma once
#include <stdint.h>

typedef struct pixel {
  uint8_t _;
  uint8_t r;
  uint8_t g;
  uint8_t b;
} pixel_t;

typedef struct framebuf {
  pixel_t  *addr;
  uint32_t size;
} framebuf_t;

extern pixel_t *buffer;
extern uint32_t buffer_size;
extern uint32_t buffer_width;
extern uint32_t buffer_height;

int framebuf_init(uint32_t width, uint32_t height);
