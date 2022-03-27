/**
  ******************************************************************************
  * @file    Voltage.c
  * @author  Xie、Xu
  * @version V1
  * @date    2021年11月24日
  * @brief   ADC电压处理应用函数库
					功能：
						 电压检测
						 频率检测
						 波形滤波
						 波形识别
						 波形显示
  ******************************************************************************
**/	
#include "Voltage.h"
#include "math.h"
#include "stdio.h"
#include "adc.h"
#include "oled.h"

float TEMP_Data[Buf_ADC_len];	//运行缓存空间
char 	ADC_Fifo[Buf_ADC_len];	//临时缓存空间


/**
  * @brief  波形识别（RMS均方根）
  * @param  float* Input ：波形数据
  * @retval	None 
  * @note		识别结果 1：矩形波 2：三角波 3：正弦波
  */
uint8_t RMS(float* Input,uint16_t Size)
{
	float RMS_Data  = 0;			//RMS数据
	float Max_Data  = 0;			//最大值
	float Min_Data  = 0;			//最小值
	float Mid_Value = 0;			//中间值	
	for (uint16_t i = 0; i < Size; i++)
	{
		if (Max_Data < Input[i])
		{
			Max_Data = Input[i];
		}
	}
	Min_Data=Max_Data;
	for (uint16_t i = 0; i < Size; i++)
	{
		if (Min_Data > Input[i])
		{
			Min_Data = Input[i];
		}
	}
	
	Mid_Value = (Max_Data-Min_Data) / 2;
	
	for (uint16_t z = 0; z < Size; z++)
	{
		RMS_Data += fabsf((Input[z]-Mid_Value))* fabsf((Input[z] - Mid_Value));
	}
	RMS_Data  /=  Size;
	RMS_Data = sqrt(RMS_Data);
	RMS_Data =  Max_Data/RMS_Data/2;
//printf("%0.4f \r\n",RMS_Data);

	if(RMS_Data>(float)0.9 && RMS_Data<(float)1.2){//矩形波
		return 1;		
	}else if(RMS_Data>(float)1.2 && RMS_Data<(float)1.65){//正弦波
		return 3;	
	}else if(RMS_Data>(float)1.65 && RMS_Data<(float)1.8){//三角波	
		return 2;	
	}
	return 0;
}

/**
  * @brief  峰峰值获取
  * @param  float* Input ：波形数据
  * @retval	None 
  * @note		（取前最大20位数据取中位）
  */
float Get_Vpp_Voltage(float* Input)
{
	float Vpp[20] = {0};
	int location= 0;

	for (uint16_t x = 0; x < Buf_ADC_len; x++)
	{
		TEMP_Data[x] = Input[x];
	}
	for (uint16_t x1 = 0; x1 < 20; x1++)
	{
		location = 0;
		for (uint16_t x2 = 0; x2 < Buf_ADC_len; x2++)
		{
			if (Vpp[x1] < TEMP_Data[x2])
			{
				Vpp[x1] = TEMP_Data[x2];
				location = x2;
			}
		}
		TEMP_Data[location] = 0;
	}
	float n = sqrt(((Vpp[9] * Vpp[9]) + (Vpp[10] * Vpp[10])) / 2);
	return n;
}

/**
  * @brief  获取周期
	* @param  input:波形的数组 size:数组的长度
  * @retval	周期的中位数 
  * @note		
  */
