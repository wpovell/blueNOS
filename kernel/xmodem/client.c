#include "util/stdio.h"
#include "xmodem/client.h"
#include "xmodem/xmodem.h"

extern inline int checksum(uint8_t *buf);

int initiate_transfer(void) {
  uint8_t buf[PACKET_SIZE];
  uint8_t pac_num = 1;

  putc(NAK);
  if (getc() != SOH) {
    putc(CAN);
    return 1;
  }

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
        return 1;
      }
      putc(ACK);
      return 0;
    } else { // Error
      putc(CAN);
      return 1;
    }
    // <- Packet # Complement
    if (getc() != (255 - pac_num)) {
      putc(CAN);
      return 1;
    }

    // <- Packet
    for (int i = 0; i < PACKET_SIZE; i++) {
      buf[i] = getc();
    }

    // <- Checksum
    uint8_t csum = getc();
    if (checksum(buf) == csum) {
      putc(ACK);
    } else {
      putc(NAK);
    }
    pac_num++;
  }
}
