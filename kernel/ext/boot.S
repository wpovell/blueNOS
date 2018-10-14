.section ".text.boot"

.globl _start
_start:
  mrs x4, mpidr_el1                      // Get processor id
  and x4, x4,#0xFF                       // Get first two bytes
  cbz x4, start                          // If CPU == 0, break to start
  b hang                                 // Hang all other processors

hang:                                    // Loop de loop!
  b hang

start:
  /* SETUP STACK */
  mov x0, #0x80000
  mov sp, x0                             // Set stack to grown downwards from kernel

  /* CLEAR BSS */
  adr  x4, __bss_start                     // Store start of bss in x4
  adr x5, __bss_end
  sub x5, x5, x4                         // Store size of bss in x5
  bl memzero

  bl kmain                         // Go to kernel main
  b hang                                 // If we ever return, just hang

memzero:
  str xzr, [x4], #8                      // Zero out the byte at x4 and add 8 to x4
  subs x5, x5, #8                        // Subtract a byte from the bss length in x5
  b.gt memzero                           // If x5 != 0, loop!
  ret
