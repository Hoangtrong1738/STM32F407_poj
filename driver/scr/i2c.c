/*
 * i2c.c
 *
 *  Created on: May 12, 2026
 *      Author: GB Center
 */


#include "i2c.h"

/*
void i2c2_init()
{

	GPIO_Handle_t B8;
	B8.pGPIOx = GPIOB;
	B8.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFM;
	B8.GPIO_PinConfig.GPIO_PinAltFunMode = 4;

	B8.GPIO_PinConfig.GPIO_PinNumber = PIN_7;
	GPIO_Init(&B8);


	B8.GPIO_PinConfig.GPIO_PinNumber = PIN_8;
	GPIO_Init(&B8);


	I2C1_PCLK_EN();

	u32 *I2C_CR1 = (u32*)(I2C2_BASE + 0x00);
	*I2C_CR1 |= 1 << 0;



}

*/

u16 AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
u8 APB1_PreScaler[4] = {2,4,8,16};
u32 RCC_GetPLLOutputClock()
{
	return 0;
}
u32 RCC_GetPCLK1Value()
{
	u32 pclk1, SystemClk;

	u8 clksrc, temp, ahbp, apb1;
	clksrc = (RCC->CFGR >> 2) & 0X3;

	if(clksrc ==0)
	{
		SystemClk = 16000000;

	}
	else if(clksrc == 1)
	{
		SystemClk = 8000000;
	}else if(clksrc == 2)
	{
		SystemClk = RCC_GetPLLOutputClock();
	}

	//for AHB
	temp = ((RCC -> CFGR )>> 4) & 0xF;

	if(temp < 8)
	{
		ahbp = 1;

	}else
	{
		ahbp = AHB_PreScaler[temp - 8];
	}

	// for APB1
	temp = ((RCC -> CFGR )>> 4) & 0x7;

	if(temp < 4)
	{
		apb1 = 1;

	}else
	{
		apb1 = APB1_PreScaler[temp - 4];
	}
	pclk1 = (SystemClk / ahbp ) / apb1;
	return pclk1;
}
void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	u32 tempreg = 0;
	//ACK control bit
	tempreg |= pI2CHandle->I2C_Config.I2C_ACKControl << 10; 
	pI2CHandle->pI2Cx->CR1 = tempreg;
	
	// configure the FREQ field of CR2 

	tempreg = 0;
	tempreg = RCC_GetPCLK1Value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 = (tempreg & 0x3F);

	//program the device owm address
	tempreg = pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
	tempreg |= (1 << 14);
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	//CCR calculations

	u16 ccr_value = 0;
	tempreg = 0;

	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		
	}
	




}
void I2C_DeInit(I2C_RegDef_t *pI2Cx);
void I2C_PClkControl(I2C_RegDef_t *pI2Cx,u8 EnorDi);
















