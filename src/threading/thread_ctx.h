/*
 * thread_ctx.h
 *
 * Created: 18/12/2015 20:56:19
 *  Author: jcahier
 */ 


#ifndef THREAD_CTX_H_
#define THREAD_CTX_H_

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

  
#endif /* THREAD_CTX_H_ */
