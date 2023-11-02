/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"
#include "stm32g0xx_ll_crc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_dma.h"

#include "stm32g0xx_ll_exti.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "main.h"
#include "7seg.h"
#include "menu.h"
#include "rtc.h"
#include "adc.h"
#include "interrupts.h"
#include "interface.h"
#include "bme280.h"
#include "lis3mdl.h"
//#include "eeprom_emul.h"
#include "backup.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BACKUP_Pin GPIO_PIN_0
#define BACKUP_GPIO_Port GPIOA
#define BACKUP_EXTI_IRQn EXTI0_1_IRQn
#define ENCODER_ACTIVE_Pin GPIO_PIN_1
#define ENCODER_ACTIVE_GPIO_Port GPIOA
#define LED_STANDBY_Pin GPIO_PIN_4
#define LED_STANDBY_GPIO_Port GPIOA
#define LED_WRITE_Pin GPIO_PIN_5
#define LED_WRITE_GPIO_Port GPIOA
#define DISP_0_Pin GPIO_PIN_6
#define DISP_0_GPIO_Port GPIOA
#define DISP_A_Pin GPIO_PIN_7
#define DISP_A_GPIO_Port GPIOA
#define DISP_F_Pin GPIO_PIN_0
#define DISP_F_GPIO_Port GPIOB
#define DISP_1_Pin GPIO_PIN_1
#define DISP_1_GPIO_Port GPIOB
#define DISP_2_Pin GPIO_PIN_2
#define DISP_2_GPIO_Port GPIOB
#define DISP_B_Pin GPIO_PIN_8
#define DISP_B_GPIO_Port GPIOA
#define DISP_3_Pin GPIO_PIN_9
#define DISP_3_GPIO_Port GPIOA
#define DISP_E_Pin GPIO_PIN_6
#define DISP_E_GPIO_Port GPIOC
#define DISP_D_Pin GPIO_PIN_10
#define DISP_D_GPIO_Port GPIOA
#define DISP_DP_Pin GPIO_PIN_11
#define DISP_DP_GPIO_Port GPIOA
#define DISP_C_Pin GPIO_PIN_12
#define DISP_C_GPIO_Port GPIOA
#define DISP_G_Pin GPIO_PIN_15
#define DISP_G_GPIO_Port GPIOA
#define MODE_Pin GPIO_PIN_3
#define MODE_GPIO_Port GPIOB
#define MODE_EXTI_IRQn EXTI2_3_IRQn
#define BUTTON2_Pin GPIO_PIN_4
#define BUTTON2_GPIO_Port GPIOB
#define BUTTON2_EXTI_IRQn EXTI4_15_IRQn
#define BUTTON1_Pin GPIO_PIN_5
#define BUTTON1_GPIO_Port GPIOB
#define BUTTON1_EXTI_IRQn EXTI4_15_IRQn
#define ENC1_Pin GPIO_PIN_6
#define ENC1_GPIO_Port GPIOB
#define ENC1_EXTI_IRQn EXTI4_15_IRQn
#define ENC2_Pin GPIO_PIN_7
#define ENC2_GPIO_Port GPIOB
#define ENC2_EXTI_IRQn EXTI4_15_IRQn
/* USER CODE BEGIN Private defines */



//LEVEL 0
#define CLOCK 0
#define CHRONO 1

#define BAROMETER 3
#define HIGROMETER 4
#define ALTITUDE 5
#define COMPASS 6

#define TEMPERATURE 2
#define SETTINGS 7

//LEVEL 1
#define CLOCK_DATE 0
#define CLOCK_SECONDHAND 1
#define CHRONO_HUNDREDTHS 2
#define CHRONO_HOURS 3
//#define CHRONO_START 4 //deprecated
#define CHRONO_RESET 4
#define SETTINGS_CLOCK 6
#define SETTINGS_CALENDAR 7
#define SETTINGS_CORRECTION 8
#define SETTINGS_REFERENCE 9
#define SETTINGS_SAVE 10

//LEVEL 3
#define SETTINGS_CLOCK_HOUR 0
#define SETTINGS_CLOCK_MINUTE 1
#define SETTINGS_CLOCK_SECONDS 2
#define SETTINGS_CALENDAR_DAY 3
#define SETTINGS_CALENDAR_MONTH 4
#define SETTINGS_CALENDAR_YEAR 5
#define SETTINGS_CORRECTION_VALUE 6
#define SETTINGS_REFERENCE_VALUE 7


//LEVELS
#define SETTINGS_TIME_LEVEL 2
#define SETTINGS_OTHER_LEVEL 1
#define CHRONO_LEVEL 0
#define CHRONO_HOURS_LEVEL 1
#define CHRONO_HUNDREDTHS_LEVEL 1
#define CHRONO_START_LEVEL 1
#define CHRONO_RESET_LEVEL 1
#define TEMPERATURE_LEVEL 0
#define SETTINGS_CORRECTION_VALUE_LEVEL 2
#define SETTINGS_REFERENCE_VALUE_LEVEL 2
#define BAROMETER_LEVEL 0
#define HIGROMETER_LEVEL 0
#define ALTITUDE_LEVEL 0
#define SETTINGS_SAVE_LEVEL 1
#define COMPASS_LEVEL 0

//Editable
#define EDITABLE 1
#define NOT_EDITABLE 0

//ADC array
#define ADC_TEMPERATURE
#define ADC_VOLTAGE


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
