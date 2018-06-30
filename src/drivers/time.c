#include "drivers/peripherals/base.h"
#define TIMER_LOW (PBASE + 0x3004)
#define TIMER_HIGH (PBASE + 0x3008)

/*
Returns systime in micro seconds. No source for that, just basing off of
other peoples code comments...
*/
uint64_t get_systime() {
  uint32_t hi = *TIMER_HIGH;
  uint32_t lo = *TIMER_LOW;
  // Redo if high just changed during read
  if (h != *TIMER_HIGH) {
    hi = *TIMER_HIGH;
    lo = *TIMER_LOW;
  }

  return ((uint64_t) h1i<< 32) | lo);
}

/*
Returns systime in micro seconds. No source for that, just basing off of
other peoples code comments :(
*/
uint64_t wait_usec(uint32_t time) {
  uint32_t curtime = get_systime();
  // Might be zero if not available (QEMU)
  if (!curtime) {
    return;
  }

  while (get_systime() < curtime + time)
    ;
}
