#ifndef _MLX90614_H
#define _MLX90614_H
#include "sys.h"

#define ACK	 0 //应答
#define	NACK 1 //无应答
#define SA				0x00 //Slave address 单个MLX90614时地址为0x00,多个时地址默认为0x5a
#define RAM_ACCESS		0x00 //RAM access command RAM存取命令
#define EEPROM_ACCESS	0x20 //EEPROM access command EEPROM存取命令
#define RAM_TOBJ1		0x07 //To1 address in the eeprom 目标1温度,检测到的红外温度 -70.01 ~ 382.19度

#define SMBUS_SCK_H()	    PCout(12)=1 //置高电平
#define SMBUS_SCK_L()	    PCout(12)=0 //置低电平
#define SMBUS_SDA_H()	    PDout(2)=1	//置高电平 
#define SMBUS_SDA_L()	    PDout(2)=0 	//置低电平

#define MLX90614_SDA_IN()  {GPIOD->MODER&=~(3<<(2*2));GPIOD->MODER|=0<<2*2;}	//PB9输入模式
#define MLX90614_SDA_OUT() {GPIOD->MODER&=~(3<<(2*2));GPIOD->MODER|=1<<2*2;} //PB9输出模式

#define SMBUS_SDA_PIN()   PDin(2)

void MLX90614_Init(void);
void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(u8);
u8 SMBus_SendByte(u8);
u8 SMBus_ReceiveBit(void);
u8 SMBus_ReceiveByte(u8);
void SMBus_Delay(u16);
u16 SMBus_ReadMemory(u8, u8);
u8 PEC_Calculation(u8*);
float SMBus_ReadTemp(void); //获取温度值
void SMBus_Delay(u16 time);

#endif

