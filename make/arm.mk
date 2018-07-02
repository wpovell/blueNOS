CC = $(CROSS)-gcc
LD = $(CROSS)-ld
OBJCOPY = $(CROSS)-objcopy

CFLAGS += -nostdlib -nostartfiles -ffreestanding -I../include
