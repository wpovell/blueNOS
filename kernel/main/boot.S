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
	adr	x4, bss_begin   // Zero out BSS
	adr	x5, bss_end
	sub	x5, x5, x4
	bl 	memzero

	mov	sp, #0x80000     // Set stack to grown downwards from kernel
	bl	kernel_main
	b 	hang

memzero:
	str xzr, [x4], #8
	subs x5, x5, #8
	b.gt memzero
	ret
