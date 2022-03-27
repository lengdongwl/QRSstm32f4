/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-21 15:12:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-21 15:19:24
 */
#ifndef __PAJ7620U2_IIC_H
#define __PAJ7620U2_IIC_H

#include "sys.h"



#define GS_SDA_IN()  {GPIOA->MODER&=~(3<<(4*2));GPIOA->MODER|=0<<4*2;}	//PA4输入模式
#define GS_SDA_OUT() {GPIOA->MODER&=~(3<<(4*2));GPIOA->MODER|=1<<4*2;}    //PA4输出模式

//IO操作函数
#define GS_IIC_SCL    PBout(3) 		//SCL
#define GS_IIC_SDA    PAout(4) 		//SDA	 
#define GS_READ_SDA   PAin(4) 		    //输入SDA 

u8 GS_Write_Byte(u8 REG_Address,u8 REG_data);
u8 GS_Read_Byte(u8 REG_Address);
u8 GS_Read_nByte(u8 REG_Address,u16 len,u8 *buf);
void GS_i2c_init(void);
void GS_WakeUp(void);

#endif


