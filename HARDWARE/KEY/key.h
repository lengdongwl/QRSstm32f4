

#ifndef _KEY_H
#define _KEY_H
#include "sys.h"

#define KEY_A PBin(2)     //����1  S1
#define KEY_B PBin(0)     //����2  S2
#define KEY_C PBin(1)     //����3  S3
#define EC11_KEY PEin(10) //EC11����

#define KEY_UP PAin(0)    //KEYUP
#define KEY_S1 PBin(2)    //����1  S1
#define KEY_S2 PBin(0)    //����2  S2
#define KEY_S3 PBin(1)    //����3  S3
void KEY_Init(void);      //LED��ʼ��
void KEY_Init_IT(void);
unsigned char KEY_Scan(u8 mode);      //��ȡ��ǰ����ֵ
#endif
