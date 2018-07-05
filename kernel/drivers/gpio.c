#include "drivers/base.h"
#include "drivers/gpio.h"
#include "util/stdlib.h"

/* All the magic here is described in Section 6 */
// QUESTION: What's the point of GPIO pins above 27?

#define GPBASE (PBASE + 0x00200000)

#define FSEL_REG(x) (GPBASE + (x)*0x4)

/*
  Set GPIO behavior, either input, output, or an alterative function.
*/
int gpio_set_func(uint8_t pin, gpfunc_t f) {
  // Invalid pin
  if (pin < 2 || pin > 27) {
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
#define GPPUDCLK1 (GPBASE + 0x9C) // Not used

int gpio_set_pull(uint8_t pin, gppull_t pull) {
  // Invalid pin
  if (pin < 2 || pin > 27) {
    return 1;
  }

  // Set pull
  put32(GPPUD, pull);
  delay(150);
  // Set clock
  put32(GPPUDCLK0, 1 << pin);
  delay(150);
  // Clear clock
  put32(GPPUD, 0);
  put32(GPPUDCLK0, 0);

  return 0;
}

#define GPSET0 (GPBASE + 0x1C)
#define GPSET1 (GPBASE + 0x20) // Not used
#define GPCLR0 (GPBASE + 0x28)
#define GPCLR1 (GPBASE + 0x2C) // Not used

int gpio_set(uint8_t pin, int val) {
  // Invalid pin
  if (pin < 2 || pin > 27) {
    return 1;
  }

  uint64_t REG = val ? GPSET0 : GPCLR0;
  put32(REG, 1 << pin);

  return 0;
}
