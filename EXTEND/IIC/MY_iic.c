/**
  ******************************************************************************
  * @file    MY_IIC.c
  * @author  xie
  * @version V2
  * @date    2021��11��2��
  * @brief   ��չ�˿�ģ�鹫��IIC����������
	
						�˿ڶ��壺
							PC12:	MY_IIC_SCL		MPU6050IIC���ݽ�	
							PD2:	MY_IIC_SDA		MPU6050IICʱ�ӽ�	

	******************************************************************************
**/	

#include "MY_IIC.h"
#include "delay.h"

//IIC��ʼ��
void MY_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	 
	__HAL_RCC_GPIOC_CLK_ENABLE();           		//����GPIOCʱ��
	__HAL_RCC_GPIOD_CLK_ENABLE();           		//����GPIODʱ��
	
	GPIO_Initure.Pin=GPIO_PIN_12;								//PC12
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		//�������
	GPIO_Initure.Pull=GPIO_PULLUP;          		//����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;   			//����
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);	

	GPIO_Initure.Pin=GPIO_PIN_2;								//PD2
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		//�������
	GPIO_Initure.Pull=GPIO_PULLUP;          		//����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;    			//����
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);		
	
	MY_IIC_SDA=1;
	MY_IIC_SCL=1;  
}

//����IIC��ʼ�ź�
void MY_IIC_Start(void)
{
	MY_SDA_OUT();     //sda�����
	MY_IIC_SDA=1;	  	  
	MY_IIC_SCL=1;
	delay_us(4);
 	MY_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	MY_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void MY_IIC_Stop(void)
{
	MY_SDA_OUT();//sda�����
	MY_IIC_SCL=0;
	MY_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	MY_IIC_SCL=1; 
	MY_IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MY_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MY_SDA_IN();      //SDA����Ϊ����  
	MY_IIC_SDA=1;delay_us(1);	   
	MY_IIC_SCL=1;delay_us(1);	 
	while(MY_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MY_IIC_Stop();
			return 1;
		}
	}
	MY_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void MY_IIC_Ack(void)
{
	MY_IIC_SCL=0;
	MY_SDA_OUT();
	MY_IIC_SDA=0;
	delay_us(2);
	MY_IIC_SCL=1;
	delay_us(2);
	MY_IIC_SCL=0;
}
//������ACKӦ��		    
void MY_IIC_NAck(void)
{
	MY_IIC_SCL=0;
	MY_SDA_OUT();
	MY_IIC_SDA=1;
	delay_us(2);
	MY_IIC_SCL=1;
	delay_us(2);
	MY_IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void MY_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	MY_SDA_OUT(); 	    
    MY_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        MY_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		MY_IIC_SCL=1;
		delay_us(2); 
		MY_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 MY_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MY_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        MY_IIC_SCL=0; 
        delay_us(2);
		MY_IIC_SCL=1;
        receive<<=1;
        if(MY_READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        MY_IIC_NAck();//����nACK
    else
        MY_IIC_Ack(); //����ACK   
       return receive;
}


