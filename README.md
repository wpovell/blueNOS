# BlueNOS

My work on an ARMv8 OS for the Raspberry Pi 3.

*(An OS only Blueno would use)*

## TODO

- Memory Management
  - Paging
  - kmalloc / kfree
  - slab allocator
- Graphics
  - Improve framebuf mailbox interface (structs)
  - Make faster
  - Start work on tty / line discipline stuff
- Interrupt Controller Driver
  - Timer
- Processes
- Filesystem
  - SD Driver
  - FAT32
  - S5FS
- Virtual Memory
- USB
  - Keyboard
  - Ethernet -> Networking stack
- Audio?
- Multicore?
- Misc
  - Onboard LED
  - Cleanup bootloader Makefile

## Requirements
### Software
- `aarch64-linux-gnu v8.1.0` cross-compiler toolchain
- `qemu-system-aarch64 v2.12.0` to emulate the RPi
- `minicom` to interact with RPi over UART

### Hardware
- Raspberry Pi 3 B+
- USB to TTL Serial Cable ([for example][usb_to_ttl])
- MicroSD Card

## Build & Running
### Make

`make` should just work.

`run` will start QEMU.

`dbg` target will start QEMU with GDB.

### Scripts
- `sudo install.sh [sd card dev]`: Will install image to SD Card. Defaults to `/dev/mmcblk0p1`
- `uart.sh [usb dev]`: Starts screen connected to UART. Defaults to `/dev/ttyUSB0`

## Kernel Memory Layout
(not to scale)
```
---------- MEM_MAX
heap
----------
bss
----------
data
----------
rodata
----------
text
----------
text.boot
---------- 0x80000
stack
---------- 0
```

## Thanks / Resources

The [BCM2835 Peripheral Manual](https://web.stanford.edu/class/cs140e/docs/BCM2837-ARM-Peripherals.pdf)
has lots of the random addresses used for driver stuff. If you see a reference to `(Section X.X)`
in comments it's referring to this.

Bits pulled from:
- [bztsrc's "Bare Metal Programming" tutorial](https://github.com/bztsrc/raspi3-tutorial)
- [s-matyukevich's RPi OS](https://github.com/s-matyukevich/raspberry-pi-os)
- [jsandler18's Kernel for 32bit ARM](https://github.com/jsandler18/raspi-kernel)
- [Stanford's CS140e](https://web.stanford.edu/class/cs140e)

[usb_to_ttl]: https://www.amazon.com/JBtek-WINDOWS-Supported-Raspberry-Programming/dp/B00QT7LQ88/
