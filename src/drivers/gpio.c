#include "drivers/base.h"
#include "drivers/gpio.h"
#include "util/stdlib.h"

/* All the magic here is described in Section 6 */

#define GPBASE (PBASE + 0x00200000)

#define FSEL_REG(x) (GPBASE + (x)*0x4)

int gpio_set_func(uint8_t pin, gpfunc_t f) {
  // Invalid pin
  if (pin > 53) {
    return 1;
  }

  // Get register
  uint64_t REG = FSEL_REG(pin / 10);
  uint32_t cur = get32(REG);
  // Each pin takes a 3bit func code
  int shift = 3 * (pin % 10);
  // Clear pin
  cur &= ~(0b111 << shift);
  // Set pin
  cur |= (f << shift);

  put32(REG, cur);

  return 0;
}

#define GPPUD (GPBASE + 0x94)
#define GPPUDCLK0 (GPBASE + 0x98)
#define GPPUDCLK1 (GPBASE + 0x9C)

int gpio_set_pull(uint8_t pin, gppull_t pull) {
  // Invalid pin
  if (pin > 53) {
    return 1;
  }

  // Get relevant clock register
  uint64_t CLOCK = pin < 32 ? GPPUDCLK0 : GPPUDCLK1;

  // Set pull
  put32(GPPUD, pull);
  delay(150);
  // Set clock
  put32(CLOCK, 0b1 << (pin % 32));
  delay(150);
  // Clear clock
  put32(CLOCK, 0);

  return 0;
}
