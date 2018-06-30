#include "drivers/peripherals/base.h"

/*
Code from raspi3-tutorial
Seems that it's related to forcing the watchdog timer to expire
See (https://en.wikipedia.org/wiki/Watchdog_timer)
TODO: Understand what's actually going on here.
*/

#define PM_RSTC ((volatile unsigned int *)(PBASE + 0x0010001c))
#define PM_RSTS ((volatile unsigned int *)(PBASE + 0x00100020))
#define PM_WDOG ((volatile unsigned int *)(PBASE + 0x00100024))
#define PM_WDOG_MAGIC 0x5a000000
#define PM_RSTC_FULLRST 0x00000020

void reboot() {
  unsigned int r;

  r = *PM_RSTS;
  r &= ~0xfffffaaa;
  *PM_RSTS = PM_WDOG_MAGIC | r;
  *PM_WDOG = PM_WDOG_MAGIC | 10;
  *PM_RSTC = PM_WDOG_MAGIC | PM_RSTC_FULLRST;
}
