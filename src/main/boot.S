.section ".text.boot"

.globl _start
_start:
	mrs	x0, mpidr_el1   // Get processor id
	and	x0, x0,#0xFF	  // Get first two bytes
	cbz	x0, start		    // If CPU == 0, break to start
	b	hang              // Hang all other processors

hang:
	b hang

start:
	adr	x0, bss_begin   // Zero out BSS
	adr	x1, bss_end
	sub	x1, x1, x0
	bl 	memzero

	mov	sp, #0x80000    // Set stack to grown downwards from kernel
	bl	kernel_main
	b 	hang

memzero:
	str xzr, [x0], #8
	subs x1, x1, #8
	b.gt memzero
	ret
