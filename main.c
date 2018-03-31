#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>
#include "avr_uart.h"
#include "display.h"
#include <Arduino.h>
#include "dht22.h"

#define SENSOR_POWER_PORT PORTB
#define SENSOR_POWER_DDR DDRB
#define SENSOR_POWER_PIN PORTB2

#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED_PIN PORTB5


#define READ_INTERVAL 270     // in seconds !!!!
#define NB_VALUES     320   // 640s on screen => 10'40''

uint16_t timer0_macro_counter = READ_INTERVAL;
uint8_t  timer0_micro_counter = 250; // see setup_TIMER0, such as TIMER1_COMPA_vect is called @250Hz
volatile unsigned char tick=0, tick_err=0;
/*
void setup_TIMER1() {
    cli();
    DDRB |= (1 << PORTB5); // set LED pin as output
    TCCR1A &= ~((1 << WGM10) | (1 << WGM11));
    TCCR1B |= (1 << WGM12); 
    TCCR1B &= ~(1 << WGM13); // configure timer1 for CTC mode
    TIMSK1 |= (1 << OCIE1A); // enable the CTC interrupt
    OCR1A   = 25000; // set the CTC compare value
    TCCR1B |= ((1 << CS11) | (1 << CS10)); // start the timer at 20MHz/64
    tick=0;
    tick_err=0;
    sei();
}

ISR(TIMER1_COMPA_vect) {
    timer1_macro_counter--;
    
    if (!timer1_macro_counter) {
        if (tick) tick_err=1; 
        else tick=1;
        PORTB ^= (1 << PORTB5);
        timer1_macro_counter = READ_INTERVAL * 10;
    }
}
*/


void setup_TIMER0() {
    cli();
    // configure timer1 for CTC mode
    TCCR0A = 0x02;
//     TCCR0A &= ~(1 << WGM00);
//     TCCR0A |= (1 << WGM01);
//     TCCR0B &= ~(1 << WGM02);

    // enable the CTC interrupt
    TIMSK0 |= (1 << OCIE0A);
    // start the timer at 16MHz/256
//     TCCR0B |= (1 << CS02);
//     TCCR0B &= ~( (1 << CS01) | (1 << CS01));
    TCCR0B = 0x04;
    // set the CTC compare value
    OCR0A  = 250;

    // with those values, TIMER0_COMPA_vect will be called @ 250Hz
    tick=0;
    tick_err=0;
    sei();
}

ISR(TIMER0_COMPA_vect) {
    timer0_micro_counter--;
    if (!timer0_micro_counter) { // 1 second elapsed
        timer0_micro_counter = 250;
        timer0_macro_counter--;
        if (!timer0_macro_counter) { // READ_INTERVAL elapsed
            if (tick) tick_err=1;
            else tick=1;
            PORTB ^= (1 << PORTB5);
            timer0_macro_counter = READ_INTERVAL;
        }
    }
}


void main(void) {
    init();
    uint8_t temps[NB_VALUES] = {0};
    uint8_t humids[NB_VALUES] = {0};
    int val_idx = 0;

    // init UART for serial communication
    avr_uart_init();
    dht22_init();


    stdout = &avr_uart_output;
    stdin  = &avr_uart_input_echo;

    printf("Temperature sensor with DHT22 & LCD screen, %d sec.\n", READ_INTERVAL);
    printf("CLKPR : %x (CLKPCE:%s)n\n", CLKPR&0x7F, CLKPR&0x80?"1":"0");
    display_init(stdout);
/*    display_error("Test error screen\n ... 3"); _delay_ms(1000);
    display_error("Test error screen\n ... 2"); _delay_ms(1000);
    display_error("Test error screen\n ... 1"); _delay_ms(1000);*/
    display_error(NULL);
    
    display_update(temps, humids, NB_VALUES, 0);
   
    printf("Blink led...\n");
    DDRB |= (1 << PORTB5);
    PORTB |= (1 << PORTB5);
    _delay_ms(1000);
    PORTB &= ~(1 << PORTB5);
    printf("...done\n");

    setup_TIMER0();
    char caption_str[16] = {0};
    uint8_t res = 0;
    int16_t temp = 0, humid = 0;

    while(1) {
        if (tick_err) goto error;
        cli(); // avoid race conditions with TIMER0
        if (tick) {
            tick=0;
            sei();

            res = dht22_read(&temp, &humid);
            switch(res) {
                case DHT22_OK:
                    temps[val_idx] = (temp-50)&0xFF;
                    humids[val_idx] = (humid/10);
                    printf("T : %d; H : %d -- idx:%d\n\n", temp, humid, val_idx);
                    break;
                case DHT22_ERR_NOLOWSTATE:
                    printf("ERROR : no low state on DTH22 DATA pin...\n");
                    break;
                case DHT22_ERR_CHECKSUM:
                    printf("ERROR : checksum calculation\n");
                    break;
                case DHT22_ERR_TIMEOUT:
                    printf("ERROR : timeout on bit acquisition\n");
                    break;
                default:
                    printf("UNKNOW error code !!\n"); // this should NEVER happend
                    while(1) {_delay_ms(500);}
            }
            if (res != DHT22_OK) {
                temps[val_idx] = 0;
            }
            display_update_step(temps, humids, NB_VALUES, 0, val_idx-1, 1);
            if (res == DHT22_OK) {
                sprintf(caption_str, "%.1f*C %.1f%%", ((float)temp)/10.0, ((float)humid)/10.0);
                caption(caption_str);
            }
            val_idx++;
            if (val_idx == NB_VALUES) {val_idx=0;}
        } else {
            sei();
        }

    }
    
error:
    display_error("Timing interrupt error, previous not ACKed");
    while(1) {}
}
