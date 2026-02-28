/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <brainx_rs485.h>
#include <brainx_usbcdc.h>
#include <brainx_sys.h>
#include <eeprom_emul.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

RS485_Device powerManagementBoard;
RS485_Device imuSensorBoard;
RS485_Device tofSensorBoard;
RS485_Device bumperSensorBoard;

extern uint8_t usbcdcRxFlag;
extern uint8_t usbcdcRxOkFlag;
extern uint8_t dataContent;
extern uint8_t data;

extern BrainX_Power_Status brainxPowerStatus;

uint16_t VirtAddVarTab[NB_OF_VAR] = {0x0001};

uint16_t mt_id;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void BrainX_USBCDC_RxCallback();

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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_UART4_Init();
  MX_USB_DEVICE_Init();
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  BrainX_RS485_Init(&powerManagementBoard, PWR_MNG_BOARD_ID, DE_2_GPIO_Port, DE_2_Pin, RE_2_GPIO_Port, RE_2_Pin);

  BrainX_RS485_RX_Enable(&powerManagementBoard);

  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, powerManagementBoard.rxBuffer, (sizeof(powerManagementBoard.rxBuffer)/sizeof(powerManagementBoard.rxBuffer[0])));

  HAL_FLASH_Unlock();

  if(EE_Init() != EE_OK){
	  Error_Handler();
  }

  if(EE_ReadVariable(0x0001, &mt_id) != HAL_OK){
  	Error_Handler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  BrainX_USBCDC_RxCallback();

	  if(brainxPowerStatus == LOW){
		  HAL_TIM_Base_Start_IT(&htim1);
	  }
	  else{
		  HAL_TIM_Base_Stop_IT(&htim1);
		  BrainX_DisplayLED();
	  }

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void BrainX_USBCDC_RxCallback(){
	if(usbcdcRxFlag){
		BrainX_USBCDC_RX_ReceiveData();
		usbcdcRxFlag = 0;
	}

	if(usbcdcRxOkFlag){
		if(dataContent == PWR_STATUS_ID){
			switch(data){
				case 0x00:
					BrainX_UpdateSystemInfo(LOW);
					break;
				case 0x01:
					BrainX_UpdateSystemInfo(OK);
					break;
				case 0x02:
					BrainX_UpdateSystemInfo(CHARGING);
					break;
			}
		}
		else if(dataContent == MT_DRIVER_UNIQUE_BOARD_ID){
			if(EE_WriteVariable(0x0001, data) != HAL_OK){
				Error_Handler();
			}
			else{
				if(EE_ReadVariable(0x0001, &mt_id) != HAL_OK){
					Error_Handler();
				}

			}
			HAL_FLASH_Lock();
		}

		usbcdcRxOkFlag = 0;

	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART2){
		BrainX_RS485_RX_ReceiveData(&powerManagementBoard);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, powerManagementBoard.rxBuffer, 10);
	}
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
