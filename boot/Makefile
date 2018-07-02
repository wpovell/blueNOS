include ../make/common.mk
include ../make/arm.mk

BUILD_DIR = ../build/boot

IMG_FILE = $(BUILD_DIR)/kernel8.img
ELF_FILE = $(BUILD_DIR)/kernel8.elf
KERN_DIR = ../kernel

C_FILES += $(KERN_DIR)/drivers/uart.c $(KERN_DIR)/drivers/gpio.c
C_FILES += $(KERN_DIR)/xmodem/client.c
C_FILES += $(KERN_DIR)/util/stdlib.c $(KERN_DIR)/util/stdio.c
ASM_FILES = $(KERN_DIR)/main/boot.S $(KERN_DIR)/util/utils.S

OBJ_FILES = $(patsubst $(KERN_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_FILES))
OBJ_FILES += $(patsubst $(KERN_DIR)/%.S, $(BUILD_DIR)/%.o, $(ASM_FILES))

.PHONY: clean all
clean:
	rm -rf $(BUILD_DIR)

all: $(IMG_FILE)

$(BUILD_DIR)/main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(KERN_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(KERN_DIR)/%.S
	mkdir -p $(@D)
	$(CC) -c $< -o $@

$(IMG_FILE): $(OBJ_FILES) $(BUILD_DIR)/main.o
	$(LD) -T $(LD_SCRIPT) -o $(ELF_FILE)  $(OBJ_FILES) $(BUILD_DIR)/main.o
	$(OBJCOPY) $(ELF_FILE) -O binary $(IMG_FILE)
