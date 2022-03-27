#include "MYGUI_FUNC.h"

#include "MYGUI.h"
#include "stdio.h"
#include "stm32f4xx.h"
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
#include "Voltage.H"
#include "RNG.h"
#include "RTC.h"
u8 Task_Status[66]; //执行任务状态
char oledbuf[100];  //oled显示字符串缓存区
uint32_t oledWavebuf[256];
float Buf_Waveform[Buf_ADC_len];       //采集电压缓存
uint8_t Buf_ADCValue[Buf_ADC_len];     //ADC采集缓存
uint32_t oledWavebuf_len = 160;        //决定波形宽度 4Hz=515静止，517动态,3Hz=160静止，192移动，5HZ=303移动
float Buf_VPP_Voltage = 0;             //峰峰值电压
uint16_t Buf_Period = 0;               //周期值
uint16_t Buf_Duty_Cycle = 0;           //占空比值
float Buf_frequency = 0;               //频率值
uint16_t data_temp = 0, data_humi = 0; //温度湿度
uint16_t Buf_DAC_frequency = 0;        //频率值
uint16_t Buf_DAC_wavetype = 0;         //波形
uint16_t Buf_DAC_open = OFF;
uint16_t Buf_ADC_CHANNEL = 1; //外部通道，0内部通道采集
uint8_t bmp[] = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x80,
    0x80,
    0x80,
    0x40,
    0x40,
    0x40,
    0xA0,
    0xA0,
    0x20,
    0x20,
    0x50,
    0x50,
    0x10,
    0x00,
    0x28,
    0x28,
    0x08,
    0x98,
    0x54,
    0x54,
    0x04,
    0x04,
    0x44,
    0x88,
    0x0A,
    0x0A,
    0x0A,
    0x0A,
    0x02,
    0x02,
    0x42,
    0x12,
    0x72,
    0x84,
    0x14,
    0x44,
    0x05,
    0x15,
    0x85,
    0x05,
    0x05,
    0x05,
    0x05,
    0x05,
    0x65,
    0x05,
    0x05,
    0x04,
    0x24,
    0x04,
    0x42,
    0x02,
    0x02,
    0x02,
    0x02,
    0x8A,
    0x8A,
    0x8A,
    0xC8,
    0x20,
    0x04,
    0x04,
    0x84,
    0x54,
    0x14,
    0x08,
    0x08,
    0x28,
    0x28,
    0x10,
    0x10,
    0x50,
    0x50,
    0x50,
    0x20,
    0xA0,
    0xA0,
    0x40,
    0x40,
    0xC0,
    0x80,
    0x80,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x80,
    0xC0,
    0x40,
    0x20,
    0xB0,
    0x10,
    0x10,
    0x48,
    0x2C,
    0x04,
    0x16,
    0x0A,
    0x8B,
    0xC5,
    0x45,
    0x00,
    0x02,
    0x00,
    0x19,
    0x11,
    0x00,
    0x20,
    0x20,
    0x40,
    0x00,
    0xA0,
    0x80,
    0x48,
    0x40,
    0x00,
    0x21,
    0x20,
    0x02,
    0x14,
    0x10,
    0x00,
    0x05,
    0x00,
    0x00,
    0x0A,
    0x08,
    0x08,
    0x00,
    0x01,
    0x01,
    0x04,
    0x05,
    0x04,
    0x05,
    0x04,
    0x04,
    0x05,
    0x85,
    0xC0,
    0xC0,
    0xE4,
    0xC5,
    0x81,
    0x05,
    0x05,
    0x05,
    0x04,
    0x04,
    0x04,
    0x00,
    0x00,
    0x00,
    0x08,
    0x0A,
    0x09,
    0x09,
    0x00,
    0x00,
    0x10,
    0x15,
    0x13,
    0x00,
    0x20,
    0x20,
    0x08,
    0x48,
    0x08,
    0x80,
    0x80,
    0x0C,
    0x3C,
    0x3C,
    0x00,
    0x08,
    0x0D,
    0x01,
    0x80,
    0x02,
    0x00,
    0x45,
    0x01,
    0x8A,
    0x42,
    0x14,
    0x04,
    0x28,
    0x48,
    0x48,
    0x90,
    0xB0,
    0x60,
    0x40,
    0xC0,
    0x80,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xC0,
    0xE0,
    0x38,
    0x1C,
    0x46,
    0x23,
    0x19,
    0x0D,
    0x02,
    0x02,
    0x01,
    0x80,
    0x80,
    0x10,
    0x00,
    0x20,
    0x00,
    0x00,
    0x20,
    0x05,
    0x81,
    0x40,
    0x20,
    0x12,
    0x08,
    0x09,
    0x04,
    0x02,
    0x02,
    0x00,
    0x01,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x80,
    0x90,
    0xD0,
    0xF0,
    0xF0,
    0xF8,
    0xFC,
    0xFE,
    0xFF,
    0x7F,
    0x7F,
    0x3F,
    0x7F,
    0xFF,
    0xFF,
    0xFE,
    0xFC,
    0xF8,
    0xF0,
    0xE0,
    0xC0,
    0x80,
    0x00,
    0x60,
    0x60,
    0x60,
    0x60,
    0x20,
    0x80,
    0xC0,
    0xC0,
    0xC0,
    0xC0,
    0xC0,
    0x00,
    0x00,
    0x80,
    0x80,
    0x00,
    0x01,
    0x01,
    0x02,
    0x02,
    0x02,
    0x04,
    0x08,
    0x0A,
    0x12,
    0x20,
    0x42,
    0x81,
    0x04,
    0x10,
    0x51,
    0x82,
    0x00,
    0x18,
    0x88,
    0x88,
    0x18,
    0x01,
    0x02,
    0x04,
    0x09,
    0x13,
    0x66,
    0xCC,
    0x18,
    0x30,
    0xE0,
    0x80,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xF0,
    0xFE,
    0x07,
    0x00,
    0xFC,
    0x03,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x01,
    0x01,
    0x00,
    0x00,
    0x00,
    0xE0,
    0x0C,
    0x02,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x80,
    0xC0,
    0xC0,
    0xE0,
    0xF0,
    0xF8,
    0xFC,
    0xFE,
    0xFF,
    0x7F,
    0x3F,
    0x1F,
    0x0F,
    0x07,
    0x03,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x03,
    0x07,
    0x0F,
    0x3F,
    0x7F,
    0xFF,
    0xFF,
    0xFF,
    0xFE,
    0xFC,
    0xFC,
    0x78,
    0x78,
    0xE0,
    0xE0,
    0x80,
    0xC1,
    0x41,
    0x20,
    0x43,
    0x67,
    0x6D,
    0x7F,
    0x1E,
    0x1F,
    0x1F,
    0x3C,
    0x18,
    0x18,
    0x02,
    0x0C,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x06,
    0x38,
    0xC0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x07,
    0xF8,
    0x01,
    0x0F,
    0xFE,
    0xF0,
    0x00,
    0x00,
    0x00,
    0x0F,
    0x3F,
    0xF0,
    0x80,
    0x1F,
    0x60,
    0x80,
    0x04,
    0x4C,
    0xCC,
    0xD8,
    0x90,
    0x90,
    0x18,
    0x18,
    0x0C,
    0xCE,
    0xE6,
    0xE0,
    0x03,
    0x18,
    0x60,
    0xC0,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x80,
    0xC0,
    0xE0,
    0xF0,
    0xF8,
    0xFC,
    0xFE,
    0xFF,
    0x7F,
    0x3F,
    0x3F,
    0x1F,
    0x0F,
    0x07,
    0x03,
    0x01,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x80,
    0x00,
    0x00,
    0x00,
    0x02,
    0x02,
    0x02,
    0x02,
    0x01,
    0x03,
    0x0E,
    0x1F,
    0x3E,
    0x7F,
    0xFE,
    0xFE,
    0xFE,
    0xFE,
    0xFF,
    0xFF,
    0xFF,
    0xFC,
    0xF8,
    0xF0,
    0xE0,
    0xE0,
    0x80,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0xC0,
    0x20,
    0x0E,
    0x43,
    0xE0,
    0xE6,
    0x8E,
    0x1C,
    0x1C,
    0x18,
    0x90,
    0x98,
    0xD8,
    0xDC,
    0x4C,
    0x04,
    0xC0,
    0x70,
    0x0F,
    0xC0,
    0xF8,
    0x3F,
    0x07,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x03,
    0x0E,
    0x1C,
    0x31,
    0x62,
    0xCC,
    0x90,
    0xA1,
    0x49,
    0x49,
    0x9B,
    0x9B,
    0x18,
    0x19,
    0x11,
    0x11,
    0x00,
    0x1E,
    0x1E,
    0x04,
    0x01,
    0x02,
    0x04,
    0x08,
    0x10,
    0x10,
    0x20,
    0x20,
    0x00,
    0x48,
    0x0C,
    0x8C,
    0x0E,
    0x07,
    0x07,
    0x07,
    0x07,
    0x07,
    0x07,
    0x07,
    0x03,
    0x03,
    0x03,
    0x03,
    0x03,
    0x03,
    0x03,
    0x03,
    0x03,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x01,
    0x03,
    0x03,
    0x03,
    0x03,
    0x03,
    0x03,
    0x03,
    0x03,
    0x03,
    0x03,
    0x83,
    0x83,
    0x02,
    0x40,
    0x00,
    0x00,
    0x20,
    0x10,
    0x00,
    0x08,
    0x04,
    0x02,
    0x01,
    0x0C,
    0x1E,
    0x1E,
    0x00,
    0x11,
    0x11,
    0x19,
    0x18,
    0x18,
    0x99,
    0x49,
    0x21,
    0x91,
    0x98,
    0x4C,
    0x22,
    0x19,
    0x1C,
    0x06,
    0x03,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x03,
    0x02,
    0x06,
    0x0D,
    0x0D,
    0x08,
    0x1A,
    0x14,
    0x34,
    0x28,
    0x60,
    0x50,
    0x40,
    0xA0,
    0xA0,
    0x48,
    0x48,
    0x0C,
    0x84,
    0x86,
    0x86,
    0x0A,
    0x00,
    0x00,
    0x00,
    0x05,
    0x01,
    0x00,
    0x02,
    0x02,
    0x00,
    0x04,
    0x04,
    0x00,
    0x00,
    0xC8,
    0xC8,
    0x68,
    0x28,
    0x00,
    0x00,
    0x00,
    0x10,
    0x10,
    0x10,
    0x10,
    0x00,
    0x00,
    0x00,
    0x40,
    0xC0,
    0xC0,
    0xC0,
    0x00,
    0x40,
    0xC0,
    0x80,
    0x00,
    0x10,
    0x10,
    0x10,
    0x10,
    0x10,
    0x00,
    0x00,
    0x00,
    0x08,
    0x28,
    0x28,
    0x68,
    0xC0,
    0x80,
    0x04,
    0x04,
    0x00,
    0x02,
    0x02,
    0x00,
    0x01,
    0x05,
    0x00,
    0x02,
    0x06,
    0x05,
    0x05,
    0x8D,
    0x08,
    0x18,
    0x50,
    0x00,
    0xA0,
    0x80,
    0x50,
    0x50,
    0x68,
    0x28,
    0x34,
    0x10,
    0x1A,
    0x09,
    0x09,
    0x04,
    0x06,
    0x02,
    0x01,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x01,
    0x01,
    0x03,
    0x02,
    0x02,
    0x04,
    0x04,
    0x05,
    0x05,
    0x04,
    0x0A,
    0x0A,
    0x08,
    0x08,
    0x14,
    0x14,
    0x14,
    0x12,
    0x10,
    0x08,
    0x2B,
    0x29,
    0x29,
    0x20,
    0x20,
    0x20,
    0x20,
    0x20,
    0x30,
    0x10,
    0x50,
    0x50,
    0x50,
    0x50,
    0x50,
    0x50,
    0x57,
    0x57,
    0x57,
    0x50,
    0x50,
    0x51,
    0x51,
    0x50,
    0x50,
    0x50,
    0x50,
    0x10,
    0x30,
    0x20,
    0x20,
    0x20,
    0x20,
    0x28,
    0x28,
    0x28,
    0x29,
    0x1B,
    0x10,
    0x10,
    0x14,
    0x14,
    0x04,
    0x08,
    0x08,
    0x0A,
    0x0A,
    0x04,
    0x05,
    0x05,
    0x05,
    0x00,
    0x02,
    0x02,
    0x03,
    0x01,
    0x01,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
}; //OLED图片显示数据

