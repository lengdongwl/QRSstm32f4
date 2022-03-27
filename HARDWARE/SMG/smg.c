/**
  ******************************************************************************
  * @file    smg.c
  * @author  xie
  * @version V2
  * @date    2021��10��28��
  * @brief   ���������������
	
						�˿ڶ��壺
								PD15:SER		75HC595���������
								PC8:SCK			75HC595����ʱ��
								PC7:RCK			75HC595����ʱ��
								PD10:OE			75HC595���ʹ�ܿ��ƶ�
								PD11:SMG_1	�����λѡ1
								PD12:SMG_2	�����λѡ2
								PD13:SMG_3	�����λѡ3
								PD14:SMG_4	�����λѡ4				
										
  ******************************************************************************
**/	
#include "smg.h"
uint8_t SMG_MODE=0; //��ʾģʽ
uint16_t SMG_Data;//����ܶ�̬ɨ������
int16_t SMG_DataARR[4];//����ܶ�̬ɨ�����λ���ϵ�����

uint8_t SMGSWAP = 0;
/*8λ����ܹ�������*/
//uint8_t SEG_Table[17]={0xC0,0xF9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff}; 
//����                   0   1    2     3    4    5    6    7    8    9    A    B    C    D    E    F   �ر�   
/*8λ����ܹ�������*/
uint8_t SEG_Table[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00}; 
//����                   0   1    2     3    4    5    6    7    8    9    A    B    C    D    E    F   �ر� 

TIM_HandleTypeDef TIM4_Handler;      //��ʱ����� 
void SMG_Position(uint8_t Bit_Data);
//ͨ�ö�ʱ��4�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
void TIM4_Init(u16 arr,u16 psc)
{  
    TIM4_Handler.Instance=TIM4;                          //ͨ�ö�ʱ��4
    TIM4_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM4_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM4_Handler);	  
}

//��ʱ��3�жϷ�����

uint8_t TIM4_Count=0; 
void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM4_Handler);
	if(SMG_MODE==0)//��ֵ��ʾ
	{
		TIM4_Count+=1;
		switch(TIM4_Count)
		{
			case 1:{
				SMG_Display_Bit(SMG_Data/1000,4);			//����ܵ���λ��ʾ
				break;
			}
			case 2:{
				SMG_Display_Bit(SMG_Data%1000/100,3);	//����ܵ���λ��ʾ
				break;
			}
			case 3:{
				SMG_Display_Bit(SMG_Data%100/10,2);		//����ܵ���λ��ʾ
				break;
			}
			case 4:{
				SMG_Display_Bit(SMG_Data%10,1);				//����ܵ���λ��ʾ
				break;
			}		
		}
	}else //��ʾ����λ�õ���
	{
		TIM4_Count+=1;
		switch(TIM4_Count)
		{
			case 1:{
				if(SMG_DataARR[3]!=-1)
				{
					SMG_Display_Bit(SMG_DataARR[3],4);			//����ܵ���λ��ʾ
				}
				break;
			}
			case 2:{
				if(SMG_DataARR[2]!=-1)
				{
					SMG_Display_Bit(SMG_DataARR[2],3);			//����ܵ���λ��ʾ
				}
				break;
			}
			case 3:{
				if(SMG_DataARR[1]!=-1)
				{
					SMG_Display_Bit(SMG_DataARR[1],2);			//����ܵ���λ��ʾ
				}
				break;
			}
			case 4:{
				if(SMG_DataARR[0]!=-1)
				{
					SMG_Display_Bit(SMG_DataARR[0],1);			//����ܵ���λ��ʾ
				}
				break;
			}		
		}


	}
	if(TIM4_Count>=4){//�����ռ���
		TIM4_Count=0;
	}

}
void SMG_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();           //����GPIOCʱ��
    __HAL_RCC_GPIOD_CLK_ENABLE();           //����GPIODʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|
			GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;	//PD11,12,13,14,15
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);			//��ʼ���˿�

    GPIO_Initure.Pin=GPIO_PIN_7|GPIO_PIN_8;	//PC7,8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);			//��ʼ���˿�	
	
		OE=1;//���ʹ��
	
		TIM4_Init(200-1,84-1);      //��ʱ��4��ʼ��

}
void SMG_DeInit(void)
{
	HAL_GPIO_DeInit(GPIOD,GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|
			GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);	
	HAL_GPIO_DeInit(GPIOC,GPIO_PIN_7|GPIO_PIN_8);
	HAL_TIM_Base_DeInit(&TIM4_Handler);
}


/**
  * @brief  ����ܵ���λ��ʾ
  * @param  Input_Data������λ���� Bit_Data��λ
  * @retval	None 
  * @note		None
  */
void SMG_Display_Bit(uint8_t Input_Data,uint8_t Bit_Data)
{
		/*��ѡ*/
		uint8_t Data_Bit;
		int i=0;		
		for(i=0; i<8; i++)								     //��i=1ʱ�������ʾС����     
		{
				Data_Bit  = SEG_Table[Input_Data];//���� |0x80
				if (((Data_Bit)<<i)&0x80)					//�ж����������			�� 1 ���� 0
				{
						SER=1;							
				}
				else
				{
						SER=0;
				}
				SCK = 0;									//��λ(��ȷ�����������֮�������λ�����������ݴ���Ĵ�������)
				SCK = 1;
		}					
		RCK = 0;											//�Ĵ�����������ݴ��������
		RCK = 1;
		

		/*λѡ*/
		SMG_Position(Bit_Data);
}


/**
  * @brief  �������ʾ
  * @param  Input_Data������ 
  * @param  Ctrl_Bit:���ؿ���
  * @retval	None 
  * @note		None
  */
void SMG_Display(uint16_t Input_Data,uint8_t Ctrl_Bit)
{
		if(Ctrl_Bit){
				SMG_MODE=0; 
				SMG_Data=Input_Data;
				HAL_TIM_Base_Start_IT(&TIM4_Handler); //ʹ�ܶ�ʱ��4�Ͷ�ʱ��4�����жϣ�TIM_IT_UPDATE  					
		}
		else
		{
			HAL_TIM_Base_Stop_IT(&TIM4_Handler); //�رն�ʱ��13�Ͷ�ʱ��13�����жϣ�TIM_IT_UPDATE
			SMG_1=0;
			SMG_2=0;
			SMG_3=0;
			SMG_4=0; 			
		}
}


/**
 * @description: �������ʾ����λ��
 * @param {int16_t} Input_Data4 -1�ر�
 * @param {int16_t} Input_Data3
 * @param {int16_t} Input_Data2
 * @param {int16_t} Input_Data1
 * @param {uint8_t} Ctrl_Bit ���ض�ʱ������
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
				HAL_TIM_Base_Start_IT(&TIM4_Handler); //ʹ�ܶ�ʱ��4�Ͷ�ʱ��4�����жϣ�TIM_IT_UPDATE  					
		}
		else
		{
			HAL_TIM_Base_Stop_IT(&TIM4_Handler); //�رն�ʱ��13�Ͷ�ʱ��13�����жϣ�TIM_IT_UPDATE
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

	switch(SMGSWAP) //��ͨ�����ڴ�ֵ���������λ��ѡ��
	{
		case 0:
			//����
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

