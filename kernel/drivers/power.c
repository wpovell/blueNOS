#include "drivers/base.h"
#include "util/stdlib.h"

/*
Code from raspi3-tutorial
Seems that it's related to forcing the watchdog timer to expire
See: (https://en.wikipedia.org/wiki/Watchdog_timer)
@TODO: Understand what's actually going on here.
       What's with the "magic"?
*/

#define PM_RSTC (PBASE + 0x0010001c)
#define PM_RSTS (PBASE + 0x00100020)
#define PM_WDOG (PBASE + 0x00100024)

#define PM_WDOG_MAGIC 0x5a000000
#define PM_RSTC_FULLRST 0x00000020

void reboot(void) {
  unsigned int r;

  r = get32(PM_RSTS);
  r &= ~0xfffffaaa;
  put32(PM_RSTS, PM_WDOG_MAGIC | r);
  put32(PM_WDOG, PM_WDOG_MAGIC | 10);
  put32(PM_RSTC, PM_WDOG_MAGIC | PM_RSTC_FULLRST);
}
