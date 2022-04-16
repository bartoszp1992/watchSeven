/*
 * 7seg.h
 *
 *  Created on: 11 mar 2022
 *      Author: bartosz
 */

#ifndef SRC_7DISP_SEG_7DISP_SEG_H_
#define SRC_7DISP_SEG_7DISP_SEG_H_

#include "main.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "stm32g0xx_hal.h"

//USER CONFIGURATION

//logical states settings
#define FIELD_ON 1
#define FIELD_OFF 0
#define SEGMENT_ON 0
#define SEGMENT_OFF 1

//number of fields
#define LED_FIELDS 4

//prescaler for animation
#define LED_TRANSITION_PRE 50

//transition separator size
#define LED_TRANSITION_SEPARATOR_SIZE 1

//END OF USER CONFIGURATION

typedef struct {
	uint16_t SegAPin;
	GPIO_TypeDef *SegAPort;
	uint16_t SegBPin;
	GPIO_TypeDef *SegBPort;
	uint16_t SegCPin;
	GPIO_TypeDef *SegCPort;
	uint16_t SegDPin;
	GPIO_TypeDef *SegDPort;
	uint16_t SegEPin;
	GPIO_TypeDef *SegEPort;
	uint16_t SegFPin;
	GPIO_TypeDef *SegFPort;
	uint16_t SegGPin;
	GPIO_TypeDef *SegGPort;
	uint16_t SegDOTPin;
	GPIO_TypeDef *SegDOTPort;

	uint16_t Field0Pin;
	GPIO_TypeDef *Field0Port;
	uint16_t Field1Pin;
	GPIO_TypeDef *Field1Port;
	uint16_t Field2Pin;
	GPIO_TypeDef *Field2Port;
	uint16_t Field3Pin;
	GPIO_TypeDef *Field3Port;

	uint8_t digitBuffer[LED_FIELDS];
//	uint8_t dotBuffer[LED_FIELDS];
	uint8_t dotPosition;

	//actual display field
	uint8_t actualField;

	//actual moment of transition
	uint8_t transitionStage;

	//flag of direction processing transition(1/2), or inactive(0) - LED_TRANS
	uint8_t transitionStatus;

	char transitionSeparator[LED_TRANSITION_SEPARATOR_SIZE];

	//multiplexer prescaler counter for transitions
	uint16_t prescalerCounter;

	uint8_t digitTransitionBuffer[LED_FIELDS];

} LEDdisplayTypeDef;

//extern TIM_HandleTypeDef htim1;

#if LED_FIELDS == 1
#define MAX_NUMBER 9
#endif

#if LED_FIELDS == 2
#define MAX_NUMBER 99
#endif

#if LED_FIELDS == 3
#define MAX_NUMBER 999
#endif

#if LED_FIELDS == 4
#define MAX_NUMBER 9999
#endif

#define LED_TRANSITION_DIR_LEFT 1
#define LED_TRANSITION_DIR_RIGHT 2
#define LED_TRANSITION_DISABLED 0



#define DOT_DISABLED LED_FIELDS

#if LED_FIELDS >= 1
#define FIELD_0 0
#endif
#if LED_FIELDS >=2
#define FIELD_1 1
#endif
#if LED_FIELDS >= 3
#define FIELD_2 2
#endif
#if LED_FIELDS >= 4
#define FIELD_3 3
#endif

void LEDmultiplexing(LEDdisplayTypeDef *LEDdisplay);
void LEDinit(LEDdisplayTypeDef *LEDdisplay,
#if LED_FIELDS > 0
		GPIO_TypeDef *D0Port, uint16_t D0Pin,
#endif
#if LED_FIELDS > 1
		GPIO_TypeDef *D1Port, uint16_t D1Pin,
#endif
#if LED_FIELDS > 2
		GPIO_TypeDef *D2Port, uint16_t D2Pin,
#endif
#if LED_FIELDS > 3
		GPIO_TypeDef *D3Port, uint16_t D3Pin,
#endif
		GPIO_TypeDef *SAPort, uint16_t SAPin, GPIO_TypeDef *SBPort,
		uint16_t SBPin, GPIO_TypeDef *SCPort, uint16_t SCPin,
		GPIO_TypeDef *SDPort, uint16_t SDPin, GPIO_TypeDef *SEPort,
		uint16_t SEPin, GPIO_TypeDef *SFPort, uint16_t SFPin,
		GPIO_TypeDef *SGPort, uint16_t SGPin, GPIO_TypeDef *SDPPort,
		uint16_t SDPPin
		);
void LEDseparator(LEDdisplayTypeDef *LEDdisplay, char* separator);
void LEDclear(LEDdisplayTypeDef *LEDdisplay);
void LEDchar(LEDdisplayTypeDef *LEDdisplay, uint8_t offset, uint8_t digit);
void LEDint(LEDdisplayTypeDef *LEDdisplay, uint8_t offset, uint16_t number,
		uint8_t transition);
void LEDstr(LEDdisplayTypeDef *LEDdisplay, uint8_t offset, char *str,
		uint8_t transition);
void LEDdot(LEDdisplayTypeDef *LEDdisplay, uint8_t dot/*, uint8_t active*/);

#endif /* SRC_7DISP_SEG_7DISP_SEG_H_ */
