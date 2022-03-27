/*
 * @Description: 手势识别
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
//PAJ2670 I2C初始化
void GS_i2c_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitStructure.Pin = GPIO_PIN_4;          //a4
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
    GPIO_InitStructure.Pull = GPIO_PULLUP;         //上拉
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;    //高速
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = GPIO_PIN_3;           //b3
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
    GPIO_InitStructure.Pull = GPIO_PULLUP;         //上拉
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;    //高速
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

    //GPIO_SetBits(GPIOA,GPIO_Pin_4);//PB10,PB11 输出高
    //GPIO_SetBits(GPIOB,GPIO_Pin_3);//PB10,PB11 输出高

}

//产生IIC起始信号
static void GS_IIC_Start(void)
{
    GS_SDA_OUT();//sda线输出
    GS_IIC_SDA=1;
    GS_IIC_SCL=1;
    delay_us(4);
    GS_IIC_SDA=0;//START:when CLK is high,DATA change form high to low
    delay_us(4);
    GS_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
}

//产生IIC停止信号
static void GS_IIC_Stop(void)
{
    GS_SDA_OUT();//sda线输出
    GS_IIC_SCL=0;
    GS_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    GS_IIC_SCL=1;
    GS_IIC_SDA=1;//发送I2C总线结束信号
    delay_us(4);
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
static u8 GS_IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    GS_SDA_IN();  //SDA设置为输入
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
    GS_IIC_SCL=0;//时钟输出0
    return 0;
}

//产生ACK应答
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

//不产生ACK应答
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

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
static void GS_IIC_Send_Byte(u8 txd)
{
    u8 t;
    GS_SDA_OUT();
    GS_IIC_SCL=0;//拉低时钟开始数据传输
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

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
static u8 GS_IIC_Read_Byte(u8 ack)
{
    u8 i,receive=0;
    GS_SDA_IN();//SDA设置为输入
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
        GS_IIC_NAck();//发送nACK
    else
        GS_IIC_Ack(); //发送ACK
    return receive;
}

//PAJ7620U2写一个字节数据
u8 GS_Write_Byte(u8 REG_Address,u8 REG_data)
{
    GS_IIC_Start();
    GS_IIC_Send_Byte(PAJ7620_ID);
    if(GS_IIC_Wait_Ack())
    {
        GS_IIC_Stop();//释放总线
        return 1;//没应答则退出

    }
    GS_IIC_Send_Byte(REG_Address);
    GS_IIC_Wait_Ack();
    GS_IIC_Send_Byte(REG_data);
    GS_IIC_Wait_Ack();
    GS_IIC_Stop();

    return 0;
}

//PAJ7620U2读一个字节数据
u8 GS_Read_Byte(u8 REG_Address)
{
    u8 REG_data;

    GS_IIC_Start();
    GS_IIC_Send_Byte(PAJ7620_ID);//发写命令
    if(GS_IIC_Wait_Ack())
    {
        GS_IIC_Stop();//释放总线
        return 0;//没应答则退出
    }
    GS_IIC_Send_Byte(REG_Address);
    GS_IIC_Wait_Ack();
    GS_IIC_Start();
    GS_IIC_Send_Byte(PAJ7620_ID|0x01);//发读命令
    GS_IIC_Wait_Ack();
    REG_data = GS_IIC_Read_Byte(0);
    GS_IIC_Stop();

    return REG_data;
}
//PAJ7620U2读n个字节数据
u8 GS_Read_nByte(u8 REG_Address,u16 len,u8 *buf)
{
    GS_IIC_Start();
    GS_IIC_Send_Byte(PAJ7620_ID);//发写命令
    if(GS_IIC_Wait_Ack())
    {
        GS_IIC_Stop();//释放总线
        return 1;//没应答则退出
    }
    GS_IIC_Send_Byte(REG_Address);
    GS_IIC_Wait_Ack();

    GS_IIC_Start();
    GS_IIC_Send_Byte(PAJ7620_ID|0x01);//发读命令
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
    GS_IIC_Stop();//释放总线

    return 0;

}
//PAJ7620唤醒
void GS_WakeUp(void)
{
    GS_IIC_Start();
    GS_IIC_Send_Byte(PAJ7620_ID);//发写命令
    GS_IIC_Stop();//释放总线
}

/******************************************************************/


