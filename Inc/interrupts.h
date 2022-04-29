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

#define FLAG_LOCKED 0
#define FLAG_SLEEP 1

//volatile uint8_t lockedForSet;
//volatile uint8_t flagSleep;

uint8_t flags[2];
//volatile uint16_t counter;


extern LEDdisplayTypeDef display;
extern MenuTypeDef menu;
extern RTCChronoTypeDef chronograph;

extern volatile int32_t temperature;//adc
extern volatile int32_t temperatureCorrection;




void encoderAction(uint8_t direction);
void GPIO_EXTI_Rising_FallingCallback(uint16_t GPIO_Pin);


#endif /* INTERRUPTS_H_ */
