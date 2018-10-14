set confirm off
target remote localhost:1234
# Not sure why this works, but doesn't work at the correct value
# Probably something to do with QEMU acting like 32bit?
add-symbol-file build/kernel.elf 0x800
break kmain
layout asm
focus cmd
