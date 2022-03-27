/*
 * @Description: RNG随机数发生器
 * @Autor: 309 Mushroom
 * @Date: 2021-12-13 14:07:54
 * @LastEditors: 309 Mushroom
 * @LastEditTime: 2021-12-20 10:40:56
 */
#include "RNG.h"
#include "delay.h"
#include "stm32f4xx.h"
void RNG_Init(void)
{  
    RCC->AHB2ENR|=1<<6;//使能RNG时钟
    //RNG->CR|=0<<3;//中断不使能
    RNG->CR|=1<<2;//使能随机发生器
}

void RNG_DeInit(void)
{
    
    RNG->CR|=0<<3;//中断不使能
    RNG->CR|=0<<2;//失能随机发生器
    RCC->AHB2ENR|=0<<6;//RNG时钟失能
}

/**
 * @description: 获取0-rand_max的值
 * @param {int} rand_max 最大值 但包括
 * @return {*}
 */
int RNG_GetRandom(int rand_max)
{
    int count=0;
    while (count<1000)
    {
        if ((RNG->SR&0x01)==1)//等待数据准备就绪
        {
            break;
        }
        delay_us(1000); 
        count++;
    }
    
    if (count<1000)
    {
        return RNG->DR % rand_max;
    }
    return 0;
}


/**
 * @description: 获取范围内的随机数
 * @param {int} rand_min
 * @param {int} rand_max
 * @return {*}
 */
int RNG_GetRandomRange(int rand_min,int rand_max)
{
    return RNG_GetRandom(rand_max - rand_min + 1) + rand_min; // randNumber 将被赋值为一个 MIN 和 MAX 范围内的随机数
}