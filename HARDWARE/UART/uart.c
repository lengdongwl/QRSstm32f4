/**
  ******************************************************************************
  * @file    uart.c
  * @author  xie
  * @version V1
  * @date    2021年11月1日
  * @brief   异步串口驱动函数库(语音播报及识别)
	
						端口定义：
						PA9： UART1_TX 		异步串口1发送端
						PA10：UART1_RX 	  异步串口1接收端
						PA2： UART2_TX 		异步串口2发送端
						PA3： UART2_RX 	  异步串口2接收端
  ******************************************************************************
**/
#include "uart.h"
#include "delay.h"
#include "stdio.h"
#include "math.h"
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
	int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
//重定义fputc函数
int fputc(int ch, FILE *f)
{
	//	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕
	//	USART1->DR = (u8) ch;
	//	return ch;
	return 0;
}

u8 UART1_aRxBuffer[1];			  //HAL库使用的串口接收缓冲
UART_HandleTypeDef UART1_Handler; //UART句柄

//初始化IO 串口1 bound:波特率
void UART1_Init(u32 bound)
{
	//UART 初始化设置
	UART1_Handler.Instance = USART1;					//USART1
	UART1_Handler.Init.BaudRate = bound;				//波特率
	UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B; //字长为8位数据格式
	UART1_Handler.Init.StopBits = UART_STOPBITS_1;		//一个停止位
	UART1_Handler.Init.Parity = UART_PARITY_NONE;		//无奇偶校验位
	UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; //无硬件流控
	UART1_Handler.Init.Mode = UART_MODE_TX_RX;			//收发模式
	HAL_UART_Init(&UART1_Handler);						//HAL_UART_Init()会使能UART1

	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)UART1_aRxBuffer, 1); //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{

	GPIO_InitTypeDef GPIO_Initure; //GPIO端口设置

	if (huart->Instance == USART1) //如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();  //使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE(); //使能USART1时钟

		GPIO_Initure.Pin = GPIO_PIN_9;			   //PA9
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	   //复用推挽输出
		GPIO_Initure.Pull = GPIO_PULLUP;		   //上拉
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; //高速
		GPIO_Initure.Alternate = GPIO_AF7_USART1;  //复用为UART1
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	   //初始化PA9

		GPIO_Initure.Pin = GPIO_PIN_10;		 //PA10
		GPIO_Initure.Mode = GPIO_MODE_AF_PP; //复用模式
		HAL_GPIO_Init(GPIOA, &GPIO_Initure); //初始化PA10

		HAL_NVIC_EnableIRQ(USART1_IRQn);		 //使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn, 2, 2); //抢占优先级3，子优先级3
	}
	if (huart->Instance == USART2) //如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();  //使能GPIOA时钟
		__HAL_RCC_USART2_CLK_ENABLE(); //使能USART2时钟

		GPIO_Initure.Pin = GPIO_PIN_2;			   //PA2
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	   //复用推挽输出
		GPIO_Initure.Pull = GPIO_PULLUP;		   //上拉
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; //高速
		GPIO_Initure.Alternate = GPIO_AF7_USART2;  //复用为UART2
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	   //初始化PA9

		GPIO_Initure.Pin = GPIO_PIN_3;		 //PA3
		GPIO_Initure.Mode = GPIO_MODE_AF_PP; //复用模式
		HAL_GPIO_Init(GPIOA, &GPIO_Initure); //初始化PA10

		HAL_NVIC_EnableIRQ(USART2_IRQn);		 //使能USART1中断通道
		HAL_NVIC_SetPriority(USART2_IRQn, 2, 0); //抢占优先级3，子优先级3
	}
}

//串口发送单字符
void UART1_Send(uint8_t Send_Data)
{
	HAL_UART_Transmit(&UART1_Handler, &Send_Data, 1, 1);
}

//串口发送字符字符串
void UART1_Send_String(uint8_t *Send_Data, uint8_t Size)
{
	HAL_UART_Transmit(&UART1_Handler, Send_Data, Size, Size);
}

u8 UART2_aRxBuffer[1];			  //HAL库使用的串口接收缓冲
UART_HandleTypeDef UART2_Handler; //UART句柄

