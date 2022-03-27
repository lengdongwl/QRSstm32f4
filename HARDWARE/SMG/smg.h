/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2022-03-27 19:58:38
 */
#ifndef _SMG_H
#define _SMG_H
#include "sys.h"

#define SER PDout(15)   	//75HC595数据输入段
#define SCK PCout(8)   		//75HC595串行时钟
#define RCK PCout(7)   		//75HC595并行时钟
#define OE PCout(10)   		//75HC595输出使能控制端

#define SMG_1 PDout(13)   //数码管位选1
#define SMG_2 PDout(14)   //数码管位选2
#define SMG_3 PDout(11)   //数码管位选3
#define SMG_4 PDout(12)   //数码管位选4

extern uint8_t SMGSWAP;//数码管位选选择 便于调节

void SMG_Init(void);																				//数码管初始化
void SMG_DeInit(void);
void SMG_Display_Bit(uint8_t Input_Data,uint8_t Bit_Data);	//数码管单个位显示
void SMG_Display(uint16_t Input_Data,uint8_t Ctrl_Bit);			//数码管显示
void SMG_DisplayP(int16_t Input_Data4,int16_t Input_Data3,int16_t Input_Data2,int16_t Input_Data1,uint8_t Ctrl_Bit);

#endif
