#include "drivers/base.h"
#include "drivers/gpio.h"
#include "util/stdlib.h"
#include <stdarg.h>

/* All the magic here is described in Section 2 */

#define AUX_BASE (PBASE + 0x00215000)

#define AUX_ENB (AUX_BASE + 0x04)         // Enable
#define AUX_MU_IO_REG (AUX_BASE + 0x40)   // IO
#define AUX_MU_IER_REG (AUX_BASE + 0x44)  // Interrupt
#define AUX_MU_IIR_REG (AUX_BASE + 0x48)  // FIFO
#define AUX_MU_LCR_REG (AUX_BASE + 0x4C)  // Data Format
#define AUX_MU_MCR_REG (AUX_BASE + 0x50)  // RTS
#define AUX_MU_LSR_REG (AUX_BASE + 0x54)  // Data Status
#define AUX_MU_CNTL_REG (AUX_BASE + 0x60) // Control trans/recv
#define AUX_MU_BAUD_REG (AUX_BASE + 0x68) // Baudrate

void uart_init(void) {
  gpio_set_func(14, GPF_F5);
  gpio_set_func(15, GPF_F5);

  gpio_set_pull(14, GPP_NONE);
  gpio_set_pull(15, GPP_NONE);

  put32(AUX_ENB, 1);           // Enable UART
  put32(AUX_MU_CNTL_REG, 0);   // Disable flow control + transmit + recieve
  put32(AUX_MU_IER_REG, 0);    // Disable interrupts
  put32(AUX_MU_LCR_REG, 0b11); // Set data to be 8bit
  put32(AUX_MU_MCR_REG, 0);    // Set RTS to be high (not relevant, used for FC)

  /*
  baudrate = 115200
  sys_clock_freq = 250MHz
  baudrate = (sys_clock_freq) / (8 * baudrate_reg + 1)
  baudrate_reg = (sys_clock_freq) / (8 * baudrate) - 1
  baudrate_reg = 270
  */
  put32(AUX_MU_BAUD_REG, 270);

  put32(AUX_MU_CNTL_REG, 0b11); // Enable transmit + recieve
}

void uart_putc(char c) {
  // Idle while still transmitting
  while (!(get32(AUX_MU_LSR_REG) & (1 << 6)))
    ;

  put32(AUX_MU_IO_REG, c);
}

char uart_getc_timeout(int timeout) {
  // Idle while waiting for data
  uint32_t res;
  for (int i = 0; i < timeout && !(res = (get32(AUX_MU_LSR_REG) & 1)); i++)
    ;

  if (!res) {
    return 0;
  } else {
    return (get32(AUX_MU_IO_REG) & 0xFF);
  }
}

char uart_getc(void) {
  // Idle while waiting for data
  while (!(get32(AUX_MU_LSR_REG) & 1))
    ;

  char c = get32(AUX_MU_IO_REG) & 0xFF;
#ifndef __BOOTLOADER__
  if (c == 13) {
    return 10;
  }
#endif
  return c;
}