//任务使用的所需硬件初始化
void Task_Init(void)
{
    RGB_Init();
    MY_ADC_Init();
    EC11_Init();
    UART2_Init(115200);
    RNG_Init();           //随机数发生器
    RTC_Init();           //RTC实时时钟
    RTC_Set_WakeUp(4, 0); //配置WAKE UP中断,1秒钟中断一次
}

void Task_1_Func(u8 page_index, u8 key_val)
{

    switch (page_index)
    {
    case Task_1_Option:
        OLED_ShowString(0, 0, "LED:open ", 16);
        if (key_val == KEY_ENTER)
            RGB_Set(RGB_COLOR_WHITE);
        break;
    case Task_1_Child_2:
        OLED_ShowString(0, 0, "LED:close", 16);
        if (key_val == KEY_ENTER)
        {
            RGB_Set(RGB_COLOR_BLACK);
            RGB(0, 0, 0);
            TIM5COUNT = 9999;
        }

        break;
    case Task_1_Child_3:
        OLED_ShowString(0, 0, "LED:ss   ", 16);
        if (key_val == KEY_ENTER)
        {
#if 0
            for (int i = 0; i < 3; i++)
            {
                /*RGB_Set(RGB_COLOR_RED);
                delay_ms(200);
                RGB_Set(RGB_COLOR_GREEN);
                delay_ms(200);
                RGB_Set(RGB_COLOR_BLUE);
                delay_ms(200);*/
                RGB_Set(RGB_COLOR_YELLOW);
                delay_ms(200);
                RGB_Set(RGB_COLOR_SORT);
                delay_ms(200);
                RGB_Set(RGB_COLOR_SYAN);
                delay_ms(200);
                /*RGB_Set(RGB_COLOR_WHITE);
                delay_ms(200);*/
            }
            RGB_Set(RGB_COLOR_BLACK);
#else
            /*for (int i = 0; i < 255*3; i++)
            {
                
                if (i<255)
                {
                    RGB(i,0,0);
                }else if (i>=255&&i<255*2)
                {
                    RGB(255,i-255,0);
                }else
                {
                    RGB(255,255,i-255*2);
                }
                delay_ms(10);
            }
            RGB(0,0,0);*/
            TIM5COUNT = 0;
            Task_Status[1] = 3;
            TIM5_Init(70, 8399);

#endif
        }

        break;
    case Task_1_Child_4:
        OLED_ShowString(0, 0, "LED:R    ", 16);
        if (key_val == KEY_ENTER)
            RGB_Set(RGB_COLOR_RED);
        //RGB_Set(RGB_COLOR_YELLOW);
        break;
    case Task_1_Child_5:
        OLED_ShowString(0, 0, "LED:G    ", 16);
        if (key_val == KEY_ENTER)
            RGB_Set(RGB_COLOR_GREEN);
        //RGB_Set(RGB_COLOR_SYAN);
        break;
    case Task_1_Child_6:
        OLED_ShowString(0, 0, "LED:B    ", 16);
        if (key_val == KEY_ENTER)
            RGB_Set(RGB_COLOR_BLUE);
        //RGB_Set(RGB_COLOR_SORT);
        break;
    default:
        break;
    }
}

void Task_2_Func(u8 page_index, u8 key_val)
{
    switch (page_index)
    {
    case Task_2_Option:

        OLED_ShowString(0, 0, "BEEP:open ", 16);
        if (key_val == KEY_ENTER)
        {
            Task_Status[2] = ON;
            BEE_Init();
            TIM5_Init(300, 8399);
        }
        break;
    case Task_2_Child_1:
        OLED_ShowString(0, 0, "BEEP:close", 16);
        if (key_val == KEY_ENTER)
        {
            Task_Status[2] = OFF;
            BEE_DeInit();
            TIM5_DeInit();
        }
        break;
    }
}

