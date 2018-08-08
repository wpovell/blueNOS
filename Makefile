include common.mk

.PHONY: all srv kernel boot clean
all: srv kernel boot

# Target for bootloader server
srv:
	@ cd srv && $(MAKE) all

# Main kernel target
kernel:
	@ cd kernel && $(MAKE) all

# Target for bootloader
boot:
	@ cd boot && $(MAKE) all

clean:
	@ cd srv && $(MAKE) clean
	@ cd kernel && $(MAKE) clean
	@ cd boot && $(MAKE) clean

# Kernel outputs
KERN_IMG = build/kernel/kernel8.img
KERN_ELF = build/kernel/kernel8.elf

# QEMU flags
QEMU = qemu-system-aarch64
QEMU_FLAGS = -M raspi3 -kernel $(KERN_IMG)
GDB = $(CROSS)-gdb

# Start QEMU with UART to stdout
run: kernel
	$(QEMU) $(QEMU_FLAGS) -serial null -serial stdio

# Start QEMU with gdb
dbg: kernel
	$(QEMU) $(QEMU_FLAGS) -S -s &
	$(GDB) $(KERN_ELF)
	killall $(QEMU)

# Install kernel to SD card
ikern: kernel
	sudo ./bin/install $(KERN_IMG)

# Install bootloader to SD card
iboot: boot
	sudo ./bin/install build/boot/kernel8.img
