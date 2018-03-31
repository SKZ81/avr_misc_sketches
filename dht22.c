#include <avr/io.h>
#include <avr/interrupt.h>
#include "avr_uart.h"
#include <util/delay.h>
#include <avr/cpufunc.h>
#include "dht22.h"

static volatile uint8_t dht22_status = 0;
static volatile uint8_t dht22_bit_counter = 0;

#define RUNNING         0x01
#define FIRST_LOW       0x02
#define FIRST_HIGH      0x04
// ...
#define OVERFLOW        0x40
#define READ_COMPLETE   0x80

#define DHT22_GET_STATUS(status)   (dht22_status & status)
#define DHT22_SET_STATUS(status)   (dht22_status |= status)
#define DHT22_RESET_STATUS(status) (dht22_status &= ~status)


uint8_t dht22_data[5] = {0};


ISR(PCINT0_vect) {
//     printf(".");
    if (DHT22_GET_STATUS( READ_COMPLETE )) return;

    if (PINB & (1 << PINB4)) {
        if (DHT22_GET_STATUS( FIRST_HIGH )) {
            TCCR1B = 0x02; // start counter, prescaling 1/8 (tick each O,5 µs)
            DHT22_SET_STATUS( RUNNING) ;
            PORTB |= (1 << PORTB5); // led on
        } else {
            DHT22_SET_STATUS( FIRST_HIGH );
        }
    } else {
        if (DHT22_GET_STATUS( RUNNING )) {
            TCCR1B = 0;
            if(TCNT1 > 100) { // if timing > 50µs => set corrsponding bit to 1
                dht22_data[ dht22_bit_counter/8 ] |= 1 << (7 - dht22_bit_counter%8);
            }
            TCNT1 = 0;     // init timer value
            TCCR1A = 0;    // stop counter
            TCCR1B = 0;    
            TCCR1C = 0;
            dht22_bit_counter++;
            if (dht22_bit_counter == 40) {
                DHT22_SET_STATUS( READ_COMPLETE );
                DHT22_RESET_STATUS( RUNNING );
                PORTB &= ~(1 << PORTB5); // led off
            }
        } else {
            DHT22_SET_STATUS( FIRST_LOW );            
        }
    }    
    //PORTB &= ~(1 << PORTB5); // led off
}


ISR(TIMER1_OVF_vect) {
    DHT22_SET_STATUS( OVERFLOW );
}




void dht22_init() {
    DDRB |= (1 << PINB3);
    PORTB |= (1 << PORTB3); // powerup DHT22
    _delay_ms(2000);
    DDRB &= ~(1 << PINB4); // input
    PORTB |= (1 << PORTB4); // pull-up
}

uint8_t dht22_read(int16_t *temperature, int16_t *humidity) {
    cli();
//     PORTB |= (1 << PB5); // led ON
    dht22_status = 0; // reset all status bits
    dht22_bit_counter = 0;

    dht22_data[0] = 0;
    dht22_data[1] = 0;
    dht22_data[2] = 0;
    dht22_data[3] = 0;
    dht22_data[4] = 0;
    
    DDRB |= (1 << PINB4);
    
    PORTB &= ~(1 << PORTB4);
    _delay_us(1000);
    PORTB |= (1 << PORTB4);
    _delay_us(25);
    // enable interrupts and other stuff needed to read values
    PCICR  |= (1 << PCIE0);    // enable pin state change interrupt on port B
    PCMSK0 |= (1 << PCINT4);   // enable pin state change interrupt on pin B4
    TIMSK1 |= (1 << TOIE1);    // enable TIMER1 OVERFLOW interrupt (in normal case, all value should be <32Kµs...)
//     sei();                     // enable global interrupt (in SREG)
    DDRB &= ~(1 << PINB4);     // set DHT 22 DATA pin as INPUT mode
    
    uint8_t cont = 1;
    while(cont) {
        sei();
        _delay_us(500);
        cli();
        cont =  !( DHT22_GET_STATUS(READ_COMPLETE) || DHT22_GET_STATUS(OVERFLOW) ) /*&& DHT22_GET_STATUS(RUNNING)*/; 
    }

    PCICR  &= ~(1 << PCIE0);   // disable pin state change interrupt on port C
    PCMSK0 &= ~(1 << PCINT4);  // disable pin state change interrupt on pin C0
    TIMSK1 &= ~(1 << TOIE1);   // disable TIMER1 OVERFLOW interrupt
    sei();
    
//     PORTB &= ~(1 << PB5); // led OFF
    
    if (DHT22_GET_STATUS(OVERFLOW) && DHT22_GET_STATUS(FIRST_LOW)) {
        return DHT22_ERR_TIMEOUT;
    } else if (DHT22_GET_STATUS(OVERFLOW) && !DHT22_GET_STATUS(FIRST_LOW)) {
        return DHT22_ERR_NOLOWSTATE;
    } else if ( (uint8_t)(dht22_data[0]+dht22_data[1]+dht22_data[2]+dht22_data[3]) != dht22_data[4]) {
        return DHT22_ERR_CHECKSUM;
    }
    
    *humidity = ((dht22_data[0]&0x7F) << 8) + dht22_data[1];
    *temperature = ((dht22_data[2]&0x7F) << 8) + dht22_data[3];
    if (dht22_data[2]&0x80) {*temperature *= -1;}

    return DHT22_OK;
}    

