/*
 * @Description: OLED显示多级界面操作库
 * @Autor: 309 Mushroom
 * @Date: 2021-11-16 14:19:11
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2022-03-27 20:39:18
 */
#include "MYGUI.h"
#include "MYGUI_FUNC.h"
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
#include "RTC.h"
//char oledbuf[99];//oled显示字符串缓存区
//float ADC_V=0;//adc读取MCU内部电压
//short ADC_T=0;//adc读取MCU内部温度
float ds18b20=0,ultrasionic=0;
short bh1750=0;
uint8_t rtc_hour,rtc_min,rtc_sec,rtc_ampm;
static u8 func_index = Task_1;                        //当前页面索引值
static u8 last_index = Task_7;                        //上一个界面索引值
static void (*current_operation_func)(u8, u8);        //定义一个函数指针

//索引表
Main_Menu table[] =
    {
        //Cur_Index,previous,next,enter,back,(*current_operation)(u8,u8)
        //{当前索引,上一个索引，下一个索引，确认后操作索引，返回索引，当前索引操作函数(u8 parm1,u8 parm2)},
        //注:子级菜单的索引数组位置必须处于父级菜单下

        //主界面
        {Task_1, Task_11, Task_2, Task_1_Option, Task_1, Main_Menu_Func},
        {Task_2, Task_1, Task_3, Task_2_Option, Task_1, Main_Menu_Func},
        {Task_3, Task_2, Task_4, Task_3_Option, Task_1, Main_Menu_Func},
        {Task_4, Task_3, Task_5, Task_4_Option, Task_1, Main_Menu_Func},
        {Task_5, Task_4, Task_6, Task_5_Option, Task_1, Main_Menu_Func},
        {Task_6, Task_5, Task_7, Task_6_Option, Task_1, Main_Menu_Func},
        {Task_7, Task_6, Task_8, Task_7_Option, Task_1, Main_Menu_Func},
        {Task_8, Task_7, Task_9, Task_8_Option, Task_1, Main_Menu_Func},
        {Task_9, Task_8, Task_10, Task_9_Option, Task_1, Main_Menu_Func},
        {Task_10, Task_9, Task_11, Task_10_Option, Task_1, Main_Menu_Func},
        {Task_11, Task_10, Task_1, Task_11_Option, Task_1, Main_Menu_Func},
        //菜单1+菜单1子菜单
        {Task_1_Option, Task_1_Child_6, Task_1_Child_2, Task_1_Option, Task_1, Task_1_Func},
        {Task_1_Child_2, Task_1_Option, Task_1_Child_3, Task_1_Child_2, Task_1, Task_1_Func},
        {Task_1_Child_3, Task_1_Child_2, Task_1_Child_4, Task_1_Child_3, Task_1, Task_1_Func},
        {Task_1_Child_4, Task_1_Child_3, Task_1_Child_5, Task_1_Child_4, Task_1, Task_1_Func},
        {Task_1_Child_5, Task_1_Child_4, Task_1_Child_6, Task_1_Child_5, Task_1, Task_1_Func},
        {Task_1_Child_6, Task_1_Child_5, Task_1_Option, Task_1_Child_6, Task_1, Task_1_Func},
        //菜单2+菜单2子菜单
        {Task_2_Option, Task_2_Child_1, Task_2_Child_1, Task_2_Option, Task_2, Task_2_Func},
        {Task_2_Child_1,Task_2_Option,Task_2_Option,Task_2_Child_1,Task_2,Task_2_Func},

        {Task_3_Option,Task_3_Child_3,Task_3_Child_1,Task_3_Option,Task_3,Task_3_Func},
        {Task_3_Child_1,Task_3_Option,Task_3_Child_2,Task_3_Child_1,Task_3,Task_3_Func},
        {Task_3_Child_2,Task_3_Child_1,Task_3_Child_3,Task_3_Child_2,Task_3,Task_3_Func},
        {Task_3_Child_3,Task_3_Child_2,Task_3_Option,Task_3_Child_3,Task_3,Task_3_Func},

        {Task_4_Option,Task_4_Child_3,Task_4_Child_1,Task_4_Option,Task_4,Task_4_Func},
        {Task_4_Child_1,Task_4_Child_1,Task_4_Child_2,Task_4_Child_1,Task_4,Task_4_Func},//上键为更换数据所以指向自己
        {Task_4_Child_2,Task_4_Child_2,Task_4_Child_3,Task_4_Child_2,Task_4,Task_4_Func},//上键为更换数据所以指向自己
        {Task_4_Child_3,Task_4_Child_2,Task_4_Option,Task_4_Child_3,Task_4,Task_4_Func},

        {Task_5_Option,Task_5_Child_4,Task_5_Child_1,Task_5_Option,Task_5,Task_5_Func},
        {Task_5_Child_1,Task_5_Option,Task_5_Child_2,Task_5_Child_1,Task_5,Task_5_Func},
        {Task_5_Child_2,Task_5_Child_1,Task_5_Child_3,Task_5_Child_2,Task_5,Task_5_Func},
        {Task_5_Child_3,Task_5_Child_2,Task_5_Child_4,Task_5_Child_3,Task_5,Task_5_Func},
        {Task_5_Child_4,Task_5_Child_3,Task_5_Option,Task_5_Child_4,Task_5,Task_5_Func},

        {Task_6_Option,Task_6_Child_2,Task_6_Child_1,Task_6_Option,Task_6,Task_6_Func},
        {Task_6_Child_1,Task_6_Option,Task_6_Child_2,Task_6_Child_1,Task_6,Task_6_Func},
        {Task_6_Child_2,Task_6_Child_1,Task_6_Option,Task_6_Child_2,Task_6,Task_6_Func},

        {Task_7_Option,Task_7_Child_4,Task_7_Child_1,Task_7_Option,Task_7,Task_7_Func},
        {Task_7_Child_1,Task_7_Option,Task_7_Child_2,Task_7_Child_1,Task_7,Task_7_Func},
        {Task_7_Child_2,Task_7_Child_1,Task_7_Child_3,Task_7_Child_2,Task_7,Task_7_Func},
        {Task_7_Child_3,Task_7_Child_2,Task_7_Child_4,Task_7_Child_3,Task_7,Task_7_Func},
        {Task_7_Child_4,Task_7_Child_4,Task_7_Option,Task_7_Child_4,Task_7,Task_7_Func},//指向自己

        {Task_8_Option,Task_8_Child_2,Task_8_Child_1,Task_8_Option,Task_8,Task_8_Func},//任务8二级菜单
        {Task_8_Child_1,Task_8_Option,Task_8_Child_2,Task_82_init,Task_8,Task_8_Func},
        {Task_8_Child_2,Task_8_Child_1,Task_8_Option,Task_81_init,Task_8,Task_8_Func},

        {Task_81_Option,Task_81_Child_5,Task_81_Child_1,Task_81_Option,Task_8_Child_2,Task_81_Func},//PIN4三级菜单
        {Task_81_Child_1,Task_81_Option,Task_81_Child_2,Task_81_Child_1,Task_8_Child_2,Task_81_Func},
        {Task_81_Child_2,Task_81_Child_1,Task_81_Child_3,Task_81_Child_2,Task_8_Child_2,Task_81_Func},
        {Task_81_Child_3,Task_81_Child_2,Task_81_Child_4,Task_813_Option,Task_8_Child_2,Task_81_Func},
        {Task_81_Child_4,Task_81_Child_3,Task_81_Child_5,Task_81_Child_4,Task_8_Child_2,Task_81_Func},
        {Task_81_Child_5,Task_81_Child_4,Task_81_Option,Task_81_Child_5,Task_8_Child_2,Task_81_Func},
        {Task_81_init,Task_81_Option,Task_81_Option,Task_81_init,Task_8_Child_2,Task_81_Func},

        {Task_82_Option,Task_82_Child_2,Task_82_Child_1,Task_82_Option,Task_8_Child_1,Task_82_Func},//PIN3三级菜单
        {Task_82_Child_1,Task_82_Option,Task_82_Child_2,Task_82_Child_1,Task_8_Child_1,Task_82_Func},
        {Task_82_Child_2,Task_82_Child_1,Task_82_Option,Task_82_Child_2,Task_8_Child_1,Task_82_Func},
        {Task_82_init,Task_82_Option,Task_82_Option,Task_82_init,Task_8_Child_1,Task_82_Func},

        {Task_9_Option,Task_9_Child_2,Task_9_Child_1,Task_9_Option,Task_9,Task_9_Func},
        {Task_9_Child_1,Task_9_Option,Task_9_Child_2,Task_9_Child_1,Task_9,Task_9_Func},
        {Task_9_Child_2,Task_9_Child_1,Task_9_Option,Task_9_Child_2,Task_9,Task_9_Func},

        {Task_10_Option,Task_10_Child_2,Task_10_Child_1,Task_10_Option,Task_10,Task_10_Func},
        {Task_10_Child_1,Task_10_Option,Task_10_Child_2,Task_10_Child_1,Task_10,Task_10_Func},
        {Task_10_Child_2,Task_10_Child_1,Task_10_Option,Task_10_Child_2,Task_10,Task_10_Func},

        {Task_11_Option,Task_11_Child_7,Task_11_Child_1,Task_11_Option,Task_11,Task_11_Func},
        {Task_11_Child_1,Task_11_Child_1,Task_11_Child_2,Task_11_Child_1,Task_11,Task_11_Func},//指向自己
        {Task_11_Child_2,Task_11_Child_2,Task_11_Child_3,Task_11_Child_2,Task_11,Task_11_Func},//指向自己
        {Task_11_Child_3,Task_11_Child_3,Task_11_Child_4,Task_11_Child_3,Task_11,Task_11_Func},//指向自己
        {Task_11_Child_4,Task_11_Child_4,Task_11_Child_5,Task_11_Child_4,Task_11,Task_11_Func},//指向自己
        {Task_11_Child_5,Task_11_Child_5,Task_11_Child_6,Task_11_Child_5,Task_11,Task_11_Func},//指向自己
        {Task_11_Child_6,Task_11_Child_6,Task_11_Child_7,Task_11_Child_6,Task_11,Task_11_Func},//指向自己
        {Task_11_Child_7,Task_11_Child_6,Task_11_Option,Task_11_Child_7,Task_11,Task_11_Func},

        {Task_813_Option,Task_813_Child_4,Task_813_Child_1,Task_813_Option,Task_81_Child_3,Task_813_Func},//RFID
        {Task_813_Child_1,Task_813_Child_1,Task_813_Child_2,Task_813_Child_1,Task_81_Child_3,Task_813_Func},//指向自己
        {Task_813_Child_2,Task_813_Child_2,Task_813_Child_3,Task_813_Child_2,Task_81_Child_3,Task_813_Func},//指向自己
        {Task_813_Child_3,Task_813_Child_3,Task_813_Child_4,Task_813_Child_3,Task_813_Option,Task_813_Func},//指向自己
        {Task_813_Child_4,Task_813_Child_4,Task_813_Option,Task_813_Child_4,Task_813_Option,Task_813_Func},//指向自己
        //{Task_x_Option, Task_x_close, Task_x_close, Task_x_Option, Task_x, Task_x_Func},

};

