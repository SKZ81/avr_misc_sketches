/*
 * scheduler.h
 *
 * Created: 18/12/2015 21:27:32
 *  Author: jcahier
 */ 


#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "thread_ctx.h"

#ifdef __ASSEMBLER__

/* Assembler header */

.global switch_context

#else
 /* standard C header */

void switch_context(void);

#endif /* __ASSEMBLER__ */


#endif /* SCHEDULER_H_ */
