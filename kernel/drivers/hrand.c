#include "drivers/base.h"
#include "drivers/hrand.h"
#include "util/stdlib.h"

/*
Based on this source from the linux kernel
https://github.com/thenaran/linux-rpi/blob/master/drivers/char/hw_random/bcm2708-rng.c
And this
https://github.com/bztsrc/raspi3-tutorial/blob/master/06_random/rand.c
Haven't been able to find any actual documentation for this.
*/

#define RNG_BASE (PBASE + 0x00104000)
#define RNG_CTRL (RNG_BASE)
#define RNG_STATUS (RNG_BASE + 0x4)
#define RNG_DATA (RNG_BASE + 0x8)

// For enabling rgen
// The 2nd options is faster but less random
#define RNG_RBGEN 0x1
#define RNG_RBG2X 0x2

// Initial numbers are less random
#define RNG_WARMUP_COUNT 0x40000

void hrand_init(void) {
  // NOTE: Other srcs mask interrupts here?
  put32(RNG_STATUS, RNG_WARMUP_COUNT);
  put32(RNG_CTRL, RNG_RBGEN);
}

/*
  Returns a large random number.
  Unclear what RAND_MAX is.
  NOTE: There is no checking that hrand is actually enabled.
  QUESTION: What prevents two rapid reads from getting the same number here?
            Is the underlying peripheral aware of the read?
*/
uint32_t hrand(void) {
  // Wait for some entropy
  while (!(get32(RNG_STATUS) >> 24))
    ;
  return get32(RNG_DATA);
}
