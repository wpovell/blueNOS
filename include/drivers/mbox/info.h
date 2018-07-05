#pragma once
#include <stdint.h>

typedef struct mbox_mem {
  uint32_t base;
  uint32_t size;
} mbox_mem_t;

uint32_t mbox_get_max_cpu_clock(void);
uint32_t mbox_get_cpu_clock(void);
uint32_t mbox_get_firmware(void);
uint32_t mbox_get_model(void);
uint32_t mbox_get_rev(void);
uint32_t mbox_get_serial(void);
mbox_mem_t mbox_get_mem(void);
mbox_mem_t mbox_get_vc_mem(void);
uint32_t mbox_get_temp(void);
uint32_t mbox_get_max_temp(void);

void mbox_print_info(void);
