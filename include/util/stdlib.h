#pragma once
#include <stddef.h>
#include <stdint.h>

char *itoa(int num, int base);
void *memcpy(void *dest, const void *src, size_t len);
void *memset(void *dest, int c, size_t n);

// Defined in ASM (utils.S) for simplicity
extern void delay(uint64_t);
extern void put32(uint64_t, uint32_t);
extern unsigned int get32(uint64_t);
