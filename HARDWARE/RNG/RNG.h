/*
 * @Description: RNG随机数发生器
 * @Autor: 309 Mushroom
 * @Date: 2021-12-13 14:07:54
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-12-13 15:22:04
 */
#ifndef __RNG_H  
#define __RNG_H   
void RNG_Init(void);
void RNG_DeInit(void);
int RNG_GetRandom(int rand_max);
int RNG_GetRandomRange(int rand_min,int rand_max);

#endif
