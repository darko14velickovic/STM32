#ifndef __gpio_H
#define __gpio_H


extern void MX_GPIO_Init(void);

extern void GPIOSetHigh(GPIO_TypeDef* GPIOPort, uint16_t Pin);
extern void GPIOSetLow(GPIO_TypeDef* GPIOPort, uint16_t Pin);
extern void GPIOToggle(GPIO_TypeDef* GPIOPort, uint16_t Pin);
extern GPIO_PinState GPIOReadPin(GPIO_TypeDef* GPIOPort, uint16_t Pin);
#endif /*__ pinoutConfig_H */

