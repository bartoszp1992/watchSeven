/*
 * interface.h
 *
 *  Created on: 19 kwi 2022
 *      Author: bartosz
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "bme280.h"
#include "lis3mdl.h"

#define TEMPERATURE_CORRECTION_MAX 30
#define TEMPERATURE_CORRECTION_MIN -40

#define PRESSURE_REFERENCE_MAX 1400
#define PRESSURE_REFERENCE_MIN 600

//DISPLAY DOTS
#define INTER_COLON 1
#define INTER_DOT 0
#define INTER_DISABLED LED_DOT_DISABLED

extern ADC_HandleTypeDef hadc1;
char buffer[6];

extern bme280TypeDef bme280;
extern lis3mdlTypeDef lis3mdl;

volatile int32_t temperature;

volatile int32_t temperatureCorrection;

volatile int16_t angle;



void interfaceWrite(void);
void interfaceShowActual(void);

#endif /* INTERFACE_H_ */
