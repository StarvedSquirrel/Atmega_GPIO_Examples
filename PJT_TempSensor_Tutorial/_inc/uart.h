/*
 * uart.h
 *
 * Created: 2020-10-16
 * Author : Hakhyeon Yun
 */ 
#include <stdio.h>

#define BAURD_RATE_X1_115200			8
#define BAURD_RATE_X2_115200			16

void initUart0(unsigned int UBRR1);
void transmitUart0(uint8_t data);
int tranmitUart0String(char ch, FILE *fp);

