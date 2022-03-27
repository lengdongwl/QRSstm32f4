#include "RTC.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32F407������
//RTC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/5
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//�޸�˵��
//V1.1 20140726
//����:RTC_Get_Week����,���ڸ�����������Ϣ,�õ�������Ϣ.
////////////////////////////////////////////////////////////////////////////////// 

//�ȴ�RSFͬ��
//����ֵ:0,�ɹ�;1,ʧ��;
uint8_t RTC_Wait_Synchro(void)
{ 
	uint32_t retry=0XFFFFF; 
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	RTC->ISR&=~(1<<5);		//���RSFλ 
	while(retry&&((RTC->ISR&(1<<5))==0x00))//�ȴ�Ӱ�ӼĴ���ͬ��
	{
		retry--;
	}
    if(retry==0)return 1;	//ͬ��ʧ�� 
	RTC->WPR=0xFF;			//ʹ��RTC�Ĵ���д����  
	return 0;
}
//RTC�����ʼ��ģʽ
//����ֵ:0,�ɹ�;1,ʧ��;
uint8_t RTC_Init_Mode(void)
{ 
	uint32_t retry=0X10000; 
	if(RTC->ISR&(1<<6))return 0; 
	RTC->ISR|=1<<7;	//����RTC��ʼ��ģʽ
	while(retry&&((RTC->ISR&(1<<6))==0x00))//�ȴ�����RTC��ʼ��ģʽ�ɹ�
	{
		retry--;
	}
    if(retry==0)return 1;	//ͬ��ʧ��
	else return 0; 			//ͬ���ɹ� 
}
//RTCд�������SRAM
//BKPx:�����Ĵ������,��Χ:0~19
//data:Ҫд�������,32λ����
void RTC_Write_BKR(uint32_t BKRx,uint32_t data)
{
	uint32_t temp=0; 
	temp=RTC_BASE+0x50+BKRx*4;   
	(*(uint32_t*)temp)=data; 
}
//RTC��ȡ������SRAM
//BKPx:�����Ĵ������,��Χ:0~19
//����ֵ:��ȡ��������
uint32_t RTC_Read_BKR(uint32_t BKRx)
{
	uint32_t temp=0; 
	temp=RTC_BASE+0x50+BKRx*4;  
	return (*(uint32_t*)temp);		//���ض�ȡ����ֵ
}

