#pragma once
#include <stdint.h>
#include "xmodem/xmodem.h"

extern uint8_t xmodem_buffer[PACKET_SIZE];

int xmodem_init(void);
int xmodem_read(void);
