set confirm off
target remote localhost:1234
add-symbol-file build/kernel8.elf 0x8000
break kernel_main
