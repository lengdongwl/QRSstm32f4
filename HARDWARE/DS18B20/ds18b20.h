/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-19 15:21:47
 */
#ifndef _DS18B20_H
#define _DS18B20_H
#include "sys.h"

//IO��������
#define DS18B20_IO_IN()  {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=0<<(9*2);}	//PG9����ģʽ
#define DS18B20_IO_OUT() {GPIOG->MODER&=~(3<<(9*2));GPIOG->MODER|=1<<(9*2);} 	//PG9���ģʽ
 
////IO��������											   
#define	DS18B20_DQ_OUT PEout(7) //���ݶ˿�	PE7
#define	DS18B20_DQ_IN  PEin(7)  //���ݶ˿�	PE7 
   	
uint8_t DS18B20_Init(void);			//��ʼ��DS18B20
uint8_t DS18B20_DeInit(void);
short DS18B20_Get_Temp(void);	//��ȡ�¶�
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(uint8_t dat);//д��һ���ֽ�
uint8_t DS18B20_Read_Byte(void);		//����һ���ֽ�
uint8_t DS18B20_Read_Bit(void);		//����һ��λ
uint8_t DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B20 

#endif