//初始化IO 串口1 bound:波特率
void UART2_Init(u32 bound)
{
	//UART 初始化设置
	UART2_Handler.Instance = USART2;					//USART2
	UART2_Handler.Init.BaudRate = bound;				//波特率
	UART2_Handler.Init.WordLength = UART_WORDLENGTH_8B; //字长为8位数据格式
	UART2_Handler.Init.StopBits = UART_STOPBITS_1;		//一个停止位
	UART2_Handler.Init.Parity = UART_PARITY_NONE;		//无奇偶校验位
	UART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; //无硬件流控
	UART2_Handler.Init.Mode = UART_MODE_TX_RX;			//收发模式
	HAL_UART_Init(&UART2_Handler);						//HAL_UART_Init()会使能UART2

	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)UART2_aRxBuffer, 1); //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}

void UART2_DeInit(void)
{
	HAL_UART_DeInit(&UART2_Handler);
}


//串口发送单字符
void UART2_Send(uint8_t Send_Data)
{
	HAL_UART_Transmit(&UART2_Handler, &Send_Data, 1, 1);
}

//串口发送字符字符串
void UART2_Send_String(uint8_t *Send_Data, uint8_t Size)
{
	HAL_UART_Transmit(&UART2_Handler, Send_Data, Size, Size);
}

uint8_t UART2_Back[9];		 //接收的数据
uint8_t UART2_receiving = 0; //接收次数
uint8_t UART2_Flag;			 //UART2接收完成标志位
void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UART2_Handler);			  //调用HAL库中断处理公用函数
	UART2_Back[UART2_receiving] = UART2_aRxBuffer[0]; //UART1收到的数据获取
	if (UART2_Back[0] == 0x55)						  //判断标准帧头
	{
		UART2_receiving += 1;
		if (UART2_receiving > 3)
		{
			UART2_Flag = 1;
			UART2_receiving = 0;
		}
	}
	else //帧错误
	{
		UART2_receiving = 0;
	}
	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)UART2_aRxBuffer, 1); //该函数会开启接收中断(重新注册一次，要不然下次收不到了)
}

/**********************************************************************
 * 函 数 名 ：  小创语音
 * 参    数 ：  无
 * 返 回 值 ：  语音编号
 * 全局变量 ：  无
 * 备    注 ：  无
*****************************************************************/
uint8_t XiaoChuang_ASR(void)
{
	uint16_t Counter_Fifo = 0;
	uint8_t Temp_flag = 0;
	UART2_Flag = 0;

	/*发送语音识别*/
	UART2_Send(0xFA);
	UART2_Send(0xFA);
	UART2_Send(0xFA);
	UART2_Send(0xFA);
	UART2_Send(0xA1);

	while (1)
	{
		if (UART2_Flag == 1)
		{
			if (UART2_Back[1] == 0x02 && UART2_Back[3] == 0x00) //检查指令
			{
				if (UART2_Back[2] != 0)
				{
					return UART2_Back[2];
				}
				else
				{
					return 0x00;
				}
			}
		}
		delay_ms(100);
		Counter_Fifo += 1;
		if (Counter_Fifo > 30)
		{
			Counter_Fifo = 0;
			if (Temp_flag >= 4)
			{
				break;
			}
			Temp_flag += 1;

			/*重新发送语音识别*/
			UART2_Send(0xFA);
			UART2_Send(0xFA);
			UART2_Send(0xFA);
			UART2_Send(0xFA);
			UART2_Send(0xA1);
		}
	}
	return 0x00;
}


/**
 * @description: 播报0-9
 * @param {int} number
 * @return {*}
 */
