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

char buffer[4];

void twoIntsToArray(char *destination, int8_t int1, int8_t int2);
void oneIntToArray(char *destination, uint8_t offset, int8_t integer);
void clearBuffer(void);

void interfaceWrite(void);
void interfaceShowActual(void);

void interfaceMenuInit();

#endif /* INTERFACE_H_ */
