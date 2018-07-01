#include "drivers/mbox/info.h"
#include "drivers/mbox/mbox.h"
#include "util/debug.h"
#include "util/stdio.h"

#define FIRMWARE_TAG 0x00000001
#define BOARD_MODEL_TAG 0x00010001
#define BOARD_REV_TAG 0x00010002
#define MAC_TAG 0x00010003
#define SERIAL_TAG 0x00010004

#define MEM_TAG 0x00010005
#define VC_MEM_TAG 0x00010006

#define CMDLINE_TAG 0x00050001

#define TEMP_TAG 0x00030006
#define MAX_TEMP_TAG 0x0003000a

uint32_t *set_msg(uint32_t tag, uint8_t bufsize) {
  uint8_t size = 6 + bufsize;
  if (size % 4 != 0) {
    size = 4 * (1 + (size / 4));
  }
  mbox[0] = size * 4;
  mbox[1] = REQ_CODE;
  mbox[2] = tag;
  mbox[3] = bufsize * 4;
  mbox[4] = REQ_CODE;

  // Buffer
  for (int i = 0; i < bufsize; i++) {
    mbox[5 + i] = 0x0;
  }

  mbox[5 + bufsize] = END_TAG;

  // Padding
  for (int i = 6 + bufsize; i < size; i++) {
    mbox[i] = PADDING;
  }

  return (uint32_t *)&mbox[5];
}

uint32_t mbox_get_firmware(void) {
  uint32_t *buf = set_msg(FIRMWARE_TAG, 4);
  mbox_call(PROP_CHANNEL);
  return buf[0];
}

uint32_t mbox_get_model(void) {
  uint32_t *buf = set_msg(BOARD_MODEL_TAG, 4);
  mbox_call(PROP_CHANNEL);
  return buf[0];
}

uint32_t mbox_get_rev(void) {
  uint32_t *buf = set_msg(BOARD_REV_TAG, 4);
  mbox_call(PROP_CHANNEL);
  return buf[0];
}

uint32_t mbox_get_serial(void) {
  uint32_t *buf = set_msg(SERIAL_TAG, 8);
  mbox_call(PROP_CHANNEL);
  return ((uint64_t)buf[0]) + (((uint64_t)buf[1]) << 32);
}

mbox_mem_t mbox_get_mem(void) {
  uint32_t *buf = set_msg(MEM_TAG, 8);
  mbox_call(PROP_CHANNEL);
  // NOTE: Casting buf -> (mbox_mem_t *) works on QEMU but not on hardware
  mbox_mem_t ret = {buf[0], buf[1]};
  return ret;
}

mbox_mem_t mbox_get_vc_mem(void) {
  uint32_t *buf = set_msg(VC_MEM_TAG, 8);
  mbox_call(PROP_CHANNEL);
  mbox_mem_t ret = {buf[0], buf[1]};
  return ret;
}

uint32_t mbox_get_temp(void) {
  uint32_t *buf = set_msg(TEMP_TAG, 8);
  mbox_call(PROP_CHANNEL);
  return buf[1];
}

uint32_t mbox_get_max_temp(void) {
  uint32_t *buf = set_msg(MAX_TEMP_TAG, 8);
  mbox_call(PROP_CHANNEL);
  return buf[1];
}

// TODO: Mac addr & cmdline

#define C_TO_F(temp) (((temp)*1.8) + 32)

void mbox_print_info(void) {
  mbox_mem_t mem = mbox_get_mem();
  mbox_mem_t vc_mem = mbox_get_vc_mem();
  printf("INFO\n"
         "\tFirmware: %d\n"
         "\tModel: %d\n"
         "\tRevision: %d\n"
         "\tSerial: %d\n"
         "\tMem:\n"
         "\t\tBase: %x\n"
         "\t\tSize: %x\n"
         "\tVC Mem:\n"
         "\t\tBase: %x\n"
         "\t\tSize: %x\n"
         "\tTemp: %d\n"
         "\tMax Temp: %d\n",
         mbox_get_firmware(), mbox_get_model(), mbox_get_rev(),
         mbox_get_serial(), mem.base, mem.size, vc_mem.base, vc_mem.size,
         (uint32_t)C_TO_F(mbox_get_temp() / 1000),
         (uint32_t)C_TO_F(mbox_get_max_temp() / 1000));
}
