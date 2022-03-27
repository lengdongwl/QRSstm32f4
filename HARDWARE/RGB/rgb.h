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

#define RGB_Red 	PDout(3)   	//RGB	��
#define RGB_Green PDout(4)   	//RGB ��
#define RGB_Blue 	PDout(5)   	//RGB ��

extern uint8_t RGB_COLOR_RED[];//��ɫ
extern uint8_t  RGB_COLOR_GREEN [];//��ɫ
extern uint8_t  RGB_COLOR_BLUE [];//��ɫ
extern uint8_t  RGB_COLOR_YELLOW [];//��ɫ
extern uint8_t  RGB_COLOR_SORT [];//Ʒɫ
extern uint8_t  RGB_COLOR_SYAN [];//��ɫ
extern uint8_t  RGB_COLOR_WHITE [];//��ɫ
extern uint8_t  RGB_COLOR_BLACK [];//��ɫ ��ɫ

void RGB_Init(void);		//LED��ʼ��
void RGB_DeInit(void);		//ɾ����ʼ��
void RGB(	uint8_t R_Data,
					uint8_t G_Data,
					uint8_t B_Data);//RGB��ɫ����
void RGB_Set(uint8_t *RGB_COLOR_x);
#endif