//ʮ����ת��ΪBCD��
//val:Ҫת����ʮ������
//����ֵ:BCD��
uint8_t RTC_DEC2BCD(uint8_t val)
{
	uint8_t bcdhigh = 0; 
	while(val>=10)
	{
		bcdhigh++;
		val-=10;
	} 
	return ((uint8_t)(bcdhigh<<4)|val);
}
//BCD��ת��Ϊʮ��������
//val:Ҫת����BCD��
//����ֵ:ʮ��������
uint8_t RTC_BCD2DEC(uint8_t val)
{
	uint8_t temp=0;
	temp=(val>>4)*10;
	return (temp+(val&0X0F));
}
//RTCʱ������
//hour,min,sec:Сʱ,����,����
//ampm:AM/PM,0=AM/24H,1=PM.
//����ֵ:0,�ɹ�
//       1,�����ʼ��ģʽʧ�� 
uint8_t RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm)
{
	uint32_t temp=0;
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	if(RTC_Init_Mode())return 1;//����RTC��ʼ��ģʽʧ��
	temp=(((uint32_t)ampm&0X01)<<22)|((uint32_t)RTC_DEC2BCD(hour)<<16)|((uint32_t)RTC_DEC2BCD(min)<<8)|(RTC_DEC2BCD(sec));
	RTC->TR=temp;
	RTC->ISR&=~(1<<7);			//�˳�RTC��ʼ��ģʽ 
	return 0; 
}
//RTC��������
//year,month,date:��(0~99),��(1~12),��(0~31)
//week:����(1~7,0,�Ƿ�!)
//����ֵ:0,�ɹ�
//       1,�����ʼ��ģʽʧ�� 
uint8_t RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week)
{
	uint32_t temp=0;
 	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA;
	RTC->WPR=0x53; 
	if(RTC_Init_Mode())return 1;//����RTC��ʼ��ģʽʧ��
	temp=(((uint32_t)week&0X07)<<13)|((uint32_t)RTC_DEC2BCD(year)<<16)|((uint32_t)RTC_DEC2BCD(month)<<8)|(RTC_DEC2BCD(date)); 
	RTC->DR=temp;
	RTC->ISR&=~(1<<7);			//�˳�RTC��ʼ��ģʽ 
	return 0; 
}
//��ȡRTCʱ��
//*hour,*min,*sec:Сʱ,����,���� 
//*ampm:AM/PM,0=AM/24H,1=PM.
void RTC_Get_Time(uint8_t *hour,uint8_t *min,uint8_t *sec,uint8_t *ampm)
{
	uint32_t temp=0;
 	while(RTC_Wait_Synchro());	//�ȴ�ͬ��  	 
	temp=RTC->TR;
	*hour=RTC_BCD2DEC((temp>>16)&0X3F);
	*min=RTC_BCD2DEC((temp>>8)&0X7F);
	*sec=RTC_BCD2DEC(temp&0X7F);
	*ampm=temp>>22; 
}
//��ȡRTC����
//*year,*mon,*date:��,��,��
//*week:����
void RTC_Get_Date(uint8_t *year,uint8_t *month,uint8_t *date,uint8_t *week)
{
	uint32_t temp=0;
 	while(RTC_Wait_Synchro());	//�ȴ�ͬ��  	 
	temp=RTC->DR;
	*year=RTC_BCD2DEC((temp>>16)&0XFF);
	*month=RTC_BCD2DEC((temp>>8)&0X1F);
	*date=RTC_BCD2DEC(temp&0X3F);
	*week=(temp>>13)&0X07; 
}
//RTC��ʼ��
//����ֵ:0,��ʼ���ɹ�;
//       1,LSE����ʧ��;
//       2,�����ʼ��ģʽʧ��;
uint8_t RTC_Init(void)
{
	uint16_t retry=0X1FFF; 
	RCC->APB1ENR|=1<<28;			//ʹ�ܵ�Դ�ӿ�ʱ��
	PWR->CR|=1<<8;					//���������ʹ��(RTC+SRAM)
	if(RTC_Read_BKR(0)!=0X5050)		//�Ƿ��һ������?
	{
		RCC->BDCR|=1<<0;			//LSE ����  
		while(retry&&((RCC->BDCR&0X02)==0))//�ȴ�LSE׼����
		{
			retry--;
			delay_ms(5);
		}
		if(retry==0)return 1;		//LSE ����ʧ��. 
		RCC->BDCR|=1<<8;			//ѡ��LSE,��ΪRTC��ʱ��
		RCC->BDCR|=1<<15;			//ʹ��RTCʱ��
 		//�ر�RTC�Ĵ���д����
		RTC->WPR=0xCA;
		RTC->WPR=0x53; 
		if(RTC_Init_Mode())return 2;//����RTC��ʼ��ģʽ
		RTC->PRER=0XFF;				//RTCͬ����Ƶϵ��(0~7FFF),����������ͬ����Ƶ,�������첽��Ƶ,Frtc=Fclks/((Sprec+1)*(Asprec+1))
		RTC->PRER|=0X7F<<16;		//RTC�첽��Ƶϵ��(1~0X7F)
		RTC->CR&=~(1<<6);			//RTC����Ϊ,24Сʱ��ʽ
		RTC->ISR&=~(1<<7);			//�˳�RTC��ʼ��ģʽ
		RTC->WPR=0xFF;				//ʹ��RTC�Ĵ���д����  
		RTC_Set_Time(23,59,56,0);	//����ʱ��
		RTC_Set_Date(14,5,5,1);		//��������
		//RTC_Set_AlarmA(7,0,0,10);	//��������ʱ��
		RTC_Write_BKR(0,0X5050);	//����Ѿ���ʼ������
	} 
	//RTC_Set_WakeUp(4,0);			//����WAKE UP�ж�,1�����ж�һ�� 
	return 0;
}
//��������ʱ��(����������,24Сʱ��)
//week:���ڼ�(1~7)
//hour,min,sec:Сʱ,����,����
void RTC_Set_AlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec)
{ 
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA; 
	RTC->WPR=0x53;
	RTC->CR&=~(1<<8);	//�ر�����A
	while((RTC->ISR&0X01)==0);//�ȴ�����A�޸����� 
	RTC->ALRMAR=0;		//���ԭ������
	RTC->ALRMAR|=1<<30;	//���������� 
	RTC->ALRMAR|=0<<22;	//24Сʱ��
	RTC->ALRMAR|=(uint32_t)RTC_DEC2BCD(week)<<24;//��������
	RTC->ALRMAR|=(uint32_t)RTC_DEC2BCD(hour)<<16;//Сʱ����
	RTC->ALRMAR|=(uint32_t)RTC_DEC2BCD(min)<<8;	//��������
	RTC->ALRMAR|=(uint32_t)RTC_DEC2BCD(sec);		//��������  
	RTC->ALRMASSR=0;						//��ʹ��SUB SEC
	RTC->CR|=1<<12;		//��������A�ж�
	RTC->CR|=1<<8;		//��������A 
	RTC->WPR=0XFF;		//��ֹ�޸�RTC�Ĵ��� 
	
	RTC->ISR&=~(1<<8);	//���RTC����A�ı�־
	EXTI->PR=1<<17;  	//���LINE17�ϵ��жϱ�־λ  
	EXTI->IMR|=1<<17;	//����line17�ϵ��ж� 
	EXTI->RTSR|=1<<17;	//line17���¼��������ش��� 
	//MY_NVIC_Init(2,2,RTC_Alarm_IRQn,2); //��ռ2�������ȼ�2����2 
    HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 2, 2); //�����ж����ȼ�
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);		 //�����ж�
}
//�����Ի��Ѷ�ʱ������
//wksel:000,RTC/16;001,RTC/8;010,RTC/4;011,RTC/2;
//      10x,ck_spre,1Hz;11x,1Hz,��cntֵ����2^16(��cnt+2^16)
//ע��:RTC����RTC��ʱ��Ƶ��,��RTCCLK!
//cnt:�Զ���װ��ֵ.����0,�����ж�.
void RTC_Set_WakeUp(uint8_t wksel,uint16_t cnt)
{ 
	//�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA; 
	RTC->WPR=0x53;
	RTC->CR&=~(1<<10);			//�ر�WAKE UP
	while((RTC->ISR&0X04)==0);	//�ȴ�WAKE UP�޸�����
	RTC->CR&=~(7<<0);			//���ԭ��������
	RTC->CR|=wksel&0X07;		//�����µ�ֵ
	RTC->WUTR=cnt;				//����WAKE UP�Զ���װ�ؼĴ���ֵ
	RTC->ISR&=~(1<<10);			//���RTC WAKE UP�ı�־
	RTC->CR|=1<<14;				//����WAKE UP ��ʱ���ж�
	RTC->CR|=1<<10;				//����WAKE UP ��ʱ����
	RTC->WPR=0XFF;				//��ֹ�޸�RTC�Ĵ��� ��
	EXTI->PR=1<<22;  			//���LINE22�ϵ��жϱ�־λ  
	EXTI->IMR|=1<<22;			//����line22�ϵ��ж� 
	EXTI->RTSR|=1<<22;			//line22���¼��������ش��� 
	//MY_NVIC_Init(2,2,RTC_WKUP_IRQn,2); //��ռ2�������ȼ�2����2 
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 2, 2); //�����ж����ȼ�
	HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);		 //�����ж�
}

//RTC�����жϷ�����
void RTC_Alarm_IRQHandler(void)
{    
	if(RTC->ISR&(1<<8))//ALARM A�ж�?
	{
		RTC->ISR&=~(1<<8);	//����жϱ�־
		printf("ALARM A!\r\n");
	}   
	EXTI->PR|=1<<17;	//����ж���17���жϱ�־ 											 
}

//RTC WAKE UP�жϷ�����
void RTC_WKUP_IRQHandler(void)
{    
	if(RTC->ISR&(1<<10))//WK_UP�ж�?
	{ 
		RTC->ISR&=~(1<<10);	//����жϱ�־
		//LED1=!LED1; 
	}   
	EXTI->PR|=1<<22;	//����ж���22���жϱ�־ 								
}
uint8_t const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//year,month,day������������ 
//����ֵ�����ں�(1~7,������1~����)																						 
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day)
{	
	uint16_t temp2;
	uint8_t yearH,yearL;
	yearH=year/100;	yearL=year%100; 
	// ���Ϊ21����,�������100  
	if (yearH>19)yearL+=100;
	// ����������ֻ��1900��֮���  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	temp2%=7;
	if(temp2==0)temp2=7;
	return temp2;
}	












