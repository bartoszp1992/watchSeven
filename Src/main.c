/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Display/7seg.h"

#include "Menu/menu.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define ENCODER_ACTION_LEFT 0
#define ENCODER_ACTION_RIGHT 1
#define ENCODER_ACTION_ENTER 2
#define ENCODER_ACTION_EXIT 3

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
//												DISPLAY INSTANCE CREATE
LEDdisplayTypeDef display;
//												MENU INSTANCE CREATE
MenuTypeDef menu;


volatile uint16_t counter = 5000;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);
static void MX_ADC1_Init(void);
static void MX_RTC_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void encoderAction(uint8_t direction);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_TIM1_Init();
	MX_ADC1_Init();
	MX_RTC_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim1);

	//												DISPLAY PIN ASSIGNMENT
	LEDinit(&display, DISP_0_GPIO_Port, DISP_0_Pin, DISP_1_GPIO_Port,
	DISP_1_Pin,
	DISP_2_GPIO_Port, DISP_2_Pin, DISP_3_GPIO_Port, DISP_3_Pin,
	DISP_A_GPIO_Port, DISP_A_Pin, DISP_B_GPIO_Port, DISP_B_Pin,
	DISP_C_GPIO_Port, DISP_C_Pin, DISP_D_GPIO_Port, DISP_D_Pin,
	DISP_E_GPIO_Port, DISP_E_Pin, DISP_F_GPIO_Port, DISP_F_Pin,
	DISP_G_GPIO_Port, DISP_G_Pin, DISP_DP_GPIO_Port, DISP_DP_Pin);

	LEDseparator(&display, " ");

	//												MENU INIT

	//												MENU ITEMS CONFIG
	//level 0
	menuItemInit(&menu, CLOCK_ENTRY, CLOCK_LEVEL, NONE, 0, 1);
	menuItemInit(&menu, BAROMETER_ENTRY, BAROMETER_LEVEL, NONE, NONE, NONE);
	menuItemInit(&menu, HIGROMETER_ENTRY, HIGROMETER_LEVEL, NONE, NONE, NONE);
	menuItemInit(&menu, HIGROMETER_ENTRY, HIGROMETER_LEVEL, NONE, NONE, NONE);
	menuItemInit(&menu, TEMPERATURE_ENTRY, TEMPERATURE_LEVEL, NONE, NONE, NONE);
	menuItemInit(&menu, ALTITUDE_ENTRY, ALTITUDE_LEVEL, NONE, NONE, NONE);
	menuItemInit(&menu, AZIMUTH_ENTRY, AZIMUTH_LEVEL, NONE, NONE,  NONE);
	menuItemInit(&menu, CHRONO_ENTRY, CHRONO_LEVEL, NONE, 2, 3);
	menuItemInit(&menu, SETTINGS_ENTRY, SETTINGS_LEVEL, NONE, 4, 7);


	//level 1
	menuItemInit(&menu, CLOCK_DATE_ENTRY, CLOCK_DATE_LEVEL, 0, NONE, NONE);
	menuItemInit(&menu, CLOCK_SECONDHAND_ENTRY, CLOCK_SECONDHAND_LEVEL, 0, NONE,
	NONE);

	//level 2


	//												MENU ITEMS VALUES
	menuItemChangeValue(&menu.items[CLOCK_ENTRY][CLOCK_LEVEL], "1210");
	menuItemChangeValue(&menu.items[CLOCK_DATE_ENTRY][CLOCK_DATE_LEVEL],
			"1604");
	menuItemChangeValue(
			&menu.items[CLOCK_SECONDHAND_ENTRY][CLOCK_SECONDHAND_LEVEL],
			"0059");

	menuItemChangeValue(&menu.items[BAROMETER_ENTRY][BAROMETER_LEVEL], "1022");
	menuItemChangeValue(&menu.items[HIGROMETER_ENTRY][HIGROMETER_LEVEL],
			"43 H");
	menuItemChangeValue(&menu.items[TEMPERATURE_ENTRY][TEMPERATURE_LEVEL],
			"20 ^");

	menuItemChangeValue(&menu.items[ALTITUDE_ENTRY][ALTITUDE_LEVEL], "102");

	menuInit(&menu);

