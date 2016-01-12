/*
 * thread2.S : another thread that do some debug_puts
 *
 * Created: 
 *  Author: jcahier
 */ 

/* NB : the 2 in "thread2" means "second". It does not imply anything on index in thread_ctx_ptrs[] list */

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
	;push r30	;
	;push r31	

	; ===== PREPARE THREAD2 CONTEXT =====
	; Z <- thread2_ctx
	mov  r30, r24
	mov  r31, r25 ; r30,r31 are call-used registers, no need to save them

	ldi  r24, lo8(pm(thread2_loop))
	ldi  r25, hi8(pm(thread2_loop)) ; r24,r25 are call-used input registers, let s squash them

	; (*thread2_ctx).PC = thread2_loop;
	std  Z+__struct_thread_ctx_PC, r24
	std  Z+__struct_thread_ctx_PC+1, r25

	cli
	jmp switch_context

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
