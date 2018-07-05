#include "shell.h"
#include "util.h"
#include <pthread.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;
int rawMode;
void disableRawMode(void) {
  if (rawMode) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    rawMode = 0;
  }
}

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON | ISIG);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  rawMode = 1;
}

void *shell_read(void *_) {
  char c;
  while (1) {
    c = gc(dev);
    pc(STDOUT_FILENO, c);
  }
}

void *shell_write(void *_) {
  char c;
  while ((c = gc(STDIN_FILENO)) != 3) {
    pc(dev, c);
  }
}

void shell(void) {
  enableRawMode();

  pthread_t thr_read, thr_write;
  pthread_create(&thr_read, 0, shell_read, 0);
  pthread_create(&thr_write, 0, shell_write, 0);
  pthread_join(thr_write, 0);
  pthread_cancel(thr_read);

  disableRawMode();
}
