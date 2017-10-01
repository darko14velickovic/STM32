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
#include "headers.h"
//#include "dsp.h"

/* USER CODE BEGIN Includes */     

/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;


/* USER CODE BEGIN Variables */
osThreadId userInteractionTaskHandle;
/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);

void UserInteractionTask(void const * argument);

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
	
	osThreadDef(userTask, UserInteractionTask, osPriorityNormal, 0, 128);
	userInteractionTaskHandle = osThreadCreate(osThread(userTask), NULL);
	
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

extern bool ButtonPressed;
extern uint8_t NumberPressed;

//uint16_t buffer[100 * 100] = {0};

//Task variables

uint32_t debounceCounter = 0x1FFFF;

uint8_t characterCount = 28;

char avaibleCharacters[28]  = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l' ,'m' , 'n', 'o',
															 'p', 'q', 'r', 's', 't', 'u' ,'v', 'w', 'x', 'y', 'z', '-', '*'};


uint8_t populateCounter = 0;															 
char shownBuffer[32] = {0};

char encryptedBuffer[32] = {0};

char OTP = 0x23;

bool ValueSelected = 0;

void UserInteractionTask(void const * argument)
{
	
	for(;;)
  {
		if((GPIOReadPin(BUTTON_PORT,BUTTON_PIN) == GPIO_PIN_SET) && ButtonPressed && !ValueSelected)
		{
			debounceCounter--;
			
			//button held down, long press, debounced with counter
			if(debounceCounter == 0)
			{
				ValueSelected = 1;
				
				// select the character
				
		
				if(NumberPressed == 0)
				{
						NumberPressed = characterCount - 1;
				}
				else
				{
					NumberPressed--;
				}
				
				char chosenChar = avaibleCharacters[NumberPressed];
				
				// its not a command is a character
				if(NumberPressed != 26 && NumberPressed != 27)
				{
					
					shownBuffer[populateCounter] = avaibleCharacters[NumberPressed];
					//counter move
					populateCounter = (populateCounter + 1) % 32;
					
					xTaskNotifyGive(defaultTaskHandle);
				}
				else
				{
					if(NumberPressed == 27)
					{
						//hash and encryption
						
						for(uint8_t i = 0 ; i < 32; i++)
						{
							encryptedBuffer[i] = shownBuffer[i] ^ OTP ^ debounceCounter;
						}
						
						//show result
						xTaskNotifyGive(defaultTaskHandle);
					}
					else if(chosenChar == '-')
					{
						//remove last character from shownBuffer
						shownBuffer[populateCounter] = ' ';
						populateCounter--;
						
						
						xTaskNotifyGive(defaultTaskHandle);
					}
						
				}
				
				
				
				
				
			}
			
			
		}
		else if((GPIOReadPin(BUTTON_PORT,BUTTON_PIN) == GPIO_PIN_SET) && !ButtonPressed)
    {
			
      ButtonPressed = true;
			NumberPressed += 1;
			NumberPressed %= characterCount;
			//HAL_StatusTypeDef result = HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_SNAPSHOT,(uint32_t)&FrameBuffer,(uint32_t)(IMG_ROWS * IMG_COLUMNS * 2/4)); 
			xTaskNotifyGive(defaultTaskHandle);
    }
    else if(GPIOReadPin(BUTTON_PORT,BUTTON_PIN) == GPIO_PIN_RESET)
    {
      ButtonPressed = false;
			ValueSelected = 0;
			
			// reset D-counter
			debounceCounter = 0x1FFFF;
			
    }
    //osDelay(1);
		taskYIELD();
  }
}

void ShowCommitedText()
{
	LCD_ILI9341_Puts(5, 40, &shownBuffer[0], &LCD_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
}

void ShowEncryptedText()
{
	LCD_ILI9341_Puts(5, 80, &encryptedBuffer[0], &LCD_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
}

void ShowCharacterList()
{
	uint8_t row = 0;
	for(uint8_t i = 0; i < characterCount; i++)
	{
		char ptr = avaibleCharacters[i];
		char oneChar[2] = { ptr, 0 };
		
		if(i == NumberPressed && oneChar[0] != '-' && oneChar[0] != '*')
		{
			//uppercase it and make it blue
			
			oneChar[0] = oneChar[0] - 32;
			
			LCD_ILI9341_Puts(5 + i * 10, row * 20, &oneChar[0], &LCD_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_BLACK);
		}
		else if(i == NumberPressed)
		{
			LCD_ILI9341_Puts(5 + i * 10, row * 20, &oneChar[0], &LCD_Font_11x18, ILI9341_COLOR_BLUE, ILI9341_COLOR_BLACK);
		}
		else
		{
			LCD_ILI9341_Puts(5 + i * 10, row * 20, &oneChar[0], &LCD_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
		}
		
		
		
	}
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
	
	
	// OV7670 configuration
	if(OV7670Init() == true)
  {
		LCD_ILI9341_Puts(100, 165, "Failed", &LCD_Font_16x26, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
		LCD_ILI9341_Puts(20, 200, "Push reset button", &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
		while(1)
    {
      taskYIELD();
		}
	}		
	else
  {
		LCD_ILI9341_Fill(ILI9341_COLOR_BLACK);
		ShowCharacterList();
		
	}
	//LCD_ILI9341_Rotate(LCD_ILI9341_Orientation_Landscape_1);
	
	
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
		xTaskNotifyWait(0, 0, 0, portMAX_DELAY);
		
		//Selection of the characters with uppercased selected one
		
		ShowCharacterList();
		ShowCommitedText();
		ShowEncryptedText();
		
		//Show buffer with selected characters
		//LCD_ILI9341_Puts(60, 140, &shownBuffer[0] , &LCD_Font_16x26, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
		taskYIELD();
		
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
