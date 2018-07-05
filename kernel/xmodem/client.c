#include "drivers/uart.h"
#include "xmodem/client.h"

extern inline int checksum(uint8_t *buf);

uint8_t xmodem_buffer[PACKET_SIZE];
uint8_t pac_num = 1;

int xmodem_init(void) {
  char c = 0;
  do {
    uart_putc(NAK);
  } while (!(c = uart_getc_timeout(100000)));

  if (c != SOH) {
    uart_putc(CAN);
    return 1;
  }

  pac_num = 1;
  return 0;
}

int xmodem_read(void) {
  while (1) {
    // <- Packet #
    uint8_t res = uart_getc();
    // TODO: What if pac_num == EOT?
    if (res == pac_num) {
      // Do nothing
    } else if (res == EOT) { // EOT
      uart_putc(NAK);
      if (uart_getc() != EOT) {
        uart_putc(CAN);
        return -1;
      }
      uart_putc(ACK);
      return 1;
    } else { // Error
      uart_putc(CAN);
      return -1;
    }
    // <- Packet # Complement
    if (uart_getc() != (255 - pac_num)) {
      uart_putc(CAN);
      return -1;
    }

    // <- Packet
    for (int i = 0; i < PACKET_SIZE; i++) {
      xmodem_buffer[i] = uart_getc();
    }

    // <- Checksum
    uint8_t csum = uart_getc();
    if (checksum(xmodem_buffer) == csum) {
      uart_putc(ACK);
    } else {
      uart_putc(NAK);
      continue;
    }
    pac_num++;
    return 0;
  }
}
