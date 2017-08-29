#include "headers.h"
#include "dsp.h"

volatile bool FrameReady = false;
bool ButtonPressed = false;


uint8_t NumberPressed = 0;
void SystemClock_Config(void);

//uint32_t* Frame1 = (uint32_t*)SDRAM_START_ADR,Frame2 = (uint32_t*)(SDRAM_START_ADR + 153600);

//uint32_t Address;
//uint8_t wBuffer[32],rBuffer[32];
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{

  /* Prevent unused argument(s) compilation warning */
  HAL_SDRAM_Write_16b(&hsdram1,(uint32_t*)SDRAM_START_ADR,(uint16_t*)FrameBuffer,76800);

	FrameReady = true;
	//__HAL_DCMI_ENABLE(hdcmi);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_DCMI_FrameEventCallback could be implemented in the user file
   */
}
int main(void)
{
  // Reset of all peripherals, Initializes the Flash interface and the Systick
  HAL_Init();
  // Configure the system clock
  SystemClock_Config();
  // Initialize all configured peripherals
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_DCMI_Init();
  MX_I2C1_Init();
  MX_SPI5_Init();
	MX_FMC_Init();
  
  LCD_ILI9341_Init();
	
//	sprintf((char*)wBuffer,"test data!");
//  /* USER CODE BEGIN 2 */
//	HAL_SDRAM_Write_8b(&hsdram1,&Address,wBuffer,(uint32_t)strlen((char*)wBuffer));
//  /* USER CODE END 2 */
//	HAL_SDRAM_Read_8b(&hsdram1,&Address,rBuffer,(uint32_t)strlen((char*)wBuffer));
  
  // LCD init page
  LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_Landscape_2);
  LCD_ILI9341_Fill(ILI9341_COLOR_BLACK);
	
	LCD_ILI9341_Puts(20, 55, "Configuring camera", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
	LCD_ILI9341_DrawRectangle(99, 110, 221, 130, ILI9341_COLOR_WHITE);
  
  
  GPIOSetHigh(LEDS_PORT,LED_GREEN_PIN | LED_RED_PIN);
  
  // OV7670 configuration
	if(OV7670Init() == true)
  {
		LCD_ILI9341_Puts(100, 165, "Failed", &LCD_Font_16x26, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
		LCD_ILI9341_Puts(20, 200, "Push reset button", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
		while(1)
    {
      
		}
	}		
	else
  {
		LCD_ILI9341_Puts(100, 165, "Success", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);   
    // LCD welcome page
    LCD_ILI9341_Fill(ILI9341_COLOR_BLACK);
    LCD_ILI9341_Puts(60, 110, "Press button", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
    //HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_CONTINUOUS,(uint32_t)&FrameBuffer,(uint32_t)(IMG_ROWS * IMG_COLUMNS * 2/4));
	}
	LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_Landscape_1);
	
	//int16_t GausBlurKernel[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };

																												
	//const int16_t SobelXDirections[] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };

															 
	//const int16_t SobelYDirections[] = { -1, -2, -1, 0, 0, 0, 1,  2,  1 };
	
	//HAL_StatusTypeDef result = HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_CONTINUOUS,(uint32_t)&FrameBuffer,(uint32_t)(IMG_ROWS * IMG_COLUMNS * 2/4));
	
	int16_t GausBlurKernel[25] = { 1, 4, 	7, 	4, 1,
																4, 16, 26, 16, 4,
																7, 26, 41, 26, 7,
																4, 16, 26, 16, 4,
																1,  4,  7,  4, 1};
	
  while (1)
  {
    if((GPIOReadPin(BUTTON_PORT,BUTTON_PIN) == GPIO_PIN_SET) && !ButtonPressed)
    {
      ButtonPressed = true;
			NumberPressed += 1;
			NumberPressed %= 4;
			HAL_StatusTypeDef result = HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_SNAPSHOT,(uint32_t)&FrameBuffer,(uint32_t)(IMG_ROWS * IMG_COLUMNS * 2/4));
			//DCMI->CR |= DCMI_CR_ENABLE;
			switch(result)
			{
				case HAL_OK:
					// = true;
					break;
				case HAL_ERROR:
					//FrameReady = false;
					break;
				case HAL_BUSY:
					//FrameReady = true;
					break;
				case HAL_TIMEOUT:
					//FrameReady = false;
					break;
				
			}
      
    }
    else if(GPIOReadPin(BUTTON_PORT,BUTTON_PIN) == GPIO_PIN_RESET)
    {
      ButtonPressed = false;
    }
    //LCD_ILI9341_DisplayImage((uint16_t*)FrameBuffer);
    if(FrameReady == true)
    {		
			// Show camera image
			
			//filtered buffer
			
			
			//grayscale(FrameBuffer,IMG_COLUMNS, IMG_ROWS, filteredBuffer);
			if(NumberPressed == 3)
			{
				test(FrameBuffer);
			}
			else if(NumberPressed == 2)
			{
				uint16_t buffer[240 * 100];
				
				convolution(FrameBuffer, GausBlurKernel, buffer);
				memcpy((uint16_t*)FrameBuffer, buffer, sizeof(buffer));
				
				convolution(&FrameBuffer[240 * 100], GausBlurKernel, buffer);
				memcpy((uint16_t*)&FrameBuffer[240 * 100], buffer, sizeof(buffer));
				
			}
			else if(NumberPressed == 0)
			{
					grayscale(FrameBuffer);
			}
			// else default image
			
			
			LCD_ILI9341_DisplayImage((uint16_t*)FrameBuffer);
			
			//LCD_ILI9341_DisplayImageSDRAM(0);
			
			FrameReady = false;
			
			HAL_StatusTypeDef result = HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_SNAPSHOT,(uint32_t)&FrameBuffer,(uint32_t)(IMG_ROWS * IMG_COLUMNS * 2/4));
		}		
  }
}


// System Clock Configuration
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  HAL_PWREx_EnableOverDrive();

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  // MCO je 8MHz, koristimo PLL X6 koji podesavamo u samoj kamerici 
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_2);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 1, 0);
}



#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif


