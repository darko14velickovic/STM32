#include "headers.h"

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
 
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  while (1)
  {
  }
}

/**
* @brief This function handles Pre-fetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  while (1)
  {
  }
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  while (1)
  {
  }
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles DMA2 stream1 global interrupt.
*/
void DMA2_Stream1_IRQHandler(void)
{
	//DCMI->CR &= ~(DCMI_CR_ENABLE);
  HAL_DMA_IRQHandler(&hdma_dcmi);
  //FrameReady = true;
}

/* USER CODE BEGIN 1 */
void DCMI_IRQHandler(void)
{
	//__HAL_DCMI_DISABLE(&hdcmi);
	HAL_DCMI_IRQHandler(&hdcmi);
	//HAL_DMA_Cmd(DMA2_Stream1, DISABLE);
		//DCMI_Cmd(DISABLE);
	//__HAL_DCMI_DISABLE(&hdcmi);
	//HAL_DCMI_Stop(&hdcmi);
	//LCD_ILI9341_DisplayImage((uint16_t*)FrameBuffer);
	//HAL_NVIC_DisableIRQ(DCMI_IRQn);
		//DCMI_CaptureCmd(DISABLE);
}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
