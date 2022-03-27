/**
  ******************************************************************************
  * @file    bh1750.c
  * @author  xie
  * @version V2
  * @date    2021年10月28日
  * @brief   bh1750光强度传感器
	
						端口定义：
							PA8：IIC_SCL_BH1750 //BH1750 IIC的数据引脚
							PC9：IIC_SDA_BH1750 //BH1750 IIC的时钟引脚	 
	******************************************************************************
**/	
#include "bh1750.h"
#include "delay.h"

#define	 SlaveAddress   0x46 			//定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
uint8_t  BUF[4];                  //接收数据缓存区
uint16_t illumination_data; 			//光照读取值

/**
  * @brief  BH1750端口初始化
  * @param  None
  * @retval	None 
  * @note		None
  */
void BH1750_PortInit(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();           //开启GPIOA时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();           //开启GPIOC时钟
	
    GPIO_Initure.Pin=GPIO_PIN_8;						//PA8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);			//初始化端口
	
	  GPIO_Initure.Pin=GPIO_PIN_9;						//PC9
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);			//初始化端口

}

/**
  * @brief  SDA输出模式
  * @param  None
  * @retval	None 
  * @note		None
  */
void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();           //开启GPIOC时钟

		GPIO_Initure.Pin=GPIO_PIN_9;						//PC9
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);			//初始化端口
}

/**
  * @brief  SDA输入模式
  * @param  None
  * @retval	None 
  * @note		None
  */
void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();           //开启GPIOC时钟

		GPIO_Initure.Pin=GPIO_PIN_9;						//PC9
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  		//推挽输出
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);			//初始化端口

}

/***************************************************************
** 功能：     产生IIC起始信号
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void BH1750_Start()
{
  SDA_OUT();     //sda线输出
	IIC_SDA_BH1750=1;	  	  
	IIC_SCL_BH1750=1;
	delay_us(4);
 	IIC_SDA_BH1750=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_BH1750=0;//钳住I2C总线，准备发送或接收数据 
}

/***************************************************************
** 功能：     产生IIC停止信号
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void BH1750_Stop()
{
	SDA_OUT();//sda线输出
	IIC_SDA_BH1750=0;//STOP:when CLK is high DATA change form low to high
	IIC_SCL_BH1750=1;
 	delay_us(4);
	IIC_SDA_BH1750=1;//发送I2C总线结束信号
	delay_us(4);
}

/***************************************************************
** 功能：     产生IIC应答信号
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void BH1750_SendACK(uint8_t ack)
{
    SDA_OUT();          //sda线输出
		if(ack)IIC_SDA_BH1750=1;   //写应答信号
		else IIC_SDA_BH1750=0;          
    IIC_SCL_BH1750=1;          //拉高时钟线
    delay_us(2);        //延时
    IIC_SCL_BH1750=0;          //拉低时钟线
    delay_us(2);        //延时
}

/***************************************************************
** 功能：     产生IIC接收信号
** 参数：	  无参数
** 返回值：    无
****************************************************************/
uint8_t BH1750_RecvACK()
{
    uint8_t data;
	  SDA_IN();         //SDA设置为输入
    IIC_SCL_BH1750=1;        //拉高时钟线
    delay_us(2);      //延时
    data = READ_SDA_BH1750;  //读应答信号
    IIC_SCL_BH1750=0;        //拉低时钟线
    delay_us(2);      //延时
    return data;
}

/***************************************************************
** 功能：     向IIC总线发送一个字节数据
** 参数：	  dat：一字节数据
** 返回值：    无
****************************************************************/
void BH1750_SendByte(uint8_t dat)
{
    uint8_t i,bit;
		SDA_OUT();               //sda线输出
    for (i=0; i<8; i++)      //8位计数器
    {
	    bit=dat&0x80;
		if(bit) IIC_SDA_BH1750=1;
		else IIC_SDA_BH1750=0;
        dat <<= 1;           //移出数据的最高位
        IIC_SCL_BH1750=1;           //拉高时钟线
        delay_us(2);         //延时
        IIC_SCL_BH1750=0;           //拉低时钟线
        delay_us(2);         //延时
    }
    BH1750_RecvACK();
}

