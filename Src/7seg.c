/*
 * 7seg.c
 * 7 SEGMENT DISPLAY HAL LIBRARY
 *
 *
 *  Created on: 11 mar 2022
 *      Author: Bartosz
 *      varsion: 1.4
 *
 *      changelog:
 *      1.3 - display as structure
 *      1.4 - added transitions
 *      1.5 - manually set logical states for on/off
 *      1.6 - reset stage if transitions overlaps
 *      1.7 - dot position as int instead of 4-int array
 *      1.8 - PIN assignment in LEDinit function, separator set by function
 *      1.9 - changed directive prefix to LED
 *
 */

#include <7seg.h>

//private defines
#define LED_TRANSITION_STAGES LED_FIELDS+LED_TRANSITION_SEPARATOR_SIZE

#define LED_SEG_A 1
#define LED_SEG_B 2
#define LED_SEG_C 3
#define LED_SEG_D 4
#define LED_SEG_E 5
#define LED_SEG_F 6
#define LED_SEG_G 7
#define LED_SEG_DOT 8

//private prototypes
static void _LEDsegmentOn(LEDdisplayTypeDef *LEDdisplay, uint8_t segment);
static void _LEDfieldOn(LEDdisplayTypeDef *LEDdisplay, uint8_t field);
static void _LEDallOff(LEDdisplayTypeDef *LEDdisplay);
static void _LEDwriteCharacter(LEDdisplayTypeDef *LEDdisplay, uint8_t character);
void _LEDtransition(LEDdisplayTypeDef *LEDdisplay, uint8_t transition);

/**
 * @brief  Multiplexing function
 *
 * @note   run cyclic, e.g. in synchronical interrupt
 *
 * @param  Display handler
 *
 * @retval None
 */
void LEDmultiplexing(LEDdisplayTypeDef *LEDdisplay) {

//every intterrupt occurs
	_LEDallOff(LEDdisplay);
	_LEDfieldOn(LEDdisplay, LEDdisplay->actualField);
	_LEDwriteCharacter(LEDdisplay,
			LEDdisplay->digitBuffer[LEDdisplay->actualField]);

	if (LEDdisplay->dotPosition == LEDdisplay->actualField) {
		HAL_GPIO_WritePin(LEDdisplay->SegDOTPort, LEDdisplay->SegDOTPin,
		LED_SEGMENT_ON);
	}

	LEDdisplay->actualField++;
	if (LEDdisplay->actualField >= LED_FIELDS)
		LEDdisplay->actualField = 0;

	LEDdisplay->prescalerCounter++;

//every interrupt divided by prescaler
	if (LEDdisplay->prescalerCounter % LED_TRANSITION_PRE == 0) {

		//transition handling
		if (LEDdisplay->transitionStatus != LED_TRANSITION_DISABLED) {

			//right dir
			if (LEDdisplay->transitionStatus == LED_TRANSITION_DIR_RIGHT) {
				for (uint8_t i = LED_FIELDS - 1; i > 0; i--) {
					LEDdisplay->digitBuffer[i] = LEDdisplay->digitBuffer[i - 1]; //fill with separator fields
				}

				if (LEDdisplay->transitionStage < LED_TRANSITION_SEPARATOR_SIZE) {
					LEDdisplay->digitBuffer[0] =
							LEDdisplay->transitionSeparator[LEDdisplay->transitionStage]; //fill with separator fields
				} else {
					LEDdisplay->digitBuffer[0] =
							LEDdisplay->digitTransitionBuffer[LED_FIELDS - 1
									- (LEDdisplay->transitionStage
											- LED_TRANSITION_SEPARATOR_SIZE)]; //fill with digits from transition buffer
				}

			}

			//left dir
			if (LEDdisplay->transitionStatus == LED_TRANSITION_DIR_LEFT) {
				for (uint8_t i = 0; i < LED_FIELDS - 1; i++) {
					LEDdisplay->digitBuffer[i] = LEDdisplay->digitBuffer[i + 1]; //digits
				}

				if (LEDdisplay->transitionStage < LED_TRANSITION_SEPARATOR_SIZE) {
					LEDdisplay->digitBuffer[LED_FIELDS - 1] =
							LEDdisplay->transitionSeparator[LEDdisplay->transitionStage]; //fill with separator fields
				} else {
					LEDdisplay->digitBuffer[LED_FIELDS - 1] =
							LEDdisplay->digitTransitionBuffer[LEDdisplay->transitionStage
									- LED_TRANSITION_SEPARATOR_SIZE]; //fill with digits from transition buffer
				}

			}

			LEDdisplay->transitionStage++;
			if (LEDdisplay->transitionStage >= LED_TRANSITION_STAGES) {
				LEDdisplay->transitionStage = 0;
				LEDdisplay->transitionStatus = LED_TRANSITION_DISABLED;
			}
		}

	}
}

