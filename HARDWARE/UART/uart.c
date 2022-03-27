/**
  ******************************************************************************
  * @file    uart.c
  * @author  xie
  * @version V1
  * @date    2021��11��1��
  * @brief   �첽��������������(����������ʶ��)
	
						�˿ڶ��壺
						PA9�� UART1_TX 		�첽����1���Ͷ�
						PA10��UART1_RX 	  �첽����1���ն�
						PA2�� UART2_TX 		�첽����2���Ͷ�
						PA3�� UART2_RX 	  �첽����2���ն�
  ******************************************************************************
**/
#include "uart.h"
#include "delay.h"
#include "stdio.h"
#include "math.h"
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;
};

FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
//�ض���fputc����
int fputc(int ch, FILE *f)
{
	//	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������
	//	USART1->DR = (u8) ch;
	//	return ch;
	return 0;
}

u8 UART1_aRxBuffer[1];			  //HAL��ʹ�õĴ��ڽ��ջ���
UART_HandleTypeDef UART1_Handler; //UART���

//��ʼ��IO ����1 bound:������
void UART1_Init(u32 bound)
{
	//UART ��ʼ������
	UART1_Handler.Instance = USART1;					//USART1
	UART1_Handler.Init.BaudRate = bound;				//������
	UART1_Handler.Init.WordLength = UART_WORDLENGTH_8B; //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits = UART_STOPBITS_1;		//һ��ֹͣλ
	UART1_Handler.Init.Parity = UART_PARITY_NONE;		//����żУ��λ
	UART1_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; //��Ӳ������
	UART1_Handler.Init.Mode = UART_MODE_TX_RX;			//�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);						//HAL_UART_Init()��ʹ��UART1

	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)UART1_aRxBuffer, 1); //�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{

	GPIO_InitTypeDef GPIO_Initure; //GPIO�˿�����

	if (huart->Instance == USART1) //����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();  //ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE(); //ʹ��USART1ʱ��

		GPIO_Initure.Pin = GPIO_PIN_9;			   //PA9
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	   //�����������
		GPIO_Initure.Pull = GPIO_PULLUP;		   //����
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; //����
		GPIO_Initure.Alternate = GPIO_AF7_USART1;  //����ΪUART1
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	   //��ʼ��PA9

		GPIO_Initure.Pin = GPIO_PIN_10;		 //PA10
		GPIO_Initure.Mode = GPIO_MODE_AF_PP; //����ģʽ
		HAL_GPIO_Init(GPIOA, &GPIO_Initure); //��ʼ��PA10

		HAL_NVIC_EnableIRQ(USART1_IRQn);		 //ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn, 2, 2); //��ռ���ȼ�3�������ȼ�3
	}
	if (huart->Instance == USART2) //����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();  //ʹ��GPIOAʱ��
		__HAL_RCC_USART2_CLK_ENABLE(); //ʹ��USART2ʱ��

		GPIO_Initure.Pin = GPIO_PIN_2;			   //PA2
		GPIO_Initure.Mode = GPIO_MODE_AF_PP;	   //�����������
		GPIO_Initure.Pull = GPIO_PULLUP;		   //����
		GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH; //����
		GPIO_Initure.Alternate = GPIO_AF7_USART2;  //����ΪUART2
		HAL_GPIO_Init(GPIOA, &GPIO_Initure);	   //��ʼ��PA9

		GPIO_Initure.Pin = GPIO_PIN_3;		 //PA3
		GPIO_Initure.Mode = GPIO_MODE_AF_PP; //����ģʽ
		HAL_GPIO_Init(GPIOA, &GPIO_Initure); //��ʼ��PA10

		HAL_NVIC_EnableIRQ(USART2_IRQn);		 //ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART2_IRQn, 2, 0); //��ռ���ȼ�3�������ȼ�3
	}
}

//���ڷ��͵��ַ�
void UART1_Send(uint8_t Send_Data)
{
	HAL_UART_Transmit(&UART1_Handler, &Send_Data, 1, 1);
}

//���ڷ����ַ��ַ���
void UART1_Send_String(uint8_t *Send_Data, uint8_t Size)
{
	HAL_UART_Transmit(&UART1_Handler, Send_Data, Size, Size);
}

u8 UART2_aRxBuffer[1];			  //HAL��ʹ�õĴ��ڽ��ջ���
UART_HandleTypeDef UART2_Handler; //UART���

//��ʼ��IO ����1 bound:������
void UART2_Init(u32 bound)
{
	//UART ��ʼ������
	UART2_Handler.Instance = USART2;					//USART2
	UART2_Handler.Init.BaudRate = bound;				//������
	UART2_Handler.Init.WordLength = UART_WORDLENGTH_8B; //�ֳ�Ϊ8λ���ݸ�ʽ
	UART2_Handler.Init.StopBits = UART_STOPBITS_1;		//һ��ֹͣλ
	UART2_Handler.Init.Parity = UART_PARITY_NONE;		//����żУ��λ
	UART2_Handler.Init.HwFlowCtl = UART_HWCONTROL_NONE; //��Ӳ������
	UART2_Handler.Init.Mode = UART_MODE_TX_RX;			//�շ�ģʽ
	HAL_UART_Init(&UART2_Handler);						//HAL_UART_Init()��ʹ��UART2

	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)UART2_aRxBuffer, 1); //�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
}

void UART2_DeInit(void)
{
	HAL_UART_DeInit(&UART2_Handler);
}


//���ڷ��͵��ַ�
void UART2_Send(uint8_t Send_Data)
{
	HAL_UART_Transmit(&UART2_Handler, &Send_Data, 1, 1);
}

