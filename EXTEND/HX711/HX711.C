/**
  ******************************************************************************
  * @file    HX711.c
  * @author  xie
  * @version V1
  * @date    2021年11月2日
  * @brief   扩展端口模块称重传感器函数库
	
						端口定义：
						PC12:	HX711_DOUT	HX711数据引脚	
						PD2:	HX711_SCK		HX711时钟引脚	
	******************************************************************************
**/	

#include "HX711.H"
#include "delay.h"

uint32_t HX711_Buffer = 0;
uint32_t Weight_Maopi = 0; 
long  Weight_object = 0;
float HX711_GapValue = 1000;	   //传感器曲率   当发现测试出来的重量偏大时，增加该数值。
//如果测试出来的重量偏小时，减小改数值。该值可以为小数，例如 429.5 等。

/**
  * @brief  HX711初始化
  * @param  None
  * @retval	None 
  * @note		None
  */
void HX711_Init(void)
{
		GPIO_InitTypeDef GPIO_Initure;
     
    __HAL_RCC_GPIOC_CLK_ENABLE();           		//开启GPIOC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();           		//开启GPIOD时钟
		
    GPIO_Initure.Pin=GPIO_PIN_12;								//PC12
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  				//输入
    GPIO_Initure.Pull=GPIO_NOPULL;          		//浮空
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;   			//高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);	

    GPIO_Initure.Pin=GPIO_PIN_2;								//PD2
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          		//浮空
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    			//高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);		
    delay_ms(1);
		Weight_Maopi = HX711_Read();	//获取称重初始值
}

/**
  * @brief  读取HX711的值
  * @param  None
  * @retval	HX711的值 
  * @note		None
  */
unsigned long HX711_Read(void)	//增益128
{
	unsigned long count; 
	uint8_t i,t=0; 
	
  	HX711_SCK=0; 
  	count = 0; 
  	while(HX711_DOUT)
    {
      delay_ms(1);
      t++;
      if(t>=2000)//超过两秒返回0
      {
        break;
      }
    }
	
  	for(i=0; i<24; i++)
	{ 
	  	HX711_SCK=1; 
	  	count = count << 1; 
		HX711_SCK=0; 
	  	if(HX711_DOUT)
			count++; 
	} 
 	HX711_SCK=1; 
    count = count ^ 0x800000;  //第25个脉冲下降沿来时，转换数据
	HX711_SCK=0;  


  /*出现初始值大于称重值时无法称重 将其除以2可以得出结果*/
  if (count>16700000)
  {
    count/=2;
  }
  
	return(count);
}

/**
  * @brief  获取毛皮重量(去皮)
  * @param  None
  * @retval	None
  * @note		None
  */
void Get_Maopi(void)
{
	Weight_Maopi = HX711_Read();	
} 

/**
  * @brief  称重
  * @param  None
  * @retval	重量
  * @note		None
  */
uint32_t Get_Weight(void)
{
	HX711_Buffer = HX711_Read();

	Weight_object = HX711_Buffer;
	Weight_object = Weight_object - Weight_Maopi;	   		// 获取实物的AD采样数值。

	if(Weight_object> 0)
	{
		Weight_object= ((float)Weight_object / HX711_GapValue); 	// 计算实物的实际重量	
	}
	else
	{
		Weight_object = 0;
	}
	return Weight_object;
}

