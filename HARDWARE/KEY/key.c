/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-17 19:22:23
 */

/**
  ******************************************************************************
  * @file    key.c
  * @author  xie
  * @version V1
  * @date    2021��10��31��
  * @brief   ��������������
	
						�˿ڶ��壺
						PB2��KEY_A 	����1
						PB0��KEY_B 	����2
						PB1��KEY_C 	����3
						PE10��EC11_KEY 	EC11����
  ******************************************************************************
**/	
#include "key.h"

/**
  * @brief  ������ʼ��
  * @param  None
  * @retval	None 
  * @note		None
  */
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOBʱ��
    __HAL_RCC_GPIOE_CLK_ENABLE();           //����GPIOEʱ��
  	__HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;//PB0,1,2,3
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  		//����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_LOW;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);			//��ʼ���˿�
	
    GPIO_Initure.Pin=GPIO_PIN_10;						//PE10
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  		//����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_LOW;     //����
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);			//��ʼ���˿�

    GPIO_Initure.Pin=GPIO_PIN_0;						//PA0
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  		//����
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_LOW;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);			//��ʼ���˿�
}

//�����жϳ�ʼ��
void KEY_Init_IT(void)
{
    GPIO_InitTypeDef GPIO_Initure;

  	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;//PB0,1,2,3
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;  		//����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_LOW;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);			//��ʼ���˿�
	
	  //�ж���9-PB9
    HAL_NVIC_SetPriority(EXTI0_IRQn,0,0);  
    HAL_NVIC_SetPriority(EXTI1_IRQn,0,0); 
    HAL_NVIC_SetPriority(EXTI2_IRQn,0,0); 
    HAL_NVIC_EnableIRQ(EXTI0_IRQn); 
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}


//@parm mode 1=�������� 0.����ֻ��ʹ��һ��
unsigned char KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY_S1==0||KEY_S2==0||KEY_S3==0||KEY_UP==1||EC11_KEY==0))
	{
		delay_ms(20);//ȥ���� 
		key_up=0;
		if(KEY_S1==0)return 1;
		else if(KEY_S2==0)return 2;
		else if(KEY_S3==0)return 3;
		else if(KEY_UP==1)return 4;
    else if(EC11_KEY==0)return 5;
	}else if(KEY_S1==1&&KEY_S2==1&&KEY_S3==1&&KEY_UP==0&&EC11_KEY==1)key_up=1; 	    
 	return 0;// �ް�������
}
