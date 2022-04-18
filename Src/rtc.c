/*
 * rtc.c
 * REAL TIME CLOCK WITH CHRONO HAL WRAPPER
 *
 *
 * 	Created on: 16 APR 2022
 *      Author: Bartosz
 *      varsion: 1.0
 *
 *      changelog:
 *
 */

#include "main.h"
#include "rtc.h"
#include <stdint.h>

void rtcInit(RTCChronoTypeDef *chrono) {
	HAL_RTC_Init(&hrtc);
	chrono->chronoMode = RTC_CHRONO_MODE_INACTIVE;

}

void rtcGetTime(RTCChronoTypeDef *chrono) {

	HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);

	chrono->actual.hour = Time.Hours;

	chrono->actual.minute = Time.Minutes;
	chrono->actual.second = Time.Seconds;

	chrono->actual.decimal = (((255 - Time.SubSeconds) * 100) / 255) / 10; //^ subsecond from RTC is 1/255 of second(becouse of RTC sync predivider)

	chrono->actual.day = Date.Date;
	chrono->actual.month = Date.Month;
	chrono->actual.year = Date.Year;

	//							MOON PHASE COUNT

	float X = (chrono->actual.month + 9) / 12;
	uint32_t A = 4716 + (chrono->actual.year + 2000) + (uint32_t) X;
	float Y = 275 * chrono->actual.month / 9;
	float V = 7 * A / 4;
	float B = 1729279.5 + 367 * (chrono->actual.year + 2000) + (uint32_t) Y
			- (uint32_t) V + chrono->actual.day;
	float Q = (A + 83) / 100;
	uint32_t C = (uint32_t) Q;
	float W = 3 * (C + 1) / 4;
	uint32_t E = (uint32_t) W;
	float JD = B + 38 - E; //julian day
	float MS = 29.5305902778; //average value of synodic month
	float phasePrimary = ((JD / MS) - 0.3033);
	uint32_t phaseInteger = (uint32_t) phasePrimary;
	float phaseFinal = (phasePrimary - (float) phaseInteger) * 100;
	chrono->actual.moonPhase = (uint32_t) phaseFinal;

	// 0 - new moon, 25- I, 50- II(full moon), 75- III

	if (chrono->chronoMode == RTC_CHRONO_MODE_INACTIVE) {
		chrono->chrono.minute = 0;
		chrono->chrono.second = 0;
		chrono->chrono.decimal = 0;
		chrono->chrono.hour = 0;
		chrono->chrono.day = 0;
		chrono->chrono.month = 0;
		chrono->chrono.year = 0;
	}
	if (chrono->chronoMode == RTC_CHRONO_MODE_RUNNING) {

		chrono->chrono.decimal = 0;
		chrono->chrono.second = chrono->actual.second
				- chrono->chronoStart.second;
		chrono->chrono.minute = chrono->actual.minute
				- chrono->chronoStart.minute;
		chrono->chrono.hour = chrono->actual.hour - chrono->chronoStart.hour;
		chrono->chrono.day = chrono->actual.day - chrono->chronoStart.day;
		chrono->chrono.month = chrono->actual.month - chrono->chronoStart.month;

		if (chrono->chrono.decimal < 0) {
			chrono->chrono.second--;
			chrono->chrono.decimal = chrono->chrono.decimal + 10;
		}

		if (chrono->chrono.second < 0) {
			chrono->chrono.minute--;
			chrono->chrono.second = chrono->chrono.second + 60;
		}

		if (chrono->chrono.minute < 0) {
			chrono->chrono.hour--;
			chrono->chrono.minute = chrono->chrono.minute + 60;
		}

		if (chrono->chrono.hour < 0) {
			chrono->chrono.day--;
			chrono->chrono.hour = chrono->chrono.hour + 24;
		}

		if (chrono->chrono.day < 0) {
			chrono->chrono.month--;
			chrono->chrono.day = chrono->chrono.day + 30;
		}

		if (chrono->chrono.month < 0) {
			chrono->chrono.year--;
			chrono->chrono.month = chrono->chrono.month + 12;
		}

		//max chrono count
		if (chrono->chrono.hour >= 5) {
			chrono->chronoMode = RTC_CHRONO_MODE_STOP;
		}
	}

}

void rtcSetTime(RTCChronoTypeDef *chrono) {

	Time.Hours = chrono->actual.hour;
	Time.Minutes = chrono->actual.minute;
	Time.Seconds = chrono->actual.second;
	Date.Date = chrono->actual.day;
	Date.Month = chrono->actual.month;
	Date.Year = chrono->actual.year;
	HAL_RTC_SetTime(&hrtc, &Time, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &Date, RTC_FORMAT_BIN);

//	HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0);

}

void rtcStartChrono(RTCChronoTypeDef *chrono) {

	HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);

	chrono->chronoStart.second = Time.Seconds;
	chrono->chronoStart.minute = Time.Minutes;
	chrono->chronoStart.decimal = (((255 - Time.SubSeconds) * 100) / 255) / 10;
	chrono->chronoStart.hour = Time.Hours;
	chrono->chronoStart.day = Date.Date;
	chrono->chronoStart.month = Date.Month;
	chrono->chronoStart.year = Date.Year;

	chrono->chronoMode = RTC_CHRONO_MODE_RUNNING;
}

void rtcStopChrono(RTCChronoTypeDef *chrono) {

	HAL_RTC_GetTime(&hrtc, &Time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &Date, RTC_FORMAT_BIN);

	chrono->actual.minute = Time.Minutes;
	chrono->actual.second = Time.Seconds;
	chrono->actual.decimal = (((255 - Time.SubSeconds) * 100) / 255) / 10;
	chrono->actual.hour = Time.Hours;
	chrono->actual.day = Date.Date;
	chrono->actual.month = Date.Month;
	chrono->actual.year = Date.Year;

	chrono->chrono.decimal = chrono->actual.decimal - chrono->chronoStart.decimal;
	chrono->chrono.second = chrono->actual.second - chrono->chronoStart.second;
	chrono->chrono.minute = chrono->actual.minute - chrono->chronoStart.minute;
	chrono->chrono.hour = chrono->actual.hour - chrono->chronoStart.hour;
	chrono->chrono.day = chrono->actual.day - chrono->chronoStart.day;
	chrono->chrono.month = chrono->actual.month - chrono->chronoStart.month;
	chrono->chrono.year = chrono->actual.year - chrono->chronoStart.year;

	chrono->chronoMode = RTC_CHRONO_MODE_STOP;
}

void rtcResetChrono(RTCChronoTypeDef *chrono) {

	chrono->chrono.decimal = 0;
	chrono->chrono.minute = 0;
	chrono->chrono.second = 0;
	chrono->chrono.hour = 0;

	chrono->chronoStart.second = 0;
	chrono->chronoStart.minute = 0;
	chrono->chronoStart.decimal = 0;
	chrono->chronoStart.hour = 0;

	chrono->chronoMode = RTC_CHRONO_MODE_INACTIVE;

}