void Task_3_Func(u8 page_index, u8 key_val)
{

    /*当索引指向任务3时 会处于一直刷新状态 可将数据刷新至此位置*/
    if (Task_Status[3] == ON && page_index == Task_3_Child_1)
    {
        sprintf(oledbuf, "DAC_OUT V:%.2f  ", Get_V(ADC_CHANNEL_4));
        OLED_ShowString(0, 0, oledbuf, 16);
        sprintf(oledbuf, "V:%.2f  ", Get_V(ADC_CHANNEL_16));
        OLED_ShowString(0, 4, oledbuf, 16);
        sprintf(oledbuf, "T:%.2f  ", (float)Get_Temprate() / 100);
        OLED_ShowString(0, 6, oledbuf, 16);
    }
    else if (Task_Status[3] == 2 && page_index == Task_3_Child_2)
    {
        //波形、峰峰值、周期、占空比
        //sprintf(oledbuf, "电压:%.2f  ", Get_V(ADC_CHANNEL_10));//内部电压
        //OLED_Show_Str(0, 0, oledbuf, 16);

        /*波形采样*/
        for (uint16_t I = 0; I < Buf_ADC_len; I++)
        {
            if (Buf_ADC_CHANNEL == 1)
            {
                Buf_Waveform[I] = Get_Adc(ADC_CHANNEL_10); //ADC_CHANNEL_4采集DAC_OUT引脚 ADC_CHANNEL_10采集PC0外部引脚输入电压
            }
            else
            {
                Buf_Waveform[I] = Get_Adc(ADC_CHANNEL_4); //ADC_CHANNEL_4采集DAC_OUT引脚 ADC_CHANNEL_10采集PC0外部引脚输入电压
            }
        }
        /*波形滤波*/
        for (uint16_t i = 0; i < Buf_ADC_len - 1; i++)
        {
            Buf_Waveform[i] = (Buf_Waveform[i] + Buf_Waveform[i + 1]) / 2;
        }
        /*波形识别*/
        switch (RMS(Buf_Waveform, Buf_ADC_len))
        {
        case 1:
            OLED_Show_Str(0, 0, "波形:矩形波 ", 16);
            break;
        case 2:
            OLED_Show_Str(0, 0, "波形:三角波", 16);
            break;
        case 3:
            OLED_Show_Str(0, 0, "波形:正弦波", 16);
            break;
        default:
            OLED_Show_Str(0, 0, "波形:      ", 16);
            break;
        }
        delay_ms(300);
        /*波形峰值*/
        Buf_VPP_Voltage = Get_Vpp_Voltage(Buf_Waveform);
        sprintf(oledbuf, "Vpp:%.2f mV", Buf_VPP_Voltage);
        OLED_Show_Str(0, 2, oledbuf, 16);

        /*电压换算*/
        for (uint16_t I = 0; I < Buf_ADC_len; I++)
        {
            if (Buf_Waveform[I] > Buf_VPP_Voltage / 2)
            {
                Buf_ADCValue[I] = 1;
            }
            else
            {
                Buf_ADCValue[I] = 0;
            }
        }
        /*周期*/
        Buf_Period = Get_Period(Buf_ADCValue, Buf_ADC_len);
        //sprintf(oledbuf, "Period:%d  ",Buf_Period);
        if (Buf_Period != 0)
        {
            Buf_frequency = 1 / (Buf_Period * ((double)2.65 / Buf_ADC_len));
        }
        //sprintf(oledbuf,"频率:%0.5fHZ",Buf_frequency);//单位HZ
        sprintf(oledbuf, "频率:%0.0f KHZ      ", Buf_frequency); //单位KHZ
        OLED_Show_Str(0, 4, oledbuf, 16);
        /*占空比*/
        sprintf(oledbuf, "DutyCycle:%d  ", Get_Duty_Cycle(Buf_ADCValue, Buf_ADC_len));
        OLED_Show_Str(0, 6, oledbuf, 16);
    }
    else if (Task_Status[3] == 3 && page_index == Task_3_Child_3)
    {
        oledWavebuf_len = EC11_Value;
        /*波形采样*/
        for (uint16_t I = 0; I < Buf_ADC_len; I++)
        {
            if (Buf_ADC_CHANNEL == 1)
            {
                Buf_Waveform[I] = Get_Adc(ADC_CHANNEL_10); //ADC_CHANNEL_4采集DAC_OUT引脚 ADC_CHANNEL_10采集PC0外部引脚输入电压
            }
            else
            {
                Buf_Waveform[I] = Get_Adc(ADC_CHANNEL_4); //ADC_CHANNEL_4采集DAC_OUT引脚 ADC_CHANNEL_10采集PC0外部引脚输入电压
            }
        }
        /*波形滤波*/
        for (uint16_t i = 0; i < Buf_ADC_len - 1; i++)
        {
            Buf_Waveform[i] = (Buf_Waveform[i] + Buf_Waveform[i + 1]) / 2;
        }
        /*电压换算*/
        for (uint16_t I = 0; I < Buf_ADC_len; I++)
        {
            if (Buf_Waveform[I] > Buf_VPP_Voltage / 2)
            {
                Buf_ADCValue[I] = 1;
            }
            else
            {
                Buf_ADCValue[I] = 0;
            }
        }
        /*波形峰值*/
        Buf_VPP_Voltage = Get_Vpp_Voltage(Buf_Waveform);
        sprintf(oledbuf, "Vpp:%.2f  ", Buf_VPP_Voltage);
        OLED_Show_Str(0, 0, oledbuf, 16);
        /*周期*/
        Buf_Period = Get_Period(Buf_ADCValue, Buf_ADC_len);
        //sprintf(oledbuf, "Period:%d  ",Buf_Period);
        if (Buf_Period != 0)
        {
            Buf_frequency = 1 / (Buf_Period * ((double)2.65 / Buf_ADC_len));
        }
        //sprintf(oledbuf,"频率:%0.2fHZ",Buf_frequency*1000);
        sprintf(oledbuf, "频率:%.4fKHz", Buf_frequency);
        OLED_Show_Str(0, 2, oledbuf, 16);
        delay_ms(50); //刷新速度
        /*显示波形*/

        for (uint16_t I = 0; I < oledWavebuf_len * 2; I++)
        {
            oledWavebuf[I / 2] = 1 << ((32 - (uint16_t)(Buf_Waveform[I] / 128) - 1));
        }
        Update_Display(oledWavebuf); //显示ADC图像
    }
    /*****************************************************/
    switch (page_index)
    {
    case Task_3_Option:
        OLED_ShowString(0, 0, "Init         ", 16);
        OLED_ShowString(0, 2, "Inide Data   ", 16);
        OLED_ShowString(0, 4, "Outside Data ", 16);
        OLED_ShowString(0, 6, "Show Wave    ", 16);
        GUI_MenuPoint(1);
        if (key_val == KEY_ENTER)
        {
            Buf_ADC_CHANNEL = 1;
        }
        break;
    case Task_3_Child_1:
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            Task_Status[3] = OFF;
            OLED_ShowString(0, 0, "Init         ", 16);
            OLED_ShowString(0, 2, "Inide Data   ", 16);
            OLED_ShowString(0, 4, "Outside Data ", 16);
            OLED_ShowString(0, 6, "Show Wave    ", 16);
            GUI_MenuPoint(2);
        }
        else if (key_val == KEY_ENTER)
        {
            Task_Status[3] = ON;
        }
        break;
    case Task_3_Child_2:
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            Task_Status[3] = OFF;
            OLED_ShowString(0, 0, "Init         ", 16);
            OLED_ShowString(0, 2, "Inide Data   ", 16);
            OLED_ShowString(0, 4, "Outside Data ", 16);
            OLED_ShowString(0, 6, "Show Wave    ", 16);
            GUI_MenuPoint(3);
        }
        else if (key_val == KEY_ENTER)
        {
            Task_Status[3] = 2;
            Buf_ADC_CHANNEL = !Buf_ADC_CHANNEL; //内外更换
        }
        break;
    case Task_3_Child_3:
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            OLED_ShowString(0, 0, "Init         ", 16);
            OLED_ShowString(0, 2, "Inide Data   ", 16);
            OLED_ShowString(0, 4, "Outside Data ", 16);
            OLED_ShowString(0, 6, "Show Wave    ", 16);
            GUI_MenuPoint(4);
        }
        else if (key_val == KEY_ENTER)
        {
            Task_Status[3] = 3;
            EC11_Value = oledWavebuf_len;
            Buf_ADC_CHANNEL = !Buf_ADC_CHANNEL; //内外更换
        }
        break;
    default:
        break;
    }
}

