#pragma once
#include <stdint.h>

typedef enum {
  GPF_INPUT,
  GPF_OUTPUT,
  GPF_F5,
  GPF_F4,
  GPF_F0,
  GPF_F1,
  GPF_F2,
  GPF_F3,
} gpfunc_t;
typedef enum { GPP_NONE, GPP_DOWN, GPP_UP } gppull_t;

int gpio_set_func(uint8_t pin, gpfunc_t f);
int gpio_set_pull(uint8_t pin, gppull_t pull);
int gpio_set(uint8_t pin, int val);
