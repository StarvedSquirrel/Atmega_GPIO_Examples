/*
 * sensor.c
 *
 * Created: 2020-10-16
 * Author : Hakhyeon Yun
 */ 
#define F_CPU		16000000UL

#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>

#include "sensor.h"

float humid = 0, temp = 0;

uint8_t getTemperature(void)
{
	uint8_t bitCnt = 0, byteCnt = 0;
	uint8_t inputByte[5];
	
	memset(inputByte, 0, sizeof(inputByte));
	
	//초기화 : PORT F를 출력으로 설정
	TEMP_DDR |= 0x01;
	//1번 핀 high
	TEMP_PORT |= 0x01;
	_delay_ms(100);
	
	//1번 핀 low
	TEMP_PORT &= ~0x01;
	_delay_ms(18);
	//1번 핀 high
	TEMP_PORT |= 0x01;
	_delay_us(1);
	//1번 핀을 출력 -> 입력으로 변경
	TEMP_DDR &= ~0x01;
	_delay_us(39);
	
	//에러 검출1
	if((TEMP_PIN & 0x01))
		return TEMP_ERROR;
	_delay_us(80);
	//에러 검출2
	if(!(TEMP_PIN & TEMP_INPUT_PIN))
		return TEMP_ERROR;
	_delay_us(80);
	
	//데이터 시트상 총 5개의 바이트가 수신 되어야 하기 때문에 반복문 5번 수행
	for(byteCnt = 0 ; byteCnt <= 4 ; byteCnt++)
	{
		uint8_t result = 0;
		
		//GPIO를 통해 한 비트씩 값을 수신해야 하기 때문에 반복문 8번 수행(1byte == 8bit)
		for(bitCnt = 0 ; bitCnt <= 7 ; bitCnt++)
		{
			//해당 GPIO핀이 low이면 loop 수행, high일 경우 탈출
			//Ex : high case logic : NOT(1 AND 1) = 0이므로 참이 아니므로 반복문 탈출
			//Ex2 : low case logic : NOT(0 AND 1) = 1이므로 참이기 때문에 반복문 수행
			//while loop는 while(조건)으로 되있는데 조건이 참일 경우 반복문을 수행합니다 
			while(!(TEMP_PIN & TEMP_INPUT_PIN));
			
			//DH11 datasheet에서 bit data가 0일 경우 high에서 26-28us동안 high였다 low로 됩니다.
			//bit data가 1일 경우 high에서 70us동안 high를 유지한 후 low로 됩니다.
			//따라서, _delay_us(30) 이후 GPIO PIN 값을 read했을 때 low이면 0이 되고, high로 유지하고 있으면 1이 됩니다.
			_delay_us(30);
			
			//GPIO pin이 high일 경우
			if(TEMP_PIN & TEMP_INPUT_PIN)
				result |= (1 << (7 - bitCnt));
			
			//다음 데이터 수신 준비를 위해 low가 될 때 까지 대기
			while(TEMP_PIN & TEMP_INPUT_PIN);
		}
		//8번 반복문으로 얻어진 데이터를 배열 변수에 저장
		inputByte[byteCnt] = result;
	}
	
	//포트 초기화
	TEMP_DDR |= TEMP_INPUT_PIN;
	TEMP_PORT |= TEMP_INPUT_PIN;
	_delay_ms(100);
	
	//데이터 정리
	/*
	inputByte[0] = 습도 정수
	inputByte[1] = 습도 소수
	inputByte[2] = 온도 정수
	inputByte[3] = 습도 정수
	inputByte[4] = checksum
	*/
	if ((0xff &(inputByte[0] + inputByte[1] + inputByte[2] + inputByte[3])) == inputByte[4])
	{
		humid = (float)(inputByte[0] + inputByte[1]);
		
		if(!(inputByte[2] & 0x80))
		temp = (float)(inputByte[2] + inputByte[3]);
		else
		{
			inputByte[2] = inputByte[2] & 0x7f;
			temp = -(float)(inputByte[2] + inputByte[3]);
		}
		return 0;
	}
	
	return TEMP_ERROR;
}

