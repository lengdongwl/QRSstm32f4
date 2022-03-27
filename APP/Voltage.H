

#ifndef _VOLTAGE_H
#define _VOLTAGE_H
#include "sys.h"
#define Buf_ADC_len 888 //ADC采集点数
void Oscilloscope(void); //示波器

uint8_t RMS(float *Input, uint16_t Size);				//波形识别（RMS均方根） 1：矩形波 2：三角波 3：正弦波
float Get_Vpp_Voltage(float *Input);					//峰峰值获取
uint16_t Get_Period(uint8_t *input, uint16_t size);		//获取周期
uint16_t Get_Duty_Cycle(uint8_t *input, uint16_t size); //获取占空比
void Displacement(uint32_t *Input_Data);				//波形数据位移
void Update_Display(uint32_t *Input_Data);				//OLED显示波形数据

/*

		模数器件的精度指标是用积分非线性度（Interger NonLiner）即INL值来表示。
	也有的器件手册用 Linearity error 来表示。他表示了ADC器件在所有的数值点
	上对应的模拟值，和真实值之间误差最大的那一点的误差值。也就是，输出数值
	偏离线性最大的距离，单位是LSB（即最低位所表示的量）。
		ADC相邻两刻度之间最大的差异就叫差分非线性值（Differencial NonLiner）。
	DNL值如果大于1，那么这个ADC甚至不能保证是单调的，输入电压增大，在某个点
	数值反而会减小。这种现象在SAR（逐位比较）型ADC中很常见。
（1）INL(Interger NonLinear,Linearity error)精度。理解为单值数据误差，对
		应该点模拟数据由于元器件及结构造成的不能精确测量产生的误差。
（2）DNL(Differential NonLinear)差分非线性值。理解为刻度间的差值，即对每
   个模拟数据按点量化，由于量化产生的误差
*/

#endif
