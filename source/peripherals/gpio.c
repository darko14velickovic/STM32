#include "headers.h"


// Pinout Configuration
void MX_GPIO_Init(void)
{

   GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, ILI9341_RST_PIN|ILI9341_WRX_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = ILI9341_CS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(ILI9341_CS_PORT, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin */
  GPIO_InitStruct.Pin = ILI9341_RST_PIN|ILI9341_WRX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  // Button
  GPIO_InitStruct.Pin = BUTTON_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);
  
  // LEDs
  GPIO_InitStruct.Pin = LED_GREEN_PIN | LED_RED_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LEDS_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = CAM_XCLK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  HAL_GPIO_Init(CAM_XCLK_GPIO_PORT, &GPIO_InitStruct);
  
}

void GPIOSetHigh(GPIO_TypeDef* GPIOPort, uint16_t Pin)
{
  GPIOPort->BSRR = Pin;
}
void GPIOSetLow(GPIO_TypeDef* GPIOPort, uint16_t Pin)
{
  GPIOPort->BSRR = (uint32_t)Pin << 16U;
}
void GPIOToggle(GPIO_TypeDef* GPIOPort, uint16_t Pin)
{
  GPIOPort->ODR ^= Pin;
}

GPIO_PinState GPIOReadPin(GPIO_TypeDef* GPIOPort, uint16_t Pin)
{
  if((GPIOPort->IDR & Pin) != (uint32_t)GPIO_PIN_RESET)
    return GPIO_PIN_SET;
  else
    return GPIO_PIN_RESET;
}

