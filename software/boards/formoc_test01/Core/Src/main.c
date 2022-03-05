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

Vector3f_t FOC(Vector3f_t, float, float);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define POWER	2000
#define SPEED	2
#define OFFSET	0

const uint16_t l_phases[6][3] = {
	{0,		POWER,	0},
	{POWER,	0,		0},
	{POWER,	0, 		0},
	{0,		0,		POWER},
	{0,		0,		POWER},
	{0,		POWER,	0}
};

const uint16_t h_phases[6][3] = {
	{0,		0,		POWER},
	{0,		0,		POWER},
	{0,		POWER,	0},
	{0,		POWER,	0},
	{POWER,	0,		0},
	{POWER,	0, 		0}
};

int8_t curr_phase = 0;

uint8_t foc_flag = 0;

Motor_t motor;
MCP8024_t mcp8024;
PhaseCurrent_t phase_current;

PID_StructTypeDef Id_controller;
PID_StructTypeDef Iq_controller;

Vector3uint16_t fill;

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
  MX_TIM7_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  //PID_Init(&Id_controller, 100, 0, 0, 1, &htim7, 0.000001f);	//?
  //PID_Init(&Iq_controller, 100, 0, 0, 1, &htim7, 0.000001f);	//?

  Motor_Init(&motor, &htim3);
  MCP8024_Init(&mcp8024, CE_GPIO_Port, CE_Pin, &huart1, &htim2, &htim1);
  //PhaseCurrent_Init(&phase_current, &hadc1, &hadc2);

  //HAL_TIM_Base_Start(&htim7);
  //HAL_TIM_Base_Start_IT(&htim16);
  //HAL_TIM_Base_Start_IT(&htim17);

  while(1) {

	  /*MCP8024_SetFill(&mcp8024, POWER, POWER, POWER, 0, 0, 0);
	  //MCP8024_SetFill(&mcp8024, MCP8024_PWM_COMPARE_MAX, MCP8024_PWM_COMPARE_MAX, MCP8024_PWM_COMPARE_MAX, 0, 0, 0);
	  HAL_Delay(1);

	  MCP8024_SetFill(&mcp8024,
			  l_phases[curr_phase][0],
			  l_phases[curr_phase][1],
			  l_phases[curr_phase][2],
			  h_phases[curr_phase][0],
			  h_phases[curr_phase][1],
			  h_phases[curr_phase][2]
	  );
	  HAL_Delay(SPEED);

	  curr_phase = floor(Motor_GetElectricalPosition(&motor)*6);

	  if(!HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) {
		  phase_current.peak_voltage = (const Vector3f_t){0};
		  phase_current.peak_current = (const Vector3f_t){0};
		  phase_current.avg_avg_voltage = (const Vector3f_t){0};
		  phase_current.peak_avg_voltage = (const Vector3f_t){0};
	  }*/

	  //curr_phase++;
	  //curr_phase %=6;

	  //PhaseCurrent_GetCurrent(&phase_current);

	  //Motor_GetElectricalPosition(&motor);

		//if(!foc_flag)
		//	continue;

		float angle = Motor_GetElectricalPosition(&motor)*_2_PI;

	  	//static float angle = 0;
	  	//angle +=0.1;

		//Vector3f_t Iabc = PhaseCurrent_GetCurrent(&phase_current);
		//Vector3f_t Iabc = Vabc;

		float theta = normalize_angle(angle);

		//Vector3f_t Iab0 = clark_transformation(Iabc);
		//Vector3f_t Idq0 = park_transformation(Iab0, theta);

		/*Vector3f_t Vdq0 = {
			PID_Update(&Id_controller, Idq0.x, 0),
			PID_Update(&Iq_controller, Idq0.y, Iq_setpoint),
			0
		};*/

		Vector3f_t Vdq0 = {2, 0, 0};

		Vector3f_t Vab0 = inverse_park_transformation(Vdq0, theta);
		Vector3f_t Vabc = space_vector_modulation(Vab0);

		fill.x = Vabc.x*2000;
		fill.y = Vabc.y*2000;
		fill.z = Vabc.z*2000;

		fill.x = MIN(MAX(fill.x, 40), 1960);
		fill.y = MIN(MAX(fill.y, 40), 1960);
		fill.z = MIN(MAX(fill.z, 40), 1960);

		const float dead_time_prc = 0.8f;

		//MCP8024_SetFill(&mcp8024, Vabc.x*4000, Vabc.y*4000, Vabc.z*4000, Vabc.x*4000, Vabc.y*4000, Vabc.z*4000);
		MCP8024_SetFill(&mcp8024,
				fill.x, fill.y, fill.z,
				fill.x*dead_time_prc, fill.y*dead_time_prc, fill.z*dead_time_prc
		);
		//MCP8024_SetFill(&mcp8024, Vabc.x*4000, Vabc.y*4000, Vabc.z*4000, 0, 0, 0);

		//HAL_Delay(5);

		//foc_flag = 0;

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

Vector3f_t FOC(Vector3f_t Iabc, float rotor_angle, float Iq_setpoint) {
	float theta = normalize_angle(rotor_angle);

	Vector3f_t Iab0 = clark_transformation(Iabc);
	Vector3f_t Idq0 = park_transformation(Iab0, theta);

	Vector3f_t Vdq0 = {
		PID_Update(&Id_controller, Idq0.x, 0),
		PID_Update(&Iq_controller, Idq0.y, Iq_setpoint),
		0
	};

	Vector3f_t Vab0 = inverse_park_transformation(Vdq0, theta);
	Vector3f_t Vabc = space_vector_modulation(Vab0);

	return Vabc;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	MCP8024_RxCpltCallback(&mcp8024);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	PhaseCurrent_ConvCpltCallback(&phase_current, hadc);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if(htim->Instance==TIM16) {
		// 10 kHz
		foc_flag = 1;

	} else if(htim->Instance==TIM17) {
		// 10 Hz
		MCP8024_GetStatus(&mcp8024);
	}/* else if(htim->Instance==TIM2) {

		// pwm period complete
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
	}*/
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

