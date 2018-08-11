#pragma once
#include <stdint.h>

// Size of transfer packets
// @NOTE: This could be increased to possibly improve transfer speed
#define PACKET_SIZE 128

// Xmodem protocol symbols
// @TODO: Make enum
#define SOH 1
#define EOT 4
#define ACK 6
#define NAK 21
#define CAN 24

/*
  Calculate checksum for buffer
  @NOTE: This is also used by /srv code
*/
inline int checksum(uint8_t *buf) {
  uint32_t sum = 0;
  for (int i = 0; i < PACKET_SIZE; i++) {
    sum += buf[i];
  }
  return (uint8_t)(sum % 256);
}
