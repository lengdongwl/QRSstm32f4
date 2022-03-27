/**
  ******************************************************************************
  * @file    ultrasonic.c
  * @author  xie
  * @version V1
  * @date    2021��10��31��
  * @brief   ��������ຯ����
	
						�˿ڶ��壺
						PB8��INC 		�����������
						PB9��INT0 	���������ն�
  ******************************************************************************
**/	
#include "ultrasonic.h"
#include "delay.h"
#include "oled.h"
#include "stdio.h"
#include "led.h"
float Ultrasonic_Value = 0;					 //�������
float dis=0;
uint32_t Ultrasonic_Num=0;					 //����ֵ
uint16_t Distance_Temp=0;						 //���봦��ǰ
uint16_t Distance=0;								 		 //����

TIM_HandleTypeDef TIM7_Handler;      //��ʱ����� 
GPIO_InitTypeDef GPIO_Initure;			 //GPIO��� 


/**
  * @brief  �������˿ڳ�ʼ��
  * @param  None
  * @retval	None 
  * @note		None
  */
void Ultrasonic_Port(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOBʱ��
	
		/*INC �����������*/	
    GPIO_Initure.Pin=GPIO_PIN_8;						//PB8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);			//��ʼ���˿�	
	
		/*INT0 ���������ն�*/
    GPIO_Initure.Pin=GPIO_PIN_9;						//PB9
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  		//����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);			//��ʼ���˿�	
	

		INC = 0;

}

/**
  * @brief  ������ ��ʱ������
  * @param  None
  * @retval	None 
  * @note		None
  */
void TIM7_Init(uint16_t arr,uint16_t psc)
{
    TIM7_Handler.Instance=TIM7;                          //ͨ�ö�ʱ��3
    TIM7_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM7_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM7_Handler);

	
}
  
//��ʱ��6�жϷ����� 
uint8_t LED_T=0;
void TIM7_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM7_Handler);
    Ultrasonic_Num++;
		if(Ultrasonic_Num==0x1fff){							//�������ȡ����ʾ
			Distance=0;	
		}
}

/**
  * @brief  ������ �ⲿ�ж�����
  * @param  None
  * @retval	None 
  * @note		None
  */
void Ultrasonic_EXTI(void)
{
		/*INT0-PB9*/
		GPIO_Initure.Pin=GPIO_PIN_9;   							//PB9
		GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;  		//�½��ش���
		GPIO_Initure.Pull=GPIO_PULLUP;          		//����
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //����
		HAL_GPIO_Init(GPIOB,&GPIO_Initure);	
	
	  //�ж���9-PB9
    HAL_NVIC_SetPriority(EXTI9_5_IRQn,1,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);             //ʹ���ж���0
}

//�жϷ�����
void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);		//�����жϴ����ú���
}

//�жϷ����������Ҫ��������
//��HAL�������е��ⲿ�жϷ�����������ô˺���
//GPIO_Pin:�ж����ź�
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_9)
	{
 		HAL_TIM_Base_Stop(&TIM7_Handler);	//ʧ�ܶ�ʱ��3
		Ultrasonic_Value = (float)Ultrasonic_Num;
		Ultrasonic_Value = (float)Ultrasonic_Value*1.72f - 20.0f;       // ������붨ʱ10us��S=Vt/2����2��������
		Distance_Temp  = (uint16_t) Ultrasonic_Value;
		dis=Ultrasonic_Value;		
	}
}

//��������ʼ��
void Ultrasonic_Init(void)
{
	Ultrasonic_Port();			// ������Ӳ���˿ڳ�ʼ��
	TIM7_Init(10-1,84-1);
	Ultrasonic_EXTI();			// ���������������жϳ�ʼ��
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
	
		Ultrasonic_Num  = 0;		// ��ʱ������
		__HAL_TIM_CLEAR_IT(&TIM7_Handler,TIM_IT_UPDATE);//��������жϱ�־λ
		HAL_TIM_Base_Start_IT(&TIM7_Handler); //ʹ�ܶ�ʱ��3

		delay_ms(30);				// �ȴ�һ��ʱ�䣬�ȴ����ͳ����������ź�
		INC = 1;
		delay_ms(5);
	
 		HAL_TIM_Base_Stop(&TIM7_Handler);	//ʧ�ܶ�ʱ��3
		return Distance_Temp;
}

//��β��ȡ��ֵ
uint16_t Ultrasonic_Distance(void)
{
	uint8_t i,j;
	uint16_t Temp=0;		 //��ʱ����
	uint16_t Arr_Data[8];//�������򻺴�
	/*ѭ����ȡ*/
	for(i=0;i<8;i++)
	{
		Ultrasonic_Ranging();     //���������
		delay_ms(10);
		Arr_Data[i]=Distance_Temp;
	}
	/*�����������*/
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
	Distance=(Arr_Data[3]+Arr_Data[4])/2;//ȡ��ֵ
	return Distance;
}

/**
 * @description: ���ͳ�������ಢ��ȡfloat���
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
		if (_count > 1000) //��ֹ��ѭ��1���޽��������ѭ��
		{
			break;
		}
		delay_ms(1);
	}
	r = dis;
	
	return r;
}