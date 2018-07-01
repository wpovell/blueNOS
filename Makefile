TOOLCHAIN = aarch64-linux-gnu
CC = $(TOOLCHAIN)-gcc
LD = $(TOOLCHAIN)-ld
OBJCOPY = $(TOOLCHAIN)-objcopy
GDB = $(TOOLCHAIN)-gdb

CFLAGS = -Wall -Wextra -Werror
CFLAGS += -nostdlib -nostartfiles -ffreestanding -Iinclude

DBG_CFLAGS = $(CFLAGS) -O0 -g
OPT_CFLAGS = $(CFLAGS) -O3

# Removed -mgeneral-regs-only to allow for floating point, may need to add back
# in future if context switcing proves difficult

BUILD_DIR = build
SRC_DIR = src

IMG_NAME  = kernel8
IMG_FILE  = $(BUILD_DIR)/$(IMG_NAME).img
ELF_FILE  = $(BUILD_DIR)/$(IMG_NAME).elf
LD_SCRIPT = $(SRC_DIR)/linker.ld

QEMU = qemu-system-aarch64
QEMU_FLAGS = -M raspi3 -kernel $(IMG_FILE) -serial null -serial stdio

C_FILES   = $(shell find src/ -type f -name "*.c")
ASM_FILES = $(shell find src/ -type f -name "*.S")
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_FILES))
OBJ_FILES += $(patsubst $(SRC_DIR)/%.S, $(BUILD_DIR)/%.o, $(ASM_FILES))

all : $(IMG_FILE)

clean :
	rm -rf $(BUILD_DIR) *.img

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(DBG_CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	mkdir -p $(@D)
	$(CC) -c $< -o $@

$(IMG_FILE): $(LD_SCRIPT) $(OBJ_FILES)
	$(LD) -T $(LD_SCRIPT) -o $(ELF_FILE)  $(OBJ_FILES)
	$(OBJCOPY) $(ELF_FILE) -O binary $(IMG_FILE)

run: $(IMG_FILE)
	$(QEMU) $(QEMU_FLAGS)

dbg: $(IMG_FILE)
	$(QEMU) $(QEMU_FLAGS) -S -s &
	$(GDB) $(ELF_FILE)
	killall $(QEMU)

# TODO: Make install / install_dbg targets
