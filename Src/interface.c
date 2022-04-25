/*
 * interface.c
 *
 *  Created on: 19 kwi 2022
 *      Author: bartosz
 */

#include "interface.h"

void interfaceWrite(void) {

	//								WRITE DATA TO CLOCK
	twoIntsToArray(buffer, chronograph.actual.hour, chronograph.actual.minute);
	if (rtcBlink(&chronograph)) {
		menuItemChangeValue(&menu, CLOCK, 0, buffer, INTER_COLON, 0);
	} else {
		menuItemChangeValue(&menu, CLOCK, 0, buffer, INTER_DISABLED, 0);
	}

	oneIntToArray(buffer, 2, chronograph.actual.second);
	if (rtcBlink(&chronograph)) {
		menuItemChangeValue(&menu, CLOCK_SECONDHAND, 1, buffer, INTER_COLON, 0);
	} else {
		menuItemChangeValue(&menu, CLOCK_SECONDHAND, 1, buffer, INTER_DISABLED,
				0);
	}


	//								WRITE DATA TO CALENDAR
	twoIntsToArray(buffer, chronograph.actual.day, chronograph.actual.month);
	menuItemChangeValue(&menu, CLOCK_DATE, 1, buffer, INTER_DOT, 0);



	//								WRITE DATA TO SETTINGS_CLOCK
	oneIntToArray(buffer, 0, chronograph.actual.hour);
	menuItemChangeValue(&menu, SETTINGS_CLOCK_HOUR, 2, buffer, INTER_COLON, 1);

	oneIntToArray(buffer, 2, chronograph.actual.minute);
	menuItemChangeValue(&menu, SETTINGS_CLOCK_MINUTE, 2, buffer, INTER_COLON,
			1);

	oneIntToArray(buffer, 2, chronograph.actual.second);
	menuItemChangeValue(&menu, SETTINGS_CLOCK_SECONDS, 2, buffer, INTER_COLON,
			1);


	//								WRITE DATA TO SETTINGS_CALENDAR
	oneIntToArray(buffer, 0, chronograph.actual.day);
	menuItemChangeValue(&menu, SETTINGS_CALENDAR_DAY, 2, buffer, INTER_DOT, 1);

	oneIntToArray(buffer, 2, chronograph.actual.month);
	menuItemChangeValue(&menu, SETTINGS_CALENDAR_MONTH, 2, buffer, INTER_DOT,
			1);

	oneIntToArray(buffer, 0, chronograph.actual.year);
	menuItemChangeValue(&menu, SETTINGS_CALENDAR_YEAR, 2, buffer,
			INTER_DISABLED, 1);

	//								WRITE CHRONO TIME
	twoIntsToArray(buffer, chronograph.chrono.minute, chronograph.chrono.second);
	menuItemChangeValue(&menu, CHRONO, CHRONO_LEVEL, buffer, INTER_COLON, 0);

	oneIntToArray(buffer, 0, chronograph.chrono.hour);
	buffer[3] = 'h';
	menuItemChangeValue(&menu, CHRONO_HOURS, CHRONO_HOURS_LEVEL, buffer, INTER_DISABLED, 0);

	oneIntToArray(buffer, 2, chronograph.chrono.hundredth);
	menuItemChangeValue(&menu, CHRONO_HUNDREDTHS, CHRONO_HUNDREDTHS_LEVEL, buffer, INTER_DOT, 0);

	//								WRITE TEMPERATURE


//direct
	uint32_t temperature = adcTemperature(adcRead(&hadc1));

	//function from adc
//	uint32_t temperature = adcAveraging(adcTemperature(adcRead(&hadc1)), temperatureArray, sizeof(temperatureArray));

	//modal
//	temperature

	oneIntToArray(buffer, 0, temperature);
	buffer[2] = '^';
	buffer[3] = 'C';
	menuItemChangeValue(&menu, TEMPERATURE, TEMPERATURE_LEVEL, buffer, INTER_DISABLED, 0);


}

void interfaceShowActual(void) {
	LEDstr(&display, 0, menu.current.value, LED_TRANSITION_DISABLED);
	LEDdot(&display, menu.current.value2);
}

void twoIntsToArray(char *destination, int8_t int1, int8_t int2) {
	memset(destination, 0, sizeof(buffer));

	char halfBuffer1[2];
	char halfBuffer2[2];

	itoa(int1, halfBuffer1, 10);
	itoa(int2, halfBuffer2, 10);

	destination[0] = halfBuffer1[0];
	destination[1] = halfBuffer1[1];
	destination[2] = halfBuffer2[0];
	destination[3] = halfBuffer2[1];

	if (destination[1] == 0) {
		destination[1] = destination[0];
		destination[0] = '0';
	}
	if (destination[3] == 0) {
		destination[3] = destination[2];
		destination[2] = '0';
	}
}

void oneIntToArray(char *destination, uint8_t offset, int8_t integer) {
	memset(destination, 0, sizeof(buffer));
	itoa(integer, &destination[offset], 10);

	if (destination[offset + 1] == 0) {
		destination[offset + 1] = destination[offset];
		destination[offset] = '0';
	}
}

void clearBuffer(void){

}
