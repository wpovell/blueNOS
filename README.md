# BlueNOS

My work on an ARMv8 OS for the Raspberry Pi 3.

*(An OS only Blueno would use)*

## TODO
- Tests
- Memory Management
  - Paging (buddy system?)
  - kmalloc / kfree
  - slab allocator
- Graphics
  - Improve framebuf mailbox interface (structs)
  - [Make faster](https://www.raspberrypi.org/forums/viewtopic.php?t=213964)
  - Start work on tty / line discipline stuff
- Interrupt Controller Driver
  - Timer
- Processes
- Filesystem
  - SD Driver
  - FAT32
  - S5FS?
- Virtual Memory
  - Get MMU working
- USB
  - Keyboard
  - Ethernet -> Networking stack
- Audio?
- Multicore?
- Misc
  - Onboard LED
  - Power off

## Requirements
### Software
- `aarch64-linux-gnu v8.1.0` cross-compiler toolchain
- `qemu-system-aarch64 v2.12.0` to emulate the RPi

### Hardware
- Raspberry Pi 3 B+
- USB to TTL Serial Cable ([for example][usb_to_ttl])
- MicroSD Card

## Build & Running
### Make
- `make all` and `make clean` work as expected.
- `make run` will start QEMU.
- `make dbg` will start QEMU with GDB.
- `make iboot` and `make ikern` install the bootloader or full kernel respectively.

### Scripts
- `sudo install.sh <image> [sd dev]`: Will install image to SD Card. Defaults to `/dev/mmcblk0p1`
- `uart.sh [usb dev]`: Starts screen connected to UART. Defaults to `/dev/ttyUSB0`
- `server <device> <file>`: Script used to transfer kernel to bootloader over UART.

### Running
There are a couple options to run the OS
- Install the kernel directly with `make ikern`
- Install the bootloader with `make iboot` and chain load the kernel over UART
- Run the kernel in QEMU with `make run`

## Kernel Memory Layout
(not to scale)
```
┏━━━━━━━━━━━━━━━━━━ 0x40000000 Max VC Mem
┃
┃   ┏━━━━━━━━━━━━━━
┃   ┃ Peripheral MMIO
┃   ┗━━━━━━━━━━━━━━ 0x3F000000
┃
┣━━━━━━━━━━━━━━━━━━ 0x3B400000 Max CPU Mem
┃
┃  ┏━━━━━━━━━━━━━━━
┃  ┃ Bootloader
┃  ┗━━━━━━━━━━━━━━━ 0x04000000
┃
┃
┃ Kernel Heap ▲
┣━━━━━━━━━━━━━━━━━━
┃ bss
┣━━━━━━━━━━━━━━━━━━
┃ data
┣━━━━━━━━━━━━━━━━━━
┃ rodata
┣━━━━━━━━━━━━━━━━━━
┃ text
┣━━━━━━━━━━━━━━━━━━
┃ text.boot
┣━━━━━━━━━━━━━━━━━━ 0x00080000 Kernel Start
┃ Kernel Stack ▼
┃
┗━━━━━━━━━━━━━━━━━━ 0x00000000
```

## Comments

I think OSs are pretty confusing to want to have pretty thorough commenting in this project,
if not only to prove to myself I understand what is going on. I'd like to keep the following norms:
- All functions get a header comment with description & args
- `// NOTE:` An interesting behavior or something to keep in mind
- `// TODO:` Something to do
- `// Q:` An unresolved question I have

## Thanks / Resources

The [BCM2835 Peripheral Manual](https://web.stanford.edu/class/cs140e/docs/BCM2837-ARM-Peripherals.pdf)
has lots of the random addresses used for driver stuff. If you see a reference to `(Section X.X)`
in comments it's referring to this.

Bits pulled from:
- [bztsrc's "Bare Metal Programming" tutorial](https://github.com/bztsrc/raspi3-tutorial)
- [s-matyukevich's RPi OS](https://github.com/s-matyukevich/raspberry-pi-os)
- [jsandler18's Kernel for 32bit ARM](http://jsandler18.github.io/)
- [Stanford's CS140e](https://web.stanford.edu/class/cs140e)

[usb_to_ttl]: https://www.amazon.com/JBtek-WINDOWS-Supported-Raspberry-Programming/dp/B00QT7LQ88/