//选择PAJ7620U2 BANK区域
void paj7620u2_selectBank(bank_e bank)
{
    switch(bank)
    {
    case BANK0:
        GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);
        break;//BANK0寄存器区域
    case BANK1:
        GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);
        break;//BANK1寄存器区域
    }

}

//PAJ7620U2唤醒
u8 paj7620u2_wakeup(void)
{
    u8 data=0x0a;
    GS_WakeUp();//唤醒PAJ7620U2
    delay_ms(5);//唤醒时间>400us
    GS_WakeUp();//唤醒PAJ7620U2
    delay_ms(5);//唤醒时间>400us
    paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
    data = GS_Read_Byte(0x00);//读取状态
    if(data!=0x20) return 0; //唤醒失败

    return 1;
}

//PAJ7620U2初始化
//返回值：0:失败 1:成功
u8 paj7620u2_init(void)
{
    u8 i;
    u8 status;

    GS_i2c_init();//IIC初始化
    status = paj7620u2_wakeup();//唤醒PAJ7620U2
    if(!status) return 0;
    paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
    for(i=0; i<INIT_SIZE; i++)
    {
        GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//初始化PAJ7620U2
    }
    paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域

    return 1;
}

//主菜单
void paj7620u2_test_ui(void)
{
  //  POINT_COLOR=BLUE;//设置字体为蓝色

}

//手势识别测试
u16 Gesture_test(void)
{
    u8 i;
    u8 status;
    u8 data[2]= {0x00};
    u16 gesture_data;//手势结果

    paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
    for(i=0; i<GESTURE_SIZE; i++)
    {
        GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//手势识别模式初始化
    }
    paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
    i=0;


    status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);//读取手势状态
    if(!status)
    {
        gesture_data =(u16)data[1]<<8 | data[0];
        if(gesture_data)
        {
            switch(gesture_data)
            {
            case GES_UP:
                // printf("Up\r\n");
                //GUI_Show_Str(160,240,RED,WHITE,"向上  ",32,0);
                break; //向上
            case GES_DOWM:
                //点亮LED1
                //GUI_Show_Str(160,240,RED,WHITE,"向下  ",32,0);
                break; //向下
            case GES_LEFT:
                //点亮LED2
                //GUI_Show_Str(160,240,RED,WHITE,"向左  ",32,0);
                break; //向左
            case GES_RIGHT:
                //GUI_Show_Str(160,240,RED,WHITE,"向右  ",32,0);
                break; //向右
            case GES_FORWARD:
                //GUI_Show_Str(160,240,RED,WHITE,"向前  ",32,0);
                break; //向前
            case GES_BACKWARD:
                //GUI_Show_Str(160,240,RED,WHITE,"向后  ",32,0);
                break; //向后
            case GES_CLOCKWISE:
                //GUI_Show_Str(160,240,RED,WHITE,"顺时针",32,0);
                break; //顺时针
            case GES_COUNT_CLOCKWISE:
                //GUI_Show_Str(160,240,RED,WHITE,"逆时针",32,0);
                break; //逆时针
            case GES_WAVE:
                //GUI_Show_Str(160,240,RED,WHITE,"挥动  ",32,0);
                break; //挥动
            default:
                break;

            }
        }
    }
    return gesture_data;
}

//接近检测测试
void Ps_test(void)
{
    u8 i;
    u8 key;
    u8 data[2]= {0x00};
    u8 obj_brightness=0;
    u16 obj_size=0;

    paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
    for(i=0; i<PROXIM_SIZE; i++)
    {
        GS_Write_Byte(proximity_arry[i][0],proximity_arry[i][1]);//接近检测模式初始化
    }
    paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
    i=0;
//    POINT_COLOR=BLUE;//设置字体为蓝色

//    POINT_COLOR=RED;//设置字体为蓝色

    while(1)
    {


        obj_brightness = GS_Read_Byte(PAJ_GET_OBJECT_BRIGHTNESS);//读取物体亮度
        data[0] = GS_Read_Byte(PAJ_GET_OBJECT_SIZE_1);//读取物体大小
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