void _LEDtransition(LEDdisplayTypeDef *LEDdisplay, uint8_t transition) {

	LEDdisplay->transitionStatus = transition;
	LEDdisplay->transitionStage = 0;

}

static void _LEDsegmentOn(LEDdisplayTypeDef *LEDdisplay, uint8_t segment) {

	if (segment == LED_SEG_A)
		HAL_GPIO_WritePin(LEDdisplay->SegAPort, LEDdisplay->SegAPin,
		LED_SEGMENT_ON);
	else if (segment == LED_SEG_B)
		HAL_GPIO_WritePin(LEDdisplay->SegBPort, LEDdisplay->SegBPin,
		LED_SEGMENT_ON);
	else if (segment == LED_SEG_C)
		HAL_GPIO_WritePin(LEDdisplay->SegCPort, LEDdisplay->SegCPin,
		LED_SEGMENT_ON);
	else if (segment == LED_SEG_D)
		HAL_GPIO_WritePin(LEDdisplay->SegDPort, LEDdisplay->SegDPin,
		LED_SEGMENT_ON);
	else if (segment == LED_SEG_E)
		HAL_GPIO_WritePin(LEDdisplay->SegEPort, LEDdisplay->SegEPin,
		LED_SEGMENT_ON);
	else if (segment == LED_SEG_F)
		HAL_GPIO_WritePin(LEDdisplay->SegFPort, LEDdisplay->SegFPin,
		LED_SEGMENT_ON);
	else if (segment == LED_SEG_G)
		HAL_GPIO_WritePin(LEDdisplay->SegGPort, LEDdisplay->SegGPin,
		LED_SEGMENT_ON);
	else if (segment == LED_SEG_DOT)
		HAL_GPIO_WritePin(LEDdisplay->SegDOTPort, LEDdisplay->SegDOTPin,
		LED_SEGMENT_ON);
}

static void _LEDfieldOn(LEDdisplayTypeDef *LEDdisplay, uint8_t field) {

#if LED_FIELDS >=1
	if (field == LED_FIELD_0)
		HAL_GPIO_WritePin(LEDdisplay->Field0Port, LEDdisplay->Field0Pin,
		LED_FIELD_ON);
#endif

#if LED_FIELDS >=2
	else if (field == LED_FIELD_1)
		HAL_GPIO_WritePin(LEDdisplay->Field1Port, LEDdisplay->Field1Pin,
		LED_FIELD_ON);
#endif

#if LED_FIELDS >=3
	else if (field == LED_FIELD_2)
		HAL_GPIO_WritePin(LEDdisplay->Field2Port, LEDdisplay->Field2Pin,
		LED_FIELD_ON);
#endif

#if LED_FIELDS >=4
	else if (field == LED_FIELD_3)
		HAL_GPIO_WritePin(LEDdisplay->Field3Port, LEDdisplay->Field3Pin,
		LED_FIELD_ON);
#endif
}

