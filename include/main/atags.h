#pragma once
#include <stdint.h>

#define ATAG_CORE 0x54410001
#define ATAG_NONE 0x00000000
#define ATAG_MEM 0x54410002
#define ATAG_CMD 0x54410009

typedef struct atag_header {
  uint32_t size;
  uint32_t type;
} atag_header_t;

typedef struct atag_mem {
  uint32_t size;  // Amount of memory
  uint32_t start; // Physical start of memory
} atag_mem_t;

typedef struct atag_core {
  uint32_t flags; // bit = 0 specifies read only (?)
  uint32_t pagesize;
  uint32_t rootdev;
} atag_core_t;

typedef struct atag_cmd {
  char cmdline[1];
} atag_cmd_t;

typedef struct atag {
  atag_header_t header;
  union {
    atag_mem_t mem;
    atag_core_t core;
    atag_cmd_t cmd;
  };
} atag_t;

uint32_t get_mem(void);
atag_t *next_atag(atag_t *tag);
void print_atags(void);