uint16_t Get_Period(uint8_t *input,uint16_t size)
{
	uint16_t wz[4] = { 0,0,0,0 };
	uint16_t Period[] = { 0 ,0 ,0 };	//周期缓存
	uint16_t Temp = 0;								//排序临时变量
	for (uint16_t z = 0; z < 3; z++)
	{
		for (uint16_t i = wz[2]-2; i < size; i++)
		{
			if (input[i] > 1)
			{
				return 0;
			}
			if ((input[i] - input[i + 1]) == -1)
			{
				wz[0] = i; 
				break;
			}
		}
		for (uint16_t p = wz[0] + 2; p < size; p++)
		{
			if (input[p] > 1)
			{
				return 0;
			}
			if ((input[p] - input[p + 1]) == 1)
			{
				wz[1] = p;
				break;
			}
		}
		for (uint16_t k = wz[1] + 2; k < size; k++)
		{
			if (input[k] > 1)
			{
				return 0;
			}
			if ((input[k] - input[k + 1]) == -1)
			{
				wz[2] = k;
				break;
			}
		}
		Period[z] = (wz[2] - wz[0]);
	}
	for (uint8_t i = 1; i < 3; i++)    //变量i代表比较的趟数
	{
		for (uint8_t j = 0; j < 2; j++)    //变最j代表每趟两两比较的次数
		{
			if (Period[j] > Period[j + 1])
			{
				Temp = Period[j];    //利用中间变量实现两值互换
				Period[j] = Period[j + 1];
				Period[j + 1] = Temp;
			}
		}
	}	
	if (Period[1] != 0)
	{
		return Period[1];
	}
	else
	{
		return Period[2];
	}
}

/**
  * @brief  获取占空比
	* @param  input:波形的数组 size:数组的长度
  * @retval	占空比的中位数 
  * @note		
  */
uint16_t Get_Duty_Cycle(uint8_t* input, uint16_t size)
{
	uint16_t wz[4] = { 0,0,0,0 };
	uint16_t period[] = { 0 ,0 ,0 };
	uint16_t t = 0;
	for (uint16_t z = 0; z < 3; z++)
	{
		for (uint16_t i = wz[1] - 2; i < size; i++)
		{
			if (input[i] > 1)
			{
				return 0;
			}
			if ((input[i] - input[i + 1]) == -1)
			{
				wz[0] = i;
				break;
			}
		}
		for (uint16_t p = wz[0] + 2; p < size; p++)
		{
			if (input[p] > 1)
			{
				return 0;
			}
			if ((input[p] - input[p + 1]) == 1)
			{
				wz[1] = p;
				break;
			}
		}
		period[z] = (wz[1] - wz[0]);
	}
	for (uint8_t i = 1; i < 3; i++)    //变量i代表比较的趟数
	{
		for (uint8_t j = 0; j < 2; j++)    //变最j代表每趟两两比较的次数
		{
			if (period[j] > period[j + 1])
			{
				t = period[j];    //利用中间变量实现两值互换
				period[j] = period[j + 1];
				period[j + 1] = t;
			}
		}
	}
	if (period[1] != 0)
	{
		return period[1];
	}
	else
	{
		return period[2];
	}
}

/**
  * @brief  波形数据位移
	* @param  图像缓存数据
  * @retval	None 
  * @note		None
  */
void Displacement(uint32_t *Input_Data)
{
	for(uint8_t i=0;i<128;i++){
		Input_Data[i]=Input_Data[i+1];
	}
}	

//占空比 四舍五入
uint16_t Rounding(float In_Data) {
	In_Data *= 10;
	if (((uint16_t)In_Data % 10) >= 5) {
			return (uint16_t)(In_Data / 10 + 1);
	}else {
			return (uint16_t)(In_Data / 10);
	}
}

/**
  * @brief  显示波形数据
	* @param  图像缓存数据
  * @retval	None 
  * @note		None
  */
void Update_Display(uint32_t *Input_Data)
{
	uint8_t One_Buffer=0;		//临时缓存1
	uint8_t Two_Buffer=0;		//临时缓存2
	uint8_t Three_Buffer=0;	//临时缓存3
	uint8_t Four_Buffer=0;	//临时缓存3	
		
	for(uint8_t i=0;i<128;i++){			
		/*数据拆分*/
		One_Buffer =	Input_Data[i]>>24&0xFF;
		Two_Buffer =	Input_Data[i]>>16&0xFF;
		Three_Buffer =Input_Data[i]>>8&0xFF;
		Four_Buffer =	Input_Data[i]&0xFF;
			
		/*数据显示*/
		OLED_Set_Pos(i,7);
		OLED_WR_Byte(One_Buffer,OLED_DATA);		
		OLED_Set_Pos(i,6);
		OLED_WR_Byte(Two_Buffer,OLED_DATA);		
		OLED_Set_Pos(i,5);
		OLED_WR_Byte(Three_Buffer,OLED_DATA);		
		OLED_Set_Pos(i,4);
		OLED_WR_Byte(Four_Buffer,OLED_DATA);
	}
}

