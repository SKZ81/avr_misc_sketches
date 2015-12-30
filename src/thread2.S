/*
 * thread2.S : another thread that do some debug_puts
 *
 * Created: 18/12/2015 21:26:14
 *  Author: jcahier
 */ 

#include "threading/scheduler.h"

.data

thread2_msg:
	.asciz "@--@\n"


.text

.extern debug_puts

.global thread2_start
.global thread2_loop

// int thread2_start(thread_ctx *thread2_ctx)
thread2_start:
	push r30
	push r31
	; Z <- thread2_ctx
	mov  r20, r24
	mov  r31, r25

	;
	pop  r29
	pop  r28
	ret

nop
nop
nop
nop
thread2_loop:
nop
nop
nop
nop
	ldi  r24, lo8(thread2_msg)
	ldi  r25, hi8(thread2_msg)
	call debug_puts
	jmp  thread2_loop