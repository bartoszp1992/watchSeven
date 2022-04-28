/*
 * interface.h
 *
 *  Created on: 19 kwi 2022
 *      Author: bartosz
 */

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <string.h>
#include "main.h"
#include "bme280.h"

#define TEMPERATURE_CORRECTION_MAX 30
#define TEMPERATURE_CORRECTION_MIN -40

extern ADC_HandleTypeDef hadc1;
char buffer[5];

extern bme280TypeDef bme280;

volatile int32_t temperature;
volatile int32_t temperatureCorrection;



void twoIntsToArray(char *destination, uint8_t size, int32_t int1, int32_t int2);
void oneIntToArray(char *destination, uint8_t size, uint8_t offset, int32_t integer);
void clearBuffer(void);

void interfaceWrite(void);
void interfaceShowActual(void);

void interfaceMenuInit();

#endif /* INTERFACE_H_ */
