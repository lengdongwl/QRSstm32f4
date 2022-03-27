/**
  ******************************************************************************
  * @file    rgb.c
  * @author  xie
  * @version V2
  * @date    2021年10月28日
  * @brief   全彩RGB三色灯驱动 

						端口定义：
							PD3:  RGB_Red 	RGB	红
							PD4:  RGB_Green RGB 绿
							PD5: 	RGB_Blue 	RGB 蓝
							TIM3：RGB颜色控制
  ******************************************************************************
**/
#include "rgb.h"
#include "led.h"
 uint8_t RGB_COLOR_RED[]={1,0,0};//红色
 uint8_t  RGB_COLOR_GREEN []={0,1,0};//绿色
 uint8_t  RGB_COLOR_BLUE []={0,0,1};//蓝色
 uint8_t  RGB_COLOR_YELLOW []={1,1,0};//黄色
 uint8_t  RGB_COLOR_SORT []={1,0,1};//品色
 uint8_t  RGB_COLOR_SYAN []={0,1,1};//青色
 uint8_t  RGB_COLOR_WHITE []={1,1,1};//白色
 uint8_t  RGB_COLOR_BLACK []={0,0,0};//黑色 无色
uint8_t RGB_Colour[3]; //RGB阈值

TIM_HandleTypeDef TIM3_Handler; //定时器句柄

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
void TIM3_Init(u16 arr, u16 psc)
{
	TIM3_Handler.Instance = TIM3;							  //通用定时器3
	TIM3_Handler.Init.Prescaler = psc;						  //分频系数
	TIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;		  //向上计数器
	TIM3_Handler.Init.Period = arr;							  //自动装载值
	TIM3_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //时钟分频因子
	HAL_TIM_Base_Init(&TIM3_Handler);
}

//定时器3中断服务函数


uint8_t TIM3_Count = 0;
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM3_Handler);

	if (TIM3_Count < RGB_Colour[0])
	{ //红色
		RGB_Red = 1;
	}
	else
	{
		RGB_Red = 0;
	}
	if (TIM3_Count < RGB_Colour[1])
	{ //绿色
		RGB_Green = 1;
	}
	else
	{
		RGB_Green = 0;
	}
	if (TIM3_Count < RGB_Colour[2])
	{ //蓝色
		RGB_Blue = 1;
	}
	else
	{
		RGB_Blue = 0;
	}
	TIM3_Count += 1;
	if (TIM3_Count >= 255)
	{
		TIM3_Count = 0;
	}
}

/**
  * @brief  RGB初始化
  * @param  None
  * @retval	None 
  * @note		None
  */
void RGB_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOD_CLK_ENABLE(); //开启GPIOD时钟

	GPIO_Initure.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5; //PE0
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;				 //推挽输出
	GPIO_Initure.Pull = GPIO_PULLUP;						 //上拉
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;					 //高速
	HAL_GPIO_Init(GPIOD, &GPIO_Initure);					 //初始化端口

	RGB_Red = 0;
	RGB_Green = 0;
	RGB_Blue = 0;

	TIM3_Init(50 - 1, 42 - 1); //定时器3初始化
}

//删除初始化
void RGB_DeInit(void)
{
	HAL_GPIO_DeInit(GPIOD, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
	HAL_TIM_Base_DeInit(&TIM3_Handler);
}

/**
  * @brief  RGB颜色控制
  * @param  R_Data：红色输出值 
					  G_Data：绿色输出值
					  B_Data：蓝色输出值
  * @retval	None 
  * @note		0-255
  */
void RGB(uint8_t R_Data, uint8_t G_Data, uint8_t B_Data)
{
	if (R_Data != 0 || G_Data != 0 || B_Data != 0)
	{
		RGB_Colour[0] = R_Data;				  //红
		RGB_Colour[1] = G_Data;				  //绿
		RGB_Colour[2] = B_Data;				  //黄
		HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器13和定时器13更新中断：TIM_IT_UPDATE
	}
	else
	{
		HAL_TIM_Base_Stop_IT(&TIM3_Handler); //关闭定时器13和定时器13更新中断：TIM_IT_UPDATE
		RGB_Red = 0;
		RGB_Green = 0;
		RGB_Blue = 0;
	}
}


/**
 * @description: 设置RGB灯颜色
 * @param {uint8_t} *RGB_COLOR_x
 * @return {*}
 */
void RGB_Set(uint8_t *RGB_COLOR_x)
{
	RGB_Red = RGB_COLOR_x[0];
	RGB_Green = RGB_COLOR_x[1];
	RGB_Blue = RGB_COLOR_x[2];
}