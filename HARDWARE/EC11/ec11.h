/*
 * @Description: 
 * @Autor: 309 Mushroom
 * @Date: 2021-11-03 09:49:58
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-11-19 09:47:10
 */

#ifndef _EC11_H
#define _EC11_H
#include "sys.h"
 
#define EC11_A PEin(9)  
#define EC11_B PEin(11)  
extern int EC11_Value;
void EC11_Init(void);
void EC11_DeInit(void);
int EC11_Get_Value(void);
#endif
