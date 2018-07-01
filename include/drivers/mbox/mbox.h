#pragma once
#include <stdint.h>

#define REQ_CODE 0x0
#define RES_CODE 0x80000000
#define ERR_CODE 0x80000001
#define PROP_CHANNEL 8
#define END_TAG 0x0
#define PADDING 0x0

extern volatile uint32_t mbox[36];

int mbox_call(uint8_t channel);
