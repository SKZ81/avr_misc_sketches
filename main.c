#include "avr_uart.h"
#include "dht22.h"
#include <util/delay.h>
#include <avr/io.h>

extern uint8_t dht22_data[5];

int main(void) {
    stdout = &avr_uart_output;
    stdin  = &avr_uart_input_echo;
    avr_uart_init();
    dht22_init();
    
    /*printf("Blink led...\n");
    DDRB |= (1 << PORTB5);
    PORTB |= (1 << PORTB5);
    _delay_ms(1000);
    PORTB &= ~(1 << PORTB5);
    printf("...done\n");
    */
    
    while(1) {
        int16_t temp = 0, humid = 0;
        
        switch(dht22_read(&temp, &humid)) {
            case DHT22_OK:
                printf("Temperature : %f° (%d); Humidity : %f%% (%d)\n\n", ((float)temp)/10.0, temp, ((float)humid)/10.0, humid);
                break;
            case DHT22_ERR_NOLOWSTATE:
                printf("ERROR : no low state on DTH22 DATA pin...\n");
                break;
            case DHT22_ERR_CHECKSUM:
                printf("ERROR : checksum calculation\n");
                break;
            case DHT22_ERR_TIMEOUT:
                printf("ERROR : timeout on bit acquisition");
                break;
            default:
                printf("UNKNOW error code (this shoild NEVER happend !!\n");
                while(1) {_delay_ms(500);}
        }
        /*printf("Raw Data : [ %x-%x ; %x-%x / %x ]\n", dht22_data[0],
                                                      dht22_data[1],
                                                      dht22_data[2],
                                                      dht22_data[3],
                                                      dht22_data[4]);*/
        _delay_ms(2000);
    }
    return 0;
}
