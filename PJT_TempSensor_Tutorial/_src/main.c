/*
 * PJT_TempSensor_Tutorial.c
 *
 * Created: 2020-10-16
 * Author : Hakhyeon Yun
 */ 

#define F_CPU		16000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "sensor.h"
#include "uart.h"

int main(void)
{
	FILE* fpStdio = fdevopen(tranmitUart0String, NULL);
    initUart0(BAURD_RATE_X2_115200);
    while (1) 
    {
		getTemperature();
		printf("Temperature = %.2f // Humidity = %.2f\r\n", temp, humid);
		_delay_ms(500);
    }
}