void Task_4_Func(u8 page_index, u8 key_val)
{
    if (Task_Status[4] == 2 && page_index == Task_4_Child_1)
    {
        if (EC11_Value < 0) //限制旋钮范围
        {
            EC11_Value = 6;
        }
        else if (EC11_Value > 6)
        {
            EC11_Value = 0;
        }
        switch (Buf_DAC_wavetype)
        {
        case 0:
            OLED_Show_Str(0, 2, "波形:正弦波   ", 16);
            break;
        case 1:
            OLED_Show_Str(0, 2, "波形:方波     ", 16);
            break;
        case 2:
            OLED_Show_Str(0, 2, "波形:三角波   ", 16);
            break;
        case 3:
            OLED_Show_Str(0, 2, "波形:锯齿波   ", 16);
            break;
        case 4:
            OLED_Show_Str(0, 2, "波形:梯形波   ", 16);
            break;
        case 5:
            OLED_Show_Str(0, 2, "波形:噪声波   ", 16);
            break;
        case 6:
            OLED_Show_Str(0, 2, "波形:半波     ", 16);
            break;
        default:
            break;
        }
        Buf_DAC_wavetype = EC11_Value;    //存放调节结果
        wave_gen(Buf_DAC_wavetype, 3300); //波形生成
    }
    else if (Task_Status[4] == 3 && page_index == Task_4_Child_2)
    {
        if (EC11_Value < 0) //限制旋钮范围
        {
            EC11_Value = 100;
        }
        else if (EC11_Value > 100)
        {
            EC11_Value = 0;
        }
        DAC_SetHz(EC11_Value); //设置DAC输出频率
        sprintf(oledbuf, "频率:%2d KHZ  ", EC11_Value);
        OLED_Show_Str(0, 4, oledbuf, 16);
        Buf_DAC_frequency = EC11_Value; //存放调节结果
    }

    switch (page_index)
    {
    case Task_4_Option: //Init
        OLED_Show_Str(0, 0, "Init", 16);
        GUI_MenuPoint(1);
        switch (Buf_DAC_wavetype)
        {
        case 0:
            OLED_Show_Str(0, 2, "波形:正弦波", 16);
            break;
        case 1:
            OLED_Show_Str(0, 2, "波形:方波  ", 16);
            break;
        case 2:
            OLED_Show_Str(0, 2, "波形:三角波", 16);
            break;
        case 3:
            OLED_Show_Str(0, 2, "波形:锯齿波   ", 16);
            break;
        case 4:
            OLED_Show_Str(0, 2, "波形:梯形波   ", 16);
            break;
        case 5:
            OLED_Show_Str(0, 2, "波形:噪声波   ", 16);
            break;
        case 6:
            OLED_Show_Str(0, 2, "波形:半波     ", 16);
            break;
        default:
            break;
        }

        sprintf(oledbuf, "频率:%2d KHZ", Buf_DAC_frequency);
        OLED_Show_Str(0, 4, oledbuf, 16);

        Buf_DAC_open == ON ? OLED_Show_Str(0, 6, "ON ", 16) : OLED_Show_Str(0, 6, "OFF", 16);

        if (key_val == KEY_ENTER)
        {

            Task_Status[4] = 0;
            EC11_Value = 0;
        }
        break;
    case Task_4_Child_1: //波形选择
        if (key_val == KEY_NEXT)
        {
            GUI_MenuPoint(2);
            EC11_Value = Buf_DAC_wavetype; //加载旋钮存放值
        }
        if (key_val == KEY_ENTER) //若EC11无法使用 则用S2调节
        {
            EC11_Value++;
        }
        if (key_val == KEY_PREVIOUS) ////若EC11无法使用 则用KEYUP调节
        {
            EC11_Value--;
        }
        Task_Status[4] = 2;

        break;
    case Task_4_Child_2: //频率选择
        if (key_val == KEY_NEXT)
        {
            GUI_MenuPoint(3);
            EC11_Value = Buf_DAC_frequency; //加载旋钮存放值
        }
        if (key_val == KEY_ENTER) //若EC11无法使用 则用S2调节
        {
            EC11_Value++;
        }
        if (key_val == KEY_PREVIOUS) ////若EC11无法使用 则用KEYUP调节
        {
            EC11_Value--;
        }
        Task_Status[4] = 3;

        break;
    case Task_4_Child_3: //开启关闭波形
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            GUI_MenuPoint(4);
        }

        if (key_val == KEY_ENTER)
        {
            //由于确认按钮按下会清除界面 所以在这里重绘界面
            OLED_Show_Str(0, 0, "Init", 16);
            switch (Buf_DAC_wavetype)
            {
            case 0:
                OLED_Show_Str(0, 2, "波形:正弦波", 16);
                break;
            case 1:
                OLED_Show_Str(0, 2, "波形:方波  ", 16);
                break;
            case 2:
                OLED_Show_Str(0, 2, "波形:三角波", 16);
                break;
            case 3:
                OLED_Show_Str(0, 2, "波形:锯齿波   ", 16);
                break;
            case 4:
                OLED_Show_Str(0, 2, "波形:梯形波   ", 16);
                break;
            case 5:
                OLED_Show_Str(0, 2, "波形:噪声波   ", 16);
                break;
            case 6:
                OLED_Show_Str(0, 2, "波形:半波     ", 16);
                break;
            default:
                break;
            }
            sprintf(oledbuf, "频率:%2d KHZ", Buf_DAC_frequency);
            OLED_Show_Str(0, 4, oledbuf, 16);

            GUI_MenuPoint(4);
            if (Buf_DAC_open == ON)
            {
                Buf_DAC_open = OFF;
                DAC_DeInit();
                OLED_Show_Str(0, 6, "OFF", 16);
            }
            else
            {
                Buf_DAC_open = ON;
                OLED_Show_Str(0, 6, "ON ", 16);
                DAC_Init();

                //OutPut_Waveform(Buf_DAC_wavetype, Buf_DAC_frequency, 3300, 0);
            }
        }
        break;
    default:
        break;
    }
}
int rondom = 0;
void Task_5_Func(u8 page_index, u8 key_val)
{
    if (page_index == Task_5_Option && Task_Status[5] == ON)
    {
        //sprintf(oledbuf, "光强度:%dlx  ", Get_Bh_Value()); //光照度
        sprintf(oledbuf, "光强度:%dlx  ", bh1750);
        OLED_Show_Str(0, 2, oledbuf, 16);
        //sprintf(oledbuf, "温度:%.1f℃   ", (float)DS18B20_Get_Temp() / 10);
        sprintf(oledbuf, "温度:%.1f℃   ", (float)ds18b20 / 10);
        OLED_Show_Str(0, 4, oledbuf, 16);
        //sprintf(oledbuf, "距离值:%.1fcm ", (float)Ultrasonic_Distance() / 10);
        sprintf(oledbuf, "距离值:%dmm    ", Ultrasonic_Distance());
        OLED_Show_Str(0, 6, oledbuf, 16);
    }
    else if (page_index == Task_5_Child_1 && Task_Status[5] == 2)
    {
        sprintf(oledbuf, "温度:%.1f℃   ", (float)DS18B20_Get_Temp() / 10);
        OLED_Show_Str(0, 2, oledbuf, 16);
    }
    else if (page_index == Task_5_Child_2 && Task_Status[5] == 3)
    {
        bh1750 = Get_Bh_Value();
        sprintf(oledbuf, "光强度:%d lx  ", bh1750); //光照度
        OLED_Show_Str(0, 2, oledbuf, 16);
        if (bh1750 < 50)
        {
            RGB_Set(RGB_COLOR_WHITE);
        }
        else
        {
            RGB_Set(RGB_COLOR_BLACK);
        }
        delay_ms(200); //延迟刷新结果
    }
    else if (page_index == Task_5_Child_3 && Task_Status[5] == 4)
    {
        //sprintf(oledbuf, "距离值:%.1fcm ", (float)Ultrasonic_Distance() / 10);
        sprintf(oledbuf, "距离值:%dmm      ", Ultrasonic_Distance());
        //模拟距离
        /*if(rondom<20)
        {
            sprintf(oledbuf, "距离值:%dmm      ", RNG_GetRandomRange(100,120));
        }
         if(rondom>20 && rondom<30)
        {
            sprintf(oledbuf, "距离值:%dmm      ", RNG_GetRandomRange(200,220));
        }
         if(rondom>30 &&rondom<40)
        {
            sprintf(oledbuf, "距离值:%dmm      ", RNG_GetRandomRange(300,350));
        }
         if(rondom>40 &&rondom<60)
        {
            sprintf(oledbuf, "距离值:%dmm      ", RNG_GetRandomRange(490,520));
        }
        if(rondom>60)
        {
            rondom=0;
        }*/

        OLED_Show_Str(0, 2, oledbuf, 16);
        //delay_ms(200);
        //rondom++;
    }

    switch (page_index)
    {
    case Task_5_Option:
        OLED_Show_Str(0, 0, "传感器:ALLON  ", 16);
        if (key_val == KEY_ENTER)
        {
            Task_Status[5] = ON;
            TIM5_Init(3000, 8399);
        }
        break;
    case Task_5_Child_1:
        OLED_Show_Str(0, 0, "传感器:温度    ", 16);

        if (key_val == KEY_NEXT)
        {
            OLED_Show_Str(0, 2, "              ", 16);
            OLED_Show_Str(0, 4, "              ", 16);
            OLED_Show_Str(0, 6, "              ", 16);
        }
        if (key_val == KEY_ENTER)
        {

            Task_Status[5] = 2;
        }
        break;

    case Task_5_Child_2:
        OLED_Show_Str(0, 0, "传感器:光强度", 16);
        if (key_val == KEY_NEXT)
        {
            OLED_Show_Str(0, 2, "              ", 16);
            OLED_Show_Str(0, 4, "              ", 16);
            OLED_Show_Str(0, 6, "              ", 16);
        }
        if (key_val == KEY_ENTER)
        {
            Task_Status[5] = 3;
        }
        break;

    case Task_5_Child_3:
        OLED_Show_Str(0, 0, "传感器:距离值", 16);
        if (key_val == KEY_NEXT)
        {
            OLED_Show_Str(0, 2, "              ", 16);
            OLED_Show_Str(0, 4, "              ", 16);
            OLED_Show_Str(0, 6, "              ", 16);
        }
        if (key_val == KEY_ENTER)
        {
            Task_Status[5] = 4;
            rondom = 0;
        }
        break;

    case Task_5_Child_4:
        OLED_Show_Str(0, 0, "传感器:close  ", 16);
        OLED_Show_Str(0, 2, "              ", 16);
        OLED_Show_Str(0, 4, "              ", 16);
        OLED_Show_Str(0, 6, "              ", 16);
        if (key_val == KEY_ENTER)
        {
            Task_Status[5] = OFF;
        }
        break;
    default:
        break;
    }
}
uint8_t YY = 0;
void Task_6_Func(u8 page_index, u8 key_val)
{
    switch (page_index)
    {
    case Task_6_Option:
        OLED_Show_Str(0, 0, "Init       ", 16);
        OLED_Show_Str(0, 2, "语音播报    ", 16);
        OLED_Show_Str(0, 4, "语音识别    ", 16);
        GUI_MenuPoint(1);
        Task_Status[6]=0;//重置状态标志
        TIM5_DeInit();//关闭定时器
        break;
    case Task_6_Child_1:
        GUI_MenuPoint(2);
        if (key_val == KEY_ENTER)
        {
            OLED_Show_Str(0, 2, "语音播报    ", 16);
            //UART2_Init(115200); //初始化语音 请上波语音识别开关
            for (int i = 0x06; i <= 0X09; i++)
            {
                UART2_Send(i);
                delay_ms(500);
            }

            // UART2_DeInit(); //关闭初始化
        }

        break;
    case Task_6_Child_2:
        GUI_MenuPoint(3);
        if (key_val == KEY_ENTER)
        {
            OLED_Show_Str(0, 4, "语音识别    ", 16);

            /*将使用到的模块初始化*/
            Extend_Init2(INIT_3P_DHT11); //初始化温湿度模块
            /*利用TIM5进行循环识别*/
            Task_Status[6]=2;
            TIM5_Init(10,8399);
        }
        break;
    default:
        break;
    }
}

