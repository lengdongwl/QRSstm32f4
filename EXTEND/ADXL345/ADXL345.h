/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-12 15:28:52
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-14 19:48:57
 */
#ifndef __ADXL345_H
#define __ADXL345_H
#include "delay.h"
u8 ADXL345_Init();
short ADXL345_Get_Angle(float x, float y, float z, u8 dir); //����Ƕ�
void ADXL345_ReadReg_times(short *x, short *y, short *z, u8 times);//����x��y��z������ٶ�ֵ
void ADXL345ReadReg_XYZ(short *x, short *y, short *z);
#endif