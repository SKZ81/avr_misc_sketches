/*
 * thread_ctx.h
 *
 * Created: 18/12/2015 20:56:19
 *  Author: jcahier
 */ 


#ifndef THREAD_CTX_H_
#define THREAD_CTX_H_

#ifdef __ASSEMBLER__
/* Assembler header */

/* struct thread_ctx offset definitions */
__struct_thread_ctx_regs   = 0
__struct_thread_ctx_PC     = 32
__struct_thread_ctx_SP     = 34
__struct_thread_ctx_stack  = 36
__struct_thread_ctx_SREG   = 38

.data

/* thread_ctx* thread_ctx_ptrs[]; */
.extern current_thread_id
/* uint8_t current_thread_id; */
.extern next_thread_id
/* uint8_t max_thread_ctx; */
.extern thread_ctx_ptrs

#else
/* Standard C header */
typedef struct {
        uint8_t  _regs[32];  // 32
        uint16_t _PC;        //  2
        uint16_t _SP;        //  2
        uint8_t *_stack;     //  2
        uint8_t  _SREG;      //  1
} thread_ctx;

extern thread_ctx* thread_ctx_ptrs[];
extern uint8_t current_thread_id;
extern uint8_t max_thread_ctx;

#endif /*  */
  
#endif /* THREAD_CTX_H_ */
