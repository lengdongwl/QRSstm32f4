/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-19 15:27:11
 */
#ifndef _BH1750_H
#define _BH1750_H
#include "sys.h"

#define IIC_SCL_BH1750    PAout(8) //SCL 
#define IIC_SDA_BH1750    PCout(9) //SDA ‰≥ˆ	 
#define READ_SDA_BH1750   PCin(9)  //SDA ‰»Î 

void BH1750_Init(void);
void BH1750_DeInit(void);
uint16_t Get_Bh_Value(void);
void illumination_display(void);
uint16_t TaskBoard_BH(void);
#endif
