CROSS = aarch64-none-elf
RUST_FILES = $(shell find src/ -type f -name '*.rs')
LD_SCRIPT = ext/layout.ld
BOOT = ext/boot.S
BUILD_DIR = build

# Output files
LIB = $(BUILD_DIR)/kernel.a
ELF = $(BUILD_DIR)/kernel.elf
IMG = $(BUILD_DIR)/kernel8.img

# QEMU flags
QEMU = qemu-system-aarch64
QEMU_FLAGS = -M raspi3 -kernel
GDB = aarch64-linux-gnu-gdb

.PHONY: all clean qemu dbg doc

all: $(IMG)

# Boot assembly
$(BUILD_DIR)/boot.o: ext/boot.S
	$(CROSS)-gcc -c  $< -o $@

# Rust build
$(LIB): $(RUST_FILES) Cargo.toml $(CROSS).json
	cargo fmt
	xargo build --target=$(CROSS)
	cp target/$(CROSS)/debug/libkernel.a $(LIB)

# Linking
$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/boot.o $(LIB)
	$(CROSS)-ld -o $@ $^ -T$(LD_SCRIPT)

# Link -> Binary
$(IMG): $(BUILD_DIR)/kernel.elf
	$(CROSS)-objcopy $< -O binary $@

# Start qemu
qemu: $(IMG)
	$(QEMU) $(QEMU_FLAGS) $(IMG) -serial null -serial stdio

# Start GDB
dbg: $(IMG) $(ELF)
	$(QEMU) $(QEMU_FLAGS) $(IMG) -S -s &
	$(GDB) $(ELF)
	killall $(shell pgrep qemu)

clean:
	rm -rf build/* target

doc:
	cargo rustdoc --open -- --document-private-items
