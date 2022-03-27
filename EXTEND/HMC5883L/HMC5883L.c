/**
  ******************************************************************************
  * @file    HMC5883L.c
  * @author  xie
  * @version V1
  * @date    2021��11��2��
  * @brief   ��չ�˿�ģ��HMC5883L�������̺�����
	
						�˿ڶ��壺
    
	******************************************************************************
**/
#include "HMC5883L.h"
#include "MY_IIC.h"
#include "stdio.h"
#include "delay.h"
#include "math.h" //Keil library
#include "oled.h" //Keil library

#define Slave_Address 0x3C //����������IIC�����еĴӵ�ַ


int x, y, z;
static uint32_t HMC5883_BUF[8]; //�������ݻ�����
uint32_t gainBit=1;//����λ0-7  ����Ĵ��������ͣ����ʱ��ѡ��ϸߵ�����ֵ
uint8_t CY;

/**************************************
��ʼ�ź�
**************************************/
void HMC5883_Start()
{
    HMC5883L_SDA_OUT();
    HMC5883L_IIC_SDA = 1; //����������
    HMC5883L_IIC_SCL = 1; //����ʱ����
    delay_ms(1);          //��ʱ
    HMC5883L_IIC_SDA = 0; //�����½���
    delay_ms(1);          //��ʱ
    HMC5883L_IIC_SCL = 0; //����ʱ����
}

/**************************************
ֹͣ�ź�
**************************************/
void HMC5883_Stop()
{
    //HMC5883L_SDA_OUT();
    HMC5883L_IIC_SCL = 1; //����ʱ����
    HMC5883L_IIC_SDA = 0; //����������
    delay_ms(1);          //��ʱ
    HMC5883L_IIC_SDA = 1; //����������
    delay_ms(1);          //��ʱ
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
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
    HMC5883L_IIC_SCL = 1; //����ʱ����
    delay_us(1);          //��ʱ
    HMC5883L_IIC_SCL = 0; //����ʱ����
    delay_us(1);          //��ʱ
}

/**************************************
����Ӧ���ź�
**************************************/
uint8_t HMC5883_RecvACK()
{
    HMC5883L_SDA_IN();
    HMC5883L_IIC_SCL = 1;   //����ʱ����
    delay_us(10);           //��ʱ
    CY = HMC5883L_READ_SDA; //��λ��־ ��Ӧ���ź�
    HMC5883L_IIC_SCL = 0;   //����ʱ����
    delay_us(1);            //��ʱ
    return CY;
}
//�ȴ��ӻ�Ӧ���ź�
//����ֵ��1 ����Ӧ��ʧ��
//        0 ����Ӧ��ɹ�
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
��IIC���߷���һ���ֽ�����
**************************************/
void HMC5883_SendByte(uint8_t dat)
{
    uint8_t i;
    HMC5883L_SDA_OUT();
    HMC5883L_IIC_SCL = 0;   //����ʱ�ӿ�ʼ���ݴ���
    for (i = 0; i < 8; i++) //8λ������
    {
        //dat <<= 1;              //�Ƴ����ݵ����λ
        if (dat & 0x80)
        {
            HMC5883L_IIC_SDA = 1;
        }
        else
        {
            HMC5883L_IIC_SDA = 0;
        }
        delay_us(1);          //��ʱ
        dat <<= 1;            //�Ƴ����ݵ����λ
        HMC5883L_IIC_SCL = 1; //����ʱ����
        delay_us(1);          //��ʱ
        HMC5883L_IIC_SCL = 0; //����ʱ����
    }

    HMC5883_RecvACK();
}
/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
uint8_t HMC5883_RecvByte()
{
    uint8_t i;
    uint8_t dat = 0;
    HMC5883L_SDA_IN();
    HMC5883L_IIC_SDA = 1;   //ʹ���ڲ�����,׼����ȡ����,
    for (i = 0; i < 8; i++) //8λ������
    {
        dat <<= 1;
        HMC5883L_IIC_SCL = 1; //����ʱ����
        delay_us(1);          //��ʱ
        if (HMC5883L_READ_SDA)
        {
            dat++;
        }
        //dat <<= 1;
        //dat |= SDA;             //������
        HMC5883L_IIC_SCL = 0; //����ʱ����
        delay_us(1);          //��ʱ

    }
    return dat;
}

//************************д�뵥�ֽ�����***************************

//void Single_Write_QMC5883(uint8_t REG_Address,uint8_t REG_data)
//{
//    QMC5883_Start();                  //��ʼ�ź�
//    QMC5883_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
//        //if(QMC5883L_I2C_Wait_Ack())
//        //{
//        //        QMC5883_Stop();
//        //        printf("error\r\n");
//        //}
//    QMC5883_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ����ο�����pdf
//    QMC5883_SendByte(REG_data);       //�ڲ��Ĵ������ݣ���ο�����pdf
//    QMC5883_Stop();                   //����ֹͣ�ź�
//}

