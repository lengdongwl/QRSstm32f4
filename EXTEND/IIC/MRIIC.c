#include "MRIIC.h"
#include "delay.h"

//IIC�ӳ�
void MR_IIC_Delay()
{
    delay_us(2);
}

//��ʼ�ź�
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

//�����ź�
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

//Ӧ���ź�
void MR_IIC_Ack(void) //����Ӧ����
{
    MR_SDA_OUT();
    MR_SCL_LOW();
    MR_SDA_LOW();
    MR_IIC_Delay();
    MR_SCL_HIGH();
    MR_IIC_Delay();
    MR_SCL_LOW();
}

//��Ӧ���ź�
void MR_IIC_NAck(void) //������Ӧ����
{
    MR_SDA_OUT();
    MR_SCL_LOW();
    MR_SDA_HIGH();
    MR_IIC_Delay();
    MR_SCL_HIGH();
    MR_IIC_Delay();
    MR_SCL_LOW();
}

//�ȴ�Ӧ�� 1.��ʱ0.�ɹ�
u8 MR_IIC_Wait_Ack(void) 
{
    u8 ucTimeCnt = 0;

    MR_SDA_IN(); //SDA�ܽ���Ϊ����ģʽ
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

            return 1; //��ʱ
        }
    }
    //�ȵ�Ӧ���ص�ƽ
    MR_SCL_LOW(); //ǯס���ȴ����ͻ����
    return 0;     //�ɹ�
}


void MR_IIC_Send_Byte(u8 byte)
{
    uint8_t i = 0;

    MR_SDA_OUT();
    MR_SCL_LOW(); // ֻ����SCL��Ϊ�͵�ƽʱ��SDA�߲ſ��Ըı�

    for (i = 0; i < 8; i++)
    {
        MR_IIC_SDA = (byte & 0x80) >> 7; //ÿ�η����λ
        byte<<=1; //�������λ
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
        MR_IIC_Delay();  //�ȴ����
        MR_SCL_HIGH(); //�ɶ�ȡ
        recvVal<<=1;  //�����λ�ճ�
        if(MR_READ_SDA) recvVal++;   //�ߵ�ƽ�������λΪ1
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

//1.Ӧ��0.��Ӧ��
u8 MR_IIC_ReadByte2(void)
{
    u8 recvVal=0;
    MR_SDA_IN();

    for(int i=0; i<8; i++)
    {
        MR_SCL_LOW();
        MR_IIC_Delay();  //�ȴ����
        MR_SCL_HIGH(); //�ɶ�ȡ
        recvVal<<=1;  //�����λ�ճ�
        if(MR_READ_SDA) recvVal++;   //�ߵ�ƽ�������λΪ1
        MR_IIC_Delay();        
    }
    return recvVal;
}

void MR_IIC_Init(void)
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
}