void Task_7_Func(u8 page_index, u8 key_val)
{
    if (Task_Status[7] == 4 && page_index == Task_7_Child_4)
    {
        if (EC11_Value < 0) //限制旋钮范围
        {
            EC11_Value = 23;
        }
        else if (EC11_Value > 23)
        {
            EC11_Value = 0;
        }
        SMGSWAP = EC11_Value;
        OLED_ShowInt32Num(0, 2, SMGSWAP, 2, 16);
        SMG_Display(1234, 1);
    }
    switch (page_index)
    {
    case Task_7_Option:
        OLED_ShowString(0, 0, "Init       ", 16);
        OLED_ShowString(0, 2, "Shift left ", 16);
        OLED_ShowString(0, 4, "Show Time  ", 16);
        OLED_ShowString(0, 6, "Show RTC  ", 16);
        GUI_MenuPoint(1);
        if (key_val == KEY_ENTER)
        {
            TIM5COUNT = 0;
            Task_Status[7] = 0;
            TIM5_DeInit();
            SMG_DeInit();
        }
        break;
    case Task_7_Child_1:
        OLED_ShowString(0, 0, "Init       ", 16);
        OLED_ShowString(0, 2, "Shift left ", 16);
        OLED_ShowString(0, 4, "Show Time  ", 16);
        OLED_ShowString(0, 6, "Show RTC  ", 16);
        GUI_MenuPoint(2);
        if (key_val == KEY_ENTER) //这里也可放在定时器5里使用
        {
            SMG_Init();

            //00特殊显示
            for (int n = 0; n < 70; n++)
            {
                SMG_Display_Bit(0, 2);
                delay_ms(10);
                SMG_Display_Bit(0, 1);
                delay_ms(10);
            }
            //01-EF显示
            for (int i = 0; i < 0xf; i++)
            {
                for (int n = 0; n < 70; n++)
                {
                    SMG_Display_Bit(i, 2);
                    delay_ms(10);
                    SMG_Display_Bit(i + 1, 1);
                    delay_ms(10);
                }
            }
            SMG_DeInit();
        }
        break;
    case Task_7_Child_2:
        OLED_ShowString(0, 0, "Init       ", 16);
        OLED_ShowString(0, 2, "Shift left ", 16);
        OLED_ShowString(0, 4, "Show Time  ", 16);
        OLED_ShowString(0, 6, "Show RTC  ", 16);
        GUI_MenuPoint(3);
        if (key_val == KEY_ENTER) //确认按键按下
        {

            /*
            //定时器刷新数码管
            TIM5COUNT=0;
            Task_Status[7]=2;
            SMG_Init();
            TIM5_Init(50000-1,8399);//5秒显示一次
            */

            //逐个显示数码管
            SMG_Init();
            SMG_DisplayP(2, -1, -1, -1, 1);
            delay_ms(2000);
            SMG_DisplayP(2, 0, -1, -1, 1);
            delay_ms(2000);
            SMG_DisplayP(2, 0, 2, -1, 1);
            delay_ms(2000);
            SMG_DisplayP(2, 0, 2, 1, 1);
            delay_ms(5000);
            SMG_DisplayP(1, -1, -1, -1, 1);
            delay_ms(2000);
            SMG_DisplayP(1, 2, -1, -1, 1);
            delay_ms(2000);
            SMG_DisplayP(1, 2, 2, -1, 1);
            delay_ms(2000);
            SMG_DisplayP(1, 2, 2, 5, 1);
            delay_ms(5000);
            SMG_DeInit();
        }
        break;
    case Task_7_Child_3:
        OLED_ShowString(0, 0, "Init       ", 16);
        OLED_ShowString(0, 2, "Shift left ", 16);
        OLED_ShowString(0, 4, "Show Time  ", 16);
        OLED_ShowString(0, 6, "Show RTC  ", 16);
        GUI_MenuPoint(4);
        if (key_val == KEY_ENTER) //确认按键按下
        {
            Task_Status[7] = 3;
            SMG_Init();
            TIM5_Init(10000 - 1, 8399); //1秒显示一次
        }
        break;
    case Task_7_Child_4: //利用按钮或EC11调正数码管顺序
        if (key_val == KEY_NEXT)
        {
            SMG_Init();
            OLED_ShowString(0, 0, "SMG test     ", 16);
            OLED_ShowString(0, 2, "             ", 16);
            OLED_ShowString(0, 4, "             ", 16);
            OLED_ShowString(0, 6, "             ", 16);
            GUI_MenuPoint(1);
            EC11_Value = SMGSWAP;
        }
        if (key_val == KEY_ENTER) //确认按键按下
        {
            EC11_Value++;
        }
        if (key_val == KEY_PREVIOUS)
        {
            EC11_Value--;
        }
        Task_Status[7] = 4;
        break;
    default:
        break;
    }
}

void Task_8_Func(u8 page_index, u8 key_val)
{
    switch (page_index)
    {
    case Task_8_Option:
        OLED_Show_Str(0, 0, "Init          ", 16);
        OLED_Show_Str(0, 2, "扩展模块PIN3   ", 16);
        OLED_Show_Str(0, 4, "扩展模块PIN4   ", 16);
        OLED_Show_Str(0, 6, "               ", 16);
        GUI_MenuPoint(1);
        break;
    case Task_8_Child_1:
        OLED_Show_Str(0, 0, "Init          ", 16);
        OLED_Show_Str(0, 2, "扩展模块PIN3  ", 16);
        OLED_Show_Str(0, 4, "扩展模块PIN4  ", 16);
        OLED_Show_Str(0, 6, "               ", 16);
        GUI_MenuPoint(2);
        break;
    case Task_8_Child_2:
        OLED_Show_Str(0, 0, "Init          ", 16);
        OLED_Show_Str(0, 2, "扩展模块PIN3  ", 16);
        OLED_Show_Str(0, 4, "扩展模块PIN4  ", 16);
        OLED_Show_Str(0, 6, "               ", 16);
        GUI_MenuPoint(3);
        break;
    default:
        break;
    }
}

void Task_9_Func(u8 page_index, u8 key_val)
{
    switch (page_index)
    {
    case Task_9_Option:
        OLED_Show_Str(0, 0, "Init         ", 16);
        OLED_Show_Str(0, 2, "文字显示     ", 16);
        OLED_Show_Str(0, 4, "图片显示     ", 16);
        GUI_MenuPoint(1);
        break;
    case Task_9_Child_1:
        //文字显示
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            OLED_Show_Str(0, 0, "Init         ", 16);
            OLED_Show_Str(0, 2, "文字显示     ", 16);
            OLED_Show_Str(0, 4, "图片显示     ", 16);
            GUI_MenuPoint(2);
        }

        if (key_val == KEY_ENTER)
        {
            //OLED_Show_Str(20, 2, "扬帆启航", 16);
            //OLED_Show_Str(20, 4, "追逐梦想", 16);
            //OLED_Show_Str(0, 0, "庆祝中国共产党成立100周年", 32);
            OLED_Show_Str(0, 0, "富强民主", 16);
            OLED_Show_Str(0, 2, "文明和谐", 16);
            //OLED_ShowChar_NEW(0,0,'C',16,1);
        }

        break;
    case Task_9_Child_2:
        //图片显示 可先缩放图片大小(128x64) 利用Img2Lcd软件转成BMP 最后PCtoLCD2002软件生成图片数组
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            OLED_Show_Str(0, 0, "Init         ", 16);
            OLED_Show_Str(0, 2, "文字显示     ", 16);
            OLED_Show_Str(0, 4, "图片显示     ", 16);
            GUI_MenuPoint(3);
        }

        if (key_val == KEY_ENTER)
        {
            OLED_DrawBMP(0, 0, 128, 8, bmp);
        }
        break;
    default:
        break;
    }
}