int TIM5COUNT = 0;
/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5)
    {
        if (Task_Status[2]==ON)
        {
            BEE = !BEE;
        }
        
        HAL_TIM_Base_Start_IT(&TIM5_Handler);//中断后必须添加 否则无法接收下一次中断
    }
}*/
void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM5_Handler);
	/* if ((Task_Status[2]==ON &&KEY_S2==0))//&& (Task_2_Option==func_index || Task_2_Option==Task_81_Option))
    {
        BEE = !BEE;
    }else
    {
        BEE = 0;
    }*/
    if (Task_Status[1]==3)
    {
        /* code */
        if (TIM5COUNT<255)
        {
            RGB(TIM5COUNT,0,0);
        }else if (TIM5COUNT>=255&&TIM5COUNT<255*2)
        {
            RGB(255,TIM5COUNT-255,0);
        }else if (TIM5COUNT>=255*2&&TIM5COUNT<255*3)
        {
            RGB(255,255,TIM5COUNT-255*2);
        }else 
        {
            Task_Status[1]=0;
            RGB(0,0,0);
            TIM5_DeInit();
        }
        
    }
    
    if(Task_2_Option==func_index)
    {

        if ((Task_Status[2]==ON ))//&&KEY_S2==0)) //加上即可按住响应放开停止
        {
             BEE = !BEE;
        }else //这里有bug 可能被切换菜单会不停蜂鸣器
        {
            BEE=0;
        }
    }
    if (Task_Status[7]==2)//数码管显示年份
    {
        switch (TIM5COUNT)
        {
        case 0:
            SMG_Display(1921, 1);
            break;
        case 1:
            SMG_Display(1927, 1);
            break;
        case 2:
            SMG_Display(1949, 1);
            break;
        case 3:
            SMG_Display(2021, 1);
            break;        
        default:
            Task_Status[7]=0;
            TIM5_DeInit();
            SMG_DeInit();
            break;
        }    
    }
    if (Task_Status[7]==3)//数码管显示年份
    {
        RTC_Get_Time(&rtc_hour,&rtc_min,&rtc_sec,&rtc_ampm);
        SMG_Display(rtc_min/10*1000 + rtc_min%10*100 + rtc_sec/10*10 + rtc_sec%10,1);
    }
    if(Task_5_Option==func_index && Task_Status[5] == ON)//全部传感器
    {
        ds18b20=DS18B20_Get_Temp();
        //ultrasionic=Ultrasonic_Distance();测距由外部中断读取数据不可放在时钟中断
        bh1750=Get_Bh_Value();
    }
    if(Task_5_Child_1==func_index && Task_Status[5] == 2)//温度
    {
        ds18b20=DS18B20_Get_Temp();
    }
    
    if((Task_5_Child_3==func_index && Task_Status[5] == 4)||func_index==Task_6_Child_2)//光照度
    {
        bh1750=Get_Bh_Value();
    }
    if(func_index==Task_5_Child_3&&Task_Status[2]==ON)//测距
    {
        BEE=!BEE;
        if (BEE==1)
        {
            RGB_Set(RGB_COLOR_RED);
        }else
        {
            RGB_Set(RGB_COLOR_BLACK);
        }
    }
    if (func_index==Task_6_Child_2&&Task_Status[2]==2)
    {
        Task_speechRecognition();
    }
    


    TIM5COUNT++;
}

