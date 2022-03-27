#ifndef _MLX90614_H
#define _MLX90614_H
#include "sys.h"

#define ACK	 0 //Ӧ��
#define	NACK 1 //��Ӧ��
#define SA				0x00 //Slave address ����MLX90614ʱ��ַΪ0x00,���ʱ��ַĬ��Ϊ0x5a
#define RAM_ACCESS		0x00 //RAM access command RAM��ȡ����
#define EEPROM_ACCESS	0x20 //EEPROM access command EEPROM��ȡ����
#define RAM_TOBJ1		0x07 //To1 address in the eeprom Ŀ��1�¶�,��⵽�ĺ����¶� -70.01 ~ 382.19��

#define SMBUS_SCK_H()	    PCout(12)=1 //�øߵ�ƽ
#define SMBUS_SCK_L()	    PCout(12)=0 //�õ͵�ƽ
#define SMBUS_SDA_H()	    PDout(2)=1	//�øߵ�ƽ 
#define SMBUS_SDA_L()	    PDout(2)=0 	//�õ͵�ƽ

#define MLX90614_SDA_IN()  {GPIOD->MODER&=~(3<<(2*2));GPIOD->MODER|=0<<2*2;}	//PB9����ģʽ
#define MLX90614_SDA_OUT() {GPIOD->MODER&=~(3<<(2*2));GPIOD->MODER|=1<<2*2;} //PB9���ģʽ

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
float SMBus_ReadTemp(void); //��ȡ�¶�ֵ
void SMBus_Delay(u16 time);

#endif

