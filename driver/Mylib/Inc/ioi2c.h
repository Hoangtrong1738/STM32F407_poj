/*
 * ioi2c.h
 *
 *  Created on: May 12, 2026
 *      Author: GB Center
 */

#ifndef MYLIB_INC_IOI2C_H_
#define MYLIB_INC_IOI2C_H_

#include "stm32f407xx.h"

#define I2C_Port 		GPIOB
#define I2C_SCL_Pin 	PIN_10
#define I2C_SDA_Pin 	PIN_11

#define I2C_clk_EN()          GPIOB_PCLK_EN()
/*
 * Control SCL/SDA
 */

#define SCL_HIGH()      (I2C_Port->ODR |= (1 << I2C_SCL_Pin))
#define SCL_LOW()       (I2C_Port->ODR &= ~(1 << I2C_SCL_Pin))

#define SDA_HIGH()      (I2C_Port->ODR |= (1 << I2C_SDA_Pin))
#define SDA_LOW()       (I2C_Port->ODR &= ~(1 << I2C_SDA_Pin))

#define READ_SDA        ((I2C_Port -> IDR >> I2C_SDA_Pin) & 0x1U)

/*
 * API
 */
void i2c_init();
int i2c_start();
void i2c_stop();
void i2c_send_byte(u8 txd);
u8 i2c_read_byte(u8 ack);

int i2c_wait_ack();
void i2c_ack();
void i2c_nack();


int i2c_write(u8 devaddr, u8 regaddr,u8 len,u8 *đata);
int i2c_read(u8 devaddr, u8 regaddr, u8 len, u8 *buf);

u8 i2c_writebyte(u8 devaddr,u8 regaddr, u8 data);
u8 i2c_writebytes(u8 devaddr,u8 regaddr,u8 len, u8 *data);

u8 i2c_readbyte(u8 devaddr,u8 regaddr, u8 *data);
u8 i2c_readbytes(u8 devaddr,u8 regaddr,u8 len,u8 *data);
u8 i2c_writebit(u8 devaddr,u8 regaddr,u8 bitNum, u8 data);
u8 i2c_writebits(u8 devaddr,u8 regaddr,u8 bitstart, u8 len,u8 data);


#endif /* MYLIB_INC_IOI2C_H_ */
















