/**
  ******************************************************************************
  * @file    smg.c
  * @author  xie
  * @version V2
  * @date    2021年10月28日
  * @brief   数码管驱动函数库
	
						端口定义：
								PD15:SER		75HC595数据输入段
								PC8:SCK			75HC595串行时钟
								PC7:RCK			75HC595并行时钟
								PD10:OE			75HC595输出使能控制端
								PD11:SMG_1	数码管位选1
								PD12:SMG_2	数码管位选2
								PD13:SMG_3	数码管位选3
								PD14:SMG_4	数码管位选4				
										
  ******************************************************************************
**/	
#include "smg.h"
uint8_t SMG_MODE=0; //显示模式
uint16_t SMG_Data;//数码管动态扫描数据
int16_t SMG_DataARR[4];//数码管动态扫描各个位置上的数据

uint8_t SMGSWAP = 0;
/*8位数码管共阳段码*/
//uint8_t SEG_Table[17]={0xC0,0xF9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff}; 
//共阳                   0   1    2     3    4    5    6    7    8    9    A    B    C    D    E    F   关闭   
/*8位数码管共阴段码*/
uint8_t SEG_Table[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00}; 
//共阴                   0   1    2     3    4    5    6    7    8    9    A    B    C    D    E    F   关闭 

TIM_HandleTypeDef TIM4_Handler;      //定时器句柄 
void SMG_Position(uint8_t Bit_Data);
//通用定时器4中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
void TIM4_Init(u16 arr,u16 psc)
{  
    TIM4_Handler.Instance=TIM4;                          //通用定时器4
    TIM4_Handler.Init.Prescaler=psc;                     //分频系数
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM4_Handler.Init.Period=arr;                        //自动装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM4_Handler);	  
}

//定时器3中断服务函数

uint8_t TIM4_Count=0; 
void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM4_Handler);
	if(SMG_MODE==0)//数值显示
	{
		TIM4_Count+=1;
		switch(TIM4_Count)
		{
			case 1:{
				SMG_Display_Bit(SMG_Data/1000,4);			//数码管单个位显示
				break;
			}
			case 2:{
				SMG_Display_Bit(SMG_Data%1000/100,3);	//数码管单个位显示
				break;
			}
			case 3:{
				SMG_Display_Bit(SMG_Data%100/10,2);		//数码管单个位显示
				break;
			}
			case 4:{
				SMG_Display_Bit(SMG_Data%10,1);				//数码管单个位显示
				break;
			}		
		}
	}else //显示各个位置的数
	{
		TIM4_Count+=1;
		switch(TIM4_Count)
		{
			case 1:{
				if(SMG_DataARR[3]!=-1)
				{
					SMG_Display_Bit(SMG_DataARR[3],4);			//数码管单个位显示
				}
				break;
			}
			case 2:{
				if(SMG_DataARR[2]!=-1)
				{
					SMG_Display_Bit(SMG_DataARR[2],3);			//数码管单个位显示
				}
				break;
			}
			case 3:{
				if(SMG_DataARR[1]!=-1)
				{
					SMG_Display_Bit(SMG_DataARR[1],2);			//数码管单个位显示
				}
				break;
			}
			case 4:{
				if(SMG_DataARR[0]!=-1)
				{
					SMG_Display_Bit(SMG_DataARR[0],1);			//数码管单个位显示
				}
				break;
			}		
		}


	}
	if(TIM4_Count>=4){//溢出清空计数
		TIM4_Count=0;
	}

}
void SMG_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();           //开启GPIOC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();           //开启GPIOD时钟
	
    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|
			GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;	//PD11,12,13,14,15
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);			//初始化端口

    GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8;	//PC7,8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);			//初始化端口	
	
		OE=1;//输出使能
	
		TIM4_Init(200-1,84-1);      //定时器4初始化

}
void SMG_DeInit(void)
{
	HAL_GPIO_DeInit(GPIOD,GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|
			GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);	
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_7|GPIO_PIN_8);
	HAL_TIM_Base_DeInit(&TIM4_Handler);
}


/**
  * @brief  数码管单个位显示
  * @param  Input_Data：单个位数据 Bit_Data：位
  * @retval	None 
  * @note		None
  */
