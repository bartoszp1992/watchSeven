/*
 * bme280.c
 * BOSCH BME280 SIMPLIFIED LIBRARY
 *
 *  Created on: 28 kwi 2022
 *      Author: bartosz
 *      version: 1.0
 *
 *      changelog:
 *
 *
 *
 */

#include <stdint.h>
#include "bme280.h"

//									MAIN I2C ADDRESS, STATUS AND CONTROL REGISTER
#define BME280_ADDR (0x77<<1)
#define BME280_CTRL_MEAS_REG 0xF4
#define BME280_CTRL_HUM_REG 0xF2

//									DATA REGISTERS
#define BME280_HUM_MSB_REG 0xFD
#define BME280_HUM_LSB_REG 0xFE

#define BME280_TEMP_MSB_REG 0xFA
#define BME280_TEMP_LSB_REG 0xFB
#define BME280_TEMP_XLSB_REG 0xFC

#define BME280_PRESS_MSB_REG 0xF7
#define BME280_PRESS_LSB_REG 0xF8
#define BME280_PRESS_XLSB_REG 0xF9

//									STATUS REGISTER
#define BME280_STATUS_REG 0xF3

//									COMPENSATION DATA REGISTERS
#define BME280_DIG_T1_REG 0x88

#define BME280_DIG_P1_REG 0x8E

#define BME280_DIG_H1_REG 0xA1
#define BME280_DIG_H2_REG 0xE1

//									COMP FUNCTIONS DECLARATIONS
int32_t _BME280_compensate_T_int32(bme280TypeDef *bme280, int32_t adc_T);
uint32_t _BME280_compensate_P_int64(bme280TypeDef *bme280, int32_t adc_P);
uint32_t _BME280_compensate_H_int32(bme280TypeDef *bme280, int32_t adc_H);

void bme280Init(bme280TypeDef *bme280, I2C_HandleTypeDef *I2Chandler) {

	bme280->I2Chandler = I2Chandler;

	bme280->i2cTimeout = BME280_I2C_TIMEOUT;

	bme280->ctrlHum = 0x01; //oversampling for humidity disabled
	bme280->ctrlMeas = 0x24; //oversampling for temp, pressure disabled

	uint8_t dig_T[6];
	uint8_t dig_P[18];
	uint8_t dig_H_1;
	uint8_t dig_H_2_6[7];

	//								READING CALIBRATION DATA

	if (HAL_I2C_Mem_Read(bme280->I2Chandler, BME280_ADDR, BME280_DIG_T1_REG, 1,
			dig_T, 6, bme280->i2cTimeout) != HAL_OK) {

	}
	bme280->dig_T1 = ((uint16_t) dig_T[0] | (uint16_t) dig_T[1] << 8);
	bme280->dig_T2 = ((int16_t) dig_T[2] | (int16_t) dig_T[3] << 8);
	bme280->dig_T3 = ((int16_t) dig_T[4] | (int16_t) dig_T[4] << 8);

	HAL_I2C_Mem_Read(bme280->I2Chandler, BME280_ADDR, BME280_DIG_P1_REG, 1,
			dig_P, 18, bme280->i2cTimeout);
	bme280->dig_P1 = ((uint16_t) dig_P[0] | (uint16_t) dig_P[1] << 8);
	bme280->dig_P2 = ((int16_t) dig_P[2] | (int16_t) dig_P[3] << 8);
	bme280->dig_P3 = ((int16_t) dig_P[4] | (int16_t) dig_P[5] << 8);
	bme280->dig_P4 = ((int16_t) dig_P[6] | (int16_t) dig_P[7] << 8);
	bme280->dig_P5 = ((int16_t) dig_P[8] | (int16_t) dig_P[9] << 8);
	bme280->dig_P6 = ((int16_t) dig_P[10] | (int16_t) dig_P[11] << 8);
	bme280->dig_P7 = ((int16_t) dig_P[12] | (int16_t) dig_P[13] << 8);
	bme280->dig_P8 = ((int16_t) dig_P[14] | (int16_t) dig_P[15] << 8);
	bme280->dig_P9 = ((int16_t) dig_P[16] | (int16_t) dig_P[17] << 8);

	HAL_I2C_Mem_Read(bme280->I2Chandler, BME280_ADDR, BME280_DIG_H1_REG, 1,
			&dig_H_1, 1, bme280->i2cTimeout);
	bme280->dig_H1 = ((unsigned char) dig_H_1);

	HAL_I2C_Mem_Read(bme280->I2Chandler, BME280_ADDR, BME280_DIG_H2_REG, 1,
			dig_H_2_6, 7, bme280->i2cTimeout);
	bme280->dig_H2 = ((int16_t) dig_H_2_6[0] | (int16_t) dig_H_2_6[1] << 8);
	bme280->dig_H3 = ((unsigned char) dig_H_2_6[2]);

	bme280->dig_H4 = (((int16_t) dig_H_2_6[3]) << 4
			| (((int16_t) dig_H_2_6[4]) & 0x0f));
	bme280->dig_H5 = (((int16_t) dig_H_2_6[4]) >> 4
			| ((int16_t) dig_H_2_6[5]) << 4);
	bme280->dig_H6 = ((unsigned char) dig_H_2_6[6]);

	HAL_I2C_Mem_Write(bme280->I2Chandler, BME280_ADDR, BME280_CTRL_MEAS_REG, 1,
			&bme280->ctrlMeas, 1, bme280->i2cTimeout); //write temp and press oversampling
	HAL_I2C_Mem_Write(bme280->I2Chandler, BME280_ADDR, BME280_CTRL_HUM_REG, 1,
			&bme280->ctrlHum, 1, bme280->i2cTimeout); //write humidity oversampling


}