void Task_10_Func(u8 page_index, u8 key_val)
{
    if (Task_Status[10] == 1)
    {
        OLED_ShowInt32Num(0, 6, RNG_GetRandom(4000), 10, 16);
    }

    switch (page_index)
    {
    case Task_10_Option:
        OLED_Show_Str(0, 0, "Test1  ", 16);
        OLED_Show_Str(0, 2, "Test2  ", 16);
        OLED_Show_Str(0, 4, "Test3  ", 16);
        GUI_MenuPoint(1);
        break;
    case Task_10_Child_1:
        GUI_MenuPoint(2);
        if (key_val == KEY_ENTER)
        {
            Task_Status[10] = !Task_Status[10];
        }
        break;
    case Task_10_Child_2:
        GUI_MenuPoint(3);
        if (key_val == KEY_ENTER)
        {
        }
        break;
    default:
        break;
    }
}

short mpu_aacx, mpu_aacy, mpu_aacz;
short adxl345_aacx, adxl345_aacy, adxl345_aacz;
void Task_81_Func(u8 page_index, u8 key_val)
{
    //扩展接口4pin 不适用串口1通讯 请下拨开关 否则影响p4口器件(DC供电口也影响器件数据)

    if (Task_Status[11] == 1 && page_index == Task_81_Option)
    {
        if (EC11_Value < 0) //限制旋钮范围
        {
            EC11_Value = 1500;
        }
        else if (EC11_Value > 5)
        {
            EC11_Value = 0;
        }
        sprintf(oledbuf, "weight:%d g    ", Get_Weight());
        OLED_Show_Str(0, 4, oledbuf, 16);
        sprintf(oledbuf, "v:%lu     ", HX711_Read());
        OLED_Show_Str(0, 6, oledbuf, 16);
        if (Get_Weight() > 100)
        {
            Task_Status[2] = ON; //开启蜂鸣器
            OLED_Show_Str(0, 2, "a", 16);
        }
        else
        {
            Task_Status[2] = OFF; //关闭蜂鸣器
            OLED_Show_Str(0, 2, "b", 16);
        }
    }
    else if (Task_Status[11] == 2 && page_index == Task_81_Child_1)
    {
        MPU_Get_Gyroscope(&mpu_aacx, &mpu_aacy, &mpu_aacz);
        sprintf(oledbuf, "x:%d    ", mpu_aacx);
        OLED_Show_Str(0, 2, oledbuf, 16);
        sprintf(oledbuf, "y:%d    ", mpu_aacy);
        OLED_Show_Str(0, 4, oledbuf, 16);
        sprintf(oledbuf, "z:%d    ", mpu_aacz);
        OLED_Show_Str(0, 6, oledbuf, 16);
        delay_ms(100);
    }
    else if (Task_Status[11] == 3 && page_index == Task_81_Child_2)
    {
        sprintf(oledbuf, "温度:%.2f ℃", SMBus_ReadTemp());
        OLED_Show_Str(0, 4, oledbuf, 16);
    }
    else if (Task_Status[11] == 4 && page_index == Task_81_Child_3)
    {
        //OLED_ShowInt32Num(50, 2,RC_check(), 1,16);
    }
    else if (Task_Status[11] == 5 && page_index == Task_81_Child_4)
    {
        sprintf(oledbuf, "x:%d      ", HMC5883_Read_X());
        OLED_Show_Str(0, 2, oledbuf, 16);
        sprintf(oledbuf, "y:%d      ", HMC5883_Read_Y());
        OLED_Show_Str(0, 4, oledbuf, 16);
        sprintf(oledbuf, "z:%d      ", HMC5883_Read_Z());
        OLED_Show_Str(0, 6, oledbuf, 16);
        delay_ms(10);
    }
    else if (Task_Status[11] == 6 && page_index == Task_81_Child_5)
    {
        ADXL345ReadReg_XYZ(&adxl345_aacx, &adxl345_aacy, &adxl345_aacz);
        ADXL345_ReadReg_times(&adxl345_aacx, &adxl345_aacy, &adxl345_aacz, 1);
        sprintf(oledbuf, "x:%d   ", adxl345_aacx);
        OLED_Show_Str(0, 2, oledbuf, 16);
        sprintf(oledbuf, "y:%d   ", adxl345_aacy);
        OLED_Show_Str(0, 4, oledbuf, 16);
        sprintf(oledbuf, "z:%d   ", adxl345_aacz);
        OLED_Show_Str(0, 6, oledbuf, 16);
        sprintf(oledbuf, "x:%d   ", ADXL345_Get_Angle(adxl345_aacx, adxl345_aacy, adxl345_aacz, 0));
        OLED_Show_Str(55, 2, oledbuf, 16);
        sprintf(oledbuf, "y:%d   ", ADXL345_Get_Angle(adxl345_aacx, adxl345_aacy, adxl345_aacz, 1));
        OLED_Show_Str(55, 4, oledbuf, 16);
        sprintf(oledbuf, "z:%d   ", ADXL345_Get_Angle(adxl345_aacx, adxl345_aacy, adxl345_aacz, 2));
        OLED_Show_Str(55, 6, oledbuf, 16);
        delay_ms(100);
    }

    switch (page_index)
    {
    case Task_81_init: //初始化无指向
        if (key_val == KEY_ENTER)
        {
            OLED_Show_Str(0, 0, "HX711     ", 16); //HX711称重传感器
            OLED_Show_Str(0, 2, "MPU6050   ", 16); //MPU6050姿态传感器
            OLED_Show_Str(0, 4, "MLX90614  ", 16); //MLX90614红外测温传感器
            OLED_Show_Str(0, 6, "RC522RFID ", 16); //RC522RFID读卡感器
            Task_Status[11] = 0;
        }
        break;
    case Task_81_Option: //HX711称重传感器
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            OLED_Show_Str(0, 0, "HX711     ", 16); //HX711称重传感器
            OLED_Show_Str(0, 2, "MPU6050   ", 16); //MPU6050姿态传感器
            OLED_Show_Str(0, 4, "MLX90614  ", 16); //MLX90614红外测温传感器
            OLED_Show_Str(0, 6, "RC522RFID ", 16); //RC522RFID读卡感器
            GUI_MenuPoint(1);
        }
        if (key_val == KEY_ENTER)
        {
            Task_Status[11] = 1;
            Extend_Init2(INIT_4P_HX711);
            TIM5_Init(300, 8399); //开启蜂鸣器需要开启定时器5
        }
        break;
    case Task_81_Child_1: //MPU6050姿态传感器
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            OLED_Show_Str(0, 0, "HX711     ", 16); //HX711称重传感器
            OLED_Show_Str(0, 2, "MPU6050   ", 16); //MPU6050姿态传感器
            OLED_Show_Str(0, 4, "MLX90614  ", 16); //MLX90614红外测温传感器
            OLED_Show_Str(0, 6, "RC522RFID ", 16); //RC522RFID读卡感器
            GUI_MenuPoint(2);
        }
        if (key_val == KEY_ENTER)
        {
            Task_Status[11] = 2;
            Extend_Init2(INIT_4P_MPU6050);
        }
        break;
    case Task_81_Child_2: //MLX90614红外测温传感器
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            OLED_Show_Str(0, 0, "HX711     ", 16); //HX711称重传感器
            OLED_Show_Str(0, 2, "MPU6050   ", 16); //MPU6050姿态传感器
            OLED_Show_Str(0, 4, "MLX90614  ", 16); //MLX90614红外测温传感器
            OLED_Show_Str(0, 6, "RC522RFID ", 16); //RC522RFID读卡感器
            GUI_MenuPoint(3);
        }
        if (key_val == KEY_ENTER)
        {
            Task_Status[11] = 3;
            Extend_Init2(INIT_4P_MLX90614);
        }
        break;
    case Task_81_Child_3: //RC522RFID读卡感器
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            OLED_Show_Str(0, 0, "HX711     ", 16); //HX711称重传感器
            OLED_Show_Str(0, 2, "MPU6050   ", 16); //MPU6050姿态传感器
            OLED_Show_Str(0, 4, "MLX90614  ", 16); //MLX90614红外测温传感器
            OLED_Show_Str(0, 6, "RC522RFID ", 16); //RC522RFID读卡感器
            GUI_MenuPoint(4);
        }
        /* 这里由于确定后会转到FUN813 下列语句并不会执行
        if (key_val==KEY_ENTER)
        {
           
            Extend_Init2(INIT_4P_RC522RFID);
        }*/
        break;
    case Task_81_Child_4: //HMC5883电子罗盘
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            OLED_Show_Str(0, 0, "HMC5883   ", 16); //HMC5883电子罗盘
            OLED_Show_Str(0, 2, "ADXL345   ", 16); //ADXL345加速度检测
            OLED_Show_Str(0, 4, "          ", 16);
            OLED_Show_Str(0, 6, "          ", 16);
            GUI_MenuPoint(1);
        }
        if (key_val == KEY_ENTER)
        {
            Task_Status[11] = 5;
            Extend_Init2(INIT_4P_HMC5883);
        }
        break;
    case Task_81_Child_5: //HMC5883电子罗盘
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            OLED_Show_Str(0, 0, "HMC5883   ", 16); //HMC5883电子罗盘
            OLED_Show_Str(0, 2, "ADXL345   ", 16); //ADXL345加速度检测
            OLED_Show_Str(0, 4, "          ", 16);
            OLED_Show_Str(0, 6, "          ", 16);
            GUI_MenuPoint(2);
        }
        if (key_val == KEY_ENTER)
        {
            Task_Status[11] = 6;
            Extend_Init2(INIT_4P_ADXL345);
        }
        break;
    default:
        break;
    }
}

