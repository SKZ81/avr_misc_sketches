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

void main(void) {
    char str[64];
    snprintf(str, 32, "%d\n", sizeof(thread_ctx));
    debug_puts("coucou : ");
    debug_puts(str);
    thread_ctx_ptrs[0] = malloc(sizeof(thread_ctx));
    thread_ctx_ptrs[1] = malloc(sizeof(thread_ctx));

    uint8_t toto[sizeof(thread_ctx)] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
        0x30, 0x31, // regs
        0x9c, 0x9b, // PC (9C)
        0x59, 0X58, // SP (59)
        0xAC, 0x57, // stack (57AC)
        0xFF // SREG
    };

    thread_ctx *pctx = (thread_ctx*)toto;

    for (int i=0; i<32; i++) {
        snprintf(str, 64, "r%d: %x\n", i, pctx->_regs[i]);
        debug_puts(str);
    }

    snprintf(str, 64, "PC: %x\n", pctx->_PC);
    debug_puts(str);
    snprintf(str, 64, "SP: %x\n", pctx->_SP);
    debug_puts(str);
    snprintf(str, 64, "stack@: %x\n", pctx->_stack);
    debug_puts(str);
    snprintf(str, 64, "SREG: %x\n", pctx->_SREG);
    debug_puts(str);
}

/*void main (void)
{
    // setMode(PORTC, output)
    DDRC = 0xff;
    // setMode(PORTB, input)
    DDRB = 0x00;
    // timer_freq = clock_freq/8
    TCCR0B = 0x02;
    // enable timer0 COMP_A interrupt mask
    TIMSK0 = 0x02;
    // after 5*8 = 40 cycles
    OCR0A = 0x05;

    // enable interrupts mask
    sei();

    // ==== LOOP ====
    while(1) {
        PORTC = PINB + 1;
    }
}*/

