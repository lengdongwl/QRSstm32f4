#include "MRIIC.h"
#include "delay.h"

//IIC延迟
void MR_IIC_Delay()
{
    delay_us(2);
}

//起始信号
void MR_IIC_Start(void)
{
    MR_SDA_OUT();
    MR_SCL_HIGH();
    MR_SDA_HIGH();
    MR_IIC_Delay();
    MR_SDA_LOW();
    MR_IIC_Delay();
    MR_SCL_LOW();
    MR_IIC_Delay();
}

//结束信号
void MR_IIC_Stop(void)
{
    MR_SDA_OUT();
    MR_SCL_LOW();
    MR_SDA_LOW();
    MR_IIC_Delay();
    MR_IIC_Delay();
    MR_SCL_HIGH();
    MR_SDA_HIGH();
    MR_IIC_Delay();
    MR_IIC_Delay();
}

//应答信号
void MR_IIC_Ack(void) //主机应答函数
{
    MR_SDA_OUT();
    MR_SCL_LOW();
    MR_SDA_LOW();
    MR_IIC_Delay();
    MR_SCL_HIGH();
    MR_IIC_Delay();
    MR_SCL_LOW();
}

//非应答信号
void MR_IIC_NAck(void) //主机非应答函数
{
    MR_SDA_OUT();
    MR_SCL_LOW();
    MR_SDA_HIGH();
    MR_IIC_Delay();
    MR_SCL_HIGH();
    MR_IIC_Delay();
    MR_SCL_LOW();
}

//等待应答 1.超时0.成功
u8 MR_IIC_Wait_Ack(void) 
{
    u8 ucTimeCnt = 0;

    MR_SDA_IN(); //SDA管脚设为输入模式
    MR_SDA_HIGH();
    MR_IIC_Delay();
    MR_SCL_HIGH();
    MR_IIC_Delay();
    while (MR_READ_SDA)
    {
        ucTimeCnt++;
        if (ucTimeCnt > 250)
        {
            MR_IIC_Stop();

            return 1; //超时
        }
    }
    //等到应道地电平
    MR_SCL_LOW(); //钳住，等待发送或接受
    return 0;     //成功
}


void MR_IIC_Send_Byte(u8 byte)
{
    uint8_t i = 0;

    MR_SDA_OUT();
    MR_SCL_LOW(); // 只有在SCL线为低电平时，SDA线才可以改变

    for (i = 0; i < 8; i++)
    {
        MR_IIC_SDA = (byte & 0x80) >> 7; //每次发最高位
        byte<<=1; //更新最高位
        MR_IIC_Delay();
        MR_SCL_HIGH();
        MR_IIC_Delay();
        MR_SCL_LOW();
        MR_IIC_Delay();
    }
}


u8 MR_IIC_ReadByte(u8 ack)
{
    u8 recvVal=0;
    MR_SDA_IN();

    for(int i=0; i<8; i++)
    {
        MR_SCL_LOW();
        MR_IIC_Delay();  //等待输出
        MR_SCL_HIGH(); //可读取
        recvVal<<=1;  //将最低位空出
        if(MR_READ_SDA) recvVal++;   //高电平，则最低位为1
        MR_IIC_Delay();        
    }

    if(ack)
    {
        MR_IIC_Ack();
    }
    else
    {
        MR_IIC_NAck();
    }

    return recvVal;
}

//1.应答0.不应答
u8 MR_IIC_ReadByte2(void)
{
    u8 recvVal=0;
    MR_SDA_IN();

    for(int i=0; i<8; i++)
    {
        MR_SCL_LOW();
        MR_IIC_Delay();  //等待输出
        MR_SCL_HIGH(); //可读取
        recvVal<<=1;  //将最低位空出
        if(MR_READ_SDA) recvVal++;   //高电平，则最低位为1
        MR_IIC_Delay();        
    }
    return recvVal;
}

void MR_IIC_Init(void)
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
}
