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

#define INC PBout(8)   								//�����������
extern uint16_t Distance;							//����

void Ultrasonic_Init(void);
uint16_t Ultrasonic_Ranging(void);
void Ultrasonic_DeInit(void);
uint16_t Ultrasonic_Distance(void);	//��β��ȡ��ֵ
float Ultrasonic_GetBuffer(void);
#endif
