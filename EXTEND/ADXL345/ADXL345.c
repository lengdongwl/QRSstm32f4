#include "ADXL345.h"
#include "MRIIC.h"
#include "math.h"
#include "uart.h"
#define DEVICE_ID       0x00 //����ID
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
#define ADXL_READ    0X3B //��ָ��
#define ADXL_WRITE   0X3A //дָ��
//0X0B TO OX1F Factory Reserved  
//���ALT ADDRESS��(12��)�ӵ�,ADXL��ַΪ0X53(���������λ).
//�����V3.3,��ADXL��ַΪ0X1D(���������λ).
//��Ϊ�������V3.3,����תΪ��д��ַ��,Ϊ0X3B��0X3A(�����GND,��Ϊ0XA7��0XA6)  
*/
#define ADXL_READ    0xA7 //��ָ��
#define ADXL_WRITE   0xA6 //дָ��
typedef enum
{
    X, //��x��ĽǶ�
    Y, //��y��ĽǶ�
    Z  //��z��ĽǶ�
}DIR_t;

/******************MR_IIC_**************/




//дADXL345�Ĵ���
//addr:�Ĵ�����ַ
//val:Ҫд���ֵ
//����ֵ:��
void Adxl345WRReg(u8 addr,u8 val) 
{
    MR_IIC_Start();  				 
    MR_IIC_Send_Byte(ADXL_WRITE);     	//����д����ָ��	 
    MR_IIC_Wait_Ack();	   
    MR_IIC_Send_Byte(addr);   			//���ͼĴ�����ַ
    MR_IIC_Wait_Ack(); 	 										  		   
    MR_IIC_Send_Byte(val);     		//����ֵ					   
    MR_IIC_Wait_Ack();  		    	   
    MR_IIC_Stop();						//����һ��ֹͣ���� 	   
}


//��ADXL345�Ĵ���
//addr:�Ĵ�����ַ
//����ֵ:������ֵ
u8 Adxl345ReadReg(u8 addr) 		
{
    u8 temp=0;		 
    MR_IIC_Start();  				 
    MR_IIC_Send_Byte(ADXL_WRITE);	//����д����ָ��	 
    MR_IIC_Wait_Ack();	  
    MR_IIC_Send_Byte(addr);   		//���ͼĴ�����ַ
    MR_IIC_Wait_Ack(); 	 										  		   
    MR_IIC_Start();  	 	   		//��������
    MR_IIC_Send_Byte(ADXL_READ);	//���Ͷ�����ָ��	 
    MR_IIC_Wait_Ack();	   
    temp=MR_IIC_ReadByte(1);		//��ȡһ���ֽ�,�������ٶ�,����NAK 	    	   
    MR_IIC_Stop();					//����һ��ֹͣ���� 	    
    return temp;				//���ض�����ֵ
} 

//@return 0.��ʼ��ʧ�� 1.�ɹ�
u8 ADXL345_Init()
{
    u8 d=0;

    MR_IIC_Init();

    d = Adxl345ReadReg(0);
    if(d == 0xE5)
    {
        Adxl345WRReg(DATA_FORMAT, 0x2B); //�жϵ͵�ƽ��Ч��13λȫ�ֱ���ģʽ��16g����
        Adxl345WRReg(POWER_CTL, 0x28); //����ʹ�ܣ�����ģʽ
        Adxl345WRReg(INT_ENABLE, 0x00); //��ʹ���ж�
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
    MR_IIC_Send_Byte(0x32); //�����׵�ַ
    MR_IIC_Wait_Ack();
    MR_IIC_Start();    
    MR_IIC_Send_Byte(ADXL_READ);
    MR_IIC_Wait_Ack();

    for(i = 0; i < 6; i++) //�ܹ�6��
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


//��ȡADXL��ƽ��ֵ
//x,y,z:��ȡtime�κ�ȡƽ��ֵ
void ADXL345_ReadReg_times(short *x, short *y, short *z, u8 times)
{
    u8 i = 0;
    short tx, ty, tz;

    *x = *y = *z = 0;

    for(i = 0; i < times; i++) //��ȡtimes��
    {
        ADXL345ReadReg_XYZ(&tx, &ty, &tz);

        *x += tx;
        *y += ty;
        *z += tz;       

        delay_ms(5);
    }

    *x /= times; //��ƽ��
    *y /= times;
    *z /= times;    
}


//����Ƕ�
short ADXL345_Get_Angle(float x, float y, float z, u8 dir) 
{
    float t;
    float res;

    switch(dir)
    {
        case X:
            t = x / sqrt(y*y + z*z);
            res = atan(t); //�������
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

    return (res * 180 / 3.14); //����Ƕ�
}


/*
void ADXL345_main()
{
    short x, y, z;
    short rx, ry, rz;

    Adxl345ReadReg_times(&x, &y, &z, 10); //����x��y��z������ٶ�ֵ
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


