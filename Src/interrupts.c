/*
 * interrupts.c
 *
 *  Created on: 18 kwi 2022
 *      Author: bartosz
 *
 *      Manages sync/async interrupts from internal timaers, encoder, button and switch
 */

#include "interrupts.h"

void encoderAction(uint8_t direction) {

	if (direction == ENCODER_ACTION_ENTER) {

		if (menu.current.entry == CHRONO_HUNDREDTHS
				&& menu.current.level == CHRONO_HUNDREDTHS_LEVEL) {
			if (chronograph.chronoMode == RTC_CHRONO_MODE_INACTIVE) {
				rtcStartChrono(&chronograph);
			} else if (chronograph.chronoMode == RTC_CHRONO_MODE_RUNNING) {
				rtcStopChrono(&chronograph);
			} else if (chronograph.chronoMode == RTC_CHRONO_MODE_STOP) {
				rtcResumeChrono(&chronograph);
			}

		} else if (menu.current.entry == CHRONO_RESET
				&& menu.current.level == CHRONO_RESET_LEVEL) {
			rtcResetChrono(&chronograph);

		} else if (menu.current.entry == SETTINGS_SAVE
				&& menu.current.level == SETTINGS_SAVE_LEVEL) {

			backupWrite(&chronograph);

		} else {

			if (menuSwitch(&menu, MENU_ENTER)) {
				LEDstr(&display, 0, menu.current.value,
				LED_TRANSITION_DIR_LEFT);
				LEDdot(&display, menu.current.value2);
			} else { //if theres no child under item
				if (menu.current.value3) { //check thats editable
					flags[FLAG_LOCKED] = 1;
				}
			}
		}

	}

	if (direction == ENCODER_ACTION_EXIT) {

		if (!flags[FLAG_LOCKED]) {
			if (menuSwitch(&menu, MENU_EXIT)) {
				LEDstr(&display, 0, menu.current.value,
				LED_TRANSITION_DIR_RIGHT);
				LEDdot(&display, menu.current.value2);
			}
		} else {
			flags[FLAG_LOCKED] = 0;
		}

	}

	if (direction == ENCODER_ACTION_LEFT) {

		if (!flags[FLAG_LOCKED]) {
			if (menuSwitch(&menu, MENU_UP)) {
				LEDstr(&display, 0, menu.current.value,
				LED_TRANSITION_DIR_RIGHT);
				LEDdot(&display, menu.current.value2);
			}
		} else {

			if (menu.current.level == SETTINGS_TIME_LEVEL) {

				switch (menu.current.entry) {
				case SETTINGS_CLOCK_HOUR:
					rtcChangeValue(&chronograph, RTC_CHANGE_HOUR,
					RTC_CHANGE_DIR_DOWN);
					break;
				case SETTINGS_CLOCK_MINUTE:
					rtcChangeValue(&chronograph, RTC_CHANGE_MINUTE,
					RTC_CHANGE_DIR_DOWN);
					break;
				case SETTINGS_CLOCK_SECONDS:
					rtcChangeValue(&chronograph, RTC_CHANGE_SECOND,
					RTC_CHANGE_DIR_DOWN);
					break;
				case SETTINGS_CALENDAR_DAY:
					rtcChangeValue(&chronograph, RTC_CHANGE_DAY,
					RTC_CHANGE_DIR_DOWN);
					break;
				case SETTINGS_CALENDAR_MONTH:
					rtcChangeValue(&chronograph, RTC_CHANGE_MONTH,
					RTC_CHANGE_DIR_DOWN);
					break;
				case SETTINGS_CALENDAR_YEAR:
					rtcChangeValue(&chronograph, RTC_CHANGE_YEAR,
					RTC_CHANGE_DIR_DOWN);
					break;
				case SETTINGS_CORRECTION_VALUE:
					if (temperatureCorrection > TEMPERATURE_CORRECTION_MIN)
						temperatureCorrection--;
					break;
				case SETTINGS_REFERENCE_VALUE:
					if (bme280.pressureReference > PRESSURE_REFERENCE_MIN)
						bme280.pressureReference--;
					break;
				}

			}

		}

	}
	if (direction == ENCODER_ACTION_RIGHT) {

		if (!flags[FLAG_LOCKED]) {
			if (menuSwitch(&menu, MENU_DOWN)) {
				LEDstr(&display, 0, menu.current.value,
				LED_TRANSITION_DIR_LEFT);
				LEDdot(&display, menu.current.value2);
			}
		} else {

			if (menu.current.level == SETTINGS_TIME_LEVEL) {

				switch (menu.current.entry) {
				case SETTINGS_CLOCK_HOUR:
					rtcChangeValue(&chronograph, RTC_CHANGE_HOUR,
					RTC_CHANGE_DIR_UP);
					break;
				case SETTINGS_CLOCK_MINUTE:
					rtcChangeValue(&chronograph, RTC_CHANGE_MINUTE,
					RTC_CHANGE_DIR_UP);
					break;
				case SETTINGS_CLOCK_SECONDS:
					rtcChangeValue(&chronograph, RTC_CHANGE_SECOND,
					RTC_CHANGE_DIR_UP);
					break;
				case SETTINGS_CALENDAR_DAY:
					rtcChangeValue(&chronograph, RTC_CHANGE_DAY,
					RTC_CHANGE_DIR_UP);
					break;
				case SETTINGS_CALENDAR_MONTH:
					rtcChangeValue(&chronograph, RTC_CHANGE_MONTH,
					RTC_CHANGE_DIR_UP);
					break;
				case SETTINGS_CALENDAR_YEAR:
					rtcChangeValue(&chronograph, RTC_CHANGE_YEAR,
					RTC_CHANGE_DIR_UP);
					break;
				case SETTINGS_CORRECTION_VALUE:
					if (temperatureCorrection < TEMPERATURE_CORRECTION_MAX)
						temperatureCorrection++;
					break;
				case SETTINGS_REFERENCE_VALUE:
					if (bme280.pressureReference < PRESSURE_REFERENCE_MAX)
						bme280.pressureReference++;
					break;
				}

			}

		}

	}
}

