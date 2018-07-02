#pragma once
#include <stdint.h>

#define PACKET_SIZE 128

#define SOH 1
#define EOT 4
#define ACK 6
#define NAK 21
#define CAN 24

inline int checksum(uint8_t *buf) {
  uint32_t sum = 0;
  for (int i = 0; i < PACKET_SIZE; i++) {
    sum += buf[i];
  }
  return (uint8_t)(sum % 256);
}
