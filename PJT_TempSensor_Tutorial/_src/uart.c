/*
 * uart.h
 *
 * Created: 2020-10-16
 * Author : Hakhyeon Yun
 */

#include "uart.h"

#include <avr/io.h>
#include <stdbool.h>

void initUart0(unsigned int UBRR1)
{
	UBRR0H = 0;
	UBRR0L = UBRR1;
	
	UCSR0A |= (1 << U2X1);
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);
}

void transmitUart0(uint8_t data)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

int tranmitUart0String(char ch, FILE *fp)
{
	while (!(UCSR0A & (1 << UDRE0)));

	UDR0 = ch;

	return 0;
}
