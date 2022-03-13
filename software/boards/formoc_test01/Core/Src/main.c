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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "pid.h"
#include "utilities.h"
#include "transformations.h"

#include "mcp8024.h"
#include "phase_current.h"
#include "motor.h"

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

struct {
	uint8_t foc_loop : 1;
	uint8_t mcp8024_status : 1;
} flags = {0};

Motor_t motor;
MCP8024_t mcp8024;
PhaseCurrent_t phase_current;

PID_StructTypeDef Id_controller;
PID_StructTypeDef Iq_controller;

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

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_TIM15_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  PID_Init(&Id_controller, 0.5f, 0.6f, 0.f, 1.f, 1.f);
  PID_Init(&Iq_controller, 0.75f, 0.45f, 0.f, 5.f, 1.f);

  MCP8024_Init(&mcp8024, CE_GPIO_Port, CE_Pin, &huart1, &htim2, &htim1);
  PhaseCurrent_Init(&phase_current, &hadc1, &hadc2);

  MCP8024_SetFill(&mcp8024, (float32x3_t){.3f, 0.f, 0.f});
  HAL_Delay(200);
  Motor_Init(&motor, &htim3, 0.0775f);

  __HAL_TIM_SET_COUNTER(mcp8024.mosfet_l_timer, 0);
  __HAL_TIM_SET_COUNTER(mcp8024.mosfet_h_timer, 0);
  __HAL_TIM_SET_COUNTER(&htim15, 2400);
  HAL_TIM_Base_Start(&htim15);

  HAL_TIM_Base_Start_IT(&htim16);
  //HAL_TIM_Base_Start_IT(&htim17);

  while(1) {

	  //GPIOC->BSRR = GPIO_PIN_6;
	  //GPIOC->BRR = GPIO_PIN_6;

	  if(!HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
		  PhaseCurrent_Reset(&phase_current);

	  /*if(flags.mcp8024_status) {
		  flags.mcp8024_status = 0;

		  MCP8024_GetStatus(&mcp8024);
	  }*/

	  if(flags.foc_loop) {
	  //if(PhaseCurrent_IsReady(&phase_current)) {
		  flags.foc_loop = 0;

		  float angle = Motor_GetElectricalPosition(&motor)*_2_PI;
		  float theta = normalize_angle(angle);

		  float32x3_t Iabc = PhaseCurrent_GetCurrent(&phase_current);

		  float32x3_t Iab0 = clark_transformation(Iabc);
		  float32x3_t Idq0 = park_transformation(Iab0, theta);

		  float32x3_t Vdq0 = {
				  PID_Update(&Id_controller, Idq0.x, 0),
				  PID_Update(&Iq_controller, Idq0.y, 3.f),
				  0
		  };

		  //Vector3f_t Vdq0 = {2, 0, 0};

		  Vdq0.x = (1.f - Vdq0.x)*0.5f;
		  Vdq0.y = (1.f - Vdq0.y)*0.5f;
		  Vdq0.z = (1.f - Vdq0.z)*0.5f;

		  float32x3_t Vab0 = inverse_park_transformation(Vdq0, theta);
		  float32x3_t Vabc = space_vector_modulation(Vab0);

		  /*Vector3f_t Vabc = inverse_clark_transformation(Vab0);
		  Vabc.x = (1.f - Vabc.x)*0.5f;
		  Vabc.y = (1.f - Vabc.y)*0.5f;
		  Vabc.z = (1.f - Vabc.z)*0.5f;*/

		  //Vabc = (const Vector3f_t){0.f, 0.f, 0.f};

		  MCP8024_SetFill(&mcp8024, Vabc);
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 10;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	MCP8024_RxCpltCallback(&mcp8024);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	PhaseCurrent_ConvCpltCallback(&phase_current, hadc);

	GPIOC->BSRR = GPIO_PIN_6;
	GPIOC->BRR = GPIO_PIN_6;

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if(htim->Instance==TIM16) {
		// 20 kHz
		flags.foc_loop = 1;
	} else if(htim->Instance==TIM17) {
		// 1 Hz
		flags.mcp8024_status = 1;
	}

}

/*void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance==TIM1) {
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
	}

}*/

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
  while (1)
  {
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

