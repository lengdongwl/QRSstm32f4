/**
  ******************************************************************************
  * @file    GP2Y0A21.c
  * @author  xie
  * @version V1
  * @date    2021年11月1日
  * @brief   扩展端口模块处理函数库
	
						端口定义：
							PA1: ADC2_IN 测距模拟输入
	******************************************************************************
**/	
#include "GP2Y0A21.h"
#include "delay.h"

ADC_HandleTypeDef ADC2_Handler;//ADC句柄


//初始化ADC
//ch: ADC_channels 
//通道值 0~16取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
void MY_ADC2_Init(void)
{ 
    ADC2_Handler.Instance=ADC2;
    ADC2_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=90/4=22.5MHZ
    ADC2_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12位模式
    ADC2_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADC2_Handler.Init.ScanConvMode=DISABLE;                      //非扫描模式
    ADC2_Handler.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    ADC2_Handler.Init.ContinuousConvMode=DISABLE;                //关闭连续转换
    ADC2_Handler.Init.NbrOfConversion=1;                         //1个转换在规则序列中 也就是只转换规则序列1 
    ADC2_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC2_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    ADC2_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
    ADC2_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADC2_Handler.Init.DMAContinuousRequests=DISABLE;             //关闭DMA请求
    HAL_ADC_Init(&ADC2_Handler);                                 //初始化 

    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC2_CLK_ENABLE();            //使能ADC2时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();						//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_1;            //PA1
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //模拟
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);

}

void GP2Y0A21_Init(void)
{
	MY_ADC2_Init();
}

//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
u16 Get_ADC2(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC2_ChanConf;
    
    ADC2_ChanConf.Channel=ch;                                   //通道
    ADC2_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC2_ChanConf.SamplingTime=ADC_SAMPLETIME_480CYCLES;        //采样时间
    ADC2_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC2_Handler,&ADC2_ChanConf);        //通道配置
	
    HAL_ADC_Start(&ADC2_Handler);                               //开启ADC
	
    HAL_ADC_PollForConversion(&ADC2_Handler,10);                //轮询转换
 
	return (u16)HAL_ADC_GetValue(&ADC2_Handler);	        //返回最近一次ADC2规则组的转换结果
}
//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_ADC2_Average(u32 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_ADC2(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 

//测距
double GP2Y0A21_Distance(void) 
{	
	double distemp=Get_ADC2_Average(ADC_CHANNEL_1,10)*3.3/4095;
	distemp=(-13.2*distemp*distemp*distemp)+72.84*distemp*distemp-140*distemp+107.12;
	return distemp;
}

