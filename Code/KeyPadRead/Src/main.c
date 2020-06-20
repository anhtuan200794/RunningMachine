/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MP3_TRAKING_NUM				0x03 // 0..2999
#define MP3_VOLUME					0x06 // 0..30
#define MP3_RESET					0x0C
#define MP3_PLAYBACK				0x0D
#define MP3_PAUSE					0x0E
#define MP3_STOP				0x16

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
char key = 'k';
uint32_t input;
	
int mode = 1;
int nSetup = 1;
bool isStart = false;
bool isSetup = false;
bool isMode = false;
bool isPressStop = false;
bool isOn = true;
int nStopPress = 0;
char * str = "Hello!\r\n";
uint8_t mp3_cmd_buf[10] = {0x7E, 0xFF, 0x06, 0x00, 0x01, 0x0, 0x0, 0x00, 0x00, 0xEF};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
void playVoice(char key);
void MP3_send_cmd (uint8_t cmd, uint16_t high_arg, uint16_t low_arg);
void MP3_stop(void);
void MP3_play(uint8_t index);
void MP3_setVol(uint8_t vol);
void MP3_reset(void);
/* USER CODE BEGIN PFP */

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
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	MP3_reset();
	HAL_Delay(2000);
	MP3_setVol(30);
	HAL_Delay(200);
	MP3_play(1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	//HAL_UART_Transmit(&huart3,(uint8_t *)str,8,150);
  while (1)
  {
    /* USER CODE END WHILE */
				input = GPIOA->IDR;
		if(isOn){
			if(!(input & ((uint32_t)(GPIO_PIN_6 |GPIO_PIN_0)))) // Key A incl :4
        {
          //key = 'a';
					isPressStop = false;
					if(isStart)
						MP3_play(11);
          HAL_Delay(200);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_7 |GPIO_PIN_0)))) // Key B incl: 4
        {
					isPressStop = false;
          if(isStart)
						MP3_play(12);
          HAL_Delay(200);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_8 |GPIO_PIN_0)))) // Key C incl: 12
        {
					isPressStop = false;
          if(isStart)
						MP3_play(13);
          HAL_Delay(200);
        }
        /*  Check Key input from the second pin */
        else if(!(input & ((uint32_t)(GPIO_PIN_4 |GPIO_PIN_1))))
        {
					isPressStop = false;
          key ='5';
          HAL_Delay(150);
        }
        
        else if(!(input & ((uint32_t)(GPIO_PIN_5 |GPIO_PIN_1))))//          key = Program
        {
					isPressStop = false;
//          key = '6';
//          HAL_Delay(150);
					if (isStart == false && isSetup == false) 
						{
						isMode = true;
						switch (mode) {
							case 1:
								MP3_play(3);
								break;
							case 2:
								MP3_play(4);
								break;
							case 12:
								MP3_play(5);
								break;
							case 13:
								MP3_play(6);
								break;
							case 14:
								mode = 0; // mode will be set to 1 in the end of switch
								break;
							default:
								break;
						}
						mode++;
						key = 0;// resset key after process
					}
					HAL_Delay(200);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_6 |GPIO_PIN_1)))) //key Start;
        {
					isPressStop = false;
//          key = '7';
//          HAL_Delay(150);
					if (isStart == false) 
					{ // start
						MP3_play(7);
						isStart = true;
						mode = 1 ; // reset mode
						key = 0;// resset key after process
						nSetup = 1; // reset nSetup
						isSetup = false;
						isMode = false;
					}
					HAL_Delay(200);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_7 |GPIO_PIN_1)))) //key Stop;
        {
//          
//          HAL_Delay(150);
					isPressStop = true;
					MP3_stop();
					if(isPressStop)
					{
						nStopPress++;
					}
					
					if (isStart == true) {  // stop
						MP3_play(2);
					} else if (nStopPress == 5){
						if(isOn)
							isOn = false;
						else
							isOn = true;
						nStopPress = 0;
					}
					isStart = false;
					mode = 1 ; // reset mode
					key = 0;// resset key after process
					nSetup = 1; // reset nSetup
					isSetup = false;
					isMode = false;
					HAL_Delay(200);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_8 |GPIO_PIN_1)))) // Key Mode
        { 
					isPressStop = false;
						if (isStart == false && isMode == false) // setup
						{
              isSetup = true;
              switch (nSetup) {
                case 1:
                  MP3_play(8);
                  break;
                case 2:
                  MP3_play(9);
                  break;
                case 3:
                  MP3_play(10);
                  break;
                case 4:
                  MP3_stop();
                  nSetup = 0;
                  isSetup = false;
                  break;
                default:
                  break;
              }
              nSetup++;
              key = 0;
            }
          HAL_Delay(200);
        }
        /*  Check Key input from the third pin */
        else if(!(input & ((uint32_t)(GPIO_PIN_4 |GPIO_PIN_2))))
        {
					isPressStop = false;
          key = '0';
          HAL_Delay(150);
        }
        
        else if(!(input & ((uint32_t)(GPIO_PIN_5 |GPIO_PIN_2))))
        {
					isPressStop = false;
          key = '1';
          HAL_Delay(150);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_6 |GPIO_PIN_2))))
        {
					isPressStop = false;
          key = '2';
          HAL_Delay(150);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_7 |GPIO_PIN_2))))
        {
					isPressStop = false;
          key = '3';
          HAL_Delay(150);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_8 |GPIO_PIN_2))))
        {
					isPressStop = false;
          key = '4';
          HAL_Delay(150);
        }
        
        /* Check Key input from the fourt pin */
        else if(!(input & ((uint32_t)(GPIO_PIN_6 |GPIO_PIN_3)))) // Key D speed 3
        {
					isPressStop = false;
           if(isStart)
						MP3_play(14);
          HAL_Delay(200);
        }
				else if(!(input & ((uint32_t)(GPIO_PIN_7 |GPIO_PIN_3)))) // Key E speed 6
        {
					isPressStop = false;
           if(isStart)
						MP3_play(15);
          HAL_Delay(200);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_8 |GPIO_PIN_3)))) // Key F speed 9
        {
					isPressStop = false;
           if(isStart)
						MP3_play(16);
          HAL_Delay(200);
        }
		} else {
			if(!(input & ((uint32_t)(GPIO_PIN_6 |GPIO_PIN_0)))) // Key A incl :4
        {
          //key = 'a';
					isPressStop = false;
					
          HAL_Delay(200);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_7 |GPIO_PIN_0)))) // Key B incl: 4
        {
					isPressStop = false;
          
          HAL_Delay(200);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_8 |GPIO_PIN_0)))) // Key C incl: 12
        {
					isPressStop = false;
          
          HAL_Delay(200);
        }
        /*  Check Key input from the second pin */
        else if(!(input & ((uint32_t)(GPIO_PIN_4 |GPIO_PIN_1))))
        {
					isPressStop = false;
         
          HAL_Delay(150);
        }
        
        else if(!(input & ((uint32_t)(GPIO_PIN_5 |GPIO_PIN_1))))//          key = Program
        {
					isPressStop = false;
//          
					HAL_Delay(200);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_6 |GPIO_PIN_1)))) //key Start;
        {
					isPressStop = false;
					
					HAL_Delay(200);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_7 |GPIO_PIN_1)))) //key Stop;
        {
//          
//          HAL_Delay(150);
					isPressStop = true;
					MP3_stop();
					if(isPressStop)
					{
						nStopPress++;
					}
					
					if (isStart == true) {  // stop
						MP3_play(2);
					} else if (nStopPress == 5){
						if(isOn)
							isOn = false;
						else
							isOn = true;
						nStopPress = 0;
					}
					isStart = false;
					mode = 1 ; // reset mode
					key = 0;// resset key after process
					nSetup = 1; // reset nSetup
					isSetup = false;
					isMode = false;
					HAL_Delay(200);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_8 |GPIO_PIN_1)))) // Key Mode
        { 
					isPressStop = false;
					
          HAL_Delay(200);
        }
        /*  Check Key input from the third pin */
        else if(!(input & ((uint32_t)(GPIO_PIN_4 |GPIO_PIN_2))))
        {
					isPressStop = false;
         
          HAL_Delay(150);
        }
        
        else if(!(input & ((uint32_t)(GPIO_PIN_5 |GPIO_PIN_2))))
        {
					isPressStop = false;
          
          HAL_Delay(150);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_6 |GPIO_PIN_2))))
        {
					isPressStop = false;
          
          HAL_Delay(150);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_7 |GPIO_PIN_2))))
        {
					isPressStop = false;
         
          HAL_Delay(150);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_8 |GPIO_PIN_2))))
        {
					isPressStop = false;
          
          HAL_Delay(150);
        }
        
        /* Check Key input from the fourt pin */
        else if(!(input & ((uint32_t)(GPIO_PIN_6 |GPIO_PIN_3)))) // Key D speed 3
        {
					isPressStop = false;

          HAL_Delay(200);
        }
				else if(!(input & ((uint32_t)(GPIO_PIN_7 |GPIO_PIN_3)))) // Key E speed 6
        {
					isPressStop = false;
           
          HAL_Delay(200);
        }
        else if(!(input & ((uint32_t)(GPIO_PIN_8 |GPIO_PIN_3)))) // Key F speed 9
        {
					isPressStop = false;
           
          HAL_Delay(200);
        }
		}
        /* Check Key input from the first pin */
				
				
    /* USER CODE BEGIN 3 */
