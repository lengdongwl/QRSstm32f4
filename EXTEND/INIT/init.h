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
#define INIT_4P_HX711 1 //HX711���ش�����
#define INIT_4P_MPU6050 2//��̬������
#define INIT_4P_MLX90614 3//������´�����
#define INIT_4P_RC522RFID 4//��������
#define INIT_4P_HMC5883 5// ��������
#define INIT_4P_ADXL345 9//���ٶȼ��


#define INIT_3P_DHT11 6//��ʪ�ȴ�����
#define INIT_3P_W2812 7//��ɫ��
#define	INIT_3P_GP2Y0A21 8 //��չ�˿�ģ�鴦������
*/
typedef enum _Init_Type
{
    INIT_4P_HX711=1,  //HX711���ش�����
    INIT_4P_MPU6050, //��̬������
    INIT_4P_MLX90614, //������´�����
    INIT_4P_RC522RFID, //��������
    INIT_4P_HMC5883, // ��������
    INIT_3P_DHT11, //��ʪ�ȴ�����
    INIT_3P_W2812, //��ɫ��
	INIT_3P_GP2Y0A21,  //��չ�˿�ģ�鴦������
    INIT_4P_ADXL345, //���ٶȼ��
}Init_Type;

void Extend_Init(void);
void Extend_Init2(int n);
#endif

