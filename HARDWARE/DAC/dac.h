/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-12-16 09:40:36
 */

#ifndef _DAC_H
#define _DAC_H
#include "sys.h"
typedef enum _waveType
{

    WAVE_SINE,     // ���Ҳ�
    WAVE_SQUARE,   // ����
    WAVE_TRIANGLE, // ���ǲ�
    WAVE_RAMP,     // ��ݲ�
    WAVE_T,        // ���β�
    WAVE_Noise,    // ������
    WAVE_HALF      // �벨
} waveType;

extern DAC_HandleTypeDef DAC1_Handler;   //DAC���
u16 calc_OutHZarr(int CLK_MHz, int KHz); //Ƶ�����ü���

void Sine_Wave(uint16_t Vpp_Voltage);                       //���Ҳ�
void Triangular_Wave(uint16_t Vpp_Voltage);                 //���ǲ�
void Square_Wave(uint16_t Vpp_Voltage, uint8_t Duty_Cycle); //����
//void wave_gen(waveType type, u16 len, u16 vpp);             //�������ɺ���
void wave_gen(waveType type, u16 vpp); //�������ɺ���
void DAC1_Init(void);                  //DAC��ʼ��
void DAC1_Set_Vol(u16 vol);            //DAC���õ�ѹ
void DAC_Init(void);
void DAC_DeInit(void);
void DAC_SetHz(int KHz); //����DAC���Ƶ��
void Stop_Wave(void);
void OutPut_Waveform(
    uint8_t Waveform,     //����
    uint16_t Frequency,   //Ƶ��
    uint16_t VPP_Voltage, //���ֵ
    uint8_t Duty_Cycle    //ռ�ձ�
);                        //�������
#endif
