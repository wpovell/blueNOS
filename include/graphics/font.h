#pragma once

#define PADDING 3
#define FONT_SIZE 8

#define N_ROWS (buffer_height / (PADDING + FONT_SIZE))
#define N_COLS (buffer_width / (PADDING + FONT_SIZE))

int is_printable(char c);
void draw_char(char c, uint32_t x, uint32_t y, pixel_t color);
