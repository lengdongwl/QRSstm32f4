/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2022-03-27 19:58:38
 */
#ifndef _SMG_H
#define _SMG_H
#include "sys.h"

#define SER PDout(15)   	//75HC595���������
#define SCK PCout(8)   		//75HC595����ʱ��
#define RCK PCout(7)   		//75HC595����ʱ��
#define OE PCout(10)   		//75HC595���ʹ�ܿ��ƶ�

#define SMG_1 PDout(13)   //�����λѡ1
#define SMG_2 PDout(14)   //�����λѡ2
#define SMG_3 PDout(11)   //�����λѡ3
#define SMG_4 PDout(12)   //�����λѡ4

extern uint8_t SMGSWAP;//�����λѡѡ�� ���ڵ���

void SMG_Init(void);																				//����ܳ�ʼ��
void SMG_DeInit(void);
void SMG_Display_Bit(uint8_t Input_Data,uint8_t Bit_Data);	//����ܵ���λ��ʾ
void SMG_Display(uint16_t Input_Data,uint8_t Ctrl_Bit);			//�������ʾ
void SMG_DisplayP(int16_t Input_Data4,int16_t Input_Data3,int16_t Input_Data2,int16_t Input_Data1,uint8_t Ctrl_Bit);

#endif
