#pragma once
#include <stdint.h>

uint64_t get_systime(void);
void wait_usec(uint64_t time);
void wait_msec(uint32_t time);
void wait_sec(uint32_t time);
