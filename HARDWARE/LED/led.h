#ifndef _LED_H
#define _LED_H
#include "sys.h"

#define LED_1 PEout(2)   	//ʵ���LED1
#define LED_2 PEout(1)   	//ʵ���LED2
#define LED_3 PEout(0)  	//���İ�LED �� ʵ���LED3
#define LED_4 PEout(3)   	//ʵ���LED4

void LED_Init(void);		//LED��ʼ��

#endif
