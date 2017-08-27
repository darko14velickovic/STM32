#include "headers.h"

uint8_t LCD_SPI_Send(SPI_TypeDef* SPIx, uint8_t data) 
{
	//Fill output buffer with data
	SPIx->DR = data;
	//Wait for transmission to complete
	while (!SPI_GET_FLAG(SPIx, SPI_FLAG_TXE));
	//Wait for received data to complete
	while (!SPI_GET_FLAG(SPIx, SPI_FLAG_RXNE));
	//Wait for SPI to be ready
	while (SPI_GET_FLAG(SPIx, SPI_FLAG_BSY));
	//Return data from buffer
	return SPIx->DR;
}

