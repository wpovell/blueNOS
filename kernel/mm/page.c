#include "mm/page.h"
#include "util/debug.h"
#include "util/list.h"
#include "util/stdlib.h"
#include <stddef.h>
#include <stdint.h>

static list_t pagegroup_list;
static uintptr_t page_freecount;

struct pagegroup {
  // Freelist for each order
  list_t pg_freelist[PAGE_NSIZES];
  // Map order -> buddy bitmap
  void *pg_map[PAGE_NSIZES];
  uintptr_t pg_baseaddr;
  uintptr_t pg_endaddr;
  list_link_t pg_link;
};

struct freepage {
  list_link_t fp_link;
};

/*
  Actually create the pagegroup for a block of memory
  Sets up metadata + buddy bitmaps for tracking the state of memory.
  @TODO: Comment/understand this more thoroughly
*/
static struct pagegroup *_pagegroup_create(uintptr_t start, uintptr_t end) {
  KASSERT(PAGE_NSIZES > 0);
  KASSERT(sizeof(struct pagegroup) <= PAGE_SIZE);

  uintptr_t npages = (end - start) >> PAGE_SHIFT;
  struct pagegroup *group;

  end -= sizeof(*group);
  group = (struct pagegroup *)end;

  group->pg_baseaddr = start;
  group->pg_map[0] = NULL;

  /* allocate some of the space for the buddy bit maps,
   * we allocate enough bits to track all pages even
   * though some pages will be unavailable since they
   * are being used as bitmaps */
  int order;
  for (order = 1; order < PAGE_NSIZES; ++order) {
    uintptr_t count = npages >> order;
    count = ((count - 1) & ~((uintptr_t)0x7)) + 8;
    count = count >> 3;
    end -= count;
    group->pg_map[order] = (void *)end;
    memset(group->pg_map[order], 0, count);
  }

  /* discard the remainder of the page being used for
   * mappings and read just npages */
  end = (uintptr_t)PAGE_ALIGN_DOWN(end);
  npages = (end - start) >> PAGE_SHIFT;
  group->pg_endaddr = end;

  /* put pages which do not fit nicely into the largest
   * order and add them to smaller buckets */
  for (order = 0; order < PAGE_NSIZES - 1; ++order) {
    list_init(&group->pg_freelist[order]);
    if (npages & (1 << order)) {
      end -= (1 << order) << PAGE_SHIFT;
      list_insert_head(&group->pg_freelist[order],
                       &((struct freepage *)end)->fp_link);
    }
  }

  /* put the remaining pages into the largest bucket */
  KASSERT(0 == (end - start) % (1 << order));
  list_init(&group->pg_freelist[order]);
  uintptr_t current = start;
  while (current < end) {
    list_insert_head(&group->pg_freelist[order],
                     &((struct freepage *)current)->fp_link);
    current += (1 << order) << PAGE_SHIFT;
  }

  return group;
}

// The number of free pages in the kmem system
uint32_t page_free_count() { return page_freecount; }

// Initalize page system. Most of the work happens later in page_add_range
void page_init() {
  list_init(&pagegroup_list);
  page_freecount = 0;
}

/*
  Add range to page system, after which it can be used for dynamic
  allocation
*/
void page_add_range(uintptr_t start, uintptr_t end) {
  // Page align the start and end
  start = (uintptr_t)PAGE_ALIGN_DOWN(start);
  end = (uintptr_t)PAGE_ALIGN_DOWN(end);

  struct pagegroup *group = _pagegroup_create(start, end);
  // @Q: Why is this check necessary?
  if (group->pg_baseaddr < group->pg_endaddr) {
    list_insert_tail(&pagegroup_list, &group->pg_link);
    page_freecount += ADDR_TO_PN(group->pg_endaddr - group->pg_baseaddr);
  }
}
