/**
  ******************************************************************************
  * @file    dac.c
  * @author  xie
  * @version V1
  * @date    2021年11月1日
  * @brief   DAC模数转换驱动函数库
	
						端口定义：
						PA4：DAC_OUT 	DAC电压输出引脚
  ******************************************************************************
**/
#include "dac.h"
#include "math.h"
#include "dma.h"
#include "time.h"
#include "RNG.h"
#define TIM2_HCLK 84   //时钟频率
#define Table_Size 100 //(CLK_MHz /840 /(arr+1)=f)
#define PI 3.14159
#define DAC_DHR12R1_ADDRESS 0x40007408

u16 Table[Table_Size];

/**
 *  @brief 计算arr
 *  @param CLK_MHz 时钟频率 例:72MHZ
 *  @param KHz 欲输出频率 例:10 KHZ
 */
u16 calc_OutHZarr(int CLK_MHz, int KHz)
{
  return (int)(CLK_MHz * 1000 / Table_Size / KHz) - 1;
  ; //计算Hz 正弦波(Table_Size*2)
}

/**
 *  @brief 设置输出频率 时钟不分频
 *  @param Khz 欲输出频率 例:10 KHZ
 */
void DAC_SetHz(int KHz)
{
  TIM2_Init(calc_OutHZarr(TIM2_HCLK, KHz), 0);
}

void DAC_Init()
{
  MYDMA_Config(DMA1_Stream5, DMA_CHANNEL_7); //DMA的初始化函数
  TIM2_Init(10000, 0);                       //初始化定时器，DMA模式下要开启外部中断触发的设置
  DAC1_Init();                               //初始化DAC1
}

void DAC_DeInit(void)
{
  HAL_DMA_DeInit(&DAC1_DMA_Handler);
  HAL_TIM_Base_DeInit(&TIM2_Handler); //初始化定时器，DMA模式下要开启外部中断触发的设置
  HAL_DAC_DeInit(&DAC1_Handler);
}
/**
  * @brief  关闭输出
  * @param  None
  * @retval	None 
  * @note		None
  */
void Stop_Wave(void)
{
  for (uint16_t n = 0; n < Table_Size; n++)
  {
    Table[n] = 0;
  }
}

/**
  * @brief  正弦波产生
  * @param  None
  * @retval	None 
  * @note		None
  */
void Sine_Wave(uint16_t Vpp_Voltage)
{
  for (uint16_t n = 0; n < Table_Size; n++)
  {
    Table[n] = (sin(2 * PI * n / Table_Size) + 1) * (Vpp_Voltage / 2) * 1.216545;
  }
}

/**
  * @brief  三角波产生
  * @param  None
  * @retval	None 
  * @note		None
  */
void Triangular_Wave(uint16_t Vpp_Voltage)
{
  for (uint16_t n = 0; n < Table_Size; n++)
  {
    if (n > (Table_Size / 2))
    {
      Table[n] = 3100 - (((n - Table_Size / 2) * 2) * 12);
    }
    else
    {
      Table[n] = (n * 2) * 12;
    }
    Table[n] *= 1.072821847;
    Table[n] = Table[n] * ((float)Vpp_Voltage / 3300);
    Table[n] *= 1.216545;
  }
}

/**
  * @brief  方波产生
  * @param  Vpp_Voltage 峰峰值电压  Duty_Cycle占空比
  * @retval	None 
  * @note		None
  */
void Square_Wave(uint16_t Vpp_Voltage, uint8_t Duty_Cycle)
{
  for (uint16_t n = 0; n < Table_Size; n++)
  {
    if (n > ((Table_Size * Duty_Cycle) / 100))
    {
      Table[n] = Vpp_Voltage * 1.13;
    }
    else
    {
      Table[n] = 0;
    }
  }
}
#if 0
/**
 * @brief 波形生成函数 
 * @param type 波形类型
 * @param len 生成长度
 * @param vpp 峰峰值电压
 */
