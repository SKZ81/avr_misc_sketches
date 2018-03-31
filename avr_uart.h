/* Original code from : https://github.com/tuupola/avr_demo/tree/master/blog/simple_usart */

#include <stdio.h>

#ifndef _AVR_UART_H
#define _AVR_UART_H

void avr_uart_putchar(char c, FILE *stream);
char avr_uart_getchar(FILE *stream);

void avr_uart_init(void);

extern FILE avr_uart_output;
extern FILE avr_uart_input;
extern FILE avr_uart_input_echo;
#endif
