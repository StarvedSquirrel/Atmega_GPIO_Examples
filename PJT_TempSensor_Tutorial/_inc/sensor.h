/*
 * sensor.h
 *
 * Created: 2020-10-16
 * Author : Hakhyeon Yun
 */ 

#define TEMP_PORT		PORTA
#define TEMP_DDR		DDRA
#define TEMP_PIN		PINA
#define TEMP_INPUT_PIN	1
#define TEMP_ERROR		255

extern float humid, temp;

uint8_t getTemperature(void);
