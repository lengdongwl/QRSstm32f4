
/**
  ******************************************************************************
  * @file    dma.c
  * @author  xie
  * @version V1
  * @date    2021��11��1��
  * @brief   DMA��ֱ�Ӵ洢�����ʣ��ײ�����
	
						�˿ڶ��壺
							��
  ******************************************************************************
**/
#include "dma.h"
#include "dac.h"

DMA_HandleTypeDef DAC1_DMA_Handler; //DMA���

//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_CHANNEL_0~DMA_CHANNEL_7
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx, u32 chx)
{
  if ((u32)DMA_Streamx > (u32)DMA2) //�õ���ǰstream������DMA2����DMA1
  {
    __HAL_RCC_DMA2_CLK_ENABLE(); //DMA2ʱ��ʹ��
  }
  else
  {
    __HAL_RCC_DMA1_CLK_ENABLE(); //DMA1ʱ��ʹ��
  }

  __HAL_LINKDMA(&DAC1_Handler, DMA_Handle1, DAC1_DMA_Handler); //��DMA��DAC1��ϵ����(����DMA)

  //Tx DMA����
  DAC1_DMA_Handler.Instance = DMA_Streamx; //������ѡ��
  DAC1_DMA_Handler.Init.Channel = chx;     //ͨ��ѡ��

  DAC1_DMA_Handler.Init.Direction = DMA_MEMORY_TO_PERIPH;              //�洢��������
  DAC1_DMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE;                  //���������ģʽ
  DAC1_DMA_Handler.Init.MemInc = DMA_MINC_ENABLE;                      //�洢������ģʽ
  DAC1_DMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; //�������ݳ���:8λ
  DAC1_DMA_Handler.Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;    //�洢�����ݳ���:8λ
  DAC1_DMA_Handler.Init.Mode = DMA_CIRCULAR;                           //����ѭ��ģʽ������Ϊ��ͨģʽ���γ�����
  DAC1_DMA_Handler.Init.Priority = DMA_PRIORITY_MEDIUM;                //�е����ȼ�
  DAC1_DMA_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;               //�ر�FIFOģʽ
  DAC1_DMA_Handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;       //�����ģ���Ϊ�Ѿ����ص���
  DAC1_DMA_Handler.Init.MemBurst = DMA_MBURST_SINGLE;                  //�洢��ͻ�����δ���
  DAC1_DMA_Handler.Init.PeriphBurst = DMA_PBURST_SINGLE;               //����ͻ�����δ���

  HAL_DMA_Init(&DAC1_DMA_Handler);
  //HAL_DMA_DeInit(&DAC1_DMA_Handler);
}

///�����ԣ��ú�������DMA���ж����ÿɼӿɲ��ӣ�ע�ͺ���Ҳ��Ӱ��
void MX_DMA_Init(void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}
