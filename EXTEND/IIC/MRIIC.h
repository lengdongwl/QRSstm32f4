/*
 * @Description: IIC协议操作库
 * @Autor: 309 Mushroom
 * @Date: 2021-11-13 19:16:45
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-14 19:17:21
 */
#ifndef __MRIIC_H  
#define __MRIIC_H   
#include "sys.h"


#define MR_SDA_IN()  {GPIOD->MODER&=~(3<<(2*2));GPIOD->MODER|=0<<2*2;}	//PD2输入模式
#define MR_SDA_OUT() {GPIOD->MODER&=~(3<<(2*2));GPIOD->MODER|=1<<2*2;} //PD2输出模式


//IO操作
#define MR_IIC_SCL   PCout(12) //SCL
#define MR_IIC_SDA   PDout(2) //SDA
#define MR_READ_SDA  PDin(2)  //输入SDA

#define MR_SDA_HIGH(){MR_IIC_SDA=1;}
#define MR_SDA_LOW(){MR_IIC_SDA=0;}
#define MR_SCL_HIGH(){MR_IIC_SCL=1;}
#define MR_SCL_LOW(){MR_IIC_SCL=0;}


void MR_IIC_Start(void);
void MR_IIC_Stop(void);
void MR_IIC_Ack(void);
void MR_IIC_NAck(void);
u8 MR_IIC_Wait_Ack(void); //主机等待应答函数
void MR_IIC_Send_Byte(u8 byte);
u8 MR_IIC_ReadByte(u8 ack);
u8 MR_IIC_ReadByte2(void);
void MR_IIC_Init(void);

#endif
