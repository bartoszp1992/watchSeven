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

/*
 * Software for LEDnode
 *
 * changelog:
 * v1.1 altimeter with centimeters
 * v1.2 backup system, led blinking
 * v1.3 force/ondemand mode as status
 *
 *
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

//												DISPLAY INSTANCE CREATE
LEDdisplayTypeDef display;
//												MENU INSTANCE CREATE
MenuTypeDef menu;
//												CHRONOGRAPF INSTANCE CREATE
RTCChronoTypeDef chronograph;
//												BME280 INSTANCE CREATE
bme280TypeDef bme280;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C1_Init(void);
static void MX_CRC_Init(void);
/* USER CODE BEGIN PFP */

void LPsleep(void);
void LPwakeUp(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);

	adcInit(&hadc1);
	temperatureCorrection = -5;
	bme280.pressureReference = 1013;

//												DISPLAY PIN ASSIGNMENT
	LEDinit(&display, DISP_0_GPIO_Port, DISP_0_Pin, DISP_1_GPIO_Port,
	DISP_1_Pin,
	DISP_2_GPIO_Port, DISP_2_Pin, DISP_3_GPIO_Port, DISP_3_Pin,
	DISP_A_GPIO_Port, DISP_A_Pin, DISP_B_GPIO_Port, DISP_B_Pin,
	DISP_C_GPIO_Port, DISP_C_Pin, DISP_D_GPIO_Port, DISP_D_Pin,
	DISP_E_GPIO_Port, DISP_E_Pin, DISP_F_GPIO_Port, DISP_F_Pin,
	DISP_G_GPIO_Port, DISP_G_Pin, DISP_DP_GPIO_Port, DISP_DP_Pin);

	LEDseparator(&display, "    ");

	//												MENU ITEMS CONFIG
	//level 0
	menuItemInit(&menu, CLOCK, 0, MENU_NONE, 0, 1);
	menuItemInit(&menu, CHRONO, 0, MENU_NONE, CHRONO_HUNDREDTHS,
	CHRONO_RESET);
	menuItemInit(&menu, BAROMETER, 0, MENU_NONE, MENU_NONE, MENU_NONE);
	menuItemInit(&menu, HIGROMETER, 0, MENU_NONE, MENU_NONE, MENU_NONE);
	menuItemInit(&menu, TEMPERATURE, 0, MENU_NONE, MENU_NONE, MENU_NONE);
	menuItemInit(&menu, ALTITUDE, 0, MENU_NONE, MENU_NONE, MENU_NONE);
	menuItemInit(&menu, SETTINGS, 0, MENU_NONE, SETTINGS_CLOCK,
	SETTINGS_SAVE);

	//level 1
	menuItemInit(&menu, CLOCK_DATE, 1, CLOCK, MENU_NONE, MENU_NONE);
	menuItemInit(&menu, CLOCK_SECONDHAND, 1, CLOCK, MENU_NONE, MENU_NONE);
	menuItemInit(&menu, CHRONO_HOURS, 1, CHRONO, MENU_NONE, MENU_NONE);
	menuItemInit(&menu, CHRONO_HUNDREDTHS, 1, CHRONO, MENU_NONE, MENU_NONE);
	menuItemInit(&menu, CHRONO_START, 1, CHRONO, MENU_NONE, MENU_NONE);
	menuItemInit(&menu, CHRONO_RESET, 1, CHRONO, MENU_NONE, MENU_NONE);

	menuItemInit(&menu, SETTINGS_CLOCK, 1, SETTINGS, SETTINGS_CLOCK_HOUR,
	SETTINGS_CLOCK_SECONDS);
	menuItemInit(&menu, SETTINGS_CALENDAR, 1, SETTINGS,
	SETTINGS_CALENDAR_DAY,
	SETTINGS_CALENDAR_YEAR);
	menuItemInit(&menu, SETTINGS_CORRECTION, 1, SETTINGS,
	SETTINGS_CORRECTION_VALUE,
	SETTINGS_CORRECTION_VALUE);
	menuItemInit(&menu, SETTINGS_REFERENCE, 1, SETTINGS,
	SETTINGS_REFERENCE_VALUE,
	SETTINGS_REFERENCE_VALUE);
	menuItemInit(&menu, SETTINGS_SAVE, 1, SETTINGS,
	MENU_NONE,
	MENU_NONE);

	//level 2
	menuItemInit(&menu, SETTINGS_CLOCK_HOUR, 2, SETTINGS_CLOCK, MENU_NONE,
	MENU_NONE);
	menuItemInit(&menu, SETTINGS_CLOCK_MINUTE, 2, SETTINGS_CLOCK, MENU_NONE,
	MENU_NONE);
	menuItemInit(&menu, SETTINGS_CLOCK_SECONDS, 2, SETTINGS_CLOCK,
	MENU_NONE,
	MENU_NONE);
	menuItemInit(&menu, SETTINGS_CALENDAR_DAY, 2, SETTINGS_CALENDAR,
	MENU_NONE, MENU_NONE);
	menuItemInit(&menu, SETTINGS_CALENDAR_MONTH, 2, SETTINGS_CALENDAR,
	MENU_NONE, MENU_NONE);
	menuItemInit(&menu, SETTINGS_CALENDAR_YEAR, 2, SETTINGS_CALENDAR,
	MENU_NONE,
	MENU_NONE);
	menuItemInit(&menu, SETTINGS_CORRECTION_VALUE, 2, SETTINGS_CORRECTION,
	MENU_NONE, MENU_NONE);
	menuItemInit(&menu, SETTINGS_REFERENCE_VALUE, 2, SETTINGS_REFERENCE,
	MENU_NONE, MENU_NONE);

	//												MENU ITEMS CONSTANT VALUES
	//level 0
	menuItemChangeValue(&menu, SETTINGS, 0, "SEt", INTER_DISABLED,
	NOT_EDITABLE);

	//level 1
	menuItemChangeValue(&menu, CHRONO_START, 1, "St  ", INTER_DISABLED,
	NOT_EDITABLE);
	menuItemChangeValue(&menu, CHRONO_RESET, 1, "----", INTER_COLON,
	NOT_EDITABLE);
	menuItemChangeValue(&menu, SETTINGS_CLOCK, 1, "SC  ", INTER_DISABLED,
	NOT_EDITABLE);
	menuItemChangeValue(&menu, SETTINGS_CALENDAR, 1, "SCAL", INTER_DISABLED,
	NOT_EDITABLE);
	menuItemChangeValue(&menu, SETTINGS_CORRECTION, 1, "tC  ",
	INTER_DISABLED, NOT_EDITABLE);
	menuItemChangeValue(&menu, SETTINGS_REFERENCE, 1, "P0  ",
	INTER_DISABLED, NOT_EDITABLE);
	menuItemChangeValue(&menu, SETTINGS_SAVE, 1, "SA  ",
	INTER_DISABLED, NOT_EDITABLE);

	bme280Init(&bme280, &hi2c1);
	HAL_GPIO_WritePin(ENCODER_ACTIVE_GPIO_Port, ENCODER_ACTIVE_Pin, 1);
	HAL_Delay(20);
	menuResetCurrent(&menu);

	//										FLASH RESTORE SECTION

	backupInit();
	backupRestore(&chronograph);

	HAL_Delay(500);
	flags[FLAG_INITIALIZED] = 1;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {

		//												NORMAL WORK
		if (!flags[FLAG_SLEEP] || status[STATUS_MODE_SWITCH] == STATUS_MODE_SWITCH_FORCE) {
			//read RTC time
			rtcGetTime(&chronograph);

			//read sensor
			bme280Read(&bme280);

			//write actual values to menu matrix
			interfaceWrite();

			//display current item
			if (display.transitionStatus == LED_TRANSITION_DISABLED) {
				interfaceShowActual();
			}

			//turn off standby LED
			HAL_GPIO_WritePin(LED_STANDBY_GPIO_Port, LED_STANDBY_Pin, 0);
		}

		//												LOW POWER SECTION
//		if (flags[FLAG_SLEEP] && HAL_GPIO_ReadPin(MODE_GPIO_Port, MODE_Pin)) {
		if (flags[FLAG_SLEEP] && status[STATUS_MODE_SWITCH] == STATUS_MODE_SWITCH_ONDEMAND) {

			//go to sleep
			LPsleep();

			//when interrupt was caused by RTC, go to sleep
			while (status[STATUS_INT_SOURCE] == STATUS_INT_SOURCE_RTC) {
				LPsleep();
			}

			//											AFTER WAKE UP

			//when interrupt was caused by external, wake up
			if (status[STATUS_INT_SOURCE] == STATUS_INT_SOURCE_EXTI) {
				LPwakeUp();
			}

		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

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
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_79CYCLES_5;
  hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_79CYCLES_5;
  hadc1.Init.OversamplingMode = ENABLE;
  hadc1.Init.Oversampling.Ratio = ADC_OVERSAMPLING_RATIO_16;
  hadc1.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_4;
  hadc1.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
  hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* Peripheral clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC);

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  LL_CRC_SetInputDataReverseMode(CRC, LL_CRC_INDATA_REVERSE_NONE);
  LL_CRC_SetOutputDataReverseMode(CRC, LL_CRC_OUTDATA_REVERSE_NONE);
  LL_CRC_SetPolynomialCoef(CRC, LL_CRC_DEFAULT_CRC32_POLY);
  LL_CRC_SetPolynomialSize(CRC, LL_CRC_POLYLENGTH_32B);
  LL_CRC_SetInitialData(CRC, LL_CRC_DEFAULT_CRC_INITVALUE);
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10707DBC;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
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
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

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
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 21;
  sTime.Minutes = 40;
  sTime.Seconds = 0;
  sTime.SubSeconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_APRIL;
  sDate.Date = 20;
  sDate.Year = 20;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the WakeUp
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 20, RTC_WAKEUPCLOCK_CK_SPRE_16BITS) != HAL_OK)
  {
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
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 99;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 17299;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 18499;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 8999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
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
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ENCODER_ACTIVE_Pin|LED_STANDBY_Pin|LED_WRITE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DISP_0_Pin|DISP_A_Pin|DISP_B_Pin|DISP_3_Pin
                          |DISP_D_Pin|DISP_DP_Pin|DISP_C_Pin|DISP_G_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DISP_F_Pin|DISP_1_Pin|DISP_2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DISP_E_GPIO_Port, DISP_E_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : BACKUP_Pin */
  GPIO_InitStruct.Pin = BACKUP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BACKUP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ENCODER_ACTIVE_Pin LED_STANDBY_Pin LED_WRITE_Pin DISP_0_Pin
                           DISP_A_Pin DISP_B_Pin DISP_3_Pin DISP_D_Pin
                           DISP_DP_Pin DISP_C_Pin DISP_G_Pin */
  GPIO_InitStruct.Pin = ENCODER_ACTIVE_Pin|LED_STANDBY_Pin|LED_WRITE_Pin|DISP_0_Pin
                          |DISP_A_Pin|DISP_B_Pin|DISP_3_Pin|DISP_D_Pin
                          |DISP_DP_Pin|DISP_C_Pin|DISP_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DISP_F_Pin DISP_1_Pin DISP_2_Pin */
  GPIO_InitStruct.Pin = DISP_F_Pin|DISP_1_Pin|DISP_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : DISP_E_Pin */
  GPIO_InitStruct.Pin = DISP_E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DISP_E_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MODE_Pin */
  GPIO_InitStruct.Pin = MODE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(MODE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BUTTON2_Pin BUTTON1_Pin */
  GPIO_InitStruct.Pin = BUTTON2_Pin|BUTTON1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : ENC1_Pin ENC2_Pin */
  GPIO_InitStruct.Pin = ENC1_Pin|ENC2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

/* USER CODE BEGIN 4 */
void LPsleep(void) {
	HAL_TIM_Base_Stop_IT(&htim1);
	LEDclear(&display);
	flags[FLAG_SLEEP] = 0;
	HAL_GPIO_WritePin(ENCODER_ACTIVE_GPIO_Port, ENCODER_ACTIVE_Pin, 0);
	HAL_Delay(10);
	HAL_GPIO_WritePin(LED_WRITE_GPIO_Port, LED_WRITE_Pin, 0);
	HAL_GPIO_WritePin(LED_STANDBY_GPIO_Port, LED_STANDBY_Pin, 0);
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,
	PWR_STOPENTRY_WFI);
}

void LPwakeUp(void) {
	SystemClock_Config();
	HAL_GPIO_WritePin(ENCODER_ACTIVE_GPIO_Port, ENCODER_ACTIVE_Pin, 1);
	HAL_Delay(10);
	menuResetCurrent(&menu);
	flags[FLAG_LOCKED] = 0;
	HAL_TIM_Base_Start_IT(&htim1);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
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
