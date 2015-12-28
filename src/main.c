#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void main (void)
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
}