static void _LEDallOff(LEDdisplayTypeDef *LEDdisplay) {

#if LED_FIELDS >=1
	HAL_GPIO_WritePin(LEDdisplay->Field0Port, LEDdisplay->Field0Pin, LED_FIELD_OFF);
#endif
#if LED_FIELDS >=2
	HAL_GPIO_WritePin(LEDdisplay->Field1Port, LEDdisplay->Field1Pin, LED_FIELD_OFF);
#endif
#if LED_FIELDS >=3
	HAL_GPIO_WritePin(LEDdisplay->Field2Port, LEDdisplay->Field2Pin, LED_FIELD_OFF);
#endif
#if LED_FIELDS >=4
	HAL_GPIO_WritePin(LEDdisplay->Field3Port, LEDdisplay->Field3Pin, LED_FIELD_OFF);
#endif

	HAL_GPIO_WritePin(LEDdisplay->SegAPort, LEDdisplay->SegAPin, LED_SEGMENT_OFF);
	HAL_GPIO_WritePin(LEDdisplay->SegBPort, LEDdisplay->SegBPin, LED_SEGMENT_OFF);
	HAL_GPIO_WritePin(LEDdisplay->SegCPort, LEDdisplay->SegCPin, LED_SEGMENT_OFF);
	HAL_GPIO_WritePin(LEDdisplay->SegDPort, LEDdisplay->SegDPin, LED_SEGMENT_OFF);
	HAL_GPIO_WritePin(LEDdisplay->SegEPort, LEDdisplay->SegEPin, LED_SEGMENT_OFF);
	HAL_GPIO_WritePin(LEDdisplay->SegFPort, LEDdisplay->SegFPin, LED_SEGMENT_OFF);
	HAL_GPIO_WritePin(LEDdisplay->SegGPort, LEDdisplay->SegGPin, LED_SEGMENT_OFF);
	HAL_GPIO_WritePin(LEDdisplay->SegDOTPort, LEDdisplay->SegDOTPin,
	LED_SEGMENT_OFF);

}

static void _LEDwriteCharacter(LEDdisplayTypeDef *LEDdisplay, uint8_t character) {

	if (character >= 48 && character <= 57) {
		switch (character) {
		case '1':

			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			break;
		case '2':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			break;
		case '3':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			break;
		case '4':
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			break;
		case '5':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			break;
		case '6':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;
		case '7':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			break;
		case '8':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;
		case '9':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;
		case '0':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			break;
		}
	} else {
		switch (character) {
		case 'A':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;
		case 'b':
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;

		case 'C':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			break;

		case 'c':
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;

		case 'D':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			break;

		case 'd':
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;

		case 'E':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;

		case 'F':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;
		case 'H':
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;

		case 'h':
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;
		case 'J':
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			break;
		case 'L':
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			break;

		case 'O':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			break;

		case 'o':
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;
		case 'P':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;
		case 'r':
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;
		case 'S':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			break;

		case 't':
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;
		case 'U':
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_C);
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			_LEDsegmentOn(LEDdisplay, LED_SEG_E);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			break;
		case '-':
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;
		case '_':
			_LEDsegmentOn(LEDdisplay, LED_SEG_D);
			break;

		case '^':
			_LEDsegmentOn(LEDdisplay, LED_SEG_A);
			_LEDsegmentOn(LEDdisplay, LED_SEG_B);
			_LEDsegmentOn(LEDdisplay, LED_SEG_F);
			_LEDsegmentOn(LEDdisplay, LED_SEG_G);
			break;
		}
	}
}

/**
 * @brief  Initialise 7 segment display
 *
 * @note   clear display
 *
 * @param  Display handler
 *
 * @rest params GPIO Ports and Pins for each terminal
 *
 * @retval None
 */
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
		uint16_t SDPPin) {

#if LED_FIELDS > 0
	LEDdisplay->Field0Port = D0Port;
	LEDdisplay->Field0Pin = D0Pin;
#endif
#if LED_FIELDS > 1
	LEDdisplay->Field1Port = D1Port;
	LEDdisplay->Field1Pin = D1Pin;
#endif
#if LED_FIELDS > 2
	LEDdisplay->Field2Port = D2Port;
	LEDdisplay->Field2Pin = D2Pin;
#endif
#if LED_FIELDS > 3
	LEDdisplay->Field3Port = D3Port;
	LEDdisplay->Field3Pin = D3Pin;
#endif
	LEDdisplay->SegAPort = SAPort;
	LEDdisplay->SegAPin = SAPin;
	LEDdisplay->SegBPort = SBPort;
	LEDdisplay->SegBPin = SBPin;
	LEDdisplay->SegCPort = SCPort;
	LEDdisplay->SegCPin = SCPin;
	LEDdisplay->SegDPort = SDPort;
	LEDdisplay->SegDPin = SDPin;
	LEDdisplay->SegEPort = SEPort;
	LEDdisplay->SegEPin = SEPin;
	LEDdisplay->SegFPort = SFPort;
	LEDdisplay->SegFPin = SFPin;
	LEDdisplay->SegGPort = SGPort;
	LEDdisplay->SegGPin = SGPin;
	LEDdisplay->SegDOTPort = SDPPort;
	LEDdisplay->SegDOTPin = SDPPin;

	LEDclear(LEDdisplay);
	LEDdisplay->actualField = LED_FIELD_0;
}


