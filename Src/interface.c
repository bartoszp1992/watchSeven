/*
 * interface.c
 *
 *  Created on: 19 kwi 2022
 *      Author: bartosz
 */

#include "interface.h"

void _interfaceIntToArray(char *destination, uint8_t offset, int16_t integer);
void _interfaceClear(char *buffer);

void interfaceWrite(void) {

	//								WRITE DATA TO CLOCK

	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 0, chronograph.actual.hour);
	_interfaceIntToArray(buffer, 2, chronograph.actual.minute);

	if (rtcBlink(&chronograph)) {
		menuItemChangeValue(&menu, CLOCK, 0, buffer, INTER_COLON, NOT_EDITABLE);
	} else {
		menuItemChangeValue(&menu, CLOCK, 0, buffer, INTER_DISABLED,
		NOT_EDITABLE);
	}

	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 2, chronograph.actual.second);
	if (rtcBlink(&chronograph)) {
		menuItemChangeValue(&menu, CLOCK_SECONDHAND, 1, buffer, INTER_COLON,
		NOT_EDITABLE);
	} else {
		menuItemChangeValue(&menu, CLOCK_SECONDHAND, 1, buffer, INTER_DISABLED,
		NOT_EDITABLE);
	}

	//								WRITE DATA TO CALENDAR

	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 0, chronograph.actual.day);
	_interfaceIntToArray(buffer, 2, chronograph.actual.month);
	menuItemChangeValue(&menu, CLOCK_DATE, 1, buffer, INTER_DOT, NOT_EDITABLE);

	//								WRITE DATA TO SETTINGS_CLOCK

	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 0, chronograph.actual.hour);
	menuItemChangeValue(&menu, SETTINGS_CLOCK_HOUR, 2, buffer, INTER_COLON,
	EDITABLE);

	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 2, chronograph.actual.minute);
	menuItemChangeValue(&menu, SETTINGS_CLOCK_MINUTE, 2, buffer, INTER_COLON,
	EDITABLE);

	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 2, chronograph.actual.second);
	menuItemChangeValue(&menu, SETTINGS_CLOCK_SECONDS, 2, buffer, INTER_COLON,
	EDITABLE);

	//								WRITE DATA TO SETTINGS_CALENDAR

	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 0, chronograph.actual.day);
	menuItemChangeValue(&menu, SETTINGS_CALENDAR_DAY, 2, buffer, INTER_DOT,
	EDITABLE);

	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 2, chronograph.actual.month);
	menuItemChangeValue(&menu, SETTINGS_CALENDAR_MONTH, 2, buffer, INTER_DOT,
	EDITABLE);

	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 0, chronograph.actual.year);
	menuItemChangeValue(&menu, SETTINGS_CALENDAR_YEAR, 2, buffer,
	INTER_DISABLED, EDITABLE);

	//								WRITE CHRONO TIME

	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 0, chronograph.chrono.minute);
	_interfaceIntToArray(buffer, 2, chronograph.chrono.second);
	menuItemChangeValue(&menu, CHRONO, CHRONO_LEVEL, buffer, INTER_COLON,
	NOT_EDITABLE);

	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 0, chronograph.chrono.hour);
	buffer[3] = 'h';
	menuItemChangeValue(&menu, CHRONO_HOURS, CHRONO_HOURS_LEVEL, buffer,
	INTER_DISABLED, NOT_EDITABLE);

	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 2, chronograph.chrono.hundredth);
	menuItemChangeValue(&menu, CHRONO_HUNDREDTHS, CHRONO_HUNDREDTHS_LEVEL,
			buffer, INTER_DOT, NOT_EDITABLE);

	//								WRITE TEMPERATURE FROM ADC

//	oneIntToArray(buffer, sizeof(buffer), 0, temperature);
//	if(temperature>-10)buffer[2] = '^';
//	buffer[3] = 'C';
//	menuItemChangeValue(&menu, TEMPERATURE, TEMPERATURE_LEVEL, buffer,
//			INTER_DISABLED, 0);
//
//	oneIntToArray(buffer, sizeof(buffer), 0, temperatureCorrection);
//	buffer[3] = '^';
//	menuItemChangeValue(&menu, SETTINGS_CORRECTION_VALUE, SETTINGS_CORRECTION_VALUE_LEVEL, buffer,
//			INTER_DISABLED, EDITABLE);

