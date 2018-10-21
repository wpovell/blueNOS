# BluenOS
_Blueno's Operating System_

## About

This is my long-term hobby project to create an operating system for the Raspberry Pi.

Some of the overarching goals for this project include

1. Understand everything  
2. Run on real hardware  
3. Be a testbed to learn new things

## TODO

1. Write XModem lib
2. Write client
3. Write bootloader, refactor LD and boot.S
4. Refactor driver stuff into pi
5. Graphics + MBox
6. Shell
7. Malloc
8. FS
    - VFS
    - RamFS
9. SD Driver
10. FAT32 (S5FS?)
11. Proc (possibly earlier)
12. Userspace

Misc
- QEMU integration tests
- README writeup
- ACT LED
- Write some of this up

Long Term
- USB (Keyboard / Mouse)
- Networking
- Display Manager
- Scripting lang / real shell
- Image encoder / decoder
- Drone
- Sound
- Multicore

## Structure

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

## Thanks / Resources

The [BCM2835 Peripheral Manual](https://web.stanford.edu/class/cs140e/docs/BCM2837-ARM-Peripherals.pdf)
has lots of the random addresses used for driver stuff. If you see a reference to `(Section X.X)`
in comments it's referring to this.

Bits pulled from:
- [bztsrc's "Bare Metal Programming" tutorial](https://github.com/bztsrc/raspi3-tutorial)
- [s-matyukevich's RPi OS](https://github.com/s-matyukevich/raspberry-pi-os)
- [jsandler18's Kernel for 32bit ARM](http://jsandler18.github.io/)
- [Stanford's CS140e](https://web.stanford.edu/class/cs140e)