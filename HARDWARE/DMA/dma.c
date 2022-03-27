
/**
  ******************************************************************************
  * @file    dma.c
  * @author  xie
  * @version V1
  * @date    2021年11月1日
  * @brief   DMA（直接存储器访问）底层驱动
	
						端口定义：
							无
  ******************************************************************************
**/
#include "dma.h"
#include "dac.h"

DMA_HandleTypeDef DAC1_DMA_Handler; //DMA句柄

//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,@ref DMA_channel DMA_CHANNEL_0~DMA_CHANNEL_7
void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx, u32 chx)
{
  if ((u32)DMA_Streamx > (u32)DMA2) //得到当前stream是属于DMA2还是DMA1
  {
    __HAL_RCC_DMA2_CLK_ENABLE(); //DMA2时钟使能
  }
  else
  {
    __HAL_RCC_DMA1_CLK_ENABLE(); //DMA1时钟使能
  }

  __HAL_LINKDMA(&DAC1_Handler, DMA_Handle1, DAC1_DMA_Handler); //将DMA与DAC1联系起来(发送DMA)

  //Tx DMA配置
  DAC1_DMA_Handler.Instance = DMA_Streamx; //数据流选择
  DAC1_DMA_Handler.Init.Channel = chx;     //通道选择

  DAC1_DMA_Handler.Init.Direction = DMA_MEMORY_TO_PERIPH;              //存储器到外设
  DAC1_DMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE;                  //外设非增量模式
  DAC1_DMA_Handler.Init.MemInc = DMA_MINC_ENABLE;                      //存储器增量模式
  DAC1_DMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; //外设数据长度:8位
  DAC1_DMA_Handler.Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;    //存储器数据长度:8位
  DAC1_DMA_Handler.Init.Mode = DMA_CIRCULAR;                           //外设循环模式，设置为普通模式波形出不来
  DAC1_DMA_Handler.Init.Priority = DMA_PRIORITY_MEDIUM;                //中等优先级
  DAC1_DMA_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;               //关闭FIFO模式
  DAC1_DMA_Handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;       //随便设的，因为已经被关掉了
  DAC1_DMA_Handler.Init.MemBurst = DMA_MBURST_SINGLE;                  //存储器突发单次传输
  DAC1_DMA_Handler.Init.PeriphBurst = DMA_PBURST_SINGLE;               //外设突发单次传输

  HAL_DMA_Init(&DAC1_DMA_Handler);
  //HAL_DMA_DeInit(&DAC1_DMA_Handler);
}

///经测试，该函数即对DMA的中断设置可加可不加，注释好像也无影响
void MX_DMA_Init(void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}
