#include "drivers/uart.h"

#define KASSERT(x)                                                             \
  do {                                                                         \
    if (!(x))                                                                  \
      uart_printf("KASSERT Failed: %s (%s:%d)  \r\n", #x, __FILE__, __LINE__);            \
    __asm__ volatile("hlt #0");                                              \
  } while (0)
