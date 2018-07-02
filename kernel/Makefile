include ../make/common.mk
include ../make/arm.mk

BUILD_DIR = ../build/kernel

IMG_FILE = $(BUILD_DIR)/kernel8.img
ELF_FILE = $(BUILD_DIR)/kernel8.elf

C_FILES = $(shell find ./ -type f -name "*.c")
ASM_FILES = $(shell find ./ -type f -name "*.S")
OBJ_FILES = $(patsubst ./%.c, $(BUILD_DIR)/%.o, $(C_FILES))
OBJ_FILES += $(patsubst ./%.S, $(BUILD_DIR)/%.o, $(ASM_FILES))

.PHONY: all clean
all: $(IMG_FILE)

clean:
	rm -rf $(BUILD_DIR)

$(IMG_FILE): $(OBJ_FILES)
	$(LD) -T $(LD_SCRIPT) -o $(ELF_FILE)  $(OBJ_FILES)
	$(OBJCOPY) $(ELF_FILE) -O binary $(IMG_FILE)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S
	mkdir -p $(@D)
	$(CC) -c $< -o $@
