/**
  ******************************************************************************
  * @file    ultrasonic.c
  * @author  xie
  * @version V1
  * @date    2021年10月31日
  * @brief   超声波测距函数库
	
						端口定义：
						PB8：INC 		超声波发射端
						PB9：INT0 	超声波接收端
  ******************************************************************************
**/	
#include "ultrasonic.h"
#include "delay.h"
#include "oled.h"
#include "stdio.h"
#include "led.h"
float Ultrasonic_Value = 0;					 //距离计数
float dis=0;
uint32_t Ultrasonic_Num=0;					 //计数值
uint16_t Distance_Temp=0;						 //距离处理前
uint16_t Distance=0;								 		 //距离

TIM_HandleTypeDef TIM7_Handler;      //定时器句柄 
GPIO_InitTypeDef GPIO_Initure;			 //GPIO句柄 


/**
  * @brief  超声波端口初始化
  * @param  None
  * @retval	None 
  * @note		None
  */
void Ultrasonic_Port(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIOB时钟
	
		/*INC 超声波发射端*/	
    GPIO_Initure.Pin=GPIO_PIN_8;						//PB8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);			//初始化端口	
	
		/*INT0 超声波接收端*/
    GPIO_Initure.Pin=GPIO_PIN_9;						//PB9
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  		//输入
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);			//初始化端口	
	

		INC = 0;

}

/**
  * @brief  超声波 定时器配置
  * @param  None
  * @retval	None 
  * @note		None
  */
void TIM7_Init(uint16_t arr,uint16_t psc)
{
    TIM7_Handler.Instance=TIM7;                          //通用定时器3
    TIM7_Handler.Init.Prescaler=psc;                     //分频系数
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM7_Handler.Init.Period=arr;                        //自动装载值
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM7_Handler);

	
}
  
//定时器6中断服务函数 
uint8_t LED_T=0;
void TIM7_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM7_Handler);
    Ultrasonic_Num++;
		if(Ultrasonic_Num==0x1fff){							//距离过长取消显示
			Distance=0;	
		}
}

/**
  * @brief  超声波 外部中断配置
  * @param  None
  * @retval	None 
  * @note		None
  */
void Ultrasonic_EXTI(void)
{
		/*INT0-PB9*/
		GPIO_Initure.Pin=GPIO_PIN_9;   							//PB9
		GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;  		//下降沿触发
		GPIO_Initure.Pull=GPIO_PULLUP;          		//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	
	
	  //中断线9-PB9
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,1,0);       //抢占优先级为2，子优先级为0
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);             //使能中断线0
}

//中断服务函数
void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);		//调用中断处理公用函数
}

//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_9)
	{
 		HAL_TIM_Base_Stop(&TIM7_Handler);	//失能定时器3
		Ultrasonic_Value = (float)Ultrasonic_Num;
		Ultrasonic_Value = (float)Ultrasonic_Value*1.72f - 20.0f;       // 计算距离定时10us，S=Vt/2（减2是误差补尝）
		Distance_Temp  = (uint16_t) Ultrasonic_Value;
		dis=Ultrasonic_Value;		
	}
}

//超声波初始化
void Ultrasonic_Init(void)
{
	Ultrasonic_Port();			// 超声波硬件端口初始化
	TIM7_Init(10-1,84-1);
	Ultrasonic_EXTI();			// 超声波接收引脚中断初始化
}
void Ultrasonic_DeInit(void)
{
	HAL_GPIO_DeInit(GPIOB,GPIO_PIN_8|GPIO_PIN_9);
	HAL_TIM_Base_DeInit(&TIM7_Handler);
}
uint16_t Ultrasonic_Ranging(void)
{
		INC = 1;            
		delay_us(3);
		INC = 0;
	
		Ultrasonic_Num  = 0;		// 定时器清零
		__HAL_TIM_CLEAR_IT(&TIM7_Handler,TIM_IT_UPDATE);//清除更新中断标志位
		HAL_TIM_Base_Start_IT(&TIM7_Handler); //使能定时器3

		delay_ms(30);				// 等待一段时间，等待发送超声波控制信号
		INC = 1;
		delay_ms(5);
	
 		HAL_TIM_Base_Stop(&TIM7_Handler);	//失能定时器3
		return Distance_Temp;
}

//多次测距取中值
uint16_t Ultrasonic_Distance(void)
{
	uint8_t i,j;
	uint16_t Temp=0;		 //暂时缓存
	uint16_t Arr_Data[8];//距离排序缓存
	/*循环获取*/
	for(i=0;i<8;i++)
	{
		Ultrasonic_Ranging();     //超声波测距
		delay_ms(10);
		Arr_Data[i]=Distance_Temp;
	}
	/*进行排序计算*/
	for(i=0;i<8;i++) 
	{
		for(j=0;j<7-i;j++) 
		{
			if(Arr_Data[j]>Arr_Data[j+1]) 
			{
				Temp = Arr_Data[j];
				Arr_Data[j] = Arr_Data[j+1];
				Arr_Data[j+1] = Temp; 
			}
		}
	}	
	Distance=(Arr_Data[3]+Arr_Data[4])/2;//取中值
	return Distance;
}

/**
 * @description: 发送超声波测距并读取float结果
 * @param {*}
 * @return {*}
 */
float Ultrasonic_GetBuffer(void)
{
	int  _count;
	float r=0;
	dis = 0;
	Ultrasonic_Ranging();
	while (dis == 0)
	{
		_count++;
		if (_count > 1000) //防止死循环1秒无结果就跳出循环
		{
			break;
		}
		delay_ms(1);
	}
	r = dis;
	
	return r;
}