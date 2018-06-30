#include "drivers/peripherals/gpio.h"
#include "drivers/peripherals/uart.h"
#include "util/stdlib.h"
#include <stdarg.h>

// TODO: Better understand this
void uart_init(void) {
  uint32_t selector;

  selector = get32(GPFSEL1);
  selector &= ~(7 << 12); // clean gpio14
  selector |= 2 << 12;    // set alt5 for gpio14
  selector &= ~(7 << 15); // clean gpio15
  selector |= 2 << 15;    // set alt5 for gpio15
  put32(GPFSEL1, selector);

  put32(GPPUD, 0);
  delay(150);
  put32(GPPUDCLK0, (1 << 14) | (1 << 15));
  delay(150);
  put32(GPPUDCLK0, 0);

  // Enable mini uart (this also enables access to it registers)
  put32(AUX_ENABLES, 1);
  put32(AUX_MU_CNTL_REG, 0);   // Disable auto flow control and disable receiver
                               // and transmitter (for now)
  put32(AUX_MU_IER_REG, 0);    // Disable receive and transmit interrupts
  put32(AUX_MU_LCR_REG, 3);    // Enable 8 bit mode
  put32(AUX_MU_MCR_REG, 0);    // Set RTS line to be always high
  put32(AUX_MU_BAUD_REG, 270); // Set baud rate to 115200

  put32(AUX_MU_CNTL_REG, 3); // Finally, enable transmitter and receiver
}

void uart_putc(char c) {
  while (1) {
    if (get32(AUX_MU_LSR_REG) & 0x20)
      break;
  }
  put32(AUX_MU_IO_REG, c);
}

char uart_getc(void) {
  while (1) {
    if (get32(AUX_MU_LSR_REG) & 0x01)
      break;
  }
  return (get32(AUX_MU_IO_REG) & 0xFF);
}

void uart_puts(char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    uart_putc((char)str[i]);
  }
}

void uart_gets(char *buf, int buflen) {
  int i;
  char c;
  // Leave a spot for null char in buffer
  for (i = 0; (c = uart_getc()) != '\r' && buflen > 1; i++, buflen--) {
    uart_putc(c);
    buf[i] = c;
  }

  uart_putc('\n');
  if (c == '\n') {
    buf[i] = '\0';
  } else
    buf[buflen - 1] = '\0';
}

void uart_printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  for (; *fmt != '\0'; fmt++) {
    if (*fmt == '%') {
      switch (*(++fmt)) {
      case '%':
        uart_putc('%');
        break;
      case 'd':
        uart_puts(itoa(va_arg(args, int), 10));
        break;
      case 'x':
        uart_puts(itoa(va_arg(args, int), 16));
        break;
      case 's':
        uart_puts(va_arg(args, char *));
        break;
      }
    } else
      uart_putc(*fmt);
  }

  va_end(args);
}