//************************��ȡ���ֽ�����*************************
uint8_t Single_Read_HMC5883(uint8_t REG_Address)
{
    uint8_t REG_data;
    HMC5883_Start();                     //��ʼ�ź�
    HMC5883_SendByte(Slave_Address);     //�����豸��ַ+д�ź�
    HMC5883_SendByte(REG_Address);       //���ʹ洢��Ԫ��ַ����0��ʼ
    HMC5883_Start();                     //��ʼ�ź�
    HMC5883_SendByte(Slave_Address + 1); //�����豸��ַ+���ź�
    REG_data = HMC5883_RecvByte();       //�����Ĵ�������
    HMC5883_SendACK(1);
    HMC5883_Stop(); //ֹͣ�ź�
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
    HMC5883_Start();                     //��ʼ�ź�
    HMC5883_SendByte(Slave_Address);     //�����豸��ַ+д�ź�
    HMC5883_SendByte(0x09);              //���ʹ洢��Ԫ��ַ����0x00��ʼ
    HMC5883_Start();                     //��ʼ�ź�
    HMC5883_SendByte(Slave_Address + 1); //�����豸��ַ+���ź�
    return HMC5883_RecvByte();
}
//******************************************************
//��������QMC5883�ڲ��Ƕ����ݣ���ַ��Χ0x00~0x05

//�������������������Ĵ���δ����ȡ�꣬��ô�µ����ݲ��ܱ����µ���Ӧ����������Ĵ�����
//******************************************************
void Multiple_Read_HMC5883(void)
{   
   uint8_t i=0;
   HMC5883_Start();                          //��ʼ�ź�
   HMC5883_SendByte(Slave_Address);          //�����豸��ַ+д�ź�
   HMC5883_SendByte(0x03);                   //���ʹ洢��Ԫ��ַ����0x00��ʼ    
   HMC5883_Start();                          //��ʼ�ź�
   HMC5883_SendByte(Slave_Address+1);        //�����豸��ַ+���ź�
     for (i=0; i<6; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
    {
        HMC5883_BUF[i] = HMC5883_RecvByte();          //BUF[0]�洢����
        if (i == 5)
        {
           HMC5883_SendACK(1);                //���һ��������Ҫ�ط�Ӧ��NOACK
        }
        else
        {
          HMC5883_SendACK(0);                 //Ӧ��ACK
       }
   }
     x=(HMC5883_BUF[0]<<8)|(HMC5883_BUF[1]);
     if(x>32767) x=0xffff-x+1;
     z=(HMC5883_BUF[2]<<8)|(HMC5883_BUF[3]);
     if(z>32767) z=0xffff-z+1;
     y=(HMC5883_BUF[4]<<8)|(HMC5883_BUF[5]);
     if(y>32767) y=0xffff-y+1;
   HMC5883_Stop();                           //ֹͣ�ź�
   HAL_Delay(10);
}

//2��ʼ��HMC5883��������Ҫ��ο�pdf�����޸�****
void HMC5883_Init()
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOC_CLK_ENABLE(); //����GPIOCʱ��
    __HAL_RCC_GPIOD_CLK_ENABLE(); //����GPIODʱ��

    GPIO_Initure.Pin = GPIO_PIN_12;          //PC12
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; //�������
    GPIO_Initure.Pull = GPIO_PULLUP;         //����
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    //����
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_2;           //PD2
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; //�������
    GPIO_Initure.Pull = GPIO_PULLUP;         //����
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    //����
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);
    delay_ms(1);


    /************************��ʼ���Ĵ�������***********************/
    HMC5883_Start();
    HMC5883_SendByte(0x3c); //�����豸��ַ+д�ź�
    HMC5883_SendByte(0);    
    /*���üĴ���A 
    0 11 110 00 
    0 (���λ�����������ȷ����)
    11(��ÿ�β��������ѡ�����ƽ������1-8��00=1; 01=2;10=4; 11=8(ȱʡ))
    110(�����������λ)
    00(��������λ)
    */
    HMC5883_SendByte(0x78);


    HMC5883_Start();
    HMC5883_SendByte(0x3c); //�����豸��ַ+д�ź�
    HMC5883_SendByte(1);    
    /*���üĴ���B
    7-5��������λ ��0-7��
    4-0��������λ
    */
    HMC5883_SendByte(gainBit<<7);
    HMC5883_Start();
    HMC5883_SendByte(0x3c); //�����豸��ַ+д�ź�


    HMC5883_SendByte(2);    
    /*ģʽ�Ĵ���
    7-2�������� ÿһ�ε�����������MR7λ���ڲ�����Ϊ1��������ģʽ�Ĵ���������Ϊ0.
    1-0ģʽλ 0�������� 1.��һ����
    */
    HMC5883_SendByte(0);
    HMC5883_Stop();
}
