/*
 * @Description: GUI�Ӽ��˵�����������
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
extern float Task_temperature;//�ڲ������� �¶�
extern uint16_t Task_bh;//�ڲ������� ���ն�
extern float Task_distance;//�ڲ������� ���
void Task_Init(void);
void Task_1_Func(u8 page_index, u8 key_val);//��ɫ��
void Task_2_Func(u8 page_index, u8 key_val);//������
void Task_3_Func(u8 page_index, u8 key_val);//ADC�ɼ�
void Task_4_Func(u8 page_index, u8 key_val);//DAC���
void Task_5_Func(u8 page_index, u8 key_val);//�ڲ�������
void Task_6_Func(u8 page_index, u8 key_val);//��������
void Task_7_Func(u8 page_index, u8 key_val);//�����
void Task_8_Func(u8 page_index, u8 key_val);//��չģ��
void Task_81_Func(u8 page_index, u8 key_val);//��չģ��PIN4
void Task_813_Func(u8 page_index, u8 key_val);//RFID
void Task_82_Func(u8 page_index, u8 key_val);//��չģ��PIN3
void Task_9_Func(u8 page_index, u8 key_val);//OLED��ʾ
void Task_10_Func(u8 page_index, u8 key_val);//����
void Task_11_Func(u8 page_index, u8 key_val);//RTCʱ������

void Task_speechRecognition(void);//����ʶ�����ݴ���
#endif