//								WRITE BME280 DATA
	//temperature
	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 0,
			(bme280.temperatureValue / 100) + temperatureCorrection);
	if (((bme280.temperatureValue / 100) + temperatureCorrection) > -10)
		buffer[2] = '^';
	buffer[3] = 'C';
	menuItemChangeValue(&menu, TEMPERATURE, TEMPERATURE_LEVEL, buffer,
	INTER_DISABLED, NOT_EDITABLE);

	//temperature Correction
	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 0, temperatureCorrection);
	buffer[3] = '^';
	menuItemChangeValue(&menu, SETTINGS_CORRECTION_VALUE,
	SETTINGS_CORRECTION_VALUE_LEVEL, buffer,
	INTER_DISABLED, EDITABLE);

	//pressure
	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 0, bme280.pressureValue / 100);
	menuItemChangeValue(&menu, BAROMETER, BAROMETER_LEVEL, buffer,
	INTER_DISABLED, NOT_EDITABLE);

	//pressure reference(altimeter)
	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 0, bme280.pressureReference);
	menuItemChangeValue(&menu, SETTINGS_REFERENCE_VALUE,
	SETTINGS_REFERENCE_VALUE_LEVEL, buffer,
	INTER_DISABLED, EDITABLE);

	//humidity
	_interfaceClear(buffer);
	_interfaceIntToArray(buffer, 0, bme280.humidityValue);
	buffer[3] = 'H';
	menuItemChangeValue(&menu, HIGROMETER, HIGROMETER_LEVEL, buffer,
	INTER_DISABLED, NOT_EDITABLE);

	//altitude
	_interfaceClear(buffer);
	if (bme280.altitudeValue < -99999) {
//		_interfaceIntToArray(buffer, 0, bme280.altitudeValue / 100);
		menuItemChangeValue(&menu, ALTITUDE, ALTITUDE_LEVEL, "OL  ",
		INTER_DISABLED,
		NOT_EDITABLE);
	}

	else if (bme280.altitudeValue < 0) {
		_interfaceIntToArray(buffer, 0, bme280.altitudeValue / 100);
		menuItemChangeValue(&menu, ALTITUDE, ALTITUDE_LEVEL, buffer,
		INTER_DISABLED,
		NOT_EDITABLE);
	}

	else if (bme280.altitudeValue < 1000) {
		_interfaceIntToArray(buffer, 1, bme280.altitudeValue);
		menuItemChangeValue(&menu, ALTITUDE, ALTITUDE_LEVEL, buffer,
		INTER_DOT,
		NOT_EDITABLE);
	} else if (bme280.altitudeValue < 10000) {
		_interfaceIntToArray(buffer, 0, bme280.altitudeValue);
		menuItemChangeValue(&menu, ALTITUDE, ALTITUDE_LEVEL, buffer,
		INTER_DOT,
		NOT_EDITABLE);
	} else {
		_interfaceIntToArray(buffer, 0, bme280.altitudeValue / 100);
		menuItemChangeValue(&menu, ALTITUDE, ALTITUDE_LEVEL, buffer,
		INTER_DISABLED,
		NOT_EDITABLE);
	}

}

void interfaceShowActual(void) {
	LEDstr(&display, 0, menu.current.value, LED_TRANSITION_DISABLED);
	LEDdot(&display, menu.current.value2);
}

void _interfaceIntToArray(char *destination, uint8_t offset, int16_t integer) {

	if (integer < 10 && integer >= 0) {
		sprintf(destination + offset + 1, "%d", integer);
		*(destination + offset) = '0';
	} else {
		sprintf(destination + offset, "%d", integer);
	}
}

void _interfaceClear(char *buffer) {
	memset(buffer, ' ', 4);
}
