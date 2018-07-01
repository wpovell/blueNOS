#include "drivers/base.h"
#include "drivers/mbox/mbox.h"
#include "util/stdlib.h"

/*
  Some (okayish) documentation for the mailbox here:
  https://github.com/raspberrypi/firmware/wiki
*/

// Shared mailbox buffer
// NOTE: Volatile is necessary at -O2 and higher. Presumably since this array
//       is shared between CPU and GPU.
volatile uint32_t __attribute__((aligned(16))) mbox[36];

#define MBOX_BASE (PBASE + 0x0000B880)

#define MBOX_READ (MBOX_BASE + 0x0)
#define MBOX_STATUS (MBOX_BASE + 0x18)
#define MBOX_WRITE (MBOX_BASE + 0x20)

#define READ_EMPTY (1 << 30)
#define WRITE_FULL (1 << 31)

int mbox_call(uint8_t channel) {
  while (get32(MBOX_STATUS) & WRITE_FULL)
    ;

  // Last 4bits of memory have channel stored in them (hence why its aligned)
  uint32_t msg = ((uint32_t)(((uint64_t)&mbox) & (~0xF))) | (channel & 0xF);
  put32(MBOX_WRITE, msg);

  uint32_t res;
  do {
    while (get32(MBOX_STATUS) & READ_EMPTY)
      ;
    res = get32(MBOX_READ);
  } while ((res & 0xF) != channel); // Only use if it's from our channel

  return mbox[1] != RES_CODE;
}