/*
函数功能：刷新界面
参数：无
返回值：无
*/
#if MODE==3
void GUI_Refresh(void)
{
    u8 key_val = KEY_Scan(0);
    if (key_val != 0) //只有按键按下才刷屏
    {
        last_index = func_index; //更新上一界面索引值
        switch (key_val)
        {
        case KEY_PREVIOUS:
            func_index = table[func_index].previous; //更新索引值
            LED_1=1;
            LED_2=LED_3=LED_4=0;
            break;
        case KEY_ENTER:
            OLED_Clear(); //清屏
            func_index = table[func_index].enter; //更新索引值
            LED_1=LED_2=LED_4=0;
            LED_3=1;
            break;
        case KEY_NEXT:
            func_index = table[func_index].next; //更新索引值
            LED_2=1;
            LED_1=LED_3=LED_4=0;
            break;
        case KEY_BACK:
            func_index = table[func_index].back; //更新索引值
            LED_1=LED_2=LED_3=0;
            LED_4=1;
            break;
        default:
            break;
        }
        //OLED_Clear(); //清屏
    }
    current_operation_func = table[func_index].current_operation;
    (*current_operation_func)(func_index, key_val); //执行当前索引对应的函数
}
#else

void GUI_Refresh(void)
{
    u8 key_val = KEY_Scan(0);
    if (key_val != 0) //只有按键按下才刷屏
    {
        last_index = func_index; //更新上一界面索引值
        switch (key_val)
        {
        case KEY_PREVIOUS:
            func_index = table[func_index].previous; //更新索引值
            break;
        case KEY_NEXT:
            func_index = table[func_index].next; //更新索引值
            break;
        case KEY_ENTER:
            OLED_Clear(); //清屏
            func_index = table[func_index].enter; //更新索引值
            break;    
        case KEY_BACK:
            func_index = table[func_index].back; //更新索引值
            break;
        case KEY_S4_EC11:
            key_val=KEY_ENTER;
            OLED_Clear(); //清屏
            func_index = table[func_index].enter; //更新索引值
            break;
        default:
            break;
        }
        //OLED_Clear(); //清屏
    }
    if (key_val==0 
            && func_index!=Task_4_Child_2 
                &&func_index!=Task_4_Child_1 
                    &&func_index!=Task_3_Child_3
                        &&(func_index<Task_11_Option||func_index>Task_11_Child_7))//按键优先级更高，子菜单使用旋钮时 过滤旋钮切换菜单
    {  
        if (EC11_Value>0)//正转
        {  
            key_val=KEY_NEXT;
            func_index = table[func_index].next; //更新索引值
            EC11_Value=0;
        }else if (EC11_Value<0)//反转
        {
            key_val=KEY_PREVIOUS;
            func_index = table[func_index].previous; //更新索引值
            EC11_Value=0;
        }
    }
    current_operation_func = table[func_index].current_operation;
    (*current_operation_func)(func_index, key_val); //执行当前索引对应的函数
}
#endif

