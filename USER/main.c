
#include "stdio.h"
#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "w25qxx.h"
#include "24cxx.h"
#include "oled.h"
#include "rgb.h"
#include "smg.h"
#include "bee.h"
#include "bh1750.h"
#include "DS18B20.h"
#include "ultrasonic.h"
#include "time.h"
#include "key.h"
#include "ec11.h"
#include "uart.h"
#include "dac.h"
#include "dma.h"
#include "adc.h"
#include "init.h"
#include "dht11.h"
#include "GP2Y0A21.h"
#include "HX711.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "SK6812.h"
#include "MLX90614.h"
#include "rc522.h"
#include "HMC5883L.h"
#include "math.h"
#include "ADXL345.h"
#include "MYGUI_FUNC.h"
#include "MYGUI.h"
#include "RNG.h"

uint8_t Frequency = 81; //频率
uint16_t Voltage = 0;	//电压值
uint16_t Temp_Data = 0; //温度
uint16_t Humi_Data = 0; //湿度

float pitch, roll, yaw;	   //欧拉角
short aacx, aacy, aacz;	   //加速度传感器原始数据
short gyrox, gyroy, gyroz; //陀螺仪原始数据
short temp;				   //温度
uint8_t voiceNum = 0;	   //语音识别编号

u8 mode_show = 0; //oled显示按键切换所在的模式



void Hard_Init(void)
{
	/*系统初始化*/
	HAL_Init();						 //初始化HAL库
	Stm32_Clock_Init(336, 16, 2, 7); //设置时钟,168Mhz
	delay_init(168);				 //延时函数初始化
	/*设备初始化*/
	LED_Init();	 //LED初始化
	KEY_Init();	 //按键初始化
	BEE_Init();	 //蜂鸣器初始化
	OLED_Init(); //OLED初始化
	BH1750_Init();
	DS18B20_Init(); //温度传感器
	Ultrasonic_Init(); //超声波测距初始化
				 //UART1_Init(115200);//串口通讯
				 //	W25QXX_Init();												//W25Q32初始化
				 //	AT24CXX_Init();												//AT24C02初始化

	//	SMG_Init();														//数码管初始化
	//	BEE_Init();														//蜂鸣器初始化
	//	BH1750_Init();												//BH1750初始化
	//	DS18B20_Init();												//DS18B20初始化
	//	Ultrasonic_Init();										//超声波测距初始化
	//	EC11_Init();
	//	UART1_Init(115200);										//串口1初始化（调试）
	//	UART2_Init(115200);										//串口2初始化（语音）
	//	DAC_Init();															//DAC初始化
	//	MY_ADC_Init();													//ADC初始化

	//Extend_Init(); //扩展端口设备初始化
}

void oled_show(u8 mode)
{
	
	EC11_DeInit(); //每次切换 EC11删除初始化
	if (mode==255)
	{
		mode=mode_show=11;
	}
	else if (mode>11)
	{
		mode=mode_show=0;
	}
	OLED_Clear();
	switch (mode)
	{
	case 0:
		OLED_Show_Str(0,0,"百科荣创",32);
		break;
	case 1:
		OLED_Show_Str(0, 0, "BEEP", 16);
		break;
	case 2:
		OLED_Show_Str(0,0,"OLED图片文字",16);
		break;
	case 3:
		OLED_Show_Str(0, 0, "三色灯", 16); //三色灯LED1
		break;
	case 4:
		OLED_Show_Str(0, 0, "传感器", 16); //光照度
		break;
	case 5:
		OLED_Show_Str(0, 0, "语音识别", 16); //语音识别
		break;
	case 6:
		OLED_Show_Str(0, 0, "数码管", 16); //数码管
		break;
	case 7:
		OLED_Show_Str(0, 0, "EC11", 16); //EC11
		break;
	case 8:
		OLED_Show_Str(0, 0, "扩展模块PIN3", 16); //扩展接口3pin
		break;
	case 9:
		OLED_Show_Str(0, 0, "扩展模块PIN4", 16); //扩展接口4pin
		break;
	case 10:
		OLED_Show_Str(0, 0, "ADC", 16); //ADC采集波形
		break;
    case 11:
		OLED_Show_Str(0, 0, "DAC", 16); //DAC输出波形
		break;		
	default:
		OLED_ShowInt32Num(0,0,mode,3,16);
		break;
	}
}

