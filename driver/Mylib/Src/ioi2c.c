/*
 * ioi2c.c
 *
 *  Created on: May 12, 2026
 *      Author: GB Center
 */

#include "ioi2c.h"
#include "delay.h"

static void SDA_OUT()
{
	I2C_Port->MODER &= ~(0b11 << (I2C_SDA_Pin * 2));
	I2C_Port->MODER |= (1 << (I2C_SDA_Pin * 2));

	I2C_Port->OTYPER |= (1 << I2C_SDA_Pin); // Open drain

	//high speed

	I2C_Port->OSPEEDR &= ~(0b11 << (I2C_SDA_Pin * 2));
	I2C_Port->OSPEEDR |= (0b10 << (I2C_SDA_Pin * 2));

	//Pull up

	I2C_Port->PUPDR &= ~(0b11 << (I2C_SDA_Pin * 2));
	I2C_Port->PUPDR |= (1 << (I2C_SDA_Pin * 2));

}

static void SDA_IN()
{
	I2C_Port->MODER &= ~(0b11 << (I2C_SDA_Pin * 2)); // PB11 input

	//Pull up
	I2C_Port->PUPDR &= ~(0b11 << (I2C_SDA_Pin * 2));
	I2C_Port->PUPDR |= (1 << (I2C_SDA_Pin * 2));
	SDA_HIGH();
}

static void SCL_OUT()
{
	// PB10 output mode
	I2C_Port->MODER &= ~(0b11 << (I2C_SCL_Pin * 2));
	I2C_Port->MODER |= (0b01 << (I2C_SCL_Pin * 2));


	//Open drain
	I2C_Port->OTYPER |= (1 << I2C_SCL_Pin); // Open drain

	//high speed

	I2C_Port->OSPEEDR &= ~(0b11 << (I2C_SCL_Pin * 2));
	I2C_Port->OSPEEDR |= (0b10 << (I2C_SCL_Pin * 2));

	//Pull up

	I2C_Port->PUPDR &= ~(0b11 << (I2C_SCL_Pin * 2));
	I2C_Port->PUPDR |= (1 << (I2C_SCL_Pin * 2));
}


/*
 * INIT software I2C
 */

void i2c_init()
{

	I2C_clk_EN();
	SCL_OUT();
	SDA_OUT();

	SCL_HIGH();
	SDA_HIGH();
	delay_us(5);

}
int i2c_start()
{
	SDA_OUT();

	SDA_HIGH();
	SCL_HIGH();

	delay_us(2);
	if(!READ_SDA) return 0;

	SDA_LOW();
	delay_us(2);

	if(READ_SDA) return 0;

	SCL_LOW();
	delay_us(2);

	return 1;
}
void i2c_stop()
{
	SDA_OUT();

	SCL_LOW();
	SDA_LOW();
	delay_us(2);

	SCL_HIGH();
	delay_us(2);
	SDA_HIGH();
	delay_us(2);

}
int i2c_wait_ack()
{
	u8 timeout = 0;
	SDA_IN();
	SDA_HIGH();
	delay_us(1);
	SCL_HIGH();
	delay_us(1);

	while(READ_SDA)
	{
		timeout++;
		if(timeout > 100)
		{
			i2c_stop();
			return 0;
		}
		delay_us(1);
	}
	SCL_LOW();
	return 1;

}

void i2c_ack()
{
	SCL_LOW();
	SDA_OUT();

	SDA_LOW();
	delay_us(2);
	SCL_HIGH();
	delay_us(2);

	SCL_LOW();
	delay_us(2);
}
void i2c_nack()
{
	SCL_LOW();
	SDA_OUT();

	SDA_HIGH();
	delay_us(2);
	SCL_HIGH();
	delay_us(2);
	SCL_LOW();
	delay_us(2);
}

