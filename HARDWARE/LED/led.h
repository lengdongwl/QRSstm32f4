#ifndef _LED_H
#define _LED_H
#include "sys.h"

#define LED_1 PEout(2)   	//实验版LED1
#define LED_2 PEout(1)   	//实验版LED2
#define LED_3 PEout(0)  	//核心版LED 和 实验版LED3
#define LED_4 PEout(3)   	//实验版LED4

void LED_Init(void);		//LED初始化

#endif
