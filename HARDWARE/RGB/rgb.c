/**
  ******************************************************************************
  * @file    rgb.c
  * @author  xie
  * @version V2
  * @date    2021��10��28��
  * @brief   ȫ��RGB��ɫ������ 

						�˿ڶ��壺
							PD3:  RGB_Red 	RGB	��
							PD4:  RGB_Green RGB ��
							PD5: 	RGB_Blue 	RGB ��
							TIM3��RGB��ɫ����
  ******************************************************************************
**/
#include "rgb.h"
#include "led.h"
 uint8_t RGB_COLOR_RED[]={1,0,0};//��ɫ
 uint8_t  RGB_COLOR_GREEN []={0,1,0};//��ɫ
 uint8_t  RGB_COLOR_BLUE []={0,0,1};//��ɫ
 uint8_t  RGB_COLOR_YELLOW []={1,1,0};//��ɫ
 uint8_t  RGB_COLOR_SORT []={1,0,1};//Ʒɫ
 uint8_t  RGB_COLOR_SYAN []={0,1,1};//��ɫ
 uint8_t  RGB_COLOR_WHITE []={1,1,1};//��ɫ
 uint8_t  RGB_COLOR_BLACK []={0,0,0};//��ɫ ��ɫ
uint8_t RGB_Colour[3]; //RGB��ֵ

TIM_HandleTypeDef TIM3_Handler; //��ʱ�����

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
void TIM3_Init(u16 arr, u16 psc)
{
	TIM3_Handler.Instance = TIM3;							  //ͨ�ö�ʱ��3
	TIM3_Handler.Init.Prescaler = psc;						  //��Ƶϵ��
	TIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;		  //���ϼ�����
	TIM3_Handler.Init.Period = arr;							  //�Զ�װ��ֵ
	TIM3_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //ʱ�ӷ�Ƶ����
	HAL_TIM_Base_Init(&TIM3_Handler);
}

//��ʱ��3�жϷ�����


uint8_t TIM3_Count = 0;
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM3_Handler);

	if (TIM3_Count < RGB_Colour[0])
	{ //��ɫ
		RGB_Red = 1;
	}
	else
	{
		RGB_Red = 0;
	}
	if (TIM3_Count < RGB_Colour[1])
	{ //��ɫ
		RGB_Green = 1;
	}
	else
	{
		RGB_Green = 0;
	}
	if (TIM3_Count < RGB_Colour[2])
	{ //��ɫ
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
  * @brief  RGB��ʼ��
  * @param  None
  * @retval	None 
  * @note		None
  */
void RGB_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOD_CLK_ENABLE(); //����GPIODʱ��

	GPIO_Initure.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5; //PE0
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;				 //�������
	GPIO_Initure.Pull = GPIO_PULLUP;						 //����
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;					 //����
	HAL_GPIO_Init(GPIOD, &GPIO_Initure);					 //��ʼ���˿�

	RGB_Red = 0;
	RGB_Green = 0;
	RGB_Blue = 0;

	TIM3_Init(50 - 1, 42 - 1); //��ʱ��3��ʼ��
}

//ɾ����ʼ��
void RGB_DeInit(void)
{
	HAL_GPIO_DeInit(GPIOD, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
	HAL_TIM_Base_DeInit(&TIM3_Handler);
}

/**
  * @brief  RGB��ɫ����
  * @param  R_Data����ɫ���ֵ 
					  G_Data����ɫ���ֵ
					  B_Data����ɫ���ֵ
  * @retval	None 
  * @note		0-255
  */
void RGB(uint8_t R_Data, uint8_t G_Data, uint8_t B_Data)
{
	if (R_Data != 0 || G_Data != 0 || B_Data != 0)
	{
		RGB_Colour[0] = R_Data;				  //��
		RGB_Colour[1] = G_Data;				  //��
		RGB_Colour[2] = B_Data;				  //��
		HAL_TIM_Base_Start_IT(&TIM3_Handler); //ʹ�ܶ�ʱ��13�Ͷ�ʱ��13�����жϣ�TIM_IT_UPDATE
	}
	else
	{
		HAL_TIM_Base_Stop_IT(&TIM3_Handler); //�رն�ʱ��13�Ͷ�ʱ��13�����жϣ�TIM_IT_UPDATE
		RGB_Red = 0;
		RGB_Green = 0;
		RGB_Blue = 0;
	}
}


/**
 * @description: ����RGB����ɫ
 * @param {uint8_t} *RGB_COLOR_x
 * @return {*}
 */
void RGB_Set(uint8_t *RGB_COLOR_x)
{
	RGB_Red = RGB_COLOR_x[0];
	RGB_Green = RGB_COLOR_x[1];
	RGB_Blue = RGB_COLOR_x[2];
}