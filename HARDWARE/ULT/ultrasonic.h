/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-19 15:01:32
 */

#ifndef _ULTASONIC_H
#define _ULTASONIC_H
#include "sys.h"

#define INC PBout(8)   								//超声波发射端
extern uint16_t Distance;							//距离

void Ultrasonic_Init(void);
uint16_t Ultrasonic_Ranging(void);
void Ultrasonic_DeInit(void);
uint16_t Ultrasonic_Distance(void);	//多次测距取中值
float Ultrasonic_GetBuffer(void);
#endif
