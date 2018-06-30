#include "drivers/base.h"
#include "drivers/time.h"
#include "util/stdlib.h"

/* All the magic here is described in Section 12 */

#define TIMER_LOW (PBASE + 0x3004)
#define TIMER_HIGH (PBASE + 0x3008)

/*
Returns systime in micro seconds. No source for that, just basing off of
other peoples' code comments...
*/
uint64_t get_systime() {
  uint32_t hi = get32(TIMER_HIGH);
  uint32_t lo = get32(TIMER_LOW);
  // Redo if high just changed during read
  if (hi != get32(TIMER_HIGH)) {
    hi = get32(TIMER_HIGH);
    lo = get32(TIMER_LOW);
  }

  return ((uint64_t)hi << 32) | lo;
}

/*
Returns systime in micro seconds. No source for that, just basing off of
other peoples code comments :(
*/
void wait_usec(uint64_t time) {
  uint32_t curtime = get_systime();
  // Might be zero if not available (QEMU)
  if (!curtime) {
    return;
  }

  while (get_systime() < curtime + time)
    ;
}

inline void wait_msec(uint32_t time) { wait_usec(time * 1000); }
inline void wait_sec(uint32_t time) { wait_usec(time * 1000000); }
