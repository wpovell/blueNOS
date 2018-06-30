#pragma once
#include <stdint.h>

void memcpy(void *dest, const void *src, int bytes);
void bzero(void *dest, int bytes);
char *itoa(int num, int base);
int atoi(char *num);

// Defined in ASM (utils.S) for simplicity
extern void delay(uint64_t);
extern void put32(uint64_t, uint32_t);
extern unsigned int get32(uint64_t);
