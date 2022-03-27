/**
  ******************************************************************************
  * @file    dac.c
  * @author  xie
  * @version V1
  * @date    2021��11��1��
  * @brief   DACģ��ת������������
	
						�˿ڶ��壺
						PA4��DAC_OUT 	DAC��ѹ�������
  ******************************************************************************
**/
#include "dac.h"
#include "math.h"
#include "dma.h"
#include "time.h"
#include "RNG.h"
#define TIM2_HCLK 84   //ʱ��Ƶ��
#define Table_Size 100 //(CLK_MHz /840 /(arr+1)=f)
#define PI 3.14159
#define DAC_DHR12R1_ADDRESS 0x40007408

u16 Table[Table_Size];

/**
 *  @brief ����arr
 *  @param CLK_MHz ʱ��Ƶ�� ��:72MHZ
 *  @param KHz �����Ƶ�� ��:10 KHZ
 */
u16 calc_OutHZarr(int CLK_MHz, int KHz)
{
  return (int)(CLK_MHz * 1000 / Table_Size / KHz) - 1;
  ; //����Hz ���Ҳ�(Table_Size*2)
}

/**
 *  @brief �������Ƶ�� ʱ�Ӳ���Ƶ
 *  @param Khz �����Ƶ�� ��:10 KHZ
 */
void DAC_SetHz(int KHz)
{
  TIM2_Init(calc_OutHZarr(TIM2_HCLK, KHz), 0);
}

void DAC_Init()
{
  MYDMA_Config(DMA1_Stream5, DMA_CHANNEL_7); //DMA�ĳ�ʼ������
  TIM2_Init(10000, 0);                       //��ʼ����ʱ����DMAģʽ��Ҫ�����ⲿ�жϴ���������
  DAC1_Init();                               //��ʼ��DAC1
}

void DAC_DeInit(void)
{
  HAL_DMA_DeInit(&DAC1_DMA_Handler);
  HAL_TIM_Base_DeInit(&TIM2_Handler); //��ʼ����ʱ����DMAģʽ��Ҫ�����ⲿ�жϴ���������
  HAL_DAC_DeInit(&DAC1_Handler);
}
/**
  * @brief  �ر����
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
  * @brief  ���Ҳ�����
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
  * @brief  ���ǲ�����
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
  * @brief  ��������
  * @param  Vpp_Voltage ���ֵ��ѹ  Duty_Cycleռ�ձ�
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
 * @brief �������ɺ��� 
 * @param type ��������
 * @param len ���ɳ���
 * @param vpp ���ֵ��ѹ
 */
