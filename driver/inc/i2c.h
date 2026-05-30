/*
 * i2c.h
 *
 *  Created on: May 12, 2026
 *      Author: GB Center
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_
#include "stm32f407xx.h"

void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);
void I2C_PClkControl(I2C_RegDef_t *pI2Cx,u8 EnorDi);

/*
 * Data Send and Receive
 */


/*
 *
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, u32 IRQPriority);
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx,u8 EnDrDi);


u8 I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, u32 FlagName);

/*
 * Application callback
 */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,u8 AppEv);
#endif /* INC_I2C_H_ */
