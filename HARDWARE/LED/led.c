/**
  ******************************************************************************
  * @file    led.c
  * @author  xie
  * @version V2
  * @date    2021年10月28日
  * @brief   LED灯驱动函数库
	
						端口定义：
						PE0：LED 	LED灯控制引脚
  ******************************************************************************
**/	
#include "led.h"

/**
  * @brief  LED初始化
  * @param  None
  * @retval	None 
  * @note		None
  */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOE_CLK_ENABLE();           //开启GPIOF时钟
	
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;//PE0,1,2,3
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);			//初始化端口
}