void run(void)
{

	switch (mode_show)
	{
	case 1:
		//OLED_Show_Str(0, 2, "BEEP", 16);
		BEE = 1;
		delay_ms(200);
		BEE = 0;
		delay_ms(200);
		BEE = 1;
		delay_ms(200);
		BEE = 0;
		break;
	case 2:
		//OLED_Show_Str(0, 2, "OLED图片文字", 16);
		break;
	case 3:
		RGB_Init();							 //RGB灯初始化
		//OLED_Show_Str(0, 2, "三色灯", 16); //三色灯LED1
		RGB(255, 0, 0);
		delay_ms(1000);
		RGB(0, 255, 0);
		delay_ms(1000);
		RGB(0, 0, 255);
		delay_ms(1000);
		RGB_DeInit(); //删除初始化
		break;
	case 4:
		
		sprintf(oledbuf, "光强度:%dlx  ", Get_Bh_Value()); //光照度
		OLED_Show_Str(0, 2, oledbuf, 16);
		delay_ms(125);//延迟太低无法刷新
		//BH1750_DeInit();

		
		sprintf(oledbuf, "温度:%.1f℃   ", (float)DS18B20_Get_Temp() / 10);
		OLED_Show_Str(0, 4, oledbuf, 16);

		
		sprintf(oledbuf, "距离值:%.1fcm ", (float)Ultrasonic_Distance() / 10);
		OLED_Show_Str(0, 6, oledbuf, 16);
		//Ultrasonic_DeInit();
		break;
	case 5:
		//需要上拨语音识别的串口切换开关
		RGB_Init();					   //RGB灯初始化
		UART2_Init(115200);			   //初始化语音识别串口
		//OLED_Show_Str(0, 2, "YY", 16); //语音识别
		while (1)
		{
			voiceNum = XiaoChuang_ASR();
			if (voiceNum==0x17)
			{
				break;
			}
			
			switch (voiceNum)
			{
			case 0x12:
				RGB(255, 255, 255);
				break;
			case 0x13:
				RGB(0, 0, 0);
				break;
			case 0x14:
				RGB(255, 0, 0);
				break;
			case 0x15:
				RGB(0, 255, 0);
				break;
			case 0x16:
				RGB(0, 0, 255);
				break;
			default:
				break;
			}
		}
		/* 
开灯::识别成功，开灯:55021200
关灯::识别成功，关灯:55021300
红色::识别成功，红色:55021400
绿色::识别成功，绿色:55021500
蓝色::识别成功，蓝色:55021600 
退出识别::正在退出:55021700
*/

		//UART2_Send(0x01);
		UART2_DeInit();
		RGB_DeInit();
		break;
	case 6:
		SMG_Init();
		//OLED_Show_Str(0, 2, "SMG", 16); //数码管
		SMG_Display(4567, 4);
		break;
	case 7:
		EC11_Init();
		//EC11_DeInit();
		break;
	case 8:

		/*sprintf(oledbuf,"PIN3:%d",DHT11_Init());
		OLED_Show_Str(0,2,oledbuf,16);*/
		//Extend_Init2(INIT_3P_W2812);
		//Extend_Init2(INIT_3P_GP2Y0A21);
		break;
	case 9:
		//Extend_Init2(INIT_4P_HX711);
		//Extend_Init2(INIT_4P_MPU6050);
		//Extend_Init2(INIT_4P_MLX90614);
		//Extend_Init2(INIT_4P_RC522RFID);
		//Extend_Init2(INIT_4P_HMC5883);
		//Extend_Init2(INIT_4P_ADXL345);
		//OLED_ShowInt32Num(60, 2,ADXL345_Init(INIT_4P_ADXL345),1 , 16); //扩展接口4pin
		//OLED_Show_Str(0, 2, "PIN4:", 16); //扩展接口4pin

		break;

	default:
		break;
	}
}

