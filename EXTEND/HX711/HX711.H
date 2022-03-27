/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:57
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-12-14 09:16:33
 */
#ifndef __HX711_H
#define __HX711_H

#include "sys.h"
extern float HX711_GapValue;            //传感器曲率   当发现测试出来的重量偏大时，增加该数值。
extern uint32_t Weight_Maopi;			//去皮参数

//#define HX711A_IN()  {GPIOC->MODER&=~(3<<(12*2));GPIOC->MODER|=0<<(12*2);}	//PC12输入模式
//#define HX711A_OUT() {GPIOC->MODER&=~(3<<(12*2));GPIOC->MODER|=1<<(12*2);} 	//PC12输出模式

//#define HX711A_DOUT_OUT 	PCout(12)
//#define HX711A_DOUT_IN 		PCin(12)
//#define HX711A_SCK  			PDout(2)
#define HX711_DOUT			PCin(12)
#define HX711_SCK  			PDout(2)

void HX711_Init(void);	//HX711初始化
//u32 HX711_Read(void);		//读取HX711-增益128
//long HX711_weigh(void); //获取重量
//long Measurement_Weigh(void);
unsigned long HX711_Read(void);	//增益128
uint32_t Get_Weight(void);

#endif
