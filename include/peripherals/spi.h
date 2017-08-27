#ifndef __spi_H
#define __spi_H

#define SPI_GET_FLAG(__HANDLE__, __FLAG__) ((((__HANDLE__)->SR) & (__FLAG__)) == (__FLAG__))

extern SPI_HandleTypeDef hspi5;



extern void MX_SPI5_Init(void);


#endif /*__ spi_H */

