/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:57
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-12 16:19:08
 */
/**
  ******************************************************************************
  * @file    init.c
  * @author  xie
  * @version V1
  * @date    2021年10月11日
  * @brief   扩展端口模块处理函数库
	
						端口定义：

	******************************************************************************
**/	
#include "init.h"
#include "dht11.h"
#include "SK6812.h"
#include "GP2Y0A21.h"
#include "HX711.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "MLX90614.h"
#include "rc522.h"
#include "HMC5883L.h"
#include "ADXL345.h"
/* 扩展端口4Pin
		0:无
		1:HX711称重传感器
		2:MPU6050姿态传感器
		3:MLX90614红外测温传感器
		4:RC522RFID读卡感器
		5:HMC5883电子罗盘
		6.ADXL345加速度检测
*/
#define EXTEND_4PIN  5


/* 扩展端口3Pin
		0:无
		1:DHT11温湿度传感器
		2:W2812三色灯
		3:红外测距传感器
*/
#define EXTEND_3PIN  2 


/**
  * @brief  扩展端口设备初始化
  * @param  None
  * @retval	None 
  * @note		None
  */
void Extend_Init(void)
{
	
	//扩展端口4Pin
	#if EXTEND_4PIN==1
		HX711_Init();				//称重传感器
	#elif EXTEND_4PIN==2
		while(MPU_Init());	//
		while(mpu_dmp_init());
	#elif EXTEND_4PIN==3
		MLX90614_Init();
	#elif EXTEND_4PIN==4
		RC522_Init();
	#elif EXTEND_4PIN==5
		HMC5883_Init();
	#elif EXTEND_4PIN==6
		ADXL345_Init();	
	#endif
	
	
	//扩展端口3Pin
	#if EXTEND_3PIN==1
		DHT11_Init();		//DHT11初始化
	#elif EXTEND_3PIN==2
		SK6812_Init();	//SK6812初始化
	#elif EXTEND_3PIN==3
		GP2Y0A21_Init();//GP2Y0A21初始化

	#endif
}
void Extend_Init2(int n)
{
	
	//扩展端口4Pin
	if (n==INIT_4P_HX711)
		HX711_Init();				//称重传感器
	else if (n==INIT_4P_MPU6050)
	{
		while(MPU_Init());	
		while(mpu_dmp_init());
	}
	else if (n==INIT_4P_MLX90614)
	{
		MLX90614_Init();
	}
	else if (n==INIT_4P_RC522RFID)
	{
		RC522_Init();
	}
	else if (n==INIT_4P_HMC5883)
	{
		HMC5883_Init();
	}
	else if (n==INIT_4P_ADXL345)
	{
		ADXL345_Init();
	}

	
	
	//扩展端口3Pin
	if (n==INIT_3P_DHT11)
		DHT11_Init();		//DHT11初始化
	else if (n==INIT_3P_W2812)
		SK6812_Init();	//SK6812初始化
	else if (n==INIT_3P_GP2Y0A21)
		GP2Y0A21_Init();//GP2Y0A21初始化

}

