#include "drivers/mbox/framebuf.h"
#include "drivers/mbox/info.h"
#include "drivers/power.h"
#include "util/stdio.h"
#include <stdint.h>

// TODO: Copy over a full strings.h
int strlen(char *str) {
  int i = 0;
  for (; str[i]; i++)
    ;
  return i;
}

int streq(char *cmd, char *line) {
  for (int i = 0; i < strlen(cmd); i++) {
    if (cmd[i] != line[i]) {
      return 0;
    }
  }
  return 1;
}

void kshell() {
  char buf[250];
  while (1) {
    printf(">> ");
    gets(buf, 250);
    if (streq("reboot", buf)) {
      reboot();
    } else if (streq("info", buf)) {
      mbox_print_info();
    }
  }
}

uint32_t getEL() {
  uint64_t currentEL = 0;
  asm("mrs     %0, CurrentEL;" : "=r"(currentEL));
  return (currentEL) >> 2;
}

uint32_t width = 1024;
uint32_t height = 768;
void kernel_main(void) {
  framebuf_init(width, height);
  printf("Welcome to BluenOS!\n");
  printf("CurrentEL: %d\n", getEL());
  kshell();
}
