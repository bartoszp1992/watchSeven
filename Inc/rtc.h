/*
 * rtc.h
 *
 *  Created on: 18 kwi 2022
 *      Author: bartosz
 */

#ifndef RTC_H_
#define RTC_H_

#define RTC_CHRONO_MODE_INACTIVE 0
#define RTC_CHRONO_MODE_RUNNING 1
#define RTC_CHRONO_MODE_STOP 2

extern RTC_HandleTypeDef hrtc;

RTC_DateTypeDef Date;
RTC_TimeTypeDef Time;

typedef struct{
	int8_t year;
	int8_t month;
	int8_t day;
	int8_t hour;
	int8_t minute;
	int8_t second;
	int8_t decimal;
	uint32_t moonPhase;
}	RTCTimeTypeDef;

typedef struct{
	RTCTimeTypeDef actual, chronoStart, chrono;
	uint8_t chronoMode;
}RTCChronoTypeDef;




void rtcInit(RTCChronoTypeDef *chrono);
void rtcGetTime(RTCChronoTypeDef *chrono);
void rtcSetTime(RTCChronoTypeDef *chrono);
void rtcStartChrono(RTCChronoTypeDef *chrono);
void rtcStopChrono(RTCChronoTypeDef *chrono);
void rtcResetChrono(RTCChronoTypeDef *chrono);



#endif /* RTC_H_ */
