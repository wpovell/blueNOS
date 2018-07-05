#include "boot.h"
#include "util.h"
#include "xmodem/xmodem.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

extern inline int checksum(uint8_t *buf);

int packet_num;
uint8_t pac_num(void) { return (uint8_t)(packet_num % 256); }
uint8_t pac_cmp(void) { return 255 - pac_num(); }

int cancel;
// sighandler_t old;

void cancelHandler(int sig) {
  cancel = 1;
  printf("\n");
}

int transfer(void) {
  uint8_t res;
  lseek(inp, 0, SEEK_SET);
  printf("Waiting for NAK...\n");

  // Setup signal handler to cancel wait for NAK
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = cancelHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  // Time to wait!
  cancel = 0;
  fd_set read_fds, write_fds, except_fds;
  struct timeval timeout;
  FD_ZERO(&read_fds);
  FD_ZERO(&write_fds);
  FD_ZERO(&except_fds);
  FD_SET(dev, &read_fds);
  while (!cancel) {
    // Setup timeout for device read
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    if (select(dev + 1, &read_fds, &write_fds, &except_fds, &timeout) == 1) {
      res = gc(dev);
      if (res != NAK) {
        printf("Bad response (%x)\n", res);
      } else {
        break;
      }
    }
  }

  signal(SIGINT, SIG_DFL);
  if (cancel) {
    return 1;
  }
  printf("NAK recieved, sending SOH and starting transfer\n");

  // Start transfer
  pc(dev, SOH);

  // Read off trailing NAKs
  while (select(dev + 1, &read_fds, &write_fds, &except_fds, &timeout)) {
    gc(dev);
  }

  uint8_t buf[PACKET_SIZE];
  int rd;
  packet_num = 1;
  do {
    rd = read(inp, buf, PACKET_SIZE);
    // Pad buffer
    if (rd < PACKET_SIZE) {
      memset(buf + rd, 0, PACKET_SIZE - rd);
    }
    do {
      // -> Packet #
      pc(dev, pac_num());
      // -> Packet # Complement
      pc(dev, pac_cmp());
      // -> Packet
      write(dev, buf, PACKET_SIZE);
      // -> Packet checksum
      pc(dev, checksum(buf));
      // <- Response
      res = gc(dev);
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
  } while (rd == PACKET_SIZE);
  printf("Sending EOT\n");
  pc(dev, EOT);
  printf("Waiting for NAK...\n");
  if ((res = gc(dev)) != NAK) {
    printf("Bad response (%d), aborting.\n", res);
    return 1;
  }
  printf("Sending 2nd EOT\n");
  pc(dev, EOT);
  printf("Waiting for ACK...\n");
  if ((res = gc(dev)) != ACK) {
    printf("Bad response (%d), aborting.\n", res);
    return 1;
  }
  printf("ACK recieved, terminating connection.\n");
  return 0;
}
