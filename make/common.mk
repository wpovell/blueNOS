# Removed -mgeneral-regs-only to allow for floating point, may need to add back
# in future if context switcing proves difficult
CFLAGS = -Wall -Wextra -Werror
CFLAGS += -O0 -g
# CFLAGS = $(CFLAGS) -O3

LD_SCRIPT = linker.ld
CROSS = aarch64-linux-gnu
