/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-12-05 19:34:23
 */
/**
  ******************************************************************************
  * @file    ec11.c
  * @author  xie
  * @version V1
  * @date    2021年10月31日
  * @brief   
	
						端口定义： EC11_A PEin(9)  
								  EC11_B PEin(11)  

******************************************************************************
**/
#include "ec11.h"
#include "led.h"
#include "delay.h"
int EC11_Value = 0;
void EC11_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure; //GPIO句柄

	__HAL_RCC_GPIOE_CLK_ENABLE(); //开启GPIOE时钟
	/*PE11 B*/
	GPIO_Initure.Pin = GPIO_PIN_11;			   //PE11
	GPIO_Initure.Mode = GPIO_MODE_IT_FALLING;   //下升沿触发
	GPIO_Initure.Pull = GPIO_PULLDOWN;		   //下拉
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; //高速
	HAL_GPIO_Init(GPIOE, &GPIO_Initure);

	//中断线11-PE11
	//HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 2); //抢占优先级为2，子优先级为0
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0); //抢占优先级为2，子优先级为0
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);			//使能中断线0

	/*PE9 A*/
	GPIO_Initure.Pin = GPIO_PIN_9;		  //PE9
	GPIO_Initure.Mode = GPIO_MODE_INPUT;  //输入
	GPIO_Initure.Pull = GPIO_PULLDOWN;	  //下拉
	GPIO_Initure.Speed = GPIO_SPEED_HIGH; //高速
	HAL_GPIO_Init(GPIOE, &GPIO_Initure);  //初始化端口
	EC11_Value = 0;
}

void EC11_DeInit(void)
{
	HAL_GPIO_DeInit(GPIOE,GPIO_PIN_11|GPIO_PIN_9);
}


//中断服务函数
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11); //调用中断处理公用函数
	delay_us(500);
	if (EC11_B == 1)
	{
		if (EC11_A == 1)
		{
			EC11_Value++; //顺时针
						   //delay_ms(10);
		}
		else
		{
			EC11_Value--; //逆时针
		}
	}
}

int EC11_Get_Value(void)
{
	return EC11_Value;
}

