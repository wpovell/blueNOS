#pragma once
#include <stdint.h>

extern uint32_t mbox[36];

void mbox_call(uint32_t channel);
