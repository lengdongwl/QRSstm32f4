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
  * @date    2021��10��11��
  * @brief   ��չ�˿�ģ�鴦������
	
						�˿ڶ��壺

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
/* ��չ�˿�4Pin
		0:��
		1:HX711���ش�����
		2:MPU6050��̬������
		3:MLX90614������´�����
		4:RC522RFID��������
		5:HMC5883��������
		6.ADXL345���ٶȼ��
*/
#define EXTEND_4PIN  5


/* ��չ�˿�3Pin
		0:��
		1:DHT11��ʪ�ȴ�����
		2:W2812��ɫ��
		3:�����ഫ����
*/
#define EXTEND_3PIN  2 


/**
  * @brief  ��չ�˿��豸��ʼ��
  * @param  None
  * @retval	None 
  * @note		None
  */
void Extend_Init(void)
{
	
	//��չ�˿�4Pin
	#if EXTEND_4PIN==1
		HX711_Init();				//���ش�����
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
	
	
	//��չ�˿�3Pin
	#if EXTEND_3PIN==1
		DHT11_Init();		//DHT11��ʼ��
	#elif EXTEND_3PIN==2
		SK6812_Init();	//SK6812��ʼ��
	#elif EXTEND_3PIN==3
		GP2Y0A21_Init();//GP2Y0A21��ʼ��

	#endif
}
void Extend_Init2(int n)
{
	
	//��չ�˿�4Pin
	if (n==INIT_4P_HX711)
		HX711_Init();				//���ش�����
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

	
	
	//��չ�˿�3Pin
	if (n==INIT_3P_DHT11)
		DHT11_Init();		//DHT11��ʼ��
	else if (n==INIT_3P_W2812)
		SK6812_Init();	//SK6812��ʼ��
	else if (n==INIT_3P_GP2Y0A21)
		GP2Y0A21_Init();//GP2Y0A21��ʼ��

}

