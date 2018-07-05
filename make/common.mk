# Removed -mgeneral-regs-only to allow for floating point, may need to add back
# in future if context switcing proves difficult
CFLAGS = -Wall -Wextra -Werror

ifeq ($(OPT), true)
	CFLAGS += -O3
else
	CFLAGS += -O0 -g
endif

LD_SCRIPT = linker.ld
CROSS = aarch64-linux-gnu
