/*
 * interrupts.h
 *
 *  Created on: 18 kwi 2022
 *      Author: bartosz
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include <stdint.h>
#include "main.h"
//#include "rtc.h"


#define ENCODER_ACTION_LEFT 0
#define ENCODER_ACTION_RIGHT 1
#define ENCODER_ACTION_ENTER 2
#define ENCODER_ACTION_EXIT 3

//flag numbers
#define FLAG_LOCKED 0 // stores 1 if menu scrolling is locked due to value setting
#define FLAG_SLEEP 1 // stores 1 if watch will go to sleep in next loop pass- set by timer2
#define FLAG_INITIALIZED 2

//status numbers
#define STATUS_INT_SOURCE 0 // stores info what caused interrupt and wake up MCU
#define STATUS_MODE_SWITCH 1 //stores info about set mode

//status variants
#define STATUS_INT_SOURCE_RTC 0
#define STATUS_INT_SOURCE_EXTI 1

#define STATUS_MODE_SWITCH_ONDEMAND 1
#define STATUS_MODE_SWITCH_FORCE 0


volatile uint8_t flags[3];
volatile uint8_t status[2];


extern LEDdisplayTypeDef display;
extern MenuTypeDef menu;
extern RTCChronoTypeDef chronograph;

extern volatile int32_t temperature;//adc
extern volatile int32_t temperatureCorrection;




void encoderAction(uint8_t direction);
void GPIO_EXTI_Rising_FallingCallback(uint16_t GPIO_Pin);
extern void LPsleep(void);


#endif /* INTERRUPTS_H_ */