void SMG_Display_Bit(uint8_t Input_Data,uint8_t Bit_Data)
{
		/*段选*/
		uint8_t Data_Bit;
		int i=0;		
		for(i=0; i<8; i++)								     //当i=1时数码管显示小数点     
		{
				Data_Bit  = SEG_Table[Input_Data];//带点 |0x80
				if (((Data_Bit)<<i)&0x80)					//判断输入的数据			是 1 或者 0
				{
						SER=1;							
				}
				else
				{
						SER=0;
				}
				SCK = 0;									//移位(当确定输入的数据之后进行移位操作，把数据存进寄存器里面)
				SCK = 1;
		}					
		RCK = 0;											//寄存器里面的数据传给数码管
		RCK = 1;
		

		/*位选*/
		SMG_Position(Bit_Data);
}


/**
  * @brief  数码管显示
  * @param  Input_Data：数据 
  * @param  Ctrl_Bit:开关控制
  * @retval	None 
  * @note		None
  */
void SMG_Display(uint16_t Input_Data,uint8_t Ctrl_Bit)
{
		if(Ctrl_Bit){
				SMG_MODE=0; 
				SMG_Data=Input_Data;
				HAL_TIM_Base_Start_IT(&TIM4_Handler); //使能定时器4和定时器4更新中断：TIM_IT_UPDATE  					
		}
		else
		{
			HAL_TIM_Base_Stop_IT(&TIM4_Handler); //关闭定时器13和定时器13更新中断：TIM_IT_UPDATE
			SMG_1=0;
			SMG_2=0;
			SMG_3=0;
			SMG_4=0; 			
		}
}


/**
 * @description: 数码管显示各个位置
 * @param {int16_t} Input_Data4 -1关闭
 * @param {int16_t} Input_Data3
 * @param {int16_t} Input_Data2
 * @param {int16_t} Input_Data1
 * @param {uint8_t} Ctrl_Bit 开关定时器控制
 * @return {*}
 */
void SMG_DisplayP(int16_t Input_Data4,int16_t Input_Data3,int16_t Input_Data2,int16_t Input_Data1,uint8_t Ctrl_Bit)
{
		if(Ctrl_Bit){
				SMG_MODE=1;
				SMG_DataARR[3]=Input_Data4;
				SMG_DataARR[2]=Input_Data3;
				SMG_DataARR[1]=Input_Data2;
				SMG_DataARR[0]=Input_Data1;
				HAL_TIM_Base_Start_IT(&TIM4_Handler); //使能定时器4和定时器4更新中断：TIM_IT_UPDATE  					
		}
		else
		{
			HAL_TIM_Base_Stop_IT(&TIM4_Handler); //关闭定时器13和定时器13更新中断：TIM_IT_UPDATE
			SMG_1=0;
			SMG_2=0;
			SMG_3=0;
			SMG_4=0; 			
		}
}




void position1(void)
{
	SMG_1=0;
	SMG_2=1;
	SMG_3=0;
	SMG_4=0;
}
void position2()
{
	SMG_1=1;
	SMG_2=0;
	SMG_3=0;
	SMG_4=0;
}
void position3()
{
	SMG_1=0;
	SMG_2=0;
	SMG_3=0;
	SMG_4=1;
}
void position4()
{
	SMG_1=0;
	SMG_2=0;
	SMG_3=1;
	SMG_4=0;
}