/**
  * @brief  示波器
  * @param  None
  * @retval	None 
  * @note		None
  */
/*电压波形图*/
float Voltage_Waveform[888];
/*峰峰值电压*/
float VPP_Voltage=0; 
/*周期值*/
uint16_t Period=0;
/*图像缓存*/
uint32_t Waveform[256];	
/*占空比值*/
uint16_t Duty_Cycle=0;
/*频率值*/
float frequency =0;
void Oscilloscope(void)
{
					/*波形采样*/
					for(uint16_t I=0;I<888;I++){
						Voltage_Waveform[I]=Get_Adc(ADC_CHANNEL_10);
					}
					
					/*波形滤波*/
					for(uint16_t i=0;i<888-1;i++){
						Voltage_Waveform[i]=(Voltage_Waveform[i]+Voltage_Waveform[i+1])/2;
					}
										
					/*打印输出*/
//					for(uint16_t I=0;I<888;I++){
//						printf("%0.3f \r\n",Voltage_Waveform[I]*(3.3/4096));
//					}		

					/*显示波形*/
					for(uint16_t I=0;I<256*2;I++){
						Waveform[I/2]=1<<((32-(uint16_t)(Voltage_Waveform[I]/128)-1));
					}
					
					Update_Display(Waveform);	//显示ADC图像
					/*电压换算*/
					for(uint16_t I=0;I<888;I++){
						Voltage_Waveform[I]=Voltage_Waveform[I]*(3.3/4096);
					}	
					
					/*峰峰值获取*/
					VPP_Voltage=Get_Vpp_Voltage(Voltage_Waveform);

					/*波形识别*/
					OLED_Show_Str(0,0,(char*)"                ",16);
					OLED_Show_Str(0,2,(char*)"                ",16);
					switch(RMS(Voltage_Waveform,888))
					{
						case 0:{
							sprintf(ADC_Fifo,"       %0.2fmV",VPP_Voltage*1000);
							OLED_Show_Str(0,0,ADC_Fifo,16);
							break;
						}
						case 1:{
							VPP_Voltage+=VPP_Voltage*(float)0.014;//误差校正
							sprintf(ADC_Fifo,"矩形波 %0.2fmV",VPP_Voltage*1000);
							OLED_Show_Str(0,0,ADC_Fifo,16);
							break;
						}
						case 2:{
							VPP_Voltage+=VPP_Voltage*(float)0.034;//误差校正
							sprintf(ADC_Fifo,"三角波 %0.2fmV",VPP_Voltage*1000);
							OLED_Show_Str(0,0,ADC_Fifo,16);
							break;
						}
						case 3:{
							VPP_Voltage+=VPP_Voltage*(float)0.017;//误差校正
							sprintf(ADC_Fifo,"正弦波 %0.2fmV",VPP_Voltage*1000);
							OLED_Show_Str(0,0,ADC_Fifo,16);
							break;
						}
					}		
					/*电压换算*/
					for(uint16_t I=0;I<888;I++){
						if(Voltage_Waveform[I]>VPP_Voltage/2){
							ADC_Fifo[I]=1;
						}else{
							ADC_Fifo[I]=0;
						}
					}					
					
					/*频率计算*/
					Period=Get_Period((uint8_t*)ADC_Fifo,888);
					if(Period!=0){
						frequency=1/(Period*((double)2.65/888));
					}	
					
					sprintf(ADC_Fifo,"频率:%0.2fHZ",frequency*1000);
					OLED_Show_Str(0,2,ADC_Fifo,16);	
					
					/*电压换算*/
					for(uint16_t I=0;I<888;I++){
						if(Voltage_Waveform[I]>VPP_Voltage/2){
							ADC_Fifo[I]=1;
						}else{
							ADC_Fifo[I]=0;
						}
					}					
					
					/*占空比获取*/
//					Duty_Cycle=Get_Duty_Cycle((uint8_t*)ADC_Fifo,888);
//					printf("ZKB:%d\r\n",Rounding(((float)Duty_Cycle/Period)*100)+1);
}

