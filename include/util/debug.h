#pragma once
#include "drivers/uart.h"

#define KASSERT(x)                                                             \
  do {                                                                         \
    if (!(x))                                                                  \
      uart_printf("KASSERT Failed: %s (%s:%d)  \r\n", #x, __FILE__, __LINE__); \
    __asm__ volatile("hlt #0");                                                \
  } while (0)

#define PANIC(x)                                                               \
  do {                                                                         \
    uart_printf("PANIC: %s\r\n", #x);                                          \
    __asm__ volatile("hlt #0");                                                \
  } while (0)
