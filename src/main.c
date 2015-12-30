                        // ============  <DEBUG> ============
    #include <stdio.h>

    /* This port correponds to the "-W 0x20,-" command line option. */
    #define special_output_port (*((volatile char *)0x20))
    /* This port correponds to the "-R 0x22,-" command line option. */
    #define special_input_port (*((volatile char *)0x22))

    /* Poll the specified string out the debug port. */
    void debug_puts(const char *str) {
        const char *c;
        for(c = str; *c; c++)
            special_output_port = *c;
    }
// ============ </DEBUG> ============


#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "threading/scheduler.h"
#include "threading/thread_ctx.h"

thread_ctx *thread_ctx_ptrs[2];
uint8_t current_thread_id=1;
uint8_t next_thread_id=0;

uint8_t stack_th0[10] = {0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9}; 
uint8_t thread0_ctx[sizeof(thread_ctx)] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
	0x20, 0x21, 0x22, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x30, 0x31,                             // regs
	0x00, 0x00,								// PC
	0xF6, 0X08,                             // SP
	0x00, 0x00,								// stack
	0x00                                    // SREG
};

#define lo8(x) (uint8_t)(x)
#define hi8(x) (uint8_t)(((uint16_t)x)>>8)

extern void thread2_start();

void main(void) {
//    thread0_ctx[32] = lo8(&&after_while);
//    thread0_ctx[33] = hi8(&&after_while);
	thread0_ctx[36] = lo8(stack_th0);
	thread0_ctx[37] = hi8(stack_th0);

    thread_ctx_ptrs[0] = (thread_ctx*)thread0_ctx;

    thread_ctx_ptrs[1] = malloc(sizeof(thread_ctx));
    thread_ctx_ptrs[1]->_stack = malloc(0xFF);

    thread_ctx *pctx = thread_ctx_ptrs[0];

    
    // setMode(PORTC, output)
    DDRC = 0xff;
    // setMode(PORTB, input)
    DDRB = 0x00;
    // timer_freq = clock_freq/64
    TCCR0B = 0x03;
    // enable timer0 COMP_A interrupt mask
    TIMSK0 = 0x02;
    // after 64*8 = 512 cycles
    OCR0A = 0x08;

    sei();

    thread2_start(thread_ctx_ptrs[1]);

    while(1) {
        debug_puts("^**^\n");
    }
}
