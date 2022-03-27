/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-30 08:35:02
 */
#ifndef __DMA_H
#define __DMA_H
#include "sys.h"

extern DMA_HandleTypeDef  DAC1_DMA_Handler;      //DMA¾ä±ú

void MX_DMA_Init(void);

void MYDMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u32 chx);

#endif
