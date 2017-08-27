#ifndef __i2c_H
#define __i2c_H

#define I2C_GET_FLAG(__HANDLE__, __FLAG__) ((((uint8_t)((__FLAG__) >> 16U)) == 0x01U)?((((__HANDLE__)->SR1) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)): \
                                                 ((((__HANDLE__)->SR2) & ((__FLAG__) & I2C_FLAG_MASK)) == ((__FLAG__) & I2C_FLAG_MASK)))

#define I2C_CLEAR_FLAG(__HANDLE__, __FLAG__) ((__HANDLE__)->SR1 = ~((__FLAG__) & I2C_FLAG_MASK))

#define I2C_CLEAR_ADDRFLAG(__HANDLE__)          \
  do{                                           \
    __IO uint32_t tmpreg = 0x00U;               \
    tmpreg = (__HANDLE__)->SR1;                 \
    tmpreg = (__HANDLE__)->SR2;                 \
    UNUSED(tmpreg);                             \
  } while(0)


typedef enum _typeI2CError
{
  I2C_NO_ERR = 0x00,
  I2C_ERR_NO_ACK,
  I2C_ERR_AR_LOST
} typeI2CError;

extern I2C_HandleTypeDef hi2c1;


extern void MX_I2C1_Init(void);
extern typeI2CError I2CWrite(I2C_TypeDef* I2CPort, uint8_t DeviceAddress, uint8_t* Buffer, uint16_t Len);


#endif /*__ i2c_H */
