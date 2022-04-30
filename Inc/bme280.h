/*
 * bme280.h
 *
 *  Created on: 28 kwi 2022
 *      Author: bartosz
 */

#ifndef BME280_H_
#define BME280_H_

#include "stm32g0xx_hal.h"
#include <math.h>

#define BME280_SKIPPED 0x00
#define BME280_OVERSAMPLING_DISABLED 0x1
#define BME280_OVERSAMPLING_X2 0x2
#define BME280_OVERSAMPLING_X4 0x3
#define BME280_OVERSAMPLING_X8 0x4
#define BME280_OVERSAMPLING_X16 0x5

//USER CONFIGURATION
#define BME280_I2C_TIMEOUT 100

#define BME280_OVERSAMPLING_HUM BME280_OVERSAMPLING_X16
#define BME280_OVERSAMPLING_TEM BME280_OVERSAMPLING_X16
#define BME280_OVERSAMPLING_PRE BME280_OVERSAMPLING_X16






typedef struct {
	uint8_t i2cAddr;
//									DATA OUTPUT
	int32_t temperatureValue; //Celsius mDegree
	uint32_t pressureValue; //Pa
	uint32_t humidityValue; //% RH
	int32_t altitudeValue; //meter
	uint32_t pressureReference; //hPa

//									MODE/OVERSAMPLING CONFIG
	uint8_t ctrlMeas;
	uint8_t ctrlHum ;

//									i2c timeout
	uint8_t i2cTimeout;

//									VARIABLES FOR STORE COMPENSATION DATA
	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;

	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;

	unsigned char dig_H1;
	int16_t dig_H2;
	unsigned char dig_H3;
	int16_t dig_H4;
	int16_t dig_H5;
	unsigned char dig_H6;

	int32_t t_fine;


	I2C_HandleTypeDef *I2Chandler;


}bme280TypeDef;

void bme280Init(bme280TypeDef *bme280, I2C_HandleTypeDef *I2Chandler);
void bme280Read(bme280TypeDef *bme280);

#endif /* BME280_H_ */
