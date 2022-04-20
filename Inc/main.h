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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include "main.h"
#include "7seg.h"
#include "menu.h"
#include "rtc.h"
#include "interrupts.h"
#include "interface.h"
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
#define INPUT_Pin GPIO_PIN_0
#define INPUT_GPIO_Port GPIOA
#define ENCODER_ACTIVE_Pin GPIO_PIN_1
#define ENCODER_ACTIVE_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_4
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_5
#define LED2_GPIO_Port GPIOA
#define DISP_B_Pin GPIO_PIN_6
#define DISP_B_GPIO_Port GPIOA
#define DISP_3_Pin GPIO_PIN_7
#define DISP_3_GPIO_Port GPIOA
#define DISP_G_Pin GPIO_PIN_0
#define DISP_G_GPIO_Port GPIOB
#define DISP_C_Pin GPIO_PIN_1
#define DISP_C_GPIO_Port GPIOB
#define DISP_E_Pin GPIO_PIN_2
#define DISP_E_GPIO_Port GPIOB
#define DISP_D_Pin GPIO_PIN_8
#define DISP_D_GPIO_Port GPIOA
#define DISP_DP_Pin GPIO_PIN_9
#define DISP_DP_GPIO_Port GPIOA
#define DISP_0_Pin GPIO_PIN_6
#define DISP_0_GPIO_Port GPIOC
#define DISP_A_Pin GPIO_PIN_10
#define DISP_A_GPIO_Port GPIOA
#define DISP_F_Pin GPIO_PIN_11
#define DISP_F_GPIO_Port GPIOA
#define DISP_1_Pin GPIO_PIN_12
#define DISP_1_GPIO_Port GPIOA
#define DISP_2_Pin GPIO_PIN_15
#define DISP_2_GPIO_Port GPIOA
#define MODE_Pin GPIO_PIN_3
#define MODE_GPIO_Port GPIOB
#define MODE_EXTI_IRQn EXTI2_3_IRQn
#define BUTTON2_Pin GPIO_PIN_4
#define BUTTON2_GPIO_Port GPIOB
#define BUTTON2_EXTI_IRQn EXTI4_15_IRQn
#define BUTTON1_Pin GPIO_PIN_5
#define BUTTON1_GPIO_Port GPIOB
#define BUTTON1_EXTI_IRQn EXTI4_15_IRQn
#define ENC2_Pin GPIO_PIN_6
#define ENC2_GPIO_Port GPIOB
#define ENC2_EXTI_IRQn EXTI4_15_IRQn
#define ENC1_Pin GPIO_PIN_7
#define ENC1_GPIO_Port GPIOB
#define ENC1_EXTI_IRQn EXTI4_15_IRQn
/* USER CODE BEGIN Private defines */
//LEVEL 0
#define CLOCK 0
#define BAROMETER 1
#define HIGROMETER 2
#define TEMPERATURE 3
#define ALTITUDE 4
#define AZIMUTH 5
#define CHRONO 6
#define SETTINGS 7

//LEVEL 1
#define CLOCK_DATE 0
#define CLOCK_SECONDHAND 1
#define CHRONO_HUNDREDTHS 2
#define CHRONO_HOURS 3
#define CHRONO_START 4
#define CHRONO_RESET 5
#define SETTINGS_CLOCK 6
#define SETTINGS_CALENDAR 7
#define SETTINGS_REFERENCE 8
#define SETTINGS_CORRECTION 9

//LEVEL 3
#define SETTINGS_CLOCK_HOUR 0
#define SETTINGS_CLOCK_MINUTE 1
#define SETTINGS_CLOCK_SECONDS 2
#define SETTINGS_CALENDAR_DAY 3
#define SETTINGS_CALENDAR_MONTH 4
#define SETTINGS_CALENDAR_YEAR 5

//LEVELS
#define SETTINGS_TIME_LEVEL 2
#define SETTINGS_OTHER_LEVEL 1
#define CHRONO_LEVEL 0
#define CHRONO_HOURS_LEVEL 1
#define CHRONO_HUNDREDTHS_LEVEL 1
#define CHRONO_START_LEVEL 1
#define CHRONO_RESET_LEVEL 1

//DISPLAY DOTS
#define INTER_COLON 1
#define INTER_DOT 0
#define INTER_DISABLED LED_DOT_DISABLED



/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
