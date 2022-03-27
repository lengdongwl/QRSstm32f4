/**
  ******************************************************************************
  * @file    MY_IIC.c
  * @author  xie
  * @version V2
  * @date    2021年11月2日
  * @brief   扩展端口模块公用IIC驱动函数库
	
						端口定义：
							PC12:	MY_IIC_SCL		MPU6050IIC数据脚	
							PD2:	MY_IIC_SDA		MPU6050IIC时钟脚	

	******************************************************************************
**/	

#include "MY_IIC.h"
#include "delay.h"

//IIC初始化
void MY_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	 
	__HAL_RCC_GPIOC_CLK_ENABLE();           		//开启GPIOC时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();           		//开启GPIOD时钟
	
	GPIO_Initure.Pin=GPIO_PIN_12;								//PC12
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		//推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          		//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;   			//高速
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);	

	GPIO_Initure.Pin=GPIO_PIN_2;								//PD2
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  		//推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;          		//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;    			//高速
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);		
	
	MY_IIC_SDA=1;
	MY_IIC_SCL=1;  
}

//产生IIC起始信号
void MY_IIC_Start(void)
{
	MY_SDA_OUT();     //sda线输出
	MY_IIC_SDA=1;	  	  
	MY_IIC_SCL=1;
	delay_us(4);
 	MY_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	MY_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void MY_IIC_Stop(void)
{
	MY_SDA_OUT();//sda线输出
	MY_IIC_SCL=0;
	MY_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	MY_IIC_SCL=1; 
	MY_IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 MY_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MY_SDA_IN();      //SDA设置为输入  
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
	MY_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void MY_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	MY_SDA_OUT(); 	    
    MY_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        MY_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		MY_IIC_SCL=1;
		delay_us(2); 
		MY_IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 MY_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MY_SDA_IN();//SDA设置为输入
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
        MY_IIC_NAck();//发送nACK
    else
        MY_IIC_Ack(); //发送ACK   
       return receive;
}


