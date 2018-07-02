#include "server.h"
#include "xmodem/xmodem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern inline int checksum(uint8_t *buf);

int packet_num = 1;
uint8_t pac_num(void) { return (uint8_t)(packet_num % 256); }
uint8_t pac_cmp(void) { return 255 - pac_num(); }

int transfer(FILE *dev, FILE *inp) {
  uint8_t res;

  printf("Waiting for NAK...\n");
  // Wait for NAK
  if ((res = getc(dev)) != NAK) {
    printf("Bad response (%x), aborting\n", res);
    return 1;
  }
  printf("NAK recieved, sending SOH and starting transfer\n");
  // Start transfer
  putc(SOH, dev);

  uint8_t buf[PACKET_SIZE];
  int read;
  do {
    read = fread(buf, 1, PACKET_SIZE, inp);
    // Pad buffer
    if (read < PACKET_SIZE) {
      memset(buf + read, 0, PACKET_SIZE - read);
    }
    do {
      // -> Packet #
      putc(pac_num(), dev);
      // -> Packet # Complement
      putc(pac_cmp(), dev);
      // -> Packet
      fwrite(buf, 1, PACKET_SIZE, dev);
      // -> Packet checksum
      putc(checksum(buf), dev);
      // <- Response
      res = getc(dev);
      if (res == NAK) { // Retry
        printf("%d: Failed, retrying\n", packet_num);
      } else if (res == ACK) { // Next
        printf("%d: Recieved, proceeding\n", packet_num);
      } else { // Error
        printf("Bad response (%d), aborting.\n", res);
        return 1;
      }
      packet_num++;
    } while (res != ACK);
  } while (read == PACKET_SIZE);
  printf("Sending EOT\n");
  putc(EOT, dev);
  printf("Waiting for NAK...\n");
  if ((res = getc(dev)) != NAK) {
    printf("Bad response (%d), aborting.\n", res);
    return 1;
  }
  printf("Sending 2nd EOT\n");
  putc(EOT, dev);
  printf("Waiting for ACK...\n");
  if ((res = getc(dev)) != ACK) {
    printf("Bad response (%d), aborting.\n", res);
    return 1;
  }
  printf("ACK recieved, terminating connection.\n");
  return 0;
}

int main(int argc, char **argv) {
  FILE *dev, *inp;
  if (argc > 2) {
    dev = fopen(argv[1], "r+");
    inp = fopen(argv[2], "r");
  } else {
    printf("transfer <device> <data>\n");
    return 1;
  }

  if (!dev || !inp) {
    printf("Failed to open file.\n");
    return 1;
  }

  transfer(dev, inp);
  char c;
  while ((c = getc(dev)) != EOF) {
    putc(c, stdout);
  }
}