int main(void)
{

	Hard_Init();
	OLED_Clear();

	

#if MODE==1
	//测试模式
	//OLED_Show_Str(0, 0, "Task: NULL-> 0", 16);
	while (1)
	{

		switch (KEY_Scan(0))
		{
		case 1: //KEY_S1
			LED_1 = 0;
			LED_2 = 1;
			LED_3 = 0;
			LED_4 = 0;
			oled_show(--mode_show);
			break;
		case 2: //KEY_S2
			LED_1 = 0;
			LED_2 = 0;
			LED_3 = 1;
			LED_4 = 0;
			run();
			break;
		case 3: //KEY_S3
			LED_1 = 0;
			LED_2 = 0;
			LED_3 = 0;
			LED_4 = 1;
			//mode_show = 11;
			break;
		case 4: //KEY_UP

			LED_1 = 1;
			LED_2 = 0;
			LED_3 = 0;
			LED_4 = 0;
			oled_show(++mode_show);
			break;
		case 5: //EC11_KEY
			OLED_ShowString(0, 6, "ec11_key", 16);
		}
		if (mode_show == 7)
		{
			sprintf(oledbuf, "EC11:%d        ", EC11_Value);
			OLED_Show_Str(0, 2, oledbuf, 16); //S4 EC11编码器
		}
		else if (mode_show == 8) //PIN3
		{
			/*DHT11_Read_Data(&Temp_Data,&Humi_Data);
			sprintf(oledbuf,"温度:%d ℃",Temp_Data);
			OLED_Show_Str(0,4,oledbuf,16);
			sprintf(oledbuf,"湿度:%d %%RH",Humi_Data);
			OLED_Show_Str(0,6,oledbuf,16);*/

			/*OLED_ShowString(0, 2,"SK6812", 16);	
			SK6812_Handle(255,0,0);
			delay_ms(200);
			SK6812_Handle(0,255,0);
			delay_ms(200);
			SK6812_Handle(0,0,255);
			delay_ms(200);*/

			/*sprintf(oledbuf,"距离:%0.2f   ",	GP2Y0A21_Distance());
			OLED_Show_Str(0,2,oledbuf,16);*/
		}
		else if (mode_show == 9) //PIN4
		{
			//扩展接口4pin 不适用串口1通讯 请下拨开关 否则影响p4口器件(DC供电口也影响器件数据)
			/*sprintf(oledbuf,"weight:%d g    ",Get_Weight());
			OLED_Show_Str(0,4,oledbuf,16);
			sprintf(oledbuf,"v:%lu     ",HX711_Read());
			OLED_Show_Str(0,6,oledbuf,16);*/

			/*
			MPU_Get_Gyroscope(&aacx,&aacy,&aacz);
			sprintf(oledbuf,"x:%d    ",aacx);
			OLED_Show_Str(0,2,oledbuf,16);
			sprintf(oledbuf,"y:%d    ",aacy);
			OLED_Show_Str(0,4,oledbuf,16);
			sprintf(oledbuf,"z:%d    ",aacz);
			OLED_Show_Str(0,6,oledbuf,16);
			delay_ms(100);*/

			/*
			sprintf(oledbuf,"温度:%.2f ℃",SMBus_ReadTemp());
			OLED_Show_Str(0,4,oledbuf,16);*/

			//OLED_ShowInt32Num(0, 2,RC_check(), 1,16);
			//Read_Card();

			/*sprintf(oledbuf,"x:%d      ",HMC5883_Read_X());
			OLED_Show_Str(0,2,oledbuf,16);
			sprintf(oledbuf,"y:%d      ",HMC5883_Read_Y());
			OLED_Show_Str(0,4,oledbuf,16);
			sprintf(oledbuf,"z:%d      ",HMC5883_Read_Z());
			OLED_Show_Str(0,6,oledbuf,16);
			delay_ms(10);*/

			/*
			//ADXL345ReadReg_XYZ(&aacx,&aacy,&aacz);
			ADXL345_ReadReg_times(&aacx,&aacy,&aacz,1);
			sprintf(oledbuf,"x:%d   ",aacx);
			OLED_Show_Str(0,2,oledbuf,16);
			sprintf(oledbuf,"y:%d   ",aacy);
			OLED_Show_Str(0,4,oledbuf,16);
			sprintf(oledbuf,"z:%d   ",aacz);
			OLED_Show_Str(0,6,oledbuf,16);
			sprintf(oledbuf,"x:%d   ",ADXL345_Get_Angle(aacx, aacy, aacz, 0));
			OLED_Show_Str(55,2,oledbuf,16);
			sprintf(oledbuf,"y:%d   ",ADXL345_Get_Angle(aacx, aacy, aacz, 1));
			OLED_Show_Str(55,4,oledbuf,16);
			sprintf(oledbuf,"z:%d   ",ADXL345_Get_Angle(aacx, aacy, aacz, 2));
			OLED_Show_Str(55,6,oledbuf,16);
			delay_ms(100);*/
		}
	}
#endif

	//任务选择模式
#if MODE==3||MODE==2
	Task_Init();
	//OLED_DrawBMP(0,0,128,8,bmp1);
	//OLED_ShowString(0, 0, "Task:1", 16);
	while (1)
	{
		GUI_Refresh();

		
	}

#endif

#if MODE==4
	int i=1;
	DAC_Init();
	RNG_Init();
	//wave_gen(WAVE_SINE,3300);
	//wave_gen(WAVE_RAMP,3300);//锯齿波
	//wave_gen(WAVE_SQUARE,3300);//方波
	wave_gen(WAVE_T,3300);//三角波
	//OLED_Show_Str(0,0,"正弦波",16);
	
	//DAC_SetHz(84,10);
	/*while (1)
	{
		wave_gen(WAVE_Noise,3300);//噪声波
		DAC_SetHz(RNG_GetRandom(100));//随机频率
	}*/
	
	while (1)
	{
		switch (KEY_Scan(0))
		{
		case 1: //KEY_S1
			i++;
			
			OLED_ShowInt32Num(0,0,i,3,16);
			break;
		case 2: //KEY_S2
			i--;
			DAC_SetHz(i);
			OLED_ShowInt32Num(0,0,i,3,16);
			break;
		}
	}
#endif
}
