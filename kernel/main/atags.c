#include "main/atags.h"
#include "util/stdio.h"
#include "util/stdlib.h"

#define ATAG_START ((atag_t *)0x100)

/*
  Documentation:
  http://www.simtec.co.uk/products/SWLINUX/files/booting_article.html
*/

/*
Get amount of memory availale (from atags).
*/
uint32_t get_mem(void) {
  atag_t *tag;
  while ((tag = next_atag(ATAG_START))->header.type != ATAG_MEM)
    ;

  return tag->mem.size;
}

/*
  Return the next atag. Returns NULL if tag is NONE.
*/
atag_t *next_atag(atag_t *tag) {
  if (tag->header.type == ATAG_NONE) {
    return NULL;
  }

  return (atag_t *)(((uint32_t *)tag) + tag->header.size);
}

/*
  Pretty prints all ATAGs.
*/
void print_atags(void) {
  atag_t *cur = ATAG_START;
  while (cur) {
    switch (cur->header.type) {
    case ATAG_CORE:
      printf("CORE (%x)\n\tFlags: %d\n\tPagesize: %d\n\tDevice: %d\n",
             cur->header.type, cur->core.flags, cur->core.pagesize,
             cur->core.rootdev);
      break;
    case ATAG_MEM:
      printf("MEM (%x)\n\tMem: %x\n\tStart: %d\n", cur->header.type,
             cur->mem.size, cur->mem.start);
      break;
    case ATAG_CMD:
      printf("CMDLINE (%x)\n\t%s\n", cur->header.type, cur->cmd.cmdline);
      break;
    case ATAG_NONE:
      printf("NONE (%x)\n", cur->header.type);
      break;
    default:
      printf("UNKOWN HEADER (%x)\n", cur->header.type);
      break;
    }

    cur = next_atag(cur);
  }
}
