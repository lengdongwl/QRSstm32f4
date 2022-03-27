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

    WAVE_SINE,     // 正弦波
    WAVE_SQUARE,   // 方波
    WAVE_TRIANGLE, // 三角波
    WAVE_RAMP,     // 锯齿波
    WAVE_T,        // 梯形波
    WAVE_Noise,    // 噪声波
    WAVE_HALF      // 半波
} waveType;

extern DAC_HandleTypeDef DAC1_Handler;   //DAC句柄
u16 calc_OutHZarr(int CLK_MHz, int KHz); //频率设置计算

void Sine_Wave(uint16_t Vpp_Voltage);                       //正弦波
void Triangular_Wave(uint16_t Vpp_Voltage);                 //三角波
void Square_Wave(uint16_t Vpp_Voltage, uint8_t Duty_Cycle); //方波
//void wave_gen(waveType type, u16 len, u16 vpp);             //波形生成函数
void wave_gen(waveType type, u16 vpp); //波形生成函数
void DAC1_Init(void);                  //DAC初始化
void DAC1_Set_Vol(u16 vol);            //DAC设置电压
void DAC_Init(void);
void DAC_DeInit(void);
void DAC_SetHz(int KHz); //更改DAC输出频率
void Stop_Wave(void);
void OutPut_Waveform(
    uint8_t Waveform,     //波形
    uint16_t Frequency,   //频率
    uint16_t VPP_Voltage, //峰峰值
    uint8_t Duty_Cycle    //占空比
);                        //输出波形
#endif