/***************************************************************
** 功能：     从IIC总线接收一个字节数据
** 参数：	  无参数
** 返回值：   dat：接收一字节数据
****************************************************************/
uint8_t BH1750_RecvByte()
{
    uint8_t i;
    uint8_t dat = 0;
		SDA_IN();                 //SDA设置为输入
    IIC_SDA_BH1750=1;              //使能内部上拉,准备读取数据,
    for (i=0; i<8; i++)     //8位计数器
    {
        dat <<= 1;
        IIC_SCL_BH1750=1;          //拉高时钟线
        delay_us(2);        //延时
		if(READ_SDA_BH1750) dat+=1;               
        IIC_SCL_BH1750=0;          //拉低时钟线
        delay_us(2);        //延时
    }
    return dat;
}

/***************************************************************
** 功能：     向bh1750写入命令
** 参数：	  无参数
** 返回值：   无
****************************************************************/
void Single_Write_BH1750(uint8_t REG_Address)
{
    BH1750_Start();                  //起始信号
    BH1750_SendByte(SlaveAddress);   //发送设备地址+写信号
    BH1750_SendByte(REG_Address);    //内部寄存器地址，
    BH1750_Stop();                   //发送停止信号
}

/***************************************************************
** 功能：     连续读出BH1750内部数据 
** 参数：	  无参数
** 返回值：   无
****************************************************************/
void Multiple_Read_BH1750(void)
{   
		uint8_t i;	
    BH1750_Start();                          //起始信号
    BH1750_SendByte(SlaveAddress+1);         //发送设备地址+读信号
	
		for (i=0; i<3; i++)                      //连续读取2个地址数据，存储中BUF
			{
					BUF[i] = BH1750_RecvByte();          //BUF[0]存储0x32地址中的数据
					if (i == 3)
					{
						 BH1750_SendACK(1);                //最后一个数据需要回NOACK
					}
					else
					{		
						BH1750_SendACK(0);                //回应ACK
				 }
		 }
    BH1750_Stop();                          //停止信号
}

/***************************************************************
** 功能：     初始化BH1750
** 参数：	  无参数
** 返回值：   无
****************************************************************/
void BH1750_Init(void)
{
   BH1750_PortInit();
   Single_Write_BH1750(0x01);  
//   ADDR = 0;  						//将ADDR位初始化拉低
}
void BH1750_DeInit(void)
{
  HAL_GPIO_DeInit(GPIOA,GPIO_PIN_8);
  HAL_GPIO_DeInit(GPIOC,GPIO_PIN_9);
}

/***************************************************************
** 功能：     读取光照度
** 参数：	  无参数
** 返回值：   data：返回光照度值
****************************************************************/
uint16_t Get_Bh_Value(void)
{  
	float temp;
	int dis_data;
	BUF[0]=0;BUF[1]=0;BUF[2]=0;BUF[3]=0;
	illumination_data=0;
    Single_Write_BH1750(0x01);			// power on
    Single_Write_BH1750(0x10);			// H- resolution mode
    Multiple_Read_BH1750();				// 连续读出数据，存储在BUF中
    dis_data=BUF[0];
    dis_data=(dis_data<<8)+BUF[1];		// 合成数据，即光照数据
    temp=(float)(dis_data/1.2);
		illumination_data=(int)temp;
    return illumination_data;//光照读取值
}
/**
 * @description: 读取光照度
 * @param {*}
 * @return {*}光照值
 */
uint16_t TaskBoard_BH(void)
{
    int i, j, b;
    int buf[10];
    for (i = 0; i < 10; i++)
    {
        buf[i] = Get_Bh_Value();
        delay_ms(1);
    }
    for (i = 0; i < 10; i++) //排序
    {
        for (j = 0; j < 10 - i - 1; j++)
        {
            if (buf[j] > buf[j + 1])
            {
                b = buf[j];
                buf[j] = buf[j + 1];
                buf[j + 1] = b;
            }
        }
    }
    //取中间数+偏差
    return (buf[5] + buf[6]) / 2;
}