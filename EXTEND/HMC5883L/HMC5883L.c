/**
  ******************************************************************************
  * @file    HMC5883L.c
  * @author  xie
  * @version V1
  * @date    2021年11月2日
  * @brief   扩展端口模块HMC5883L电子罗盘函数库
	
						端口定义：
    
	******************************************************************************
**/
#include "HMC5883L.h"
#include "MY_IIC.h"
#include "stdio.h"
#include "delay.h"
#include "math.h" //Keil library
#include "oled.h" //Keil library

#define Slave_Address 0x3C //定义器件在IIC总线中的从地址


int x, y, z;
static uint32_t HMC5883_BUF[8]; //接收数据缓存区
uint32_t gainBit=1;//增益位0-7  输出寄存器（饱和）溢出时，选择较高的增益值
uint8_t CY;

/**************************************
起始信号
**************************************/
void HMC5883_Start()
{
    HMC5883L_SDA_OUT();
    HMC5883L_IIC_SDA = 1; //拉高数据线
    HMC5883L_IIC_SCL = 1; //拉高时钟线
    delay_ms(1);          //延时
    HMC5883L_IIC_SDA = 0; //产生下降沿
    delay_ms(1);          //延时
    HMC5883L_IIC_SCL = 0; //拉低时钟线
}

