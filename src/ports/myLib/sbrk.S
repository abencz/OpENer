	.syntax unified
	.cpu cortex-m3
	.thumb
	.file	"sbrk.S"
	.align	2
	.global	sbrk
	.type	sbrk, %function


// a UNIX-like sbrk (heap manager support)
// TODO check for collision with the background stack


	.data
	.global _break
_break:	.word	_end			// the heap starts after the end of .bss

	.text

sbrk:
	mov	r1, r0			// save increment
	ldr	r3, =_break		// get address of current heap start

	ldr	r0, [r3]		// get heap start

	add	r0, #7			// round to 8 byte boundary
	and	r0, #~7

	add	r1, r0			// add increment
	str	r1, [r3]		// update break with new value
	bx	lr			// return

	.size	sbrk, .-sbrk
