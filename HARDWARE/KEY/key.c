/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-17 19:22:23
 */

/**
  ******************************************************************************
  * @file    key.c
  * @author  xie
  * @version V1
  * @date    2021年10月31日
  * @brief   按键驱动函数库
	
						端口定义：
						PB2：KEY_A 	按键1
						PB0：KEY_B 	按键2
						PB1：KEY_C 	按键3
						PE10：EC11_KEY 	EC11按键
  ******************************************************************************
**/	
#include "key.h"

/**
  * @brief  按键初始化
  * @param  None
  * @retval	None 
  * @note		None
  */
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();           //开启GPIOE时钟
  	__HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;//PB0,1,2,3
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  		//输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_LOW;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);			//初始化端口
	
    GPIO_Initure.Pin=GPIO_PIN_10;						//PE10
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  		//输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_LOW;     //高速
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);			//初始化端口

    GPIO_Initure.Pin=GPIO_PIN_0;						//PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  		//输入
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_LOW;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);			//初始化端口
}

//按键中断初始化
void KEY_Init_IT(void)
{
    GPIO_InitTypeDef GPIO_Initure;

  	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;//PB0,1,2,3
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;  		//输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_LOW;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);			//初始化端口
	
	  //中断线9-PB9
    HAL_NVIC_SetPriority(EXTI0_IRQn,0,0);  
    HAL_NVIC_SetPriority(EXTI1_IRQn,0,0); 
    HAL_NVIC_SetPriority(EXTI2_IRQn,0,0); 
    HAL_NVIC_EnableIRQ(EXTI0_IRQn); 
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}


//@parm mode 1=连续按键 0.按键只能使用一次
unsigned char KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY_S1==0||KEY_S2==0||KEY_S3==0||KEY_UP==1||EC11_KEY==0))
	{
		delay_ms(20);//去抖动 
		key_up=0;
		if(KEY_S1==0)return 1;
		else if(KEY_S2==0)return 2;
		else if(KEY_S3==0)return 3;
		else if(KEY_UP==1)return 4;
    else if(EC11_KEY==0)return 5;
	}else if(KEY_S1==1&&KEY_S2==1&&KEY_S3==1&&KEY_UP==0&&EC11_KEY==1)key_up=1; 	    
 	return 0;// 无按键按下
}
