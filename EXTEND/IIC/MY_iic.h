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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//IIC��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2017/4/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
//IO��������
#define MY_SDA_IN()  {GPIOD->MODER&=~(3<<(2*2));GPIOD->MODER|=0<<2*2;}	//PB9����ģʽ
#define MY_SDA_OUT() {GPIOD->MODER&=~(3<<(2*2));GPIOD->MODER|=1<<2*2;} //PB9���ģʽ
//IO����
#define MY_IIC_SCL   PCout(12) //SCL
#define MY_IIC_SDA   PDout(2) //SDA
#define MY_READ_SDA  PDin(2)  //����SDA

//IIC���в�������
void MY_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MY_IIC_Start(void);				//����IIC��ʼ�ź�
void MY_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MY_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 MY_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 MY_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MY_IIC_Ack(void);					//IIC����ACK�ź�
void MY_IIC_NAck(void);				//IIC������ACK�ź�

void MY_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MY_IIC_Read_One_Byte(u8 daddr,u8 addr);	 
u8 MY_IIC_RecvByte(void);
#endif

