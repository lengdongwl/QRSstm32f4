#include "ADXL345.h"
#include "MRIIC.h"
#include "math.h"
#include "uart.h"
#define DEVICE_ID       0x00 //器件ID
#define THRESH_TAP      0X1D
#define OFSX            0X1E
#define OFSY            0X1F
#define OFSZ            0X20
#define DUR             0X21
#define Latent          0X22
#define Window          0X23 
#define THRESH_ACK      0X24
#define THRESH_INACT    0X25 
#define TIME_INACT      0X26
#define ACT_INACT_CTL   0X27     
#define THRESH_FF       0X28    
#define TIME_FF     0X29 
#define TAP_AXES        0X2A  
#define ACT_TAP_STATUS  0X2B 
#define BW_RATE     0X2C 
#define POWER_CTL       0X2D 

#define INT_ENABLE      0X2E
#define INT_MAP     0X2F
#define INT_SOURCE      0X30
#define DATA_FORMAT 0X31
#define DATA_X0     0X32
#define DATA_X1     0X33
#define DATA_Y0     0X34
#define DATA_Y1     0X35
#define DATA_Z0     0X36
#define DATA_Z1     0X37
#define FIFO_CTL        0X38
#define FIFO_STATUS 0X39

/*
#define ADXL_READ    0X3B //读指令
#define ADXL_WRITE   0X3A //写指令
//0X0B TO OX1F Factory Reserved  
//如果ALT ADDRESS脚(12脚)接地,ADXL地址为0X53(不包含最低位).
//如果接V3.3,则ADXL地址为0X1D(不包含最低位).
//因为开发板接V3.3,所以转为读写地址后,为0X3B和0X3A(如果接GND,则为0XA7和0XA6)  
*/
#define ADXL_READ    0xA7 //读指令
#define ADXL_WRITE   0xA6 //写指令
typedef enum
{
    X, //与x轴的角度
    Y, //与y轴的角度
    Z  //与z轴的角度
}DIR_t;

/******************MR_IIC_**************/




//写ADXL345寄存器
//addr:寄存器地址
//val:要写入的值
//返回值:无
void Adxl345WRReg(u8 addr,u8 val) 
{
    MR_IIC_Start();  				 
    MR_IIC_Send_Byte(ADXL_WRITE);     	//发送写器件指令	 
    MR_IIC_Wait_Ack();	   
    MR_IIC_Send_Byte(addr);   			//发送寄存器地址
    MR_IIC_Wait_Ack(); 	 										  		   
    MR_IIC_Send_Byte(val);     		//发送值					   
    MR_IIC_Wait_Ack();  		    	   
    MR_IIC_Stop();						//产生一个停止条件 	   
}


//读ADXL345寄存器
//addr:寄存器地址
//返回值:读到的值
u8 Adxl345ReadReg(u8 addr) 		
{
    u8 temp=0;		 
    MR_IIC_Start();  				 
    MR_IIC_Send_Byte(ADXL_WRITE);	//发送写器件指令	 
    MR_IIC_Wait_Ack();	  
    MR_IIC_Send_Byte(addr);   		//发送寄存器地址
    MR_IIC_Wait_Ack(); 	 										  		   
    MR_IIC_Start();  	 	   		//重新启动
    MR_IIC_Send_Byte(ADXL_READ);	//发送读器件指令	 
    MR_IIC_Wait_Ack();	   
    temp=MR_IIC_ReadByte(1);		//读取一个字节,不继续再读,发送NAK 	    	   
    MR_IIC_Stop();					//产生一个停止条件 	    
    return temp;				//返回读到的值
} 

//@return 0.初始化失败 1.成功
u8 ADXL345_Init()
{
    u8 d=0;

    MR_IIC_Init();

    d = Adxl345ReadReg(0);
    if(d == 0xE5)
    {
        Adxl345WRReg(DATA_FORMAT, 0x2B); //中断低电平有效，13位全分辨率模式，16g量程
        Adxl345WRReg(POWER_CTL, 0x28); //链接使能，测量模式
        Adxl345WRReg(INT_ENABLE, 0x00); //不使用中断
        Adxl345WRReg(OFSX, 0x00);
        Adxl345WRReg(OFSY, 0x00);
        Adxl345WRReg(OFSZ, 0x00);
        return 1;
    } 
    return 0;
     
}


void ADXL345ReadReg_XYZ(short *x, short *y, short *z)
{
    u8 i = 0;
    u16 buf[6];

    MR_IIC_Start();    
    MR_IIC_Send_Byte(ADXL_WRITE);
    MR_IIC_Wait_Ack();
    MR_IIC_Send_Byte(0x32); //数据首地址
    MR_IIC_Wait_Ack();
    MR_IIC_Start();    
    MR_IIC_Send_Byte(ADXL_READ);
    MR_IIC_Wait_Ack();

    for(i = 0; i < 6; i++) //总共6个
    {
        buf[i] = MR_IIC_ReadByte2();
//      printf("I2C_recv_byte [%d] %d\r\n", i, buf[i]);
        if(i == 5)
        {
            MR_IIC_NAck();
        }
        else
        {
            MR_IIC_Ack();
        }
    }

    MR_IIC_Stop();

    *x = (short)((buf[1]<<8) + buf[0]);         
    *y = (short)((buf[3]<<8) + buf[2]);         
    *z = (short)((buf[5]<<8) + buf[4]);
}


//读取ADXL的平均值
//x,y,z:读取time次后取平均值
void ADXL345_ReadReg_times(short *x, short *y, short *z, u8 times)
{
    u8 i = 0;
    short tx, ty, tz;

    *x = *y = *z = 0;

    for(i = 0; i < times; i++) //读取times次
    {
        ADXL345ReadReg_XYZ(&tx, &ty, &tz);

        *x += tx;
        *y += ty;
        *z += tz;       

        delay_ms(5);
    }

    *x /= times; //求平均
    *y /= times;
    *z /= times;    
}


//计算角度
short ADXL345_Get_Angle(float x, float y, float z, u8 dir) 
{
    float t;
    float res;

    switch(dir)
    {
        case X:
            t = x / sqrt(y*y + z*z);
            res = atan(t); //求出弧度
            break;
        case Y:
            t = y / sqrt(x*x + z*z);
            res = atan(t);
            break;
        case Z:
            t = sqrt(x*x + y*y) / z;
            res = atan(t);
            break;
        default:
            break;
    }

    return (res * 180 / 3.14); //求出角度
}


/*
void ADXL345_main()
{
    short x, y, z;
    short rx, ry, rz;

    Adxl345ReadReg_times(&x, &y, &z, 10); //读出x，y，z方向加速度值
    printf("x %d\n", x);
    printf("y %d\n", y);
    printf("z %d\n", z);

    rx = ADXL345_get_result(x, y, z, X);
    ry = ADXL345_get_result(x, y, z, Y);
    rz = ADXL345_get_result(x, y, z, Z);
    printf("rx %d\n", rx);
    printf("ry %d\n", ry);
    printf("rz %d\n", rz);
}*/


