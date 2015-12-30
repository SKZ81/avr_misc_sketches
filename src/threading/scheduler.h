/*
 * scheduler.h
 *
 * Created: 18/12/2015 21:27:32
 *  Author: jcahier
 */ 


#ifndef SCHEDULER_H_

#include "thread_ctx.h"

#ifdef __ASSEMBLER__

/* Assembler header */
.extern current_thread_id 
.extern next_thread_id
.extern thread_ctx_ptrs

.global switch_context

#elif
 /* standard C header */

extern uint8_t current_thread_id;
extern uint8_t next_thread_id;
extern thread_ctx* thread_ctx_ptrs[];

void switch_context(void);

#define SCHEDULER_H_





#endif /* SCHEDULER_H_ */
