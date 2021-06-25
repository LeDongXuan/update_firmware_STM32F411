/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "flash.h"
#include "usart2_dma.h"
//#include "stdlib.h"

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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
__attribute__ ((section(".vector_in_ram")))char vertor_table[1024] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
char find_CTOK()__attribute__((section(".run_in_ram")));
void toggle_led()__attribute__((section(".run_in_ram")));
void update_firmware()__attribute__((section(".run_in_ram")));
void SystemTick_IQR()__attribute__((section(".run_in_ram")));
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void SystemTick_IQR()
{
	__asm("NOP");
}

char data_update[8192] = {0}; // size data 8kB

void update_firmware()
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	receive_data_dma(&data_update, sizeof(data_update));
	HAL_FLASH_Unlock();
	FlashErase(0, 2);

	uint32_t *pdata;

	while(find_CTOK()) {}; // waite for untill data_update receiving 4 characters "CTOK"

	if((sizeof(data_update) % 4) > 0)
			{
				for(int i = 0; i < (sizeof(data_update)/4 + 1 ); i++)
					{
					pdata = (uint32_t*)&data_update[4*i];
					FlashWrite(0x08000000 + 4*i, *pdata); // write data into Flash
					toggle_led();
					}
			}
			else
			{
				for(int i = 0; i < (sizeof(data_update)/4); i++)
					{
					pdata = (uint32_t*)&data_update[4*i];
					FlashWrite(0x08000000 + 4*i, *pdata); // write data into Flash
					toggle_led();
					}
			}
}
char find_CTOK()
	{
		int size = sizeof(data_update)-1;
		for(int i = size ; i > 2 ; i--)
		{
			/* if data_update receive 4 characters "CTOK", break out of the loop while(find_CTOK()) */
			if ((data_update[i] == 'K')&&(data_update[i-1] == 'O')&&(data_update[i-2] == 'T')&&(data_update[i-3] == 'C'))
				return 0;
		}
		return 1;
	}
void toggle_led()
{
	uint32_t *gpio_ODR = (uint32_t*)0x40020C14;
	if(((*gpio_ODR>>15) & 1) )
	{
		*gpio_ODR &= ~(0xFFFF);
		for(int i = 1000; i > 0; i--)
		{

		}
	}
	else
	{
		*gpio_ODR &= ~(0xFFFF);
		*gpio_ODR |= 1 << 15;
		for(int i = 1000; i > 0; i--)
		{

		}
	}
	*gpio_ODR |= 1 << 15;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	memcpy(vertor_table, (void*)(0x08000000), sizeof(vertor_table));
	uint32_t* temp =  (uint32_t*)0x2000003C;
	*temp = (uint32_t)(SystemTick_IQR)|1;
	uint32_t* VTOR = (uint32_t*)(0xe000ed08);
	*VTOR = 0x20000000;

	/* Configure Pin D12 for toggle Blue LED */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	uint32_t *gpio_MODER = (uint32_t*)0x40020C00;
	uint32_t *gpio_OTYPER = (uint32_t*)0x40020C04;
	uint32_t *gpio_OSPEEDR = (uint32_t*)0x40020C08;
	uint32_t *gpio_PUPDR = (uint32_t*)0x40020C0C;
	//uint32_t *gpio_ODR = (uint32_t*)0x40020C14;

	*gpio_MODER &= ~(0b11 << 30);
  	*gpio_MODER |= 0b01 << 30; // output mode
  	*gpio_OTYPER |= 0b0 << 15; // open drain
  	*gpio_OSPEEDR &= ~(0b11 << 30);
  	*gpio_OSPEEDR |= 0b01 << 30; // high speed
  	*gpio_PUPDR |= 0b00 << 30; // no pull up, pull down
  	/* End configure Pin D12 */



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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  update_firmware();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //update_firmware();

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  if (HAL_UART_Init(&huart2) != HAL_OK)
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