/**
 * @description: 菜单指针 
 * @param {uint8_t} position 位置1-4 其余的为清空菜单指针
 * @return {*}
 */
void GUI_MenuPoint(uint8_t position)
{
    
    switch (position)
    {
    case 0:
        OLED_Show_Str(7*16, 0, "  ", 16);
        OLED_Show_Str(7*16, 2, "  ", 16);
        OLED_Show_Str(7*16, 4, "  ", 16);
        OLED_Show_Str(7*16, 6, "  ", 16);
        break;    
    case 1:
        OLED_Show_Str(7*16, 0, "<-", 16);
        OLED_Show_Str(7*16, 2, "  ", 16);
        OLED_Show_Str(7*16, 4, "  ", 16);
        OLED_Show_Str(7*16, 6, "  ", 16);
        break;
    case 2:
        OLED_Show_Str(7*16, 2, "<-", 16);
        OLED_Show_Str(7*16, 0, "  ", 16);
        OLED_Show_Str(7*16, 4, "  ", 16);
        OLED_Show_Str(7*16, 6, "  ", 16);
        break;
    case 3:
        OLED_Show_Str(7*16, 4, "<-", 16);
        OLED_Show_Str(7*16, 2, "  ", 16);
        OLED_Show_Str(7*16, 0, "  ", 16);
        OLED_Show_Str(7*16, 6, "  ", 16);
        break;    
    case 4:
        OLED_Show_Str(7*16, 6, "<-", 16);
        OLED_Show_Str(7*16, 2, "  ", 16);
        OLED_Show_Str(7*16, 4, "  ", 16);
        OLED_Show_Str(7*16, 0, "  ", 16);
    break;    
    default:
        break;
    }
}
#if 0
/*
函数功能：主菜单显示函数
参数：u8 page_index,u8 key_val
返回值：无
*/
void Main_Menu_Func(u8 page_index, u8 key_val)
{
    if (key_val != 0)
    {
        OLED_Clear(); //清屏
    }

    switch (page_index) //显示当前所在位置
    {
    case Task_1:
        OLED_Show_Str(0, 0, "三色灯     ", 16);
        OLED_Show_Str(0, 2, "BEEP       ", 16);
        OLED_Show_Str(0, 4, "ADC采集    ", 16);
        OLED_Show_Str(0, 6, "DAC输出    ", 16);
        GUI_MenuPoint(1);
        break;
    case Task_2:
        OLED_Show_Str(0, 0, "三色灯     ", 16);
        OLED_Show_Str(0, 2, "BEEP       ", 16);
        OLED_Show_Str(0, 4, "ADC采集    ", 16);
        OLED_Show_Str(0, 6, "DAC输出    ", 16);
        GUI_MenuPoint(2);
        break;
    case Task_3:
        OLED_Show_Str(0, 0, "三色灯     ", 16);
        OLED_Show_Str(0, 2, "BEEP       ", 16);
        OLED_Show_Str(0, 4, "ADC采集    ", 16);
        OLED_Show_Str(0, 6, "DAC输出    ", 16);
        GUI_MenuPoint(3);
        break;
    case Task_4:
        OLED_Show_Str(0, 0, "三色灯     ", 16);
        OLED_Show_Str(0, 2, "BEEP       ", 16);
        OLED_Show_Str(0, 4, "ADC采集    ", 16);
        OLED_Show_Str(0, 6, "DAC输出    ", 16);
        GUI_MenuPoint(4);
        break;
    case Task_5:
        OLED_Show_Str(0, 0, "传感器     ", 16);
        OLED_Show_Str(0, 2, "语音交互   ", 16);
        OLED_Show_Str(0, 4, "数码管     ", 16);
        OLED_Show_Str(0, 6, "扩展模块   ", 16);
        GUI_MenuPoint(1);
        break;
    case Task_6:
        OLED_Show_Str(0, 0, "传感器     ", 16);
        OLED_Show_Str(0, 2, "语音交互   ", 16);
        OLED_Show_Str(0, 4, "数码管     ", 16);
        OLED_Show_Str(0, 6, "扩展模块   ", 16);
        GUI_MenuPoint(2);
        break;
    case Task_7:
        OLED_Show_Str(0, 0, "传感器     ", 16);
        OLED_Show_Str(0, 2, "语音交互   ", 16);
        OLED_Show_Str(0, 4, "数码管     ", 16);
        OLED_Show_Str(0, 6, "扩展模块   ", 16);
        GUI_MenuPoint(3);
        break;
    case Task_8:
        OLED_Show_Str(0, 0, "传感器     ", 16);
        OLED_Show_Str(0, 2, "语音交互   ", 16);
        OLED_Show_Str(0, 4, "数码管     ", 16);
        OLED_Show_Str(0, 6, "扩展模块   ", 16);
        GUI_MenuPoint(4);
        break;    
    case Task_9:
        OLED_Show_Str(0, 0, "OLED显示   ", 16);
        OLED_Show_Str(0, 2, "Other      ", 16);
        OLED_Show_Str(0, 4, "SetRTC     ", 16);
        GUI_MenuPoint(1);
        break; 
    case Task_10:
        OLED_Show_Str(0, 0, "OLED显示   ", 16);
        OLED_Show_Str(0, 2, "Other      ", 16);
        OLED_Show_Str(0, 4, "SetRTC     ", 16);
        GUI_MenuPoint(2);
        break;     
    case Task_11:
        OLED_Show_Str(0, 0, "OLED显示   ", 16);
        OLED_Show_Str(0, 2, "Other      ", 16);
        OLED_Show_Str(0, 4, "SetRTC     ", 16);
        GUI_MenuPoint(3);
        break;     
    default:
        break;
    }
}

