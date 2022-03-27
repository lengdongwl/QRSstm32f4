/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:57
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-13 14:49:36
 */

#ifndef _MY_IIC_H
#define _MY_IIC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//IIC驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/4/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//IO方向设置
#define MY_SDA_IN()  {GPIOD->MODER&=~(3<<(2*2));GPIOD->MODER|=0<<2*2;}	//PB9输入模式
#define MY_SDA_OUT() {GPIOD->MODER&=~(3<<(2*2));GPIOD->MODER|=1<<2*2;} //PB9输出模式
//IO操作
#define MY_IIC_SCL   PCout(12) //SCL
#define MY_IIC_SDA   PDout(2) //SDA
#define MY_READ_SDA  PDin(2)  //输入SDA

//IIC所有操作函数
void MY_IIC_Init(void);                //初始化IIC的IO口				 
void MY_IIC_Start(void);				//发送IIC开始信号
void MY_IIC_Stop(void);	  			//发送IIC停止信号
void MY_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 MY_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 MY_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void MY_IIC_Ack(void);					//IIC发送ACK信号
void MY_IIC_NAck(void);				//IIC不发送ACK信号

void MY_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MY_IIC_Read_One_Byte(u8 daddr,u8 addr);	 
u8 MY_IIC_RecvByte(void);
#endif