//���ڷ����ַ��ַ���
void UART2_Send_String(uint8_t *Send_Data, uint8_t Size)
{
	HAL_UART_Transmit(&UART2_Handler, Send_Data, Size, Size);
}

uint8_t UART2_Back[9];		 //���յ�����
uint8_t UART2_receiving = 0; //���մ���
uint8_t UART2_Flag;			 //UART2������ɱ�־λ
void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&UART2_Handler);			  //����HAL���жϴ����ú���
	UART2_Back[UART2_receiving] = UART2_aRxBuffer[0]; //UART1�յ������ݻ�ȡ
	if (UART2_Back[0] == 0x55)						  //�жϱ�׼֡ͷ
	{
		UART2_receiving += 1;
		if (UART2_receiving > 3)
		{
			UART2_Flag = 1;
			UART2_receiving = 0;
		}
	}
	else //֡����
	{
		UART2_receiving = 0;
	}
	HAL_UART_Receive_IT(&UART2_Handler, (u8 *)UART2_aRxBuffer, 1); //�ú����Ὺ�������ж�(����ע��һ�Σ�Ҫ��Ȼ�´��ղ�����)
}

/**********************************************************************
 * �� �� �� ��  С������
 * ��    �� ��  ��
 * �� �� ֵ ��  �������
 * ȫ�ֱ��� ��  ��
 * ��    ע ��  ��
*****************************************************************/
uint8_t XiaoChuang_ASR(void)
{
	uint16_t Counter_Fifo = 0;
	uint8_t Temp_flag = 0;
	UART2_Flag = 0;

	/*��������ʶ��*/
	UART2_Send(0xFA);
	UART2_Send(0xFA);
	UART2_Send(0xFA);
	UART2_Send(0xFA);
	UART2_Send(0xA1);

	while (1)
	{
		if (UART2_Flag == 1)
		{
			if (UART2_Back[1] == 0x02 && UART2_Back[3] == 0x00) //���ָ��
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

			/*���·�������ʶ��*/
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
 * @description: ����0-9
 * @param {int} number
 * @return {*}
 */
void XiaoChuang_PlayNUMbit(int number)
{
/*
A0::��:5503A000
A1::һ:5503A100
A2::��:5503A200
A3::��:5503A300
A4::��:5503A400
A5::��:5503A500
A6::��:5503A600
A7::��:5503A700
A8::��:5503A800
A9::��:5503A900
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
 * �� �� �� ��  С�������������� ֧�ָ���
 * ��    �� ��  ������ֵ -999 �� 999
 * �� �� ֵ ��  ��
 * ȫ�ֱ��� ��  ��
 * ��    ע �� 
*****************************************************************/
void XiaoChuang_PlayNUM(int number)
{	int t = 500; //�����ϳɼ��/us û�м�����������
	int buf = number;
	int len = 1;
	int bufARR[20];//�����ݴ��������
	if(number>9 || number<-9)
	{
		while (buf/=10)//��������λ��
		{
			if(len==1)//������λ
			{
				bufARR[len]=number%10;
			}else //�����м�λ
			{
				bufARR[len]=pow(10,(len-1));
				bufARR[len]=number/bufARR[len]%10;
			}
			
			//printf("bufARR[%d]=%d\n",len,bufARR[len]);
			len++;
		}
		if(len>1)
		{
			//����ĩλ
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
		if(bufARR[2]>1)//��ֹ����һʮ
		{
			XiaoChuang_PlayNUMbit(bufARR[2]);
			delay_us(t);
		}
		UART2_Send(0xAA);//ʮ
		delay_us(t);
		if(bufARR[1]!=0)//��ֹ����ʮ��
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
			UART2_Send(0xAA);//ʮ
			delay_us(t);
			if(bufARR[1]!=0)//��ֹ����ʮ��
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
		if(bufARR[3]!=0)//ǧ
		{
			XiaoChuang_PlayNUMbit(bufARR[3]);
			UART2_Send(0xAB);
		}//n0nn
		
		XiaoChuang_PlayNUMbit(bufARR[2]);
		UART2_Send(0xAA);//ʮ
		if(bufARR[1]!=0)//��ֹ����ʮ��
		{
			XiaoChuang_PlayNUMbit(bufARR[1]);
		}
		break;
	case 5:
		XiaoChuang_PlayNUMbit(bufARR[5]);//��
		UART2_Send(0xAD);
		XiaoChuang_PlayNUMbit(bufARR[4]);
		UART2_Send(0xAC);
		XiaoChuang_PlayNUMbit(bufARR[3]);
		UART2_Send(0xAB);
		XiaoChuang_PlayNUMbit(bufARR[2]);
		UART2_Send(0xAA);//ʮ
		if(bufARR[1]!=0)//��ֹ����ʮ��
		{
			XiaoChuang_PlayNUMbit(bufARR[1]);
		}
		break;
	case 6:
		if(bufARR[6]>1)
		{
			XiaoChuang_PlayNUMbit(bufARR[6]);
		}
		UART2_Send(0xAA);//ʮ
		XiaoChuang_PlayNUMbit(bufARR[5]);//��
		UART2_Send(0xAD);
		XiaoChuang_PlayNUMbit(bufARR[4]);
		UART2_Send(0xAC);
		XiaoChuang_PlayNUMbit(bufARR[3]);
		UART2_Send(0xAB);
		XiaoChuang_PlayNUMbit(bufARR[2]);
		UART2_Send(0xAA);//ʮ
		if(bufARR[1]!=0)//��ֹ����ʮ��
		{
			XiaoChuang_PlayNUMbit(bufARR[1]);
		}
		break;*/
	}

	
}

