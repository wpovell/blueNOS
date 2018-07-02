#include "drivers/mbox/framebuf.h"
#include "drivers/mbox/mbox.h"
#include "util/debug.h"
#include "util/stdio.h"
#include <stddef.h>

#define BUF_ALLOC_TAG 0x00040001
#define BUF_SIZE_TAG 0x00048003
#define BUF_VIRT_SIZE_TAG 0x00048004
#define BUF_VIRT_OFF_TAG 0x00048009
#define BUF_DEPTH_TAG 0x00048005
#define BUF_PIXEL_ORD_TAG 0x00040006

#define BUF_BGR 0x0
#define BUF_RGB 0x1

pixel_t *buffer = NULL;
uint32_t buffer_size = 0;
uint32_t buffer_width;
uint32_t buffer_height;

int framebuf_init(uint32_t width, uint32_t height) {
  mbox[0] = 32 * 4;
  mbox[1] = REQ_CODE;

  // Size
  mbox[2] = BUF_SIZE_TAG;
  mbox[3] = 4 * 2;
  mbox[4] = REQ_CODE;
  mbox[5] = width;
  mbox[6] = height;

  // Virtual Size
  mbox[7] = BUF_VIRT_SIZE_TAG;
  mbox[8] = 4 * 2;
  mbox[9] = REQ_CODE;
  mbox[10] = width;
  mbox[11] = height;

  // Virtual Offset
  // NOTE: This seems like a reasonable default, but will not work without!
  mbox[12] = BUF_VIRT_OFF_TAG;
  mbox[13] = 4 * 2;
  mbox[14] = 0;
  mbox[15] = 0;
  mbox[16] = 0;

  // Depth
  // NOTE: Only 32bit seems to work
  mbox[17] = BUF_DEPTH_TAG;
  mbox[18] = 4 * 1;
  mbox[19] = REQ_CODE;
  mbox[20] = 32;

  // Pixel Order
  // NOTE: This doesn't seem effective, but removing breaks things.
  mbox[21] = BUF_PIXEL_ORD_TAG;
  mbox[22] = 4 * 1;
  mbox[23] = REQ_CODE;
  mbox[24] = 0x1; // RGB (doesn't do anything)

  // Alloc Framebuffer
  mbox[25] = BUF_ALLOC_TAG;
  mbox[26] = 4 * 2;
  mbox[27] = REQ_CODE;
  mbox[28] = 4096; // Align to 4KB block
  mbox[29] = 0x0;

  mbox[30] = END_TAG;
  mbox[31] = PADDING;

  if (mbox_call(PROP_CHANNEL)) {
    return 1;
  }

  buffer = (pixel_t *)(uint64_t)(mbox[28] & 0x3FFFFFFF);
  buffer_size = mbox[29] / sizeof(pixel_t);
  buffer_width = width;
  buffer_height = height;
  return 0;
}
