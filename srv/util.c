#include "util.h"
#include <unistd.h>

extern int inp, dev;

char gc(int fd) {
  char c = 0;
  read(fd, &c, 1);
  return c;
}
int pc(int fd, char c) { return write(fd, &c, 1); }
