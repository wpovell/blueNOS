#include "boot.h"
#include "shell.h"
#include "util.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <readline/history.h>
#include <readline/readline.h>

int dev, inp;

void atExit() { disableRawMode(); }

void repl(void) {
  while (1) {
    char *line = readline("> ");
    if (!line)
      break;
    if (*line)
      add_history(line);
    if (!strcmp(line, "help")) {
      printf("Available commands:\n");
      printf("  srv: Start bootloader server.\n");
      printf("  sh: Connect to remote shell.\n");
    } else if (!strcmp(line, "srv")) {
      if (!transfer()) {
        shell();
      }
    } else if (!strcmp(line, "sh")) {
      shell();
    } else if (strlen(line) > 0) {
      printf("Command not found.\n");
    }
  }
}

int main(int argc, char **argv) {
  atexit(atExit);
  char *inpf = "build/kernel/kernel8.img";
  char *devf = "/dev/ttyUSB0";
  if (argc > 2) {
    inpf = argv[1];
    devf = argv[2];
  } else if (argc > 1) {
    inpf = argv[1];
  }

  dev = open(devf, O_RDWR);
  inp = open(inpf, O_RDONLY);

  if (dev < 0) {
    printf("Failed to open device \"%s\".\n", devf);
  }
  if (inp < 0) {
    printf("Failed to open input \"%s\".\n", inpf);
  }
  if (inp < 0 || dev < 0) {
    return 1;
  }

  repl();
  close(dev);
  close(inp);
}
