#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>
#include "avr_uart.h"
#include "display.h"
#include <Arduino.h>


#define SENSOR_POWER_PORT PORTB
#define SENSOR_POWER_DDR DDRB
#define SENSOR_POWER_PIN PORTB2

#define LED_DDR DDRB
#define LED_PORT PORTB
#define LED_PIN PORTB5

#define SENSOR_ANALOG_PIN 5// A5

void sensor_setup(void) {
    // Pin for powering the sensor
    SENSOR_POWER_DDR |= (1<<SENSOR_POWER_PIN);
    SENSOR_POWER_PORT &= ~(1<<SENSOR_POWER_PIN);

    // power sensor shortly to blink sensor led 
    _delay_ms(250);
    SENSOR_POWER_PORT |= (1<<SENSOR_POWER_PIN);
    _delay_ms(500);
    SENSOR_POWER_PORT &= ~(1<<SENSOR_POWER_PIN);
}

// #define PORT_ON(port,pin) port |= (1<<pin)
// #define PORT_OFF(port,pin) port &= ~(1<<pin)
unsigned int sensor_read(uint8_t analogChannel)
{
    if (analogChannel > 5) {
        return 0;
    }
    unsigned int adc_value; // Variable to hold ADC result

    // power sensor
    SENSOR_POWER_PORT |= (1<<SENSOR_POWER_PIN);

    _delay_ms(250);

    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS0);
    // ADEN: Set to turn on ADC , by default it is turned off
    //ADPS2: ADPS2 and ADPS0 set to make division factor 32
    ADMUX=analogChannel | (1<<REFS0); // ADC input channel set to PC5
    ADCSRA |= (1<<ADSC); // Start conversion
        while (ADCSRA & (1<<ADSC)); // wait for conversion to complete
    adc_value = ADCW; //Store ADC value

    // power off sensor
    SENSOR_POWER_PORT &= ~(1<<SENSOR_POWER_PIN);

    return adc_value;
}

#define READ_INTERVAL 10
#define NB_VALUES 320   // 10'40''
                        // 5h 20 minutes !

uint16_t timer1_macro_counter = READ_INTERVAL * 10;
volatile unsigned char tick=0, tick_err=0;

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



void main(void) {
    init();
    uint8_t values[NB_VALUES] = {255}; // display should be 240 in heigh, so it's over
    int val_idx = 0;
        
    sensor_setup();
    // init UART for serial communication
    avr_uart_init();

    stdout = &avr_uart_output;
    stdin  = &avr_uart_input_echo;

    printf("humidity sensor with LCD screen, %d sec.\n", READ_INTERVAL);    

    display_init(stdout);
    display_error("Test error screen\n ... 3"); _delay_ms(1000);
    display_error("Test error screen\n ... 2"); _delay_ms(1000);
    display_error("Test error screen\n ... 1"); _delay_ms(1000);
    display_error(NULL);
    
    display_update(values, NB_VALUES, 0);
   
    setup_TIMER1();
    
    uint16_t width = display_width();
    while(1) {
        if (tick_err) goto error;
        for(uint16_t step=0; step < width; step++) {
            cli(); // avoid race conditions with TIMER1
            if (tick) {
                tick=0;
                sei();
                uint16_t value = sensor_read(SENSOR_ANALOG_PIN);
                values[val_idx] = (value >> 2);
                val_idx++;
                printf("time %ld ; val : %d\n", micros(), value);
            } else {
                sei();
            }
            display_update_step(values, NB_VALUES, val_idx, step);
        }
        printf("new frame, data offset : %d ---- \n");
    }
    
error:
    display_error("Timing interrupt error, previous not ACKed");
    while(1) {}
}