void wave_gen(waveType type, u16 len, u16 vpp)
{
  u16 base = 2048; // 偏置，保证负电压可以正常输出
  double amp;
  u16 i; // 循环变量
  if (vpp > 3300)
    vpp = 3300;                // 若输入的vpp超过范围，则限制在最大值
  amp = vpp / 3300.0 * 2047.0; // 根据峰峰值进行放缩

  if (type == WAVE_SINE) // 正弦波
  {
    for (i = 0; i < len; i++)
    {
      Table[i] = (u16)(base + sin((double)i / len * 6.283185307) * amp);
    }
  }
  else if (type == WAVE_RAMP) // 锯齿波
  {
    for (i = 0; i < len; i++)
    {
      Table[i] = (u16)(base + ((double)i / len - 0.5) * 2 * amp);
    }
  }
  else if (type == WAVE_SQUARE) // 方波
  {
    for (i = 0; i < len / 2; i++)
    {
      Table[i] = base + amp;
    }
    for (i = len / 2; i < len; i++)
    {
      Table[i] = base - amp;
    }
  }
  else if (type == WAVE_TRIANGLE) // 三角波
  {
    for (i = 0; i < len / 2; i++)
    {
      Table[i] = (u16)(base + ((double)i / len - 0.25) * 4 * amp);
    }
    for (i = len / 2; i < len; i++)
    {
      Table[i] = (u16)(base + ((double)(len - i) / len - 0.25) * 4 * amp);
    }
  }
}
#else
/**
 * @brief 波形生成函数 
 * @param type 波形类型
 * @param vpp 峰峰值电压
 */
void wave_gen(waveType type, u16 vpp)
{
  u16 base = 2048; // 偏置，保证负电压可以正常输出
  double amp;
  u16 i;          // 循环变量
  u16 buf1, buf2; //缓存数据
  if (vpp > 3300)
    vpp = 3300;                // 若输入的vpp超过范围，则限制在最大值
  amp = vpp / 3300.0 * 2047.0; // 根据峰峰值进行放缩

  if (type == WAVE_SINE) // 正弦波
  {
    for (i = 0; i < Table_Size; i++)
    {
      Table[i] = (u16)(base + sin((double)i / Table_Size * 6.283185307) * amp);
    }
  }
  else if (type == WAVE_RAMP) // 锯齿波
  {
    for (i = 0; i < Table_Size; i++)
    {
      Table[i] = (u16)(base + ((double)i / Table_Size - 0.5) * 2 * amp);
    }
  }
  else if (type == WAVE_SQUARE) // 方波
  {
    for (i = 0; i < Table_Size / 2; i++)
    {
      Table[i] = base + amp;
    }
    for (i = Table_Size / 2; i < Table_Size; i++)
    {
      Table[i] = base - amp;
    }
  }
  else if (type == WAVE_TRIANGLE) // 三角波
  {
    for (i = 0; i < Table_Size / 2; i++)
    {
      Table[i] = (u16)(base + ((double)i / Table_Size - 0.25) * 4 * amp);
    }
    for (i = Table_Size / 2; i < Table_Size; i++)
    {
      Table[i] = (u16)(base + ((double)(Table_Size - i) / Table_Size - 0.25) * 4 * amp);
    }
  }
  else if (type == WAVE_T) //梯形波
  {
    /*梯形 
    for (i = 0; i < (int)(Table_Size / 4); i++)
    {
      Table[i] = (u16)(base + ((double)i / Table_Size - 0.25) * 4 * amp);
    }
    buf1 = Table[i - 1];
    buf2 = i;
    for (i = buf2; i < (int)(Table_Size * 3 / 4); i++)
    {
      Table[i] = buf1;
    }
    for (i = (int)(Table_Size * 3 / 4); i < Table_Size; i++)
    {
      Table[i] = (u16)(base + ((double)(Table_Size - i) / Table_Size - 0.25) * 4 * amp);
    }*/

    //梯子形
    uint16_t Trapezoidal_Voltage=vpp/7,T=Table_Size/8+1;
    for(uint16_t n=0;n<Table_Size;n++){
      if(n<T){
        Table[n]=0;
      }else if(n<T*2){
        Table[n]=Trapezoidal_Voltage*1.216545*1;					
      }else if(n<T*3){
        Table[n]=Trapezoidal_Voltage*1.216545*2;					
      }else if(n<T*4){
        Table[n]=Trapezoidal_Voltage*1.216545*3;					
      }else if(n<T*5){
        Table[n]=Trapezoidal_Voltage*1.216545*4;					
      }else if(n<T*6){
        Table[n]=Trapezoidal_Voltage*1.216545*5;				
      }else if(n<T*7){
        Table[n]=Trapezoidal_Voltage*1.216545*6;					
      }else{
        Table[n]=Trapezoidal_Voltage*1.216545*7;					
      }
    }
  }
  else if (type == WAVE_Noise) //噪声波
  {
    for (i = 0; i < Table_Size; i++)
    {
      Table[i] = RNG_GetRandomRange(0, 4096);
    }
  }
  else if (type == WAVE_HALF) //半波
  {
    for(uint16_t n=0;n<Table_Size;n++)
    {		
      if((sin(2*PI*n/Table_Size)+1)*(vpp/2)>(vpp/2)){
        Table[n] = ((sin(2*PI*n/Table_Size)+1)*(vpp/2)-(vpp/2))*1.216545;
      }else{
        Table[n] = 0;
      }	
    }
  }
}
#endif

