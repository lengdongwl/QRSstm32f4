/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-12-16 16:06:01
 */

#ifndef _UART_H
#define _UART_H
#include "sys.h"

void UART1_Init(u32 bound);
void UART1_Send(uint8_t Send_Data);
void UART1_Send_String(uint8_t* Send_Data,uint8_t Size);

void UART2_Init(u32 bound);			
void UART2_DeInit(void);
void UART2_Send(uint8_t Send_Data);
void UART2_Send_String(uint8_t* Send_Data,uint8_t Size);

uint8_t XiaoChuang_ASR(void);
void XiaoChuang_PlayNUM(int number);
#endif
