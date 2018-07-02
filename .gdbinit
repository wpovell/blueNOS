set confirm off
target remote localhost:1234
# Not sure why this works, but doesn't work at the correct value
# Probably something to do with QEMU acting like 32bit?
add-symbol-file build/kernel/kernel8.elf 0x8000
break kernel_main
layout src
focus cmd
continue
