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
  * @date    2021年10月28日
  * @brief   蜂鸣器驱动函数库
	
						端口定义：
						PD1：BEE 	蜂鸣器控制引脚
  ******************************************************************************
**/	
#include "bee.h"

/**
  * @brief  蜂鸣器初始化
  * @param  None
  * @retval	None 
  * @note		None
  */
void BEE_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOD_CLK_ENABLE();           //开启GPIOF时钟
	
    GPIO_Initure.Pin=GPIO_PIN_1;						//PD1
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);			//初始化端口
	
		BEE=0;
}

void BEE_DeInit(void)
{
    HAL_GPIO_DeInit(GPIOD,GPIO_PIN_1);			//初始化端口
}
