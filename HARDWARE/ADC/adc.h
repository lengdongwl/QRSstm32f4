/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-30 10:34:12
 */
#ifndef _ADC_H
#define _ADC_H
#include "sys.h"

void MY_ADC_Init(void);
u16 Get_Adc(u32 ch);  
u16 Get_Adc_Average(u32 ch,u8 times);
short Get_Temprate(void);//��ȡ�ڲ��¶�
float Get_V(u32 ch);//��ȡ���ܵ�·���ϵ�λ��ģ���ѹ�����ݲɼ�

void MY_ADC_Init2(void);
void MY_ADC_DeInit2(void);
void MY_ADC_run(void);//��ʼadc�ɼ�����
uint32_t MY_ADC_PeakValue(void);
extern uint8_t ADC_RxFlag;//adc��ȡ��ɱ�־
extern TIM_HandleTypeDef htim3;
#endif
