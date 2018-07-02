#pragma once
#include <stdint.h>

#define PACKET_SIZE 128

const uint8_t SOH = 1;
const uint8_t EOT = 4;
const uint8_t ACK = 6;
const uint8_t NAK = 21;
const uint8_t CAN = 24;

inline int checksum(uint8_t *buf) {
  uint32_t sum = 0;
  for (int i = 0; i < PACKET_SIZE; i++) {
    sum += buf[i];
  }
  return (uint8_t)(sum % 256);
}