void wave_gen(waveType type, u16 len, u16 vpp)
{
  u16 base = 2048; // ƫ�ã���֤����ѹ�����������
  double amp;
  u16 i; // ѭ������
  if (vpp > 3300)
    vpp = 3300;                // �������vpp������Χ�������������ֵ
  amp = vpp / 3300.0 * 2047.0; // ���ݷ��ֵ���з���

  if (type == WAVE_SINE) // ���Ҳ�
  {
    for (i = 0; i < len; i++)
    {
      Table[i] = (u16)(base + sin((double)i / len * 6.283185307) * amp);
    }
  }
  else if (type == WAVE_RAMP) // ��ݲ�
  {
    for (i = 0; i < len; i++)
    {
      Table[i] = (u16)(base + ((double)i / len - 0.5) * 2 * amp);
    }
  }
  else if (type == WAVE_SQUARE) // ����
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
  else if (type == WAVE_TRIANGLE) // ���ǲ�
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
 * @brief �������ɺ��� 
 * @param type ��������
 * @param vpp ���ֵ��ѹ
 */
void wave_gen(waveType type, u16 vpp)
{
  u16 base = 2048; // ƫ�ã���֤����ѹ�����������
  double amp;
  u16 i;          // ѭ������
  u16 buf1, buf2; //��������
  if (vpp > 3300)
    vpp = 3300;                // �������vpp������Χ�������������ֵ
  amp = vpp / 3300.0 * 2047.0; // ���ݷ��ֵ���з���

  if (type == WAVE_SINE) // ���Ҳ�
  {
    for (i = 0; i < Table_Size; i++)
    {
      Table[i] = (u16)(base + sin((double)i / Table_Size * 6.283185307) * amp);
    }
  }
  else if (type == WAVE_RAMP) // ��ݲ�
  {
    for (i = 0; i < Table_Size; i++)
    {
      Table[i] = (u16)(base + ((double)i / Table_Size - 0.5) * 2 * amp);
    }
  }
  else if (type == WAVE_SQUARE) // ����
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
  else if (type == WAVE_TRIANGLE) // ���ǲ�
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
  else if (type == WAVE_T) //���β�
  {
    /*���� 
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

    //������
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
  else if (type == WAVE_Noise) //������
  {
    for (i = 0; i < Table_Size; i++)
    {
      Table[i] = RNG_GetRandomRange(0, 4096);
    }
  }
  else if (type == WAVE_HALF) //�벨
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

DAC_HandleTypeDef DAC1_Handler; //DAC���

//��ʼ��DAC
void DAC1_Init(void)
{
  DAC_ChannelConfTypeDef DACCH1_Config;

  DAC1_Handler.Instance = DAC;

  HAL_DAC_Init(&DAC1_Handler); //��ʼ��DAC

  DACCH1_Config.DAC_Trigger = DAC_TRIGGER_T2_TRGO;           //��ʹ�ô�������
  DACCH1_Config.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE; //DAC1�������ر�

  HAL_DAC_ConfigChannel(&DAC1_Handler, &DACCH1_Config, DAC_CHANNEL_1); //DACͨ��1����

  HAL_DAC_Start_DMA(&DAC1_Handler, DAC_CHANNEL_1, (uint32_t *)Table, Table_Size, DAC_ALIGN_12B_R);
}

//DAC�ײ�������ʱ�����ã����� ����
//�˺����ᱻHAL_DAC_Init()����
//hdac:DAC���
void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
  GPIO_InitTypeDef GPIO_Initure;
  if (hdac->Instance == DAC)
  {
    __HAL_RCC_DAC_CLK_ENABLE();           //ʹ��DACʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();         //����GPIOAʱ��
    GPIO_Initure.Pin = GPIO_PIN_4;        //PA4
    GPIO_Initure.Mode = GPIO_MODE_ANALOG; //ģ��
    GPIO_Initure.Pull = GPIO_NOPULL;      //����������
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
  }
}

//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void DAC1_Set_Vol(u16 vol)
{
  double temp = vol;
  temp /= 1000;
  temp = temp * 4096 / 3.3;
  HAL_DAC_SetValue(&DAC1_Handler, DAC_CHANNEL_1, DAC_ALIGN_12B_R, temp); //12λ�Ҷ������ݸ�ʽ����DACֵ
}

/**
  * @brief  ������� Table_Size = 256
	* @param  Waveform:����
						Frequency:Ƶ��
						VPP_Voltage:���ֵ
						Duty_Cycle:ռ�ձ�
  * @retval	None 
  * @note		(���������������޷�����ռ�ձ�)
  */
void OutPut_Waveform(
    uint8_t Waveform,     //����
    uint16_t Frequency,   //Ƶ��/KHZ
    uint16_t VPP_Voltage, //���ֵ
    uint8_t Duty_Cycle    //ռ�ձ�
)
{
  /*���ο���*/
  switch (Waveform)
  {
  /*����*/
  case WAVE_SQUARE:
  {
    Square_Wave(VPP_Voltage, 100 - Duty_Cycle);
    break;
  }
  /*����*/
  case WAVE_SINE:
  {
    Sine_Wave(VPP_Voltage); //�������Ҳ�
    break;
  }
  /*����*/
  case WAVE_TRIANGLE:
  {
    Triangular_Wave(VPP_Voltage);
    break;
  }
  }

  /*Ƶ�ʿ���*/
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
