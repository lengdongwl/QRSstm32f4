/*
 * @Description: GUI子级菜单操作函数库
 * @Autor: 309 Mushroom
 * @Date: 2021-11-17 21:38:03
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2022-03-27 20:34:51
 */
#ifndef __MYGUI_FUNC_H  
#define __MYGUI_FUNC_H   
#include "sys.h"
extern char oledbuf[100];
extern u8 Task_Status[66];
extern float Task_temperature;//内部传感器 温度
extern uint16_t Task_bh;//内部传感器 光照度
extern float Task_distance;//内部传感器 测距
void Task_Init(void);
void Task_1_Func(u8 page_index, u8 key_val);//三色灯
void Task_2_Func(u8 page_index, u8 key_val);//蜂鸣器
void Task_3_Func(u8 page_index, u8 key_val);//ADC采集
void Task_4_Func(u8 page_index, u8 key_val);//DAC输出
void Task_5_Func(u8 page_index, u8 key_val);//内部传感器
void Task_6_Func(u8 page_index, u8 key_val);//语音交互
void Task_7_Func(u8 page_index, u8 key_val);//数码管
void Task_8_Func(u8 page_index, u8 key_val);//扩展模块
void Task_81_Func(u8 page_index, u8 key_val);//扩展模块PIN4
void Task_813_Func(u8 page_index, u8 key_val);//RFID
void Task_82_Func(u8 page_index, u8 key_val);//扩展模块PIN3
void Task_9_Func(u8 page_index, u8 key_val);//OLED显示
void Task_10_Func(u8 page_index, u8 key_val);//备用
void Task_11_Func(u8 page_index, u8 key_val);//RTC时钟设置

void Task_speechRecognition(void);//语音识别内容处理
#endif
