# Toolchain vars
CROSS = aarch64-linux-gnu
CC = $(CROSS)-gcc
LD = $(CROSS)-ld
OBJCOPY = $(CROSS)-objcopy

LD_SCRIPT = linker.ld

# @NOTE: Removed -mgeneral-regs-only to allow for floating point, may need to add back
#        in future if context switcing proves difficult
CFLAGS = -Wall -Wextra -Werror
CFLAGS += -nostdlib -nostartfiles -ffreestanding -I../include

# Set optimization level
ifeq ($(OPT), true)
	CFLAGS += -O3
else
	CFLAGS += -O0 -g
endif
