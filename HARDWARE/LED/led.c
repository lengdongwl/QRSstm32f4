/**
  ******************************************************************************
  * @file    led.c
  * @author  xie
  * @version V2
  * @date    2021��10��28��
  * @brief   LED������������
	
						�˿ڶ��壺
						PE0��LED 	LED�ƿ�������
  ******************************************************************************
**/	
#include "led.h"

/**
  * @brief  LED��ʼ��
  * @param  None
  * @retval	None 
  * @note		None
  */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOE_CLK_ENABLE();           //����GPIOFʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;//PE0,1,2,3
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);			//��ʼ���˿�
}