void Task_82_Func(u8 page_index, u8 key_val)
{

    if (Task_Status[12] == 1 && page_index == Task_82_Option)
    {
        DHT11_Read_Data(&data_temp, &data_humi);
        sprintf(oledbuf, "温度:%d ℃", data_temp);
        OLED_Show_Str(0, 4, oledbuf, 16);
        sprintf(oledbuf, "湿度:%d %%RH", data_humi);
        OLED_Show_Str(0, 6, oledbuf, 16);
    }
    else if (Task_Status[12] == 2 && page_index == Task_82_Child_1)
    {
        OLED_ShowString(0, 2, "SK6812", 16);
        SK6812_Handle(255, 0, 0);
        delay_ms(200);
        SK6812_Handle(0, 255, 0);
        delay_ms(200);
        SK6812_Handle(0, 0, 255);
        delay_ms(200);
    }
    else if (Task_Status[12] == 3 && page_index == Task_82_Child_2)
    {
        sprintf(oledbuf, "距离:%0.2f   ", GP2Y0A21_Distance());
        OLED_Show_Str(0, 2, oledbuf, 16);
    }

    switch (page_index)
    {
    case Task_82_init: //初始化无指向
        if (key_val == KEY_ENTER)
        {
            OLED_Show_Str(0, 0, "DHT11     ", 16); //DHT11温湿度传感器
            OLED_Show_Str(0, 2, "SK6812   ", 16);  //W2812三色灯
            OLED_Show_Str(0, 4, "GP2Y0A21  ", 16); //红外测距传感器
            OLED_Show_Str(0, 6, "          ", 16);
            Task_Status[12] = 0;
        }
    case Task_82_Option:
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            OLED_Show_Str(0, 0, "DHT11     ", 16); //DHT11温湿度传感器
            OLED_Show_Str(0, 2, "SK6812    ", 16); //W2812三色灯
            OLED_Show_Str(0, 4, "GP2Y0A21  ", 16); //红外测距传感器
            OLED_Show_Str(0, 6, "          ", 16);
            GUI_MenuPoint(1);
        }
        if (key_val == KEY_ENTER)
        {
            Task_Status[12] = 1;
            Extend_Init2(INIT_3P_DHT11);
        }
        break;
    case Task_82_Child_1:
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            OLED_Show_Str(0, 0, "DHT11     ", 16); //DHT11温湿度传感器
            OLED_Show_Str(0, 2, "SK6812    ", 16); //W2812三色灯
            OLED_Show_Str(0, 4, "GP2Y0A21  ", 16); //红外测距传感器
            OLED_Show_Str(0, 6, "          ", 16);
            GUI_MenuPoint(2);
        }
        if (key_val == KEY_ENTER)
        {
            Task_Status[12] = 2;
            Extend_Init2(INIT_3P_W2812);
        }
        break;
    case Task_82_Child_2:
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            OLED_Show_Str(0, 0, "DHT11     ", 16); //DHT11温湿度传感器
            OLED_Show_Str(0, 2, "SK6812    ", 16); //W2812三色灯
            OLED_Show_Str(0, 4, "GP2Y0A21  ", 16); //红外测距传感器
            OLED_Show_Str(0, 6, "          ", 16);
            GUI_MenuPoint(3);
        }
        if (key_val == KEY_ENTER)
        {
            Task_Status[12] = 3;
            Extend_Init2(INIT_3P_GP2Y0A21);
        }
        break;
    default:
        break;
    }
}