/**************************************
停止信号
**************************************/
void HMC5883_Stop()
{
    //HMC5883L_SDA_OUT();
    HMC5883L_IIC_SCL = 1; //拉高时钟线
    HMC5883L_IIC_SDA = 0; //拉低数据线
    delay_ms(1);          //延时
    HMC5883L_IIC_SDA = 1; //产生上升沿
    delay_ms(1);          //延时
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void HMC5883_SendACK(uint8_t ack)
{
    HMC5883L_SDA_OUT();
    if (ack == 1)
    {
        HMC5883L_IIC_SDA = 1;
    }
    else
    {
        HMC5883L_IIC_SDA = 0;
    }
    HMC5883L_IIC_SCL = 1; //拉高时钟线
    delay_us(1);          //延时
    HMC5883L_IIC_SCL = 0; //拉低时钟线
    delay_us(1);          //延时
}

/**************************************
接收应答信号
**************************************/
uint8_t HMC5883_RecvACK()
{
    HMC5883L_SDA_IN();
    HMC5883L_IIC_SCL = 1;   //拉高时钟线
    delay_us(10);           //延时
    CY = HMC5883L_READ_SDA; //进位标志 读应答信号
    HMC5883L_IIC_SCL = 0;   //拉低时钟线
    delay_us(1);            //延时
    return CY;
}
//等待从机应答信号
//返回值：1 接收应答失败
//        0 接收应答成功
uint8_t HMC5883L_I2C_Wait_Ack(void)
{
    uint8_t tempTime = 0;

    HMC5883L_SDA_IN();

    HMC5883L_IIC_SDA = 1;
    delay_us(1);
    HMC5883L_IIC_SCL = 1;
    delay_us(1);

    while (HMC5883L_READ_SDA)
    {
        tempTime++;
        if (tempTime > 250)
        {
            HMC5883_Stop();
            return 1;
        }
    }

    HMC5883L_IIC_SCL = 0;
    return 0;
}
/**************************************
向IIC总线发送一个字节数据
**************************************/
void HMC5883_SendByte(uint8_t dat)
{
    uint8_t i;
    HMC5883L_SDA_OUT();
    HMC5883L_IIC_SCL = 0;   //拉低时钟开始数据传输
    for (i = 0; i < 8; i++) //8位计数器
    {
        //dat <<= 1;              //移出数据的最高位
        if (dat & 0x80)
        {
            HMC5883L_IIC_SDA = 1;
        }
        else
        {
            HMC5883L_IIC_SDA = 0;
        }
        delay_us(1);          //延时
        dat <<= 1;            //移出数据的最高位
        HMC5883L_IIC_SCL = 1; //拉高时钟线
        delay_us(1);          //延时
        HMC5883L_IIC_SCL = 0; //拉低时钟线
    }

    HMC5883_RecvACK();
}
/**************************************
从IIC总线接收一个字节数据
**************************************/
uint8_t HMC5883_RecvByte()
{
    uint8_t i;
    uint8_t dat = 0;
    HMC5883L_SDA_IN();
    HMC5883L_IIC_SDA = 1;   //使能内部上拉,准备读取数据,
    for (i = 0; i < 8; i++) //8位计数器
    {
        dat <<= 1;
        HMC5883L_IIC_SCL = 1; //拉高时钟线
        delay_us(1);          //延时
        if (HMC5883L_READ_SDA)
        {
            dat++;
        }
        //dat <<= 1;
        //dat |= SDA;             //读数据
        HMC5883L_IIC_SCL = 0; //拉低时钟线
        delay_us(1);          //延时

    }
    return dat;
}

//************************写入单字节数据***************************

//void Single_Write_QMC5883(uint8_t REG_Address,uint8_t REG_data)
//{
//    QMC5883_Start();                  //起始信号
//    QMC5883_SendByte(Slave_Address);   //发送设备地址+写信号
//        //if(QMC5883L_I2C_Wait_Ack())
//        //{
//        //        QMC5883_Stop();
//        //        printf("error\r\n");
//        //}
//    QMC5883_SendByte(REG_Address);    //内部寄存器地址，请参考中文pdf
//    QMC5883_SendByte(REG_data);       //内部寄存器数据，请参考中文pdf
//    QMC5883_Stop();                   //发送停止信号
//}

//************************读取单字节数据*************************
uint8_t Single_Read_HMC5883(uint8_t REG_Address)
{
    uint8_t REG_data;
    HMC5883_Start();                     //起始信号
    HMC5883_SendByte(Slave_Address);     //发送设备地址+写信号
    HMC5883_SendByte(REG_Address);       //发送存储单元地址，从0开始
    HMC5883_Start();                     //起始信号
    HMC5883_SendByte(Slave_Address + 1); //发送设备地址+读信号
    REG_data = HMC5883_RecvByte();       //读出寄存器数据
    HMC5883_SendACK(1);
    HMC5883_Stop(); //停止信号
    return REG_data;
}
uint32_t HMC5883_Read_X(void)
{
    return Single_Read_HMC5883(3)+Single_Read_HMC5883(4);
}
uint32_t HMC5883_Read_Y(void)
{
    return Single_Read_HMC5883(7)+Single_Read_HMC5883(8);
}
uint32_t HMC5883_Read_Z(void)
{
    return Single_Read_HMC5883(5)+Single_Read_HMC5883(6);
}

uint8_t Multiple_Read_Status()
{
    HMC5883_Start();                     //起始信号
    HMC5883_SendByte(Slave_Address);     //发送设备地址+写信号
    HMC5883_SendByte(0x09);              //发送存储单元地址，从0x00开始
    HMC5883_Start();                     //起始信号
    HMC5883_SendByte(Slave_Address + 1); //发送设备地址+读信号
    return HMC5883_RecvByte();
}
//******************************************************
//连续读出QMC5883内部角度数据，地址范围0x00~0x05

//如果所有六种数据输出寄存器未被读取完，那么新的数据不能被更新到相应的数据输出寄存器。
//******************************************************
void Multiple_Read_HMC5883(void)
{   
   uint8_t i=0;
   HMC5883_Start();                          //起始信号
   HMC5883_SendByte(Slave_Address);          //发送设备地址+写信号
   HMC5883_SendByte(0x03);                   //发送存储单元地址，从0x00开始    
   HMC5883_Start();                          //起始信号
   HMC5883_SendByte(Slave_Address+1);        //发送设备地址+读信号
     for (i=0; i<6; i++)                      //连续读取6个地址数据，存储中BUF
    {
        HMC5883_BUF[i] = HMC5883_RecvByte();          //BUF[0]存储数据
        if (i == 5)
        {
           HMC5883_SendACK(1);                //最后一个数据需要回非应答NOACK
        }
        else
        {
          HMC5883_SendACK(0);                 //应答ACK
       }
   }
     x=(HMC5883_BUF[0]<<8)|(HMC5883_BUF[1]);
     if(x>32767) x=0xffff-x+1;
     z=(HMC5883_BUF[2]<<8)|(HMC5883_BUF[3]);
     if(z>32767) z=0xffff-z+1;
     y=(HMC5883_BUF[4]<<8)|(HMC5883_BUF[5]);
     if(y>32767) y=0xffff-y+1;
   HMC5883_Stop();                           //停止信号
   HAL_Delay(10);
}

//2初始化HMC5883，根据需要请参考pdf进行修改****
void HMC5883_Init()
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOC_CLK_ENABLE(); //开启GPIOC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE(); //开启GPIOD时钟

    GPIO_Initure.Pin = GPIO_PIN_12;          //PC12
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;         //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    //高速
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_2;           //PD2
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;         //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    //高速
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);
    delay_ms(1);


    /************************初始化寄存器配置***********************/
    HMC5883_Start();
    HMC5883_SendByte(0x3c); //发送设备地址+写信号
    HMC5883_SendByte(0);    
    /*配置寄存器A 
    0 11 110 00 
    0 (这个位必须清除以正确运行)
    11(在每次测量输出中选择采样平均数（1-8）00=1; 01=2;10=4; 11=8(缺省))
    110(数据输出速率位)
    00(测量配置位)
    */
    HMC5883_SendByte(0x78);


    HMC5883_Start();
    HMC5883_SendByte(0x3c); //发送设备地址+写信号
    HMC5883_SendByte(1);    
    /*配置寄存器B
    7-5增益配置位 （0-7）
    4-0必须清零位
    */
    HMC5883_SendByte(gainBit<<7);
    HMC5883_Start();
    HMC5883_SendByte(0x3c); //发送设备地址+写信号


    HMC5883_SendByte(2);    
    /*模式寄存器
    7-2必须清零 每一次单测量操作后MR7位在内部设置为1。当配置模式寄存器是设置为0.
    1-0模式位 0连续测量 1.单一测量
    */
    HMC5883_SendByte(0);
    HMC5883_Stop();
}
