#include "util/stdio.h"
#include "xmodem/client.h"

extern inline int checksum(uint8_t *buf);

uint8_t xmodem_buffer[PACKET_SIZE];
uint8_t pac_num = 1;

int xmodem_init(void) {
  pac_num = 1;
  putc(NAK);
  if (getc() != SOH) {
    putc(CAN);
    return 1;
  }

  return 0;
}

int xmodem_read(void) {
  while (1) {
    // <- Packet #
    uint8_t res = getc();
    // TODO: What if pac_num == EOT?
    if (res == pac_num) {
      // Do nothing
    } else if (res == EOT) { // EOT
      putc(NAK);
      if (getc() != EOT) {
        putc(CAN);
        return -1;
      }
      putc(ACK);
      return 1;
    } else { // Error
      putc(CAN);
      return -1;
    }
    // <- Packet # Complement
    if (getc() != (255 - pac_num)) {
      putc(CAN);
      return -1;
    }

    // <- Packet
    for (int i = 0; i < PACKET_SIZE; i++) {
      xmodem_buffer[i] = getc();
    }

    // <- Checksum
    uint8_t csum = getc();
    if (checksum(xmodem_buffer) == csum) {
      putc(ACK);
    } else {
      putc(NAK);
      continue;
    }
    pac_num++;
    return 0;
  }
}