uint8_t hour, min, sec, ampm;
uint8_t year, month, date, week;
void Task_11_Func(u8 page_index, u8 key_val)
{

    if (Task_Status[11 + 2] == 1 && page_index == Task_11_Child_1)
    {
        if (EC11_Value < 0) //限制旋钮范围
        {
            EC11_Value = 24;
        }
        else if (EC11_Value > 24)
        {
            EC11_Value = 0;
        }
        hour = EC11_Value; //存放调节结果
    }
    if (Task_Status[11 + 2] == 2 && page_index == Task_11_Child_2)
    {
        if (EC11_Value < 0) //限制旋钮范围
        {
            EC11_Value = 60;
        }
        else if (EC11_Value > 60)
        {
            EC11_Value = 0;
        }
        min = EC11_Value; //存放调节结果
    }
    if (Task_Status[11 + 2] == 3 && page_index == Task_11_Child_3)
    {
        if (EC11_Value < 0) //限制旋钮范围
        {
            EC11_Value = 60;
        }
        else if (EC11_Value > 60)
        {
            EC11_Value = 0;
        }
        sec = EC11_Value; //存放调节结果
    }
    if (Task_Status[11 + 2] == 4 && page_index == Task_11_Child_4)
    {
        if (EC11_Value < 0) //限制旋钮范围
        {
            EC11_Value = 60;
        }
        else if (EC11_Value > 60)
        {
            EC11_Value = 0;
        }
        year = EC11_Value; //存放调节结果
    }
    if (Task_Status[11 + 2] == 5 && page_index == Task_11_Child_5)
    {
        if (EC11_Value < 0) //限制旋钮范围
        {
            EC11_Value = 12;
        }
        else if (EC11_Value > 12)
        {
            EC11_Value = 0;
        }
        month = EC11_Value; //存放调节结果
    }
    if (Task_Status[11 + 2] == 6 && page_index == Task_11_Child_6)
    {
        if (EC11_Value < 0) //限制旋钮范围
        {
            EC11_Value = 31;
        }
        else if (EC11_Value > 31)
        {
            EC11_Value = 0;
        }
        date = EC11_Value; //存放调节结果
    }
    if (Task_Status[11 + 2] >= 1 && Task_Status[11 + 2] <= 6)
    {
        sprintf(oledbuf, "Time:%2d:%2d:%2d", hour, min, sec);
        OLED_ShowString(0, 0, oledbuf, 16);
        sprintf(oledbuf, "Date:%2d:%2d:%2d", year, month, date);
        OLED_ShowString(0, 4, oledbuf, 16);
        sprintf(oledbuf, "Save");
        OLED_ShowString(0, 6, oledbuf, 16);
    }
    switch (page_index)
    {
    case Task_11_Option:
        if (key_val == KEY_ENTER)
        {
            RTC_Get_Time(&hour, &min, &sec, &ampm);
            RTC_Get_Date(&year, &month, &date, &week);
            sprintf(oledbuf, "Time:%2d:%2d:%2d", hour, min, sec);
            OLED_ShowString(0, 0, oledbuf, 16);
            sprintf(oledbuf, "Date:%2d:%2d:%2d", year, month, date);
            OLED_ShowString(0, 4, oledbuf, 16);
            sprintf(oledbuf, "Save");
            OLED_ShowString(0, 6, oledbuf, 16);
            Task_Status[11 + 2] = 0;
        }
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            Task_Status[11 + 2] = 0;
            OLED_ShowString(50, 2, " ", 16);
            OLED_ShowString(50, 6, "  ", 16);
        }
        break;
    case Task_11_Child_1:

        if (key_val == KEY_NEXT)
        {

            OLED_ShowString(50, 2, "^", 16);
            OLED_ShowString(50 + 24, 2, " ", 16);
            OLED_ShowString(50, 6, "   ", 16);
            EC11_Value = hour;
            Task_Status[11 + 2] = 1;
        }
        if (key_val == KEY_ENTER)
            EC11_Value++; //这里S2为增加 UP键为减少 防止EC11无法使用
        if (key_val == KEY_PREVIOUS)
            EC11_Value--;

        break;
    case Task_11_Child_2:
        if (key_val == KEY_NEXT)
        {

            OLED_ShowString(50, 2, " ", 16);
            OLED_ShowString(50 + 24, 2, "^", 16);
            OLED_ShowString(50 + 48, 2, " ", 16);
            EC11_Value = min;
            Task_Status[11 + 2] = 2;
        }
        if (key_val == KEY_ENTER)
            EC11_Value++;
        if (key_val == KEY_PREVIOUS)
            EC11_Value--;
        break;
    case Task_11_Child_3:
        if (key_val == KEY_NEXT)
        {

            OLED_ShowString(50 + 24, 2, " ", 16);
            OLED_ShowString(50 + 48, 2, "^", 16);
            OLED_ShowString(50, 6, " ", 16);
            EC11_Value = sec;
            Task_Status[11 + 2] = 3;
        }
        if (key_val == KEY_ENTER)
            EC11_Value++;
        if (key_val == KEY_PREVIOUS)
            EC11_Value--;
        break;
    case Task_11_Child_4:
        if (key_val == KEY_NEXT)
        {

            OLED_ShowString(50 + 48, 2, " ", 16);
            OLED_ShowString(50, 6, "^", 16);
            OLED_ShowString(50 + 24, 6, " ", 16);
            EC11_Value = year;
            Task_Status[11 + 2] = 4;
        }
        if (key_val == KEY_ENTER)
            EC11_Value++;
        if (key_val == KEY_PREVIOUS)
            EC11_Value--;
        break;
    case Task_11_Child_5:
        if (key_val == KEY_NEXT)
        {

            OLED_ShowString(50, 6, " ", 16);
            OLED_ShowString(50 + 24, 6, "^", 16);
            OLED_ShowString(50 + 48, 6, " ", 16);
            EC11_Value = month;
            Task_Status[11 + 2] = 5;
        }
        if (key_val == KEY_ENTER)
            EC11_Value++;
        if (key_val == KEY_PREVIOUS)
            EC11_Value--;
        break;
    case Task_11_Child_6:
        if (key_val == KEY_NEXT)
        {
            OLED_ShowString(50 + 24, 6, " ", 16);
            OLED_ShowString(50 + 48, 6, "^", 16);
            OLED_ShowString(50, 6, "   ", 16);
            EC11_Value = date;
            Task_Status[11 + 2] = 6;
        }
        if (key_val == KEY_ENTER)
            EC11_Value++;
        if (key_val == KEY_PREVIOUS)
            EC11_Value--;
        break;
    case Task_11_Child_7:
        if (key_val == KEY_NEXT || key_val == KEY_PREVIOUS)
        {
            Task_Status[11 + 2] = 7;
            OLED_ShowString(50 + 48, 6, " ", 16);
            OLED_ShowString(50, 6, "<-", 16);
            OLED_ShowString(50, 2, " ", 16);
            //EC11_Value=0;
        }
        if (key_val == KEY_ENTER)
        {
            if (RTC_Set_Time(hour, min, sec, ampm) == 0)
            {
                OLED_ShowString(0, 0, "Time:success", 16);
                sprintf(oledbuf, "Time:%2d:%2d:%2d", hour, min, sec);
                OLED_ShowString(0, 2, oledbuf, 16);
            }
            else
            {
                OLED_ShowString(0, 0, "Time:error", 16);
            }
            if (RTC_Set_Date(year, month, date, week) == 0)
            {
                OLED_ShowString(0, 4, "Date:success", 16);
                sprintf(oledbuf, "Date:%2d:%2d:%2d", year, month, date);
                OLED_ShowString(0, 6, oledbuf, 16);
            }
            else
            {
                OLED_ShowString(0, 4, "Date:error", 16);
            }
        }
        break;

    default:
        break;
    }
}
int buf_S = 0, buf_K = 0; //扇区，块区
uint8_t Task_813_FuncStatus = 0;
uint8_t RFIDKEY[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
uint8_t *RFIDDATA = "test"; //write data
void Task_813_Func(u8 page_index, u8 key_val)
{
    if (key_val == KEY_ENTER && Task_813_Option == page_index)
    {

        Extend_Init2(INIT_4P_RC522RFID);
    }

    if (Task_813_FuncStatus == 1 && page_index == Task_813_Child_1)
    {
        if (EC11_Value < 0) //限制旋钮范围
        {
            EC11_Value = 36;
        }
        else if (EC11_Value > 36)
        {
            EC11_Value = 0;
        }
        buf_S = EC11_Value; //存放调节结果
        sprintf(oledbuf, "S:%2d", buf_S);
        OLED_ShowString(0, 0, oledbuf, 16);
    }
    if (Task_813_FuncStatus == 2 && page_index == Task_813_Child_2)
    {
        if (EC11_Value < 0) //限制旋钮范围
        {
            EC11_Value = 36;
        }
        else if (EC11_Value > 36)
        {
            EC11_Value = 0;
        }
        buf_K = EC11_Value; //存放调节结果
        sprintf(oledbuf, "K:%2d", buf_K);
        OLED_ShowString(0, 2, oledbuf, 16);
    }
    /*if(RC_check())//检测成功LIE灯全亮 影响ec11调节
    {
        LED_1=LED_2=LED_3=LED_4=1;
    }else
    {
        LED_1=LED_2=LED_3=LED_4=0;
    }*/
    if (Task_813_Child_3 == page_index) //停留在read 自动读取
    {
        if (RC_check_read(RC_Get_address(buf_S, buf_K), RFIDKEY))
        {
            OLED_Clear();
            sprintf(oledbuf, "%s", RC_Get_buffer());
            OLED_ShowString(0, 0, oledbuf, 16);
        }
    }

    switch (page_index)
    {
    case Task_813_Option:

        sprintf(oledbuf, "S:%2d        ", buf_S);
        OLED_ShowString(0, 0, oledbuf, 16);
        sprintf(oledbuf, "K:%2d        ", buf_K);
        OLED_ShowString(0, 2, oledbuf, 16);
        OLED_ShowString(0, 4, "Read", 16);
        OLED_ShowString(0, 6, "Write", 16);
        GUI_MenuPoint(0);

        break;
    case Task_813_Child_1:
        if (key_val == KEY_NEXT)
        {

            GUI_MenuPoint(1);
            EC11_Value = buf_S;
            Task_813_FuncStatus = 1;
        }
        if (key_val == KEY_ENTER)
            EC11_Value++;
        if (key_val == KEY_PREVIOUS)
            EC11_Value--;
        break;
    case Task_813_Child_2:
        if (key_val == KEY_NEXT)
        {

            GUI_MenuPoint(2);
            EC11_Value = buf_K;
            Task_813_FuncStatus = 2;
        }
        if (key_val == KEY_ENTER)
            EC11_Value++;
        if (key_val == KEY_PREVIOUS)
            EC11_Value--;
        break;
    case Task_813_Child_3: //read
        if (key_val == KEY_NEXT)
        {
            Task_813_FuncStatus = 0;
            GUI_MenuPoint(3);
        }
        if (key_val == KEY_ENTER)
        {
            if (RC_check_read(RC_Get_address(buf_S, buf_K), RFIDKEY))
            {
                OLED_ShowString(0, 0, "Read OK", 16);
                sprintf(oledbuf, "%s", RC_Get_buffer());
                OLED_ShowString(0, 2, oledbuf, 16);
            }
            else
            {
                OLED_ShowString(0, 0, "Read Error", 16);
            }
        }
        break;
    case Task_813_Child_4: //write
        if (key_val == KEY_NEXT)
        {
            Task_813_FuncStatus = 0;
            GUI_MenuPoint(4);
        }
        if (key_val == KEY_ENTER)
        {
            if (RC_write(RC_Get_address(buf_S, buf_K), RFIDKEY, RFIDDATA))
            {
                OLED_ShowString(0, 0, "Write OK", 16);
            }
            else
            {
                OLED_ShowString(0, 0, "Write Error", 16);
            }
        }
        break;
    default:
        break;
    }
}

void Task_speechRecognition(void)
{

    /* while (1)
            {
                YY = ;
                if (YY == 0x17) //退出识别
                {
                    break;
                }

                */

    switch (XiaoChuang_ASR())//小创语音读取识别编号判断结果
    {
    case 0x12: //开灯
        RGB_Set(RGB_COLOR_WHITE);
        break;
    case 0x13: //关灯
        RGB_Set(RGB_COLOR_BLACK);
        break;
    case 0x14: //红色
        RGB_Set(RGB_COLOR_RED);
        break;
    case 0x15: //绿色
        RGB_Set(RGB_COLOR_GREEN);
        break;
    case 0x16: //蓝色
        RGB_Set(RGB_COLOR_BLUE);
        break;
    case 0x1F: //查询温湿度
        if (DHT11_Read_Data(&data_temp, &data_humi) == 0)
        {
            /*
                        18::当前::55031800
                        19::温度::55031900
                        1A::湿度::55031A00
                        1B::摄氏度::55031B00
                        1C::百分之::55031C00
                        1D::查询失败::55031D00
                        */
            UART2_Send(0x18);
            delay_ms(500);
            UART2_Send(0x19);
            delay_ms(500);
            XiaoChuang_PlayNUM(data_temp);
            //UART2_Send(0xA0+data_temp);
            delay_ms(500);
            UART2_Send(0x1B);
            delay_ms(500);
            UART2_Send(0x18);
            delay_ms(500);
            UART2_Send(0x1A);
            delay_ms(500);
            UART2_Send(0x1C);
            delay_ms(500);
            XiaoChuang_PlayNUM(data_humi);
            //UART2_Send(0xA0+data_humi);
        }
        else
        {
            UART2_Send(0x1D); //查询失败
            delay_ms(500);
        }
        break;
    case 0x21:
        BEE = 1;
        break;
    case 0x22:
        BEE = 0;
        break;
    default:
        break;
    }
}
