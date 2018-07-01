#include "main/atags.h"
#include "util/stdio.h"
#include "util/stdlib.h"
#include <stdint.h>

/*
  Documentation:
  http://www.simtec.co.uk/products/SWLINUX/files/booting_article.html
  TODO: Currently this only finds a blank CORE tag on my pi, not sure why.
*/

#define CORE 0x54410001
#define NONE 0x00000000
#define MEM 0x54410002
#define CMDLINE 0x54410009

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

#define ATAG_START ((atag_t *)0x100)
void print_atags(void) {
  atag_t t;
  t.header.type = 1;

  atag_t *cur = ATAG_START;
  while (t.header.type != NONE) {
    memcpy(&t, cur, sizeof(atag_t));
    switch (t.header.type) {
    case CORE:
      printf("CORE (%x)\n\tFlags: %d\n\tPagesize: %d\n\tDevice: %d\n",
             t.header.type, t.core.flags, t.core.pagesize, t.core.rootdev);
      break;
    case MEM:
      printf("MEM (%x)\n\tMEM: %d\n\tStart: %d\n", t.header.type, t.mem.size,
             t.mem.start);
      break;
    case CMDLINE:
      printf("CMDLINE (%x)\n\t%s\n", t.header.type, t.cmd.cmdline);
      break;
    case NONE:
      printf("NONE (%x)\n", t.header.type);
      break;
    default:
      printf("UNKOWN HEADER (%x)\n", t.header.type);
      break;
    }
    cur += t.header.size * 4;
  }
}