//		if(key != 'k'){
//			//HAL_UART_Transmit(&huart3,(uint8_t *)&key,1,432);
//			playVoice(key);
//			key = 'k';
//		}
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 
                           PA4 PA5 PA6 PA7 
                           PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
                          |GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  //GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* Calculate checksum
 */
uint16_t MP3_checksum (void) {
	uint16_t sum = 0;
	uint8_t i;
	for (i=1; i<7; i++) {
		sum += mp3_cmd_buf[i];
	}
	return -sum;
}

/* Send command to DFPlayer
 */
void MP3_send_cmd (uint8_t cmd, uint16_t high_arg, uint16_t low_arg) {
	
	uint16_t checksum;

	mp3_cmd_buf[3] = cmd;

	mp3_cmd_buf[5] = high_arg;
	mp3_cmd_buf[6] = low_arg;

	checksum = MP3_checksum();
	mp3_cmd_buf[7] = (uint8_t) ((checksum >> 8) & 0x00FF);
	mp3_cmd_buf[8] = (uint8_t) (checksum & 0x00FF);

	// Send command to UART3
	/*
	for (i=0; i<10; i++) {
		char* data = &mp3_cmd_buf[0];
		int size = sizeof(mp3_cmd_buf);
		HAL_UART_Transmit(&huart3,data,size,2000);
//        USART_SendData(USART3, mp3_cmd_buf[i]);
//        while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
				{
				}
		}
		*/
		int size = sizeof(mp3_cmd_buf);

//		while((status != HAL_OK) || i <= 10)
//			{
				HAL_UART_Transmit(&huart3,mp3_cmd_buf,size,300);
//				i++;
//		}
		//memset(&mp3_cmd_buf[0], 0, sizeof mp3_cmd_buf);
}
void MP3_stop(void)
{
	
	MP3_send_cmd(MP3_STOP,0,0);
	//MP3_send_cmd(MP3_RESET,0,0);
}

void MP3_play(uint8_t index)
{
	MP3_send_cmd(MP3_STOP,0,0);
	HAL_Delay(500);
	MP3_send_cmd(MP3_TRAKING_NUM,0,index);
}
void MP3_setVol(uint8_t vol)
{
	MP3_send_cmd(MP3_VOLUME,0,vol);
}

void MP3_reset()
{
	MP3_send_cmd(MP3_RESET,0,0);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
