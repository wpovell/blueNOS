#pragma once
#include <stdint.h>

char *itoa(int num, int base);

// Defined in ASM (utils.S) for simplicity
extern void delay(uint64_t);
extern void put32(uint64_t, uint32_t);
extern unsigned int get32(uint64_t);
