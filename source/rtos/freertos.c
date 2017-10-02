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
uint32_t rng = 0xFFFFFFFF;

uint8_t characterCount = 28;

char avaibleCharacters[28]  = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l' ,'m' , 'n', 'o',
															 'p', 'q', 'r', 's', 't', 'u' ,'v', 'w', 'x', 'y', 'z', '-', '*'};


uint8_t populateCounter = 0;															 
char shownBuffer[32] = {0};

char encryptedBuffer[64] = {0};

char OTP = 0xAA;

bool ValueSelected = 0;

bool Encrypt = 1;

static const unsigned char base64_enc_map[64] =
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
    'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '+', '/'
};

static const unsigned char base64_dec_map[128] =
{
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
    127, 127, 127,  62, 127, 127, 127,  63,  52,  53,
     54,  55,  56,  57,  58,  59,  60,  61, 127, 127,
    127,  64, 127, 127, 127,   0,   1,   2,   3,   4,
      5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
     25, 127, 127, 127, 127, 127, 127,  26,  27,  28,
     29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
     39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
     49,  50,  51, 127, 127, 127, 127, 127
};

/*
 * Decode a base64-formatted buffer
 */
int base64_decode( unsigned char *dst, size_t *dlen,
                   const unsigned char *src, size_t slen )
{
    size_t i, n;
    uint32_t j, x;
    unsigned char *p;

    for( i = j = n = 0; i < slen; i++ )
    {
        if( ( slen - i ) >= 2 &&
            src[i] == '\r' && src[i + 1] == '\n' )
            continue;

        if( src[i] == '\n' )
            continue;

        if( src[i] == '=' && ++j > 2 )
            return( -2 );

        if( src[i] > 127 || base64_dec_map[src[i]] == 127 )
            return( -2);

        if( base64_dec_map[src[i]] < 64 && j != 0 )
            return( -2 );

        n++;
    }

    if( n == 0 )
        return( 0 );

    n = ((n * 6) + 7) >> 3;

    if( *dlen < n )
    {
        *dlen = n;
        return( -3 );
    }

   for( j = 3, n = x = 0, p = dst; i > 0; i--, src++ )
   {
        if( *src == '\r' || *src == '\n' )
            continue;

        j -= ( base64_dec_map[*src] == 64 );
        x  = (x << 6) | ( base64_dec_map[*src] & 0x3F );

        if( ++n == 4 )
        {
            n = 0;
            if( j > 0 ) *p++ = (unsigned char)( x >> 16 );
            if( j > 1 ) *p++ = (unsigned char)( x >>  8 );
            if( j > 2 ) *p++ = (unsigned char)( x       );
        }
    }

    *dlen = p - dst;

    return( 0 );
}



/*
 * Encode a buffer into base64 format
 */
int base64_encode( unsigned char *dst, size_t *dlen,
                   const unsigned char *src, size_t slen )
{
    size_t i, n;
    int C1, C2, C3;
    unsigned char *p;

    if( slen == 0 )
        return( 0 );

    n = (slen << 3) / 6;

    switch( (slen << 3) - (n * 6) )
    {
        case  2: n += 3; break;
        case  4: n += 2; break;
        default: break;
    }

    if( *dlen < n + 1 )
    {
        *dlen = n + 1;
        return( -1 );
    }

    n = (slen / 3) * 3;

    for( i = 0, p = dst; i < n; i += 3 )
    {
        C1 = *src++;
        C2 = *src++;
        C3 = *src++;

        *p++ = base64_enc_map[(C1 >> 2) & 0x3F];
        *p++ = base64_enc_map[(((C1 &  3) << 4) + (C2 >> 4)) & 0x3F];
        *p++ = base64_enc_map[(((C2 & 15) << 2) + (C3 >> 6)) & 0x3F];
        *p++ = base64_enc_map[C3 & 0x3F];
    }

    if( i < slen )
    {
        C1 = *src++;
        C2 = ((i + 1) < slen) ? *src++ : 0;

        *p++ = base64_enc_map[(C1 >> 2) & 0x3F];
        *p++ = base64_enc_map[(((C1 & 3) << 4) + (C2 >> 4)) & 0x3F];

        if( (i + 1) < slen )
             *p++ = base64_enc_map[((C2 & 15) << 2) & 0x3F];
        else *p++ = '=';

        *p++ = '=';
    }

    *dlen = p - dst;
    *p = 0;

    return( 0 );
}

void UserInteractionTask(void const * argument)
{
	
	for(;;)
  {
		if((GPIOReadPin(BUTTON_PORT,BUTTON_PIN) == GPIO_PIN_SET) && ButtonPressed)
		{
			rng--;
		}
		
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
						if(Encrypt == 1)
						{
							for(uint8_t i = 0 ; i < 32; i++)
							{
								if(shownBuffer[i] == ' ')
								{
									shownBuffer[i] = 0;
								}
								
								encryptedBuffer[i] = shownBuffer[i] ^ OTP ^ rng;
							}
							size_t len = sizeof(encryptedBuffer);
							
							base64_encode( encryptedBuffer, &len, shownBuffer, 32 );
						}
						else
						{
							size_t len = sizeof(encryptedBuffer);
							base64_decode( encryptedBuffer, &len, shownBuffer, 64 );
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
	
		LCD_ILI9341_Fill(ILI9341_COLOR_BLACK);
		ShowCharacterList();
		
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
