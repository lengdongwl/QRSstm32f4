/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-17 15:14:40
 */
/**
  ******************************************************************************
  * @file    bee.c
  * @author  xie
  * @version V1
  * @date    2021��10��28��
  * @brief   ����������������
	
						�˿ڶ��壺
						PD1��BEE 	��������������
  ******************************************************************************
**/	
#include "bee.h"

/**
  * @brief  ��������ʼ��
  * @param  None
  * @retval	None 
  * @note		None
  */
void BEE_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOD_CLK_ENABLE();           //����GPIOFʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_1;						//PD1
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);			//��ʼ���˿�
	
		BEE=0;
}

void BEE_DeInit(void)
{
    HAL_GPIO_DeInit(GPIOD,GPIO_PIN_1);			//��ʼ���˿�
}
