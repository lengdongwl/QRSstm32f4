#ifndef _HMC5883L_H
#define _HMC5883L_H
#include "sys.h"
/*
#define registerA 0x00
#define registerB 0x01　
#define registerMODE 0x02　
#define DATAOUTX_MSB 0x03　
#define DATAOUTX_LSB 0x04　
#define DATAOUTZ_MSB 0x05　
#define DATAOUTZ_LSB 0x06　
#define DATAOUTY_MSB 0x07
#define DATAOUTY_LSB 0x08　
#define registerSTATE 0x09

10　识别寄存器A　//r
11　识别寄存器B　//r
12　识别寄存器C　//r

*/
extern int x,y,z;


#define HMC5883L_SDA_IN()  {GPIOD->MODER&=~(3<<(2*2));GPIOD->MODER|=0<<2*2;}	//PB9输入模式
#define HMC5883L_SDA_OUT() {GPIOD->MODER&=~(3<<(2*2));GPIOD->MODER|=1<<2*2;} //PB9输出模式
//IO操作
#define HMC5883L_IIC_SCL   PCout(12) //SCL
#define HMC5883L_IIC_SDA   PDout(2) //SDA
#define HMC5883L_READ_SDA  PDin(2)  //输入SDA


void HMC5883_Init(void);
double Read_HMC5883(void);
void Multiple_Read_HMC5883(void);
uint8_t Single_Read_HMC5883(uint8_t REG_Address);
uint32_t HMC5883_Read_X(void);
uint32_t HMC5883_Read_Y(void);
uint32_t HMC5883_Read_Z(void);
#endif