#else

#define MenuCount 11 //主菜单数量
#define MenuPage 4//每页菜单的数量
static char MenuName[MenuCount][16]=
{
"三色灯     ",
"BEEP       ",
"ADC采集    ",
"DAC输出    ",
"传感器     ",
"语音交互   ",
"数码管     ",
"扩展模块   ",
"OLED显示   ",
"Other      ",
"SetRTC     ",
};
void Main_Menu_Func(u8 page_index, u8 key_val)
{
    if (key_val != 0)
    {
        OLED_Clear(); //清屏
    }

    if (page_index<MenuCount) //显示当前所在位置
    {
			  
				int i,y,k=page_index%MenuPage;
				if(MenuCount-page_index>3)
				{

					for(i=0,y=0;i<MenuPage;i++,y+=2)
					{
							//printf("%s\n",MenuName[page_index+i]);
							OLED_Show_Str(0, y, MenuName[page_index+i-k], 16);
					}
					
				}else
				{
					//printf("s = %d\n",MenuCount-page_index-1);
					for(i=0,y=0;i<MenuCount%MenuPage;i++,y+=2)
					{
							//printf("%s\n",MenuName[MenuCount-MenuCount%4+i]);
							OLED_Show_Str(0, y, MenuName[MenuCount-MenuCount%MenuPage+i], 16);
					}
					
				}
				GUI_MenuPoint(page_index%MenuPage+1);//printf("p = %d\n",page_index%4+1);
				//
    }
		
}

#endif

