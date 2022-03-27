/*
 * @Description: ����ʶ��
 * @Autor: 309 Mushroom
 * @Date: 2021-11-21 15:12:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-28 18:59:58
 */
#include "paj7620u2.h"
#include "paj7620u2_cfg.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "stdio.h"
//#include "ili9486.h"


/******************************************************************/
//PAJ2670 I2C��ʼ��
void GS_i2c_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitStructure.Pin = GPIO_PIN_4;          //a4
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //�������
    GPIO_InitStructure.Pull = GPIO_PULLUP;         //����
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;    //����
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_3;           //b3
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //�������
    GPIO_InitStructure.Pull = GPIO_PULLUP;         //����
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;    //����
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    //GPIO_SetBits(GPIOA,GPIO_Pin_4);//PB10,PB11 �����
    //GPIO_SetBits(GPIOB,GPIO_Pin_3);//PB10,PB11 �����

}

//����IIC��ʼ�ź�
static void GS_IIC_Start(void)
{
    GS_SDA_OUT();//sda�����
    GS_IIC_SDA=1;
    GS_IIC_SCL=1;
    delay_us(4);
    GS_IIC_SDA=0;//START:when CLK is high,DATA change form high to low
    delay_us(4);
    GS_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������
}

//����IICֹͣ�ź�
static void GS_IIC_Stop(void)
{
    GS_SDA_OUT();//sda�����
    GS_IIC_SCL=0;
    GS_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    GS_IIC_SCL=1;
    GS_IIC_SDA=1;//����I2C���߽����ź�
    delay_us(4);
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
static u8 GS_IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    GS_SDA_IN();  //SDA����Ϊ����
    GS_IIC_SDA=1;
    delay_us(3);
    GS_IIC_SCL=1;
    delay_us(3);
    while(GS_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            GS_IIC_Stop();
            return 1;
        }
    }
    GS_IIC_SCL=0;//ʱ�����0
    return 0;
}

//����ACKӦ��
static void GS_IIC_Ack(void)
{
    GS_IIC_SCL=0;
    GS_SDA_OUT();
    GS_IIC_SDA=0;
    delay_us(3);
    GS_IIC_SCL=1;
    delay_us(3);
    GS_IIC_SCL=0;
}

//������ACKӦ��
static void GS_IIC_NAck(void)
{
    GS_IIC_SCL=0;
    GS_SDA_OUT();
    GS_IIC_SDA=1;
    delay_us(2);
    GS_IIC_SCL=1;
    delay_us(2);
    GS_IIC_SCL=0;
}

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
static void GS_IIC_Send_Byte(u8 txd)
{
    u8 t;
    GS_SDA_OUT();
    GS_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0; t<8; t++)
    {
        if((txd&0x80)>>7)
            GS_IIC_SDA=1;
        else
            GS_IIC_SDA=0;
        txd<<=1;
        delay_us(5);
        GS_IIC_SCL=1;
        delay_us(5);
        GS_IIC_SCL=0;
        delay_us(5);
    }
}

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
static u8 GS_IIC_Read_Byte(u8 ack)
{
    u8 i,receive=0;
    GS_SDA_IN();//SDA����Ϊ����
    for(i=0; i<8; i++ )
    {
        GS_IIC_SCL=0;
        delay_us(4);
        GS_IIC_SCL=1;
        receive<<=1;
        if(GS_READ_SDA)receive++;
        delay_us(4);
    }
    if (!ack)
        GS_IIC_NAck();//����nACK
    else
        GS_IIC_Ack(); //����ACK
    return receive;
}

//PAJ7620U2дһ���ֽ�����
u8 GS_Write_Byte(u8 REG_Address,u8 REG_data)
{
    GS_IIC_Start();
    GS_IIC_Send_Byte(PAJ7620_ID);
    if(GS_IIC_Wait_Ack())
    {
        GS_IIC_Stop();//�ͷ�����
        return 1;//ûӦ�����˳�

    }
    GS_IIC_Send_Byte(REG_Address);
    GS_IIC_Wait_Ack();
    GS_IIC_Send_Byte(REG_data);
    GS_IIC_Wait_Ack();
    GS_IIC_Stop();

    return 0;
}

//PAJ7620U2��һ���ֽ�����
u8 GS_Read_Byte(u8 REG_Address)
{
    u8 REG_data;

    GS_IIC_Start();
    GS_IIC_Send_Byte(PAJ7620_ID);//��д����
    if(GS_IIC_Wait_Ack())
    {
        GS_IIC_Stop();//�ͷ�����
        return 0;//ûӦ�����˳�
    }
    GS_IIC_Send_Byte(REG_Address);
    GS_IIC_Wait_Ack();
    GS_IIC_Start();
    GS_IIC_Send_Byte(PAJ7620_ID|0x01);//��������
    GS_IIC_Wait_Ack();
    REG_data = GS_IIC_Read_Byte(0);
    GS_IIC_Stop();

    return REG_data;
}
//PAJ7620U2��n���ֽ�����
u8 GS_Read_nByte(u8 REG_Address,u16 len,u8 *buf)
{
    GS_IIC_Start();
    GS_IIC_Send_Byte(PAJ7620_ID);//��д����
    if(GS_IIC_Wait_Ack())
    {
        GS_IIC_Stop();//�ͷ�����
        return 1;//ûӦ�����˳�
    }
    GS_IIC_Send_Byte(REG_Address);
    GS_IIC_Wait_Ack();

    GS_IIC_Start();
    GS_IIC_Send_Byte(PAJ7620_ID|0x01);//��������
    GS_IIC_Wait_Ack();
    while(len)
    {
        if(len==1)
        {
            *buf = GS_IIC_Read_Byte(0);
        }
        else
        {
            *buf = GS_IIC_Read_Byte(1);
        }
        buf++;
        len--;
    }
    GS_IIC_Stop();//�ͷ�����

    return 0;

}
//PAJ7620����
void GS_WakeUp(void)
{
    GS_IIC_Start();
    GS_IIC_Send_Byte(PAJ7620_ID);//��д����
    GS_IIC_Stop();//�ͷ�����
}

