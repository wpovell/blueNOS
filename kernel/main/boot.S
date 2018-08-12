.section ".text.boot"

.globl _start
_start:
  mrs	x4, mpidr_el1   // Get processor id
  and	x4, x4,#0xFF	  // Get first two bytes
  cbz	x4, start	      // If CPU == 0, break to start
  b	hang              // Hang all other processors

hang:                 // Loop de loop!
  b hang

start:
  /* SWITCH EL2 -> EL1
  The raspberry pi starts in EL2, which is normally used for hypervisors
  We want to be in EL1 where kernel code usually runs
  */

  // Enable CNTP for EL1
  // See: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0488d/ch09s03s01.html
  // Q: I don't have a strong understanding of what exactly this does
  mrs     x0, cnthctl_el2
  orr     x0, x0, #0b11
  msr     cnthctl_el2, x0  // Seems to be enabling a timer
  msr     cntvoff_el2, xzr // Something to do with virtual offset

  // Enable AArch64 in EL1
  // See: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0500d/CIHJHAAG.html
  mov x0, #(1 << 31)      // Enable AArch64
  orr x0, x0, #(1 << 1)   // Enable SWIO
                          // Q: This has to do with cache invalidation but I don't
                          //    actually understand what this is for.
  msr hcr_el2, x0
  mrs x0, hcr_el2


  /* Change to EL1!
  Essentially what we're doing here is setting up PSTATE so we can pretend
  we're returning from an exception in EL2 and jump back down to EL1
  0x3c4 =
  0b 1 1 1 1 0 0 [ 0 1 0 0 ]
     ^ ^ ^ ^   ^     ^ Origin of the exception, EL1
     | | | |   |- Execution State, AArch64
     | | | |- FIQ Mask
     | | |- IRQ Mask
     | |- System Error Mask
     |- Debug Mask
  See: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.den0024a/BABJGFCI.html
  */
  mov x2, #0x3c4
  msr spsr_el2, x2     // Set the forged PSTATE
  adr x2, main         // Store start as addr to jump to on exception ret
  msr elr_el2, x2
  eret                 // "Return" from exception

main:
  // Enable NEON floating-point registers
  // See: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.den0024a/BABGBFBF.html
  mrs x1, cpacr_el1
  orr x1, x1, #(0b11 << 20)
  msr cpacr_el1, x1
  isb  // Flushes pipeline so change takes effect before any other instructions are executed
       // This isn't actually necessary since any floating point ops are pretty far removed from here.

  // SETUP STACK //
  mov x0, #0x80000
  mov	sp, x0          // Set stack to grown downwards from kernel

  // CLEAR BSS //
  adr	x4, bss_begin   // Store start of bss in x4
  adr	x5, bss_end
  sub	x5, x5, x4      // Store size of bss in x5
  bl 	memzero

  bl   kernel_main    // Go to kernel main
  b 	hang            // If we ever return, just hang

memzero:
  str xzr, [x4], #8   // Zero out the byte at x4 and add 8 to x4
  subs x5, x5, #8     // Subtract a byte from the bss length in x5
  b.gt memzero        // If x5 != 0, loop!
  ret
