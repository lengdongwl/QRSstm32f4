/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-12-13 17:00:07
 */

#ifndef _TIME_H
#define _TIME_H
#include "sys.h"
extern TIM_HandleTypeDef TIM12_Handler;      //定时器句柄
extern TIM_HandleTypeDef TIM2_Handler;      //定时器句柄 
extern TIM_HandleTypeDef TIM5_Handler;
extern uint8_t TIME12_Flag;
void TIM2_Init(u16 arr,u16 psc);
void TIM5_Init(u32 arr, u32 psc);
void TIM5_DeInit(void);
#endif

