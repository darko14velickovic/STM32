/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
//#include "headers.h"
//#include "dsp.h"

/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;


/* USER CODE BEGIN Variables */
//osThreadId userInteractionTaskHandle;
/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
	
	
	// userInteractionTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

//extern bool ButtonPressed;
//extern uint8_t NumberPressed;


void UserInteractionTask(void const * argument)
{
	
	for(;;)
  {
//		if((GPIOReadPin(BUTTON_PORT,BUTTON_PIN) == GPIO_PIN_SET) && !ButtonPressed)
//    {
//      ButtonPressed = true;
//			NumberPressed += 1;
//			NumberPressed %= 4;
//			HAL_StatusTypeDef result = HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_SNAPSHOT,(uint32_t)&FrameBuffer,(uint32_t)(IMG_ROWS * IMG_COLUMNS * 2/4));     
//    }
//    else if(GPIOReadPin(BUTTON_PORT,BUTTON_PIN) == GPIO_PIN_RESET)
//    {
//      ButtonPressed = false;
//    }
    osDelay(1);
  }
}


/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
	
	
	// OV7670 configuration
//	if(OV7670Init() == true)
//  {
//		LCD_ILI9341_Puts(100, 165, "Failed", &LCD_Font_16x26, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
//		LCD_ILI9341_Puts(20, 200, "Push reset button", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
//		while(1)
//    {
//      
//		}
//	}		
//	else
//  {
//		LCD_ILI9341_Puts(100, 165, "Success", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);   
//    // LCD welcome page
//    LCD_ILI9341_Fill(ILI9341_COLOR_BLACK);
//    LCD_ILI9341_Puts(60, 110, "Press button", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLUE);
//    //HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_CONTINUOUS,(uint32_t)&FrameBuffer,(uint32_t)(IMG_ROWS * IMG_COLUMNS * 2/4));
//	}
//	LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_Landscape_1);
	
	int16_t GausBlurKernel[25] = { 1, 4, 	7, 	4, 1,
																4, 16, 26, 16, 4,
																7, 26, 41, 26, 7,
																4, 16, 26, 16, 4,
																1,  4,  7,  4, 1};
	
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
//		if(FrameReady == true)
//    {		
//			// Show camera image
//			
//			if(NumberPressed == 3)
//			{
//				test(FrameBuffer);
//			}
//			else if(NumberPressed == 2)
//			{
//				uint16_t buffer[240 * 100];
//				
//				convolution(FrameBuffer, GausBlurKernel, buffer);
//				memcpy((uint16_t*)FrameBuffer, buffer, sizeof(buffer));
//				
//				convolution(&FrameBuffer[240 * 100], GausBlurKernel, buffer);
//				memcpy((uint16_t*)&FrameBuffer[240 * 100], buffer, sizeof(buffer));
//				
//			}
//			else if(NumberPressed == 0)
//			{
//					grayscale(FrameBuffer);
//			}
//			// else default image
//			
//			
//			LCD_ILI9341_DisplayImage((uint16_t*)FrameBuffer);
//			
//			//LCD_ILI9341_DisplayImageSDRAM(0);
//			
//			FrameReady = false;
//			
//			HAL_StatusTypeDef result = HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_SNAPSHOT,(uint32_t)&FrameBuffer,(uint32_t)(IMG_ROWS * IMG_COLUMNS * 2/4));
//		}		

    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
