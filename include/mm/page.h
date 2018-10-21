#pragma once

#define PAGE_SHIFT         12
#define PAGE_SIZE          ((uint32_t)(1UL<<PAGE_SHIFT))
#define PAGE_MASK          (0xffffffff<<PAGE_SHIFT)

#define PAGE_ALIGN_DOWN(x) ((void*)(((uintptr_t)(x))&PAGE_MASK))
#define PAGE_ALIGN_UP(x)   ((void*)(((((uintptr_t)(x))-1)&PAGE_MASK)+PAGE_SIZE))
#define PAGE_OFFSET(x)     ((uintptr_t)(x)&~PAGE_MASK)

#define PN_TO_ADDR(x) ((void *)(((uint32_t)(x)) << PAGE_SHIFT))
#define ADDR_TO_PN(x) (((uint32_t)(x)) >> PAGE_SHIFT)

#define PAGE_ALIGNED(x) (0 == ((uintptr_t)(x)) % PAGE_SIZE)

#define PAGE_NSIZES  8

#define PAGE_SAME(addr1, addr2) (PAGE_ALIGN_DOWN(addr1) == PAGE_ALIGN_DOWN(addr2))
