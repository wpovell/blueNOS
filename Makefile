include make/common.mk

.PHONY: all server kernel boot clean
all: server kernel boot

server:
	@ cd server && $(MAKE) all

kernel:
	@ cd kernel && $(MAKE) all

boot:
	@ cd boot && $(MAKE) all

clean:
	@ cd server && $(MAKE) clean
	@ cd kernel && $(MAKE) clean
	@ cd boot && $(MAKE) clean

KERN_IMG = build/kernel/kernel8.img
KERN_ELF = build/kernel/kernel8.elf

QEMU = qemu-system-aarch64
QEMU_FLAGS = -M raspi3 -kernel $(KERN_IMG)
GDB = $(CROSS)-gdb

run: kernel
	$(QEMU) $(QEMU_FLAGS) -serial null -serial stdio

dbg: kernel
	$(QEMU) $(QEMU_FLAGS) -S -s &
	$(GDB) $(KERN_ELF)
	killall $(QEMU)

ikern: kernel
	sudo ./bin/install.sh $(KERN_IMG)

iboot: boot
	sudo ./bin/install.sh build/boot/kernel8.img
