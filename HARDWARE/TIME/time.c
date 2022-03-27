
/**
  ******************************************************************************
  * @file    time.c
  * @author  xie
  * @version V1
  * @date    2021年10月31日
  * @brief   定时器底层驱动库
	
						端口定义：
							无
  ******************************************************************************
**/
#include "time.h"
#include "led.h"
TIM_HandleTypeDef TIM5_Handler;
TIM_HandleTypeDef TIM2_Handler;	 //定时器句柄
TIM_HandleTypeDef TIM12_Handler; //定时器句柄
//通用定时器2初始化 84MHz
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
void TIM2_Init(u16 arr, u16 psc)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	TIM2_Handler.Instance = TIM2;							  //通用定时器2
	TIM2_Handler.Init.Prescaler = psc;						  //分频
	TIM2_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;		  //向上计数器
	TIM2_Handler.Init.Period = arr;							  //自动装载值
	TIM2_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //时钟分频因子
	HAL_TIM_Base_Init(&TIM2_Handler);
	//定时器时钟源配置
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&TIM2_Handler, &sClockSourceConfig) != HAL_OK)
	{
		//			Error_Handler();
	}
	///下面两个句柄在DMA模式下都需要进行配置，包括上面的时钟源
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;		 //定时器更新触发
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE; //定时器主从机模式关闭

	if (HAL_TIMEx_MasterConfigSynchronization(&TIM2_Handler, &sMasterConfig) != HAL_OK)
	{
		//			Error_Handler();
	}

	HAL_TIM_Base_Start(&TIM2_Handler); //Time Base Start
}

//通用定时器12中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
void TIM12_Init(u16 arr, u16 psc)
{
	TIM12_Handler.Instance = TIM12;							   //通用定时器3
	TIM12_Handler.Init.Prescaler = psc;						   //分频系数
	TIM12_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;	   //向上计数器
	TIM12_Handler.Init.Period = arr;						   //自动装载值
	TIM12_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //时钟分频因子
	HAL_TIM_Base_Init(&TIM12_Handler);

	//    HAL_TIM_Base_Start_IT(&TIM12_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE
}
//定时器12中断服务函数
uint8_t TIME12_Flag = 0;
void TIM8_BRK_TIM12_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM12_Handler);
	TIME12_Flag = 1;
}

void TIM5_Init(u32 arr, u32 psc)
{
	__HAL_RCC_TIM5_CLK_ENABLE();
	TIM5_Handler.Instance = TIM5;							  //通用定时器4
	TIM5_Handler.Init.Prescaler = psc;						  //分频系数
	TIM5_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;		  //向上计数器
	TIM5_Handler.Init.Period = arr;							  //自动装载值
	TIM5_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //时钟分频因子
	HAL_TIM_Base_Init(&TIM5_Handler);						  //定时器初始化
	HAL_TIM_Base_Start_IT(&TIM5_Handler);
}

void TIM5_DeInit(void)
{

	// HAL_TIM_Base_Stop_IT(&TIM5_Handler);
	HAL_TIM_Base_DeInit(&TIM5_Handler);
}


//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE(); //使能TIM2时钟
	}
	if (htim->Instance == TIM3)
	{

		__HAL_RCC_TIM3_CLK_ENABLE(); //使能TIM3时钟

		HAL_NVIC_SetPriority(TIM3_IRQn, 1, 3); //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);		   //开启ITM3中断
	}
	if (htim->Instance == TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();		   //使能TIM4时钟
		HAL_NVIC_SetPriority(TIM4_IRQn, 1, 2); //设置中断优先级，抢占优先级1，子优先级2
		HAL_NVIC_EnableIRQ(TIM4_IRQn);		   //开启ITM4中断
	}
	if (htim->Instance == TIM7)
	{
		__HAL_RCC_TIM7_CLK_ENABLE();		   //使能TIM7时钟
		HAL_NVIC_SetPriority(TIM7_IRQn, 2, 1); //设置中断优先级，抢占优先级1，子优先级0
		HAL_NVIC_EnableIRQ(TIM7_IRQn);		   //开启TIM7中断
	}
	if (htim->Instance == TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM5_IRQn, 1, 1); //设置中断优先级，抢占优先级1，子优先级0
		HAL_NVIC_EnableIRQ(TIM5_IRQn);
	}
	if (htim->Instance == TIM12)
	{
		__HAL_RCC_TIM12_CLK_ENABLE();					 //使能TIM12时钟
		HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 3, 3); //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);		 //开启ITM3中断
	}
}
