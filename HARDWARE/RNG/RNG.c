/*
 * @Description: RNG�����������
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
    RCC->AHB2ENR|=1<<6;//ʹ��RNGʱ��
    //RNG->CR|=0<<3;//�жϲ�ʹ��
    RNG->CR|=1<<2;//ʹ�����������
}

void RNG_DeInit(void)
{
    
    RNG->CR|=0<<3;//�жϲ�ʹ��
    RNG->CR|=0<<2;//ʧ�����������
    RCC->AHB2ENR|=0<<6;//RNGʱ��ʧ��
}

/**
 * @description: ��ȡ0-rand_max��ֵ
 * @param {int} rand_max ���ֵ ������
 * @return {*}
 */
int RNG_GetRandom(int rand_max)
{
    int count=0;
    while (count<1000)
    {
        if ((RNG->SR&0x01)==1)//�ȴ�����׼������
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
 * @description: ��ȡ��Χ�ڵ������
 * @param {int} rand_min
 * @param {int} rand_max
 * @return {*}
 */
int RNG_GetRandomRange(int rand_min,int rand_max)
{
    return RNG_GetRandom(rand_max - rand_min + 1) + rand_min; // randNumber ������ֵΪһ�� MIN �� MAX ��Χ�ڵ������
}