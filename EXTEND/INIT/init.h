/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:57
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-15 15:35:36
 */

#ifndef _INIT_H
#define _INIT_H
#include "sys.h"

/*
#define INIT_4P_HX711 1 //HX711称重传感器
#define INIT_4P_MPU6050 2//姿态传感器
#define INIT_4P_MLX90614 3//红外测温传感器
#define INIT_4P_RC522RFID 4//读卡感器
#define INIT_4P_HMC5883 5// 电子罗盘
#define INIT_4P_ADXL345 9//加速度检测


#define INIT_3P_DHT11 6//温湿度传感器
#define INIT_3P_W2812 7//三色灯
#define	INIT_3P_GP2Y0A21 8 //扩展端口模块处理函数库
*/
typedef enum _Init_Type
{
    INIT_4P_HX711=1,  //HX711称重传感器
    INIT_4P_MPU6050, //姿态传感器
    INIT_4P_MLX90614, //红外测温传感器
    INIT_4P_RC522RFID, //读卡感器
    INIT_4P_HMC5883, // 电子罗盘
    INIT_3P_DHT11, //温湿度传感器
    INIT_3P_W2812, //三色灯
	INIT_3P_GP2Y0A21,  //扩展端口模块处理函数库
    INIT_4P_ADXL345, //加速度检测
}Init_Type;

void Extend_Init(void);
void Extend_Init2(int n);
#endif

