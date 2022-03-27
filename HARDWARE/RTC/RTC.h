#ifndef __RTC_H  
#define __RTC_H   
#include "stm32f4xx.h"
uint8_t RTC_Init(void);		 			//��ʼ��RTC
void RTC_Set_WakeUp(uint8_t wksel,uint16_t cnt);		//����WAKE UP�ж�,1�����ж�һ�� 
void RTC_Get_Time(uint8_t *hour,uint8_t *min,uint8_t *sec,uint8_t *ampm);//��ȡʱ��
void RTC_Get_Date(uint8_t *year,uint8_t *month,uint8_t *date,uint8_t *week);//��ȡ����
uint8_t RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm);//����ʱ��
uint8_t RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week);//��������

#endif