void GPIO_EXTI_Rising_FallingCallback(uint16_t GPIO_Pin) {

	status[STATUS_INT_SOURCE] = STATUS_INT_SOURCE_EXTI;

	//change status
	if(GPIO_Pin == MODE_Pin){
		if(HAL_GPIO_ReadPin(MODE_GPIO_Port, MODE_Pin)){
			status[STATUS_MODE_SWITCH] = STATUS_MODE_SWITCH_ONDEMAND;
		}else{
			status[STATUS_MODE_SWITCH] = STATUS_MODE_SWITCH_FORCE;
		}
	}


	if (GPIO_Pin == BACKUP_Pin && flags[FLAG_INITIALIZED]) {

		//turn off force mode
		status[STATUS_MODE_SWITCH] = STATUS_MODE_SWITCH_ONDEMAND;

		//save clock if battery is pulled out
		HAL_GPIO_WritePin(LED_WRITE_GPIO_Port, LED_WRITE_Pin, 1);
		rtcGetTime(&chronograph);
		backupWrite(&chronograph);
		HAL_GPIO_WritePin(LED_WRITE_GPIO_Port, LED_WRITE_Pin, 0);
		flags[FLAG_SLEEP] = 1;

	} else {
		TIM2->CNT = 0; // reset wakeup timer if interrupt occurs
	}

	if (GPIO_Pin == ENC1_Pin) {
		if (HAL_GPIO_ReadPin(ENC1_GPIO_Port, ENC1_Pin)
				!= HAL_GPIO_ReadPin(ENC2_GPIO_Port, ENC2_Pin)) {
			encoderAction(ENCODER_ACTION_LEFT);
		}

	}

	if (GPIO_Pin == ENC2_Pin) {
		if (HAL_GPIO_ReadPin(ENC1_GPIO_Port, ENC1_Pin)
				!= HAL_GPIO_ReadPin(ENC2_GPIO_Port, ENC2_Pin)) {
			encoderAction(ENCODER_ACTION_RIGHT);
		}
	}

	if (GPIO_Pin == BUTTON1_Pin) {
		encoderAction(ENCODER_ACTION_ENTER);
	}

	if (GPIO_Pin == BUTTON2_Pin) {
		encoderAction(ENCODER_ACTION_EXIT);
	}

	if (GPIO_Pin == MODE_Pin) {

	}

}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {
	GPIO_EXTI_Rising_FallingCallback(GPIO_Pin);
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {

	GPIO_EXTI_Rising_FallingCallback(GPIO_Pin);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM1) {//led multiplexing and transition - 320Hz
		LEDmultiplexing(&display);
	}
	if (htim->Instance == TIM2) {//going to sleep - 0,2Hz
		flags[FLAG_SLEEP] = 1;
	}
	if (htim->Instance == TIM3) {//calibrating compass - 1Hz
		//								READ REF VOLTAGE
//		adcSetChannel(&hadc1, ADC_CHANNEL_VREFINT);
//		uint32_t voltageRef = adcVoltage(adcRead(&hadc1));
//
//		adcSetChannel(&hadc1, ADC_CHANNEL_TEMPSENSOR);
//		temperature = adcTemperature(adcRead(&hadc1), voltageRef)
//				+ temperatureCorrection;

		lis3mdlGetCalibrationSample(&lis3mdl);

	}
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc) {
	HAL_GPIO_WritePin(LED_STANDBY_GPIO_Port, LED_STANDBY_Pin, 1);
	status[STATUS_INT_SOURCE] = STATUS_INT_SOURCE_RTC;
}