//	menuCurrentItem = menu.items[CLOCK];

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		HAL_Delay(1);

//		LEDint(&display, 0, counter, LED_TRANSITION_DISABLED);
		//HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
			| RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void) {

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */

	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.LowPowerAutoPowerOff = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_1CYCLE_5;
	hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
	hadc1.Init.OversamplingMode = DISABLE;
	hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.Timing = 0x00303D5B;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void) {

	/* USER CODE BEGIN RTC_Init 0 */

	/* USER CODE END RTC_Init 0 */

	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };

	/* USER CODE BEGIN RTC_Init 1 */

	/* USER CODE END RTC_Init 1 */

	/** Initialize RTC Only
	 */
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
	if (HAL_RTC_Init(&hrtc) != HAL_OK) {
		Error_Handler();
	}

	/* USER CODE BEGIN Check_RTC_BKUP */

	/* USER CODE END Check_RTC_BKUP */

	/** Initialize RTC and set the Time and Date
	 */
	sTime.Hours = 0;
	sTime.Minutes = 0;
	sTime.Seconds = 0;
	sTime.SubSeconds = 0;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
	sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	sDate.Month = RTC_MONTH_JANUARY;
	sDate.Date = 1;
	sDate.Year = 0;

	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN RTC_Init 2 */

	/* USER CODE END RTC_Init 2 */

}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 4;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 1999;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA,
			ENCODER_ACTIVE_Pin | LED1_Pin | LED2_Pin | DISP_B_Pin | DISP_3_Pin
					| DISP_D_Pin | DISP_DP_Pin | DISP_A_Pin | DISP_F_Pin
					| DISP_1_Pin | DISP_2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, DISP_G_Pin | DISP_C_Pin | DISP_E_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(DISP_0_GPIO_Port, DISP_0_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : ENCODER_ACTIVE_Pin LED1_Pin LED2_Pin DISP_B_Pin
	 DISP_3_Pin DISP_D_Pin DISP_DP_Pin DISP_A_Pin
	 DISP_F_Pin DISP_1_Pin DISP_2_Pin */
	GPIO_InitStruct.Pin = ENCODER_ACTIVE_Pin | LED1_Pin | LED2_Pin | DISP_B_Pin
			| DISP_3_Pin | DISP_D_Pin | DISP_DP_Pin | DISP_A_Pin | DISP_F_Pin
			| DISP_1_Pin | DISP_2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : DISP_G_Pin DISP_C_Pin DISP_E_Pin */
	GPIO_InitStruct.Pin = DISP_G_Pin | DISP_C_Pin | DISP_E_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : DISP_0_Pin */
	GPIO_InitStruct.Pin = DISP_0_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DISP_0_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : MODE_Pin BUTTON2_Pin BUTTON1_Pin */
	GPIO_InitStruct.Pin = MODE_Pin | BUTTON2_Pin | BUTTON1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : ENC2_Pin ENC1_Pin */
	GPIO_InitStruct.Pin = ENC2_Pin | ENC1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM1) {
		LEDmultiplexing(&display);
	}
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {

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

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
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

}

void encoderAction(uint8_t direction) {

	if (direction == ENCODER_ACTION_ENTER) {
		if (menuSwitch(&menu, ENTER)) {
			LEDstr(&display, 0, menu.current.value, LED_TRANSITION_DIR_LEFT);
		}
	}

	if (direction == ENCODER_ACTION_EXIT) {
		if (menuSwitch(&menu, EXIT)) {
			LEDstr(&display, 0, menu.current.value, LED_TRANSITION_DIR_RIGHT);
		}
	}

	if (direction == ENCODER_ACTION_LEFT) {

		if (menuSwitch(&menu, UP)) {
			LEDstr(&display, 0, menu.current.value, LED_TRANSITION_DIR_LEFT);
		}
		counter--;
	}
	if (direction == ENCODER_ACTION_RIGHT) {

		if (menuSwitch(&menu, DOWN)) {
			LEDstr(&display, 0, menu.current.value, LED_TRANSITION_DIR_RIGHT);
		}
		counter++;
	}
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