DAC_HandleTypeDef DAC1_Handler; //DAC句柄

//初始化DAC
void DAC1_Init(void)
{
  DAC_ChannelConfTypeDef DACCH1_Config;

  DAC1_Handler.Instance = DAC;

  HAL_DAC_Init(&DAC1_Handler); //初始化DAC

  DACCH1_Config.DAC_Trigger = DAC_TRIGGER_T2_TRGO;           //不使用触发功能
  DACCH1_Config.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE; //DAC1输出缓冲关闭

  HAL_DAC_ConfigChannel(&DAC1_Handler, &DACCH1_Config, DAC_CHANNEL_1); //DAC通道1配置

  HAL_DAC_Start_DMA(&DAC1_Handler, DAC_CHANNEL_1, (uint32_t *)Table, Table_Size, DAC_ALIGN_12B_R);
}

//DAC底层驱动，时钟配置，引脚 配置
//此函数会被HAL_DAC_Init()调用
//hdac:DAC句柄
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
  GPIO_InitTypeDef GPIO_Initure;
  if (hdac->Instance == DAC)
  {
    __HAL_RCC_DAC_CLK_ENABLE();           //使能DAC时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();         //开启GPIOA时钟
    GPIO_Initure.Pin = GPIO_PIN_4;        //PA4
    GPIO_Initure.Mode = GPIO_MODE_ANALOG; //模拟
    GPIO_Initure.Pull = GPIO_NOPULL;      //不带上下拉
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
  }
}

//设置通道1输出电压
//vol:0~3300,代表0~3.3V
void DAC1_Set_Vol(u16 vol)
{
  double temp = vol;
  temp /= 1000;
  temp = temp * 4096 / 3.3;
  HAL_DAC_SetValue(&DAC1_Handler, DAC_CHANNEL_1, DAC_ALIGN_12B_R, temp); //12位右对齐数据格式设置DAC值
}

/**
  * @brief  输出波形 Table_Size = 256
	* @param  Waveform:波形
						Frequency:频率
						VPP_Voltage:峰峰值
						Duty_Cycle:占空比
  * @retval	None 
  * @note		(除方波外其他的无法调节占空比)
  */
void OutPut_Waveform(
    uint8_t Waveform,     //波形
    uint16_t Frequency,   //频率/KHZ
    uint16_t VPP_Voltage, //峰峰值
    uint8_t Duty_Cycle    //占空比
)
{
  /*波形控制*/
  switch (Waveform)
  {
  /*方波*/
  case WAVE_SQUARE:
  {
    Square_Wave(VPP_Voltage, 100 - Duty_Cycle);
    break;
  }
  /*正弦*/
  case WAVE_SINE:
  {
    Sine_Wave(VPP_Voltage); //生成正弦波
    break;
  }
  /*三角*/
  case WAVE_TRIANGLE:
  {
    Triangular_Wave(VPP_Voltage);
    break;
  }
  }

  /*频率控制*/
  switch (Frequency)
  {
  case 1:
  {
    TIM2_Init(326, 0);
    break;
  }
  case 2:
  {
    TIM2_Init(162, 0);
    break;
  }
  case 3:
  {
    TIM2_Init(109, 0);
    break;
  }
  case 4:
  {
    TIM2_Init(81, 0);
    break;
  }
  case 5:
  {
    TIM2_Init(64, 0);
    break;
  }
  case 6:
  {
    TIM2_Init(54, 0);
    break;
  }
  case 7:
  {
    TIM2_Init(46, 0);
    break;
  }
  case 8:
  {
    TIM2_Init(40, 0);
    break;
  }
  case 9:
  {
    TIM2_Init(35, 0);
    break;
  }
  case 10:
  {
    TIM2_Init(32, 0);

    break;
  }
  }
}