/******************************************************************/


//ѡ��PAJ7620U2 BANK����
void paj7620u2_selectBank(bank_e bank)
{
    switch(bank)
    {
    case BANK0:
        GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);
        break;//BANK0�Ĵ�������
    case BANK1:
        GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);
        break;//BANK1�Ĵ�������
    }

}

//PAJ7620U2����
u8 paj7620u2_wakeup(void)
{
    u8 data=0x0a;
    GS_WakeUp();//����PAJ7620U2
    delay_ms(5);//����ʱ��>400us
    GS_WakeUp();//����PAJ7620U2
    delay_ms(5);//����ʱ��>400us
    paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
    data = GS_Read_Byte(0x00);//��ȡ״̬
    if(data!=0x20) return 0; //����ʧ��

    return 1;
}

//PAJ7620U2��ʼ��
//����ֵ��0:ʧ�� 1:�ɹ�
u8 paj7620u2_init(void)
{
    u8 i;
    u8 status;

    GS_i2c_init();//IIC��ʼ��
    status = paj7620u2_wakeup();//����PAJ7620U2
    if(!status) return 0;
    paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
    for(i=0; i<INIT_SIZE; i++)
    {
        GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//��ʼ��PAJ7620U2
    }
    paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������

    return 1;
}

//���˵�
void paj7620u2_test_ui(void)
{
  //  POINT_COLOR=BLUE;//��������Ϊ��ɫ

}

//����ʶ�����
u16 Gesture_test(void)
{
    u8 i;
    u8 status;
    u8 data[2]= {0x00};
    u16 gesture_data;//���ƽ��

    paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
    for(i=0; i<GESTURE_SIZE; i++)
    {
        GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//����ʶ��ģʽ��ʼ��
    }
    paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
    i=0;


    status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);//��ȡ����״̬
    if(!status)
    {
        gesture_data =(u16)data[1]<<8 | data[0];
        if(gesture_data)
        {
            switch(gesture_data)
            {
            case GES_UP:
                // printf("Up\r\n");
                //GUI_Show_Str(160,240,RED,WHITE,"����  ",32,0);
                break; //����
            case GES_DOWM:
                //����LED1
                //GUI_Show_Str(160,240,RED,WHITE,"����  ",32,0);
                break; //����
            case GES_LEFT:
                //����LED2
                //GUI_Show_Str(160,240,RED,WHITE,"����  ",32,0);
                break; //����
            case GES_RIGHT:
                //GUI_Show_Str(160,240,RED,WHITE,"����  ",32,0);
                break; //����
            case GES_FORWARD:
                //GUI_Show_Str(160,240,RED,WHITE,"��ǰ  ",32,0);
                break; //��ǰ
            case GES_BACKWARD:
                //GUI_Show_Str(160,240,RED,WHITE,"���  ",32,0);
                break; //���
            case GES_CLOCKWISE:
                //GUI_Show_Str(160,240,RED,WHITE,"˳ʱ��",32,0);
                break; //˳ʱ��
            case GES_COUNT_CLOCKWISE:
                //GUI_Show_Str(160,240,RED,WHITE,"��ʱ��",32,0);
                break; //��ʱ��
            case GES_WAVE:
                //GUI_Show_Str(160,240,RED,WHITE,"�Ӷ�  ",32,0);
                break; //�Ӷ�
            default:
                break;

            }
        }
    }
    return gesture_data;
}

//�ӽ�������
void Ps_test(void)
{
    u8 i;
    u8 key;
    u8 data[2]= {0x00};
    u8 obj_brightness=0;
    u16 obj_size=0;

    paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
    for(i=0; i<PROXIM_SIZE; i++)
    {
        GS_Write_Byte(proximity_arry[i][0],proximity_arry[i][1]);//�ӽ����ģʽ��ʼ��
    }
    paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������
    i=0;
//    POINT_COLOR=BLUE;//��������Ϊ��ɫ

//    POINT_COLOR=RED;//��������Ϊ��ɫ

    while(1)
    {


        obj_brightness = GS_Read_Byte(PAJ_GET_OBJECT_BRIGHTNESS);//��ȡ��������
        data[0] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_1);//��ȡ�����С
        data[1] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_2);
        obj_size = ((u16)data[1] & 0x0f)<<8 | data[0];

        printf("obj_brightness: %d\r\n",obj_brightness);
        printf("obj_size: %d\r\n",obj_size);

        delay_ms(100);
        i++;
        if(i==5)
        {

            i=0;
        }
    }

}