void XiaoChuang_PlayNUMbit(int number)
{
/*
A0::零:5503A000
A1::一:5503A100
A2::二:5503A200
A3::三:5503A300
A4::四:5503A400
A5::五:5503A500
A6::六:5503A600
A7::七:5503A700
A8::八:5503A800
A9::九:5503A900
*/
	UART2_Send(0xA0+(number));
	/*if(number<0)
	{
		UART2_Send(0xA0+(number*-1));
	}else
	{
		
	}*/
}
/**********************************************************************
 * 函 数 名 ：  小创语音播报数字 支持负数
 * 参    数 ：  语音数值 -999 到 999
 * 返 回 值 ：  无
 * 全局变量 ：  无
 * 备    注 ： 
*****************************************************************/
void XiaoChuang_PlayNUM(int number)
{	int t = 500; //语音合成间隔/us 没有间隔会造成阻塞
	int buf = number;
	int len = 1;
	int bufARR[20];//将数据存放至数组
	if(number>9 || number<-9)
	{
		while (buf/=10)//计算数字位数
		{
			if(len==1)//读出各位
			{
				bufARR[len]=number%10;
			}else //读出中间位
			{
				bufARR[len]=pow(10,(len-1));
				bufARR[len]=number/bufARR[len]%10;
			}
			
			//printf("bufARR[%d]=%d\n",len,bufARR[len]);
			len++;
		}
		if(len>1)
		{
			//读出末位
			bufARR[len]=pow(10,(len-1));
			bufARR[len]=number/bufARR[len];
			//printf("bufARR[%d]=%d\n",len,bufARR[len]);
		}
	}else
	{
		bufARR[len]=number;
	}
	//printf("len=%d\n",len);
	
	//1 2345 6 7 8 9
	switch(len)
	{
	case 1: 
		XiaoChuang_PlayNUMbit(bufARR[1]);
		break;
	case 2:
		if(bufARR[2]>1)//防止播报一十
		{
			XiaoChuang_PlayNUMbit(bufARR[2]);
			delay_us(t);
		}
		UART2_Send(0xAA);//十
		delay_us(t);
		if(bufARR[1]!=0)//防止播报十零
		{
			XiaoChuang_PlayNUMbit(bufARR[1]);
			delay_us(t);
		}
		break;
	case 3://100   320  409    111
		XiaoChuang_PlayNUMbit(bufARR[3]);
		delay_us(t);
		UART2_Send(0xAB);
		delay_us(t);
		if(bufARR[2]!=0)//110 111
		{
			XiaoChuang_PlayNUMbit(bufARR[2]);
			delay_us(t);
			UART2_Send(0xAA);//十
			delay_us(t);
			if(bufARR[1]!=0)//防止播报十零
			{
				XiaoChuang_PlayNUMbit(bufARR[1]);
				delay_us(t);
			}
		}else//101 
		{
			if(bufARR[2]==0 && bufARR[1]!=0)//x0x
			{
				XiaoChuang_PlayNUMbit(bufARR[2]);
				delay_us(t);
				XiaoChuang_PlayNUMbit(bufARR[1]);
				delay_us(t);
			}
		}
		break;
	/*case 4:
		XiaoChuang_PlayNUMbit(bufARR[4]);
		UART2_Send(0xAC);
		if(bufARR[3]!=0)//千
		{
			XiaoChuang_PlayNUMbit(bufARR[3]);
			UART2_Send(0xAB);
		}//n0nn
		
		XiaoChuang_PlayNUMbit(bufARR[2]);
		UART2_Send(0xAA);//十
		if(bufARR[1]!=0)//防止播报十零
		{
			XiaoChuang_PlayNUMbit(bufARR[1]);
		}
		break;
	case 5:
		XiaoChuang_PlayNUMbit(bufARR[5]);//万
		UART2_Send(0xAD);
		XiaoChuang_PlayNUMbit(bufARR[4]);
		UART2_Send(0xAC);
		XiaoChuang_PlayNUMbit(bufARR[3]);
		UART2_Send(0xAB);
		XiaoChuang_PlayNUMbit(bufARR[2]);
		UART2_Send(0xAA);//十
		if(bufARR[1]!=0)//防止播报十零
		{
			XiaoChuang_PlayNUMbit(bufARR[1]);
		}
		break;
	case 6:
		if(bufARR[6]>1)
		{
			XiaoChuang_PlayNUMbit(bufARR[6]);
		}
		UART2_Send(0xAA);//十
		XiaoChuang_PlayNUMbit(bufARR[5]);//万
		UART2_Send(0xAD);
		XiaoChuang_PlayNUMbit(bufARR[4]);
		UART2_Send(0xAC);
		XiaoChuang_PlayNUMbit(bufARR[3]);
		UART2_Send(0xAB);
		XiaoChuang_PlayNUMbit(bufARR[2]);
		UART2_Send(0xAA);//十
		if(bufARR[1]!=0)//防止播报十零
		{
			XiaoChuang_PlayNUMbit(bufARR[1]);
		}
		break;*/
	}

	
}

