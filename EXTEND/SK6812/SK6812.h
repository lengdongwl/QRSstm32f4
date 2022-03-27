#ifndef __SK6812_H
#define __SK6812_H

#include "sys.h"
#define SK6812_IO PAout(1)

void SK6812_Init(void);
void SK6812_Set_1(void);
void SK6812_Set_0(void);
void SK6812_Reset(void);
//void SK6812_Handle(uint32_t G8R8B8);
void SK6812_Handle(uint8_t R_Data,uint8_t G_Data,uint8_t B_Data);

#endif