void bme280Read(bme280TypeDef *bme280) {
	uint8_t temperatureMSB = 0;
	uint8_t temperatureLSB = 0;
	uint8_t temperatureXLSB = 0;

	uint8_t pressureMSB = 0;
	uint8_t pressureLSB = 0;
	uint8_t pressureXLSB = 0;

	uint8_t humidityMSB = 0;
	uint8_t humidityLSB = 0;

	//adc data
	int32_t temperatureADC = 0;
	int32_t pressureADC = 0;
	int32_t humidityADC = 0;

	//start measure by setting las bit of ctrlMeas register
	uint8_t startConversion = (bme280->ctrlMeas) + 1;
	HAL_I2C_Mem_Write(bme280->I2Chandler, BME280_ADDR, BME280_CTRL_MEAS_REG, 1,
			&startConversion, 1, bme280->i2cTimeout);

	HAL_Delay(70); //wait for measurement

	//	uint8_t status = 0;
//	while ((status >> 3) & 1) {
//		HAL_I2C_Mem_Read(bme280->I2Chandler, BME280_ADDR, BME280_STATUS_REG, 1,
//				&status, 1, bme280->i2cTimeout);
//	}

	//TEMPERATURE

	HAL_I2C_Mem_Read(bme280->I2Chandler, BME280_ADDR, BME280_TEMP_MSB_REG, 1,
			&temperatureMSB, 1, bme280->i2cTimeout);
	HAL_I2C_Mem_Read(bme280->I2Chandler, BME280_ADDR, BME280_TEMP_LSB_REG, 1,
			&temperatureLSB, 1, bme280->i2cTimeout);
	HAL_I2C_Mem_Read(bme280->I2Chandler, BME280_ADDR, BME280_TEMP_XLSB_REG, 1,
			&temperatureXLSB, 1, bme280->i2cTimeout);

	temperatureADC =
			((((int32_t) temperatureMSB << 12) | ((int32_t) temperatureLSB << 4)
					| ((int32_t) temperatureXLSB >> 4)));

	bme280->temperatureValue = (_BME280_compensate_T_int32(bme280,
			temperatureADC));

	//PRESSURE

	HAL_I2C_Mem_Read(bme280->I2Chandler, BME280_ADDR, BME280_PRESS_MSB_REG, 1,
			&pressureMSB, 1, bme280->i2cTimeout);
	HAL_I2C_Mem_Read(bme280->I2Chandler, BME280_ADDR, BME280_PRESS_LSB_REG, 1,
			&pressureLSB, 1, bme280->i2cTimeout);
	HAL_I2C_Mem_Read(bme280->I2Chandler, BME280_ADDR, BME280_PRESS_XLSB_REG, 1,
			&pressureXLSB, 1, bme280->i2cTimeout);

	pressureADC = ((((int32_t) pressureMSB << 12) | ((int32_t) pressureLSB << 4)
			| ((int32_t) pressureXLSB >> 4)));

	bme280->pressureValue = _BME280_compensate_P_int64(bme280, pressureADC)
			/ 256;

	//HUMIDITY

	HAL_I2C_Mem_Read(bme280->I2Chandler, BME280_ADDR, BME280_HUM_MSB_REG, 1,
			&humidityMSB, 1, bme280->i2cTimeout);
	HAL_I2C_Mem_Read(bme280->I2Chandler, BME280_ADDR, BME280_HUM_LSB_REG, 1,
			&humidityLSB, 1, bme280->i2cTimeout);

	humidityADC = (((int32_t) humidityMSB << 8) | (int32_t) humidityLSB);

	bme280->humidityValue = _BME280_compensate_H_int32(bme280, humidityADC)
			/ 1024;

	//ALTITUDE
	/*
	 * barometrical formula:
	 *
	 * p = p0 exp ( -(uhg/RT )
	 * h= -(RT/gu)*ln(p/p0)
	 *
	 * p- pressure[Pa]
	 * p0- reference pressure[Pa]
	 * u- molar mass of air[kg/mol]
	 * h- altitude[m]
	 * g- Earth acceleration[m/s^2]
	 * R- gas constant[J/(mol*K)]
	 * T- air temperature[K]
	 *
	 */

	bme280->altitudeValue = -((8.314
			* ((float) (bme280->temperatureValue / 100) + 273.15))
			/ (10 * 0.0289))
			* log(
					(float) bme280->pressureValue
							/ ((float) bme280->pressureReference * 100));
}