void i2c_send_byte(u8 txd)
{
	u8 i;
	SDA_OUT();
	SCL_LOW();

	for(i = 0;i < 8; i++)
	{
		if(txd & 0x80)
		{
			SDA_HIGH();
		}else
		{
			SDA_LOW();
		}
		txd <<= 1;
		delay_us(2);

		SCL_HIGH();
		delay_us(2);
		SCL_LOW();
		delay_us(2);
	}
	SDA_HIGH();
}
u8 i2c_read_byte(u8 ack)
{
	u8 i;
	u8 receive = 0;

	SDA_IN();
	for(i =0; i<8; i++)
	{
		SCL_LOW();
		delay_us(2);

		SCL_HIGH();
		delay_us(2);
		receive <<= 1;
		if(READ_SDA) receive ++;
		SCL_LOW();

		delay_us(2);
	}

	if(ack)
	{
		i2c_ack();

	}else
	{
		i2c_nack();
	}
	return receive;
}

int i2c_write(u8 devaddr, u8 regaddr,u8 len,u8 *data)
{
	u8 i;
	if(!i2c_start()){
		return 1;
	}

	i2c_send_byte(devaddr << 1);
	if(!i2c_wait_ack())
	{
		i2c_stop();
		return 1;
	}
	i2c_send_byte(regaddr);
	if(!i2c_wait_ack())
	{
		i2c_stop();
		return 1;
	}


	for(i = 0; i < len; i++)
	{
		i2c_send_byte(data[i]);

		if(!i2c_wait_ack())
		{
			i2c_stop();
			return 1;
		}
	}
	i2c_stop();
	return 0;
}
int i2c_read(u8 devaddr, u8 regaddr, u8 len, u8 *buf)
{
	if(!i2c_start())
	{
		return 1;
	}

	i2c_send_byte(devaddr << 1);
	if(!i2c_wait_ack())
	{
		i2c_stop();
		return 1;
	}

	i2c_send_byte(regaddr);

	if(!i2c_wait_ack())
	{
		i2c_stop();
		return 1;
	}

	if(!i2c_start())
	{
		return 1;
	}
	i2c_send_byte((devaddr << 1) | 1);

	if(!i2c_wait_ack())
	{
		i2c_stop();
		return 1;
	}
	while(len)
	{
		if(len == 1)
		{
			*buf = i2c_read_byte(0);
		}
		else
		{
			*buf = i2c_read_byte(1);
		}
		buf++;
		len--;
	}
	i2c_stop();

	return 0;
}

u8 i2c_writebyte(u8 devaddr,u8 regaddr, u8 data)
{
	return i2c_write(devaddr, regaddr, 1, &data) == 0;
}
u8 i2c_writebytes(u8 devaddr,u8 regaddr,u8 len, u8 *data)
{
	return i2c_write(devaddr, regaddr, len, data) == 0;
}

u8 i2c_readbyte(u8 devaddr,u8 regaddr, u8 *data)
{
	return i2c_read(devaddr, regaddr, 1, data) == 0;
}
u8 i2c_readbytes(u8 devaddr,u8 regaddr,u8 len,u8 *data)
{
	return i2c_read(devaddr, regaddr, len, data) == 0;
}
u8 i2c_writebit(u8 devaddr,u8 regaddr,u8 bitNum, u8 data)
{
	u8 b;
	if(!i2c_readbyte(devaddr, regaddr, &b))
	{
		return 0;

	}
	if(data)
	{
		b |= (1 << bitNum);
	}
	else
	{
		b &= ~(1 << bitNum);

	}

	return i2c_writebyte(devaddr, regaddr, b);

}
u8 i2c_writebits(u8 devaddr,u8 regaddr,u8 bitstart, u8 len,u8 data)
{
	u8 b;
	u8 mask;

	if(len == 0 || len > 8 || bitstart > 7 || bitstart < (len - 1))
	{
		return 0;
	}

	if(!i2c_readbyte(devaddr, regaddr, &b))
	{
		return 0;
	}

	mask = ((1 << len) - 1) << (bitstart - len + 1);
	data <<= (bitstart - len + 1);
	data &= mask;

	b &= ~mask;
	b |= data;
	return i2c_writebyte(devaddr, regaddr, b);
}




























