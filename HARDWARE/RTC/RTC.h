#ifndef __RTC_H  
#define __RTC_H   
#include "stm32f4xx.h"
uint8_t RTC_Init(void);		 			//初始化RTC
void RTC_Set_WakeUp(uint8_t wksel,uint16_t cnt);		//配置WAKE UP中断,1秒钟中断一次 
void RTC_Get_Time(uint8_t *hour,uint8_t *min,uint8_t *sec,uint8_t *ampm);//获取时间
void RTC_Get_Date(uint8_t *year,uint8_t *month,uint8_t *date,uint8_t *week);//获取日期
uint8_t RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm);//设置时间
uint8_t RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week);//设置日期

#endif