void SMG_Position(uint8_t Bit_Data)
{
/*
1 2 3 4
1 2 4 3
1 3 2 4 
1 3 4 2
1 4 2 3 
1 4 3 2

2 1 3 4
2 1 4 3
2 3 1 4
2 3 4 1
2 4 1 3
2 4 3 1

3 1 2 4
3 1 4 2
3 2 1 4
3 2 4 1
3 4 1 2
3 4 2 1

4 1 2 3 
4 1 3 2
4 2 1 3
4 2 3 1
4 3 1 2
4 3 2 4
*/

	switch(SMGSWAP) //可通过调节此值调节数码管位置选择
	{
		case 0:
			//正常
			switch(Bit_Data)
			{
				case 1:position1();break;
				case 2:position2();break;	
				case 3:position3();break;
				case 4:position4();break;
			}
		break;
		case 1:
			
			switch(Bit_Data)
			{
				case 1:position1();break;
				case 2:position2();break;	
				case 3:position4();break;
				case 4:position3();break;
			}
		break;
		case 2:
			
			switch(Bit_Data)
			{
				case 1:position1();break;
				case 2:position3();break;	
				case 3:position2();break;
				case 4:position4();break;
			}
		break;
		case 3:
			
			switch(Bit_Data)
			{
				case 1:position1();break;
				case 2:position3();break;	
				case 3:position4();break;
				case 4:position2();break;
			}
		break;
		case 4:
			
			switch(Bit_Data)
			{
				case 1:position1();break;
				case 2:position4();break;	
				case 3:position2();break;
				case 4:position3();break;
			}
		break;
		case 5:
			
			switch(Bit_Data)
			{
				case 1:position1();break;
				case 2:position4();break;	
				case 3:position3();break;
				case 4:position2();break;
			}
		break;
		case 6:
			
			switch(Bit_Data)
			{
				case 1:position2();break;
				case 2:position1();break;	
				case 3:position3();break;
				case 4:position4();break;
			}
		break;
		case 7:
			
			switch(Bit_Data)
			{
				case 1:position2();break;
				case 2:position1();break;	
				case 3:position4();break;
				case 4:position3();break;
			}
		break;
		case 8:
			
			switch(Bit_Data)
			{
				case 1:position2();break;
				case 2:position3();break;	
				case 3:position1();break;
				case 4:position4();break;
			}
		break;
		case 9:
			
			switch(Bit_Data)
			{
				case 1:position2();break;
				case 2:position3();break;	
				case 3:position4();break;
				case 4:position1();break;
			}
		break;
		case 10:
			
			switch(Bit_Data)
			{
				case 1:position2();break;
				case 2:position4();break;	
				case 3:position1();break;
				case 4:position3();break;
			}
		break;
		case 11:
			
			switch(Bit_Data)
			{
				case 1:position2();break;
				case 2:position4();break;	
				case 3:position3();break;
				case 4:position1();break;
			}
		break;
		case 12:
			
			switch(Bit_Data)
			{
				case 1:position3();break;
				case 2:position1();break;	
				case 3:position2();break;
				case 4:position4();break;
			}
		break;
		case 13:
			
			switch(Bit_Data)
			{
				case 1:position3();break;
				case 2:position1();break;	
				case 3:position4();break;
				case 4:position2();break;
			}
		break;
		case 14:
			
			switch(Bit_Data)
			{
				case 1:position3();break;
				case 2:position2();break;	
				case 3:position1();break;
				case 4:position4();break;
			}
		break;
		case 15:
			
			switch(Bit_Data)
			{
				case 1:position3();break;
				case 2:position2();break;	
				case 3:position4();break;
				case 4:position1();break;
			}
		break;
		case 16:
			
			switch(Bit_Data)
			{
				case 1:position4();break;
				case 2:position1();break;	
				case 3:position2();break;
				case 4:position3();break;
			}
		break;
		case 17:
			
			switch(Bit_Data)
			{
				case 1:position4();break;
				case 2:position1();break;	
				case 3:position3();break;
				case 4:position2();break;
			}
		break;
		case 18:
			
			switch(Bit_Data)
			{
				case 1:position4();break;
				case 2:position2();break;	
				case 3:position1();break;
				case 4:position3();break;
			}
		break;
		case 19:
			
			switch(Bit_Data)
			{
				case 1:position4();break;
				case 2:position3();break;	
				case 3:position1();break;
				case 4:position2();break;
			}
		break;
		case 20:
			
			switch(Bit_Data)
			{
				case 1:position4();break;
				case 2:position3();break;	
				case 3:position2();break;
				case 4:position1();break;
			}
		break;
		case 21:
			
			switch(Bit_Data)
			{
				case 1:position3();break;
				case 2:position4();break;	
				case 3:position1();break;
				case 4:position2();break;
			}
		break;
		case 22:
			
			switch(Bit_Data)
			{
				case 1:position3();break;
				case 2:position4();break;	
				case 3:position2();break;
				case 4:position1();break;
			}
		break;
		case 23:
			
			switch(Bit_Data)
			{
				case 1:position4();break;
				case 2:position2();break;	
				case 3:position1();break;
				case 4:position3();break;
			}
		break;
		


	}
}