//									COMP FUNCTIONS DEFINITIONS

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value

int32_t _BME280_compensate_T_int32(bme280TypeDef *bme280, int32_t adc_T) {
	int32_t var1, var2, T;
	var1 = ((((adc_T >> 3) - ((int32_t) bme280->dig_T1 << 1)))
			* ((int32_t) bme280->dig_T2)) >> 11;
	var2 = (((((adc_T >> 4) - ((int32_t) bme280->dig_T1))
			* ((adc_T >> 4) - ((int32_t) bme280->dig_T1))) >> 12)
			* ((int32_t) bme280->dig_T3)) >> 14;
	bme280->t_fine = var1 + var2;
	T = (bme280->t_fine * 5 + 128) >> 8;
	return T;
}
// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
uint32_t _BME280_compensate_P_int64(bme280TypeDef *bme280, int32_t adc_P) {
	int64_t var1, var2, p;
	var1 = ((int64_t) bme280->t_fine) - 128000;
	var2 = var1 * var1 * (int64_t) bme280->dig_P6;
	var2 = var2 + ((var1 * (int64_t) bme280->dig_P5) << 17);
	var2 = var2 + (((int64_t) bme280->dig_P4) << 35);
	var1 = ((var1 * var1 * (int64_t) bme280->dig_P3) >> 8)
			+ ((var1 * (int64_t) bme280->dig_P2) << 12);
	var1 = (((((int64_t) 1) << 47) + var1)) * ((int64_t) bme280->dig_P1) >> 33;
	if (var1 == 0) {
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576 - adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = (((int64_t) bme280->dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((int64_t) bme280->dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t) bme280->dig_P7) << 4);
	return (uint32_t) p;
}
// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
// Output value of “47445” represents 47445/1024 = 46.333 %RH
uint32_t _BME280_compensate_H_int32(bme280TypeDef *bme280, int32_t adc_H) {
	int32_t v_x1_u32r;
	v_x1_u32r = (bme280->t_fine - ((int32_t) 76800));

	v_x1_u32r = (((((adc_H << 14) - (((int32_t) bme280->dig_H4) << 20)
			- (((int32_t) bme280->dig_H5) * v_x1_u32r)) + ((int32_t) 16384))
			>> 15)
			* (((((((v_x1_u32r * ((int32_t) bme280->dig_H6)) >> 10)
					* (((v_x1_u32r * ((int32_t) bme280->dig_H3)) >> 11)
							+ ((int32_t) 32768))) >> 10) + ((int32_t) 2097152))
					* ((int32_t) bme280->dig_H2) + 8192) >> 14));
	v_x1_u32r = (v_x1_u32r
			- (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7)
					* ((int32_t) bme280->dig_H1)) >> 4));
	v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
	v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
	return (uint32_t) (v_x1_u32r >> 12);
}

