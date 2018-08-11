#pragma once
#include "util/stdio.h"

#define KASSERT(x)                                                             \
  do {                                                                         \
    if (!(x)) {                                                                \
      printf("KASSERT Failed: %s (%s:%d)  \r\n", #x, __FILE__, __LINE__);      \
      __asm__ volatile("hlt #0");                                              \
    }                                                                          \
  } while (0)

#define PANIC(x)                                                               \
  do {                                                                         \
    printf("PANIC: %s\r\n", #x);                                               \
    __asm__ volatile("hlt #0");                                                \
  } while (0)

// @TODO: Add more functions here such as dbg
