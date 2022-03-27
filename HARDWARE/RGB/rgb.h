/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2022-03-27 20:05:36
 */

#ifndef _RGB_H
#define _RGB_H
#include "sys.h"

#define RGB_Red 	PDout(3)   	//RGB	红
#define RGB_Green PDout(4)   	//RGB 绿
#define RGB_Blue 	PDout(5)   	//RGB 蓝

extern uint8_t RGB_COLOR_RED[];//红色
extern uint8_t  RGB_COLOR_GREEN [];//绿色
extern uint8_t  RGB_COLOR_BLUE [];//蓝色
extern uint8_t  RGB_COLOR_YELLOW [];//黄色
extern uint8_t  RGB_COLOR_SORT [];//品色
extern uint8_t  RGB_COLOR_SYAN [];//青色
extern uint8_t  RGB_COLOR_WHITE [];//白色
extern uint8_t  RGB_COLOR_BLACK [];//黑色 无色

void RGB_Init(void);		//LED初始化
void RGB_DeInit(void);		//删除初始化
void RGB(	uint8_t R_Data,
					uint8_t G_Data,
					uint8_t B_Data);//RGB颜色控制
void RGB_Set(uint8_t *RGB_COLOR_x);
#endif
