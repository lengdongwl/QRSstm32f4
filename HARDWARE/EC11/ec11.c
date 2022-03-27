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
  * @date    2021��10��31��
  * @brief   
	
						�˿ڶ��壺 EC11_A PEin(9)  
								  EC11_B PEin(11)  

******************************************************************************
**/
#include "ec11.h"
#include "led.h"
#include "delay.h"
int EC11_Value = 0;
void EC11_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure; //GPIO���

	__HAL_RCC_GPIOE_CLK_ENABLE(); //����GPIOEʱ��
	/*PE11 B*/
	GPIO_Initure.Pin = GPIO_PIN_11;			   //PE11
	GPIO_Initure.Mode = GPIO_MODE_IT_FALLING;   //�����ش���
	GPIO_Initure.Pull = GPIO_PULLDOWN;		   //����
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; //����
	HAL_GPIO_Init(GPIOE, &GPIO_Initure);

	//�ж���11-PE11
	//HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 2); //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0); //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);			//ʹ���ж���0

	/*PE9 A*/
	GPIO_Initure.Pin = GPIO_PIN_9;		  //PE9
	GPIO_Initure.Mode = GPIO_MODE_INPUT;  //����
	GPIO_Initure.Pull = GPIO_PULLDOWN;	  //����
	GPIO_Initure.Speed = GPIO_SPEED_HIGH; //����
	HAL_GPIO_Init(GPIOE, &GPIO_Initure);  //��ʼ���˿�
	EC11_Value = 0;
}

void EC11_DeInit(void)
{
	HAL_GPIO_DeInit(GPIOE,GPIO_PIN_11|GPIO_PIN_9);
}


//�жϷ�����
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11); //�����жϴ����ú���
	delay_us(500);
	if (EC11_B == 1)
	{
		if (EC11_A == 1)
		{
			EC11_Value++; //˳ʱ��
						   //delay_ms(10);
		}
		else
		{
			EC11_Value--; //��ʱ��
		}
	}
}

int EC11_Get_Value(void)
{
	return EC11_Value;
}