/**
 * @brief  Set separator for transition
 *
 * @note   none
 *
 * @param  Display handler
 *
 * @param separator string
 *
 * @retval None
 */
void LEDseparator(LEDdisplayTypeDef *LEDdisplay, char* separator){
	for(uint8_t i = 0; i <= LED_TRANSITION_SEPARATOR_SIZE; i++){
		LEDdisplay->transitionSeparator[i] = separator[i];
	}
}

/**
 * @brief  Clear whole display
 *
 * @note   None
 *
 * @param  Display handler
 *
 * @retval None
 */
void LEDclear(LEDdisplayTypeDef *LEDdisplay) {
	memset(LEDdisplay->digitBuffer, ' ', LED_FIELDS);
	LEDdisplay->dotPosition = LED_DOT_DISABLED;
	_LEDallOff(LEDdisplay);
}

/**
 * @brief  Show one char on selected field
 *
 * @note   Offset starts from 0
 *
 * @param  Display handler
 *
 * @param  selected field
 *
 * @param character to display
 *
 * @retval None
 */
void LEDchar(LEDdisplayTypeDef *LEDdisplay, uint8_t offset, uint8_t character) {
	LEDdisplay->digitBuffer[offset] = character;
}

/**
 * @brief  Show integer starting at selected field
 *
 * @note   Offset starts from 0
 *
 * @param  Display handler
 *
 * @param  cursor position
 *
 * @param  number for display
 *
 * @param transition type: LED_TRANSITION_DIR_LEFT/RIGHT
 *
 * @retval None
 */
void LEDint(LEDdisplayTypeDef *LEDdisplay, uint8_t offset, uint16_t number,
		uint8_t transition) {

	if (number > LED_MAX_NUMBER)
		number = LED_MAX_NUMBER;

	//determine size of destination place
	uint8_t size;
	if (number < 10)
		size = 1;
	else if (number < 100)
		size = 2;
	else if (number < 1000)
		size = 3;
	else if (number < 10000)
		size = 4;

	char buffer[5];

	itoa(number, buffer, 10);

	if (transition == LED_TRANSITION_DISABLED) {
		memcpy(LEDdisplay->digitBuffer, buffer, size);
	} else {
		memcpy(LEDdisplay->digitTransitionBuffer, buffer, size);
	}

	_LEDtransition(LEDdisplay, transition);
}

/**
 * @brief  Show string starting at selected field
 *
 * @note   Offset starts from 0
 *
 * @param  Display handler
 *
 * @param  cursor position
 *
 * @param  text
 *
 * @param LED_TRANSITION_DIR_LEFT/RIGHT
 *
 * @retval None
 */
void LEDstr(LEDdisplayTypeDef *LEDdisplay, uint8_t offset, char *str,
		uint8_t transition) {
	for (uint8_t i = 0; i < sizeof(str); i++) {

		if (transition == LED_TRANSITION_DISABLED) {
			LEDdisplay->digitBuffer[i] = str[i];
		} else {
			LEDdisplay->digitTransitionBuffer[i] = str[i];
			_LEDtransition(LEDdisplay, transition);
		}

	}
}

/**
 * @brief  Show dot on selected field
 *
 * @note   Offset starts from 0
 *
 * @param  Display handler
 *
 * @param  Dot position- USE LED_DOT_DISABLED for turn off dot
 *
 * @retval None
 */
void LEDdot(LEDdisplayTypeDef *LEDdisplay, uint8_t dotNum/*, uint8_t active*/) {

	LEDdisplay->dotPosition = dotNum;
//	if (active == DOT_ACTIVE)
//		LEDdisplay->dotBuffer[dotNum] = 1;
//	else if (active == DOT_INACTIVE)
//		LEDdisplay->dotBuffer[dotNum] = 0;
}

