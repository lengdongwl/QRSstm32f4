
/**
  ******************************************************************************
  * @file    time.c
  * @author  xie
  * @version V1
  * @date    2021��10��31��
  * @brief   ��ʱ���ײ�������
	
						�˿ڶ��壺
							��
  ******************************************************************************
**/
#include "time.h"
#include "led.h"
TIM_HandleTypeDef TIM5_Handler;
TIM_HandleTypeDef TIM2_Handler;	 //��ʱ�����
TIM_HandleTypeDef TIM12_Handler; //��ʱ�����
//ͨ�ö�ʱ��2��ʼ�� 84MHz
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM2_Init(u16 arr, u16 psc)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};

	TIM_MasterConfigTypeDef sMasterConfig = {0};

	TIM2_Handler.Instance = TIM2;							  //ͨ�ö�ʱ��2
	TIM2_Handler.Init.Prescaler = psc;						  //��Ƶ
	TIM2_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;		  //���ϼ�����
	TIM2_Handler.Init.Period = arr;							  //�Զ�װ��ֵ
	TIM2_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //ʱ�ӷ�Ƶ����
	HAL_TIM_Base_Init(&TIM2_Handler);
	//��ʱ��ʱ��Դ����
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&TIM2_Handler, &sClockSourceConfig) != HAL_OK)
	{
		//			Error_Handler();
	}
	///�������������DMAģʽ�¶���Ҫ�������ã����������ʱ��Դ
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;		 //��ʱ�����´���
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE; //��ʱ�����ӻ�ģʽ�ر�

	if (HAL_TIMEx_MasterConfigSynchronization(&TIM2_Handler, &sMasterConfig) != HAL_OK)
	{
		//			Error_Handler();
	}

	HAL_TIM_Base_Start(&TIM2_Handler); //Time Base Start
}

//ͨ�ö�ʱ��12�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM12_Init(u16 arr, u16 psc)
{
	TIM12_Handler.Instance = TIM12;							   //ͨ�ö�ʱ��3
	TIM12_Handler.Init.Prescaler = psc;						   //��Ƶϵ��
	TIM12_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;	   //���ϼ�����
	TIM12_Handler.Init.Period = arr;						   //�Զ�װ��ֵ
	TIM12_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //ʱ�ӷ�Ƶ����
	HAL_TIM_Base_Init(&TIM12_Handler);

	//    HAL_TIM_Base_Start_IT(&TIM12_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE
}
//��ʱ��12�жϷ�����
uint8_t TIME12_Flag = 0;
void TIM8_BRK_TIM12_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM12_Handler);
	TIME12_Flag = 1;
}

void TIM5_Init(u32 arr, u32 psc)
{
	__HAL_RCC_TIM5_CLK_ENABLE();
	TIM5_Handler.Instance = TIM5;							  //ͨ�ö�ʱ��4
	TIM5_Handler.Init.Prescaler = psc;						  //��Ƶϵ��
	TIM5_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;		  //���ϼ�����
	TIM5_Handler.Init.Period = arr;							  //�Զ�װ��ֵ
	TIM5_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //ʱ�ӷ�Ƶ����
	HAL_TIM_Base_Init(&TIM5_Handler);						  //��ʱ����ʼ��
	HAL_TIM_Base_Start_IT(&TIM5_Handler);
}

void TIM5_DeInit(void)
{

	// HAL_TIM_Base_Stop_IT(&TIM5_Handler);
	HAL_TIM_Base_DeInit(&TIM5_Handler);
}


//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE(); //ʹ��TIM2ʱ��
	}
	if (htim->Instance == TIM3)
	{

		__HAL_RCC_TIM3_CLK_ENABLE(); //ʹ��TIM3ʱ��

		HAL_NVIC_SetPriority(TIM3_IRQn, 1, 3); //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);		   //����ITM3�ж�
	}
	if (htim->Instance == TIM4)
	{
		__HAL_RCC_TIM4_CLK_ENABLE();		   //ʹ��TIM4ʱ��
		HAL_NVIC_SetPriority(TIM4_IRQn, 1, 2); //�����ж����ȼ�����ռ���ȼ�1�������ȼ�2
		HAL_NVIC_EnableIRQ(TIM4_IRQn);		   //����ITM4�ж�
	}
	if (htim->Instance == TIM7)
	{
		__HAL_RCC_TIM7_CLK_ENABLE();		   //ʹ��TIM7ʱ��
		HAL_NVIC_SetPriority(TIM7_IRQn, 2, 1); //�����ж����ȼ�����ռ���ȼ�1�������ȼ�0
		HAL_NVIC_EnableIRQ(TIM7_IRQn);		   //����TIM7�ж�
	}
	if (htim->Instance == TIM5)
	{
		__HAL_RCC_TIM5_CLK_ENABLE();
		HAL_NVIC_SetPriority(TIM5_IRQn, 1, 1); //�����ж����ȼ�����ռ���ȼ�1�������ȼ�0
		HAL_NVIC_EnableIRQ(TIM5_IRQn);
	}
	if (htim->Instance == TIM12)
	{
		__HAL_RCC_TIM12_CLK_ENABLE();					 //ʹ��TIM12ʱ��
		HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 3, 3); //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
		HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);		 //����ITM3�ж�
	}
}
