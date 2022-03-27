#ifndef __OLED_H_
#define __OLED_H_

#include "sys.h"

#define OLED_CMD  0		//д����
#define OLED_DATA 1		//д����    

#define OLED_DC 	PCout(2) 		//DC    	��������ѡ��˿�
#define OLED_RST 	PBout(15) 	//RES	 	��λ����
#define OLED_SCLK PBout(13)		//SCLK	 ʱ������
#define OLED_SDIN PCout(3) 		//SDIN	 �����������
#define OLED_CS 	PBout(12)		//CS		 Ƭѡ����


//OLED�����ú���
void OLED_Hardware_Init(void);
void OLED_WR_Byte(uint8_t dat,uint8_t cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Draw(uint8_t byte);

void OLED_ShowChar(uint16_t x,uint16_t y, uint8_t num, uint8_t size);
void OLED_ShowString(uint16_t x,uint16_t y,char *p, uint8_t size);
void OLED_ShowInt32Num(uint16_t x,uint16_t y, int32_t num, uint8_t len, uint8_t size);
void OLED_DrawFont16(uint16_t x, uint16_t y, char *s);
void OLED_DrawFont32(uint16_t x, uint16_t y, char *s);
void OLED_Show_Str(uint16_t x, uint16_t y, char *str,uint8_t size);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_Set_Pos(unsigned char x, unsigned char y); 

#endif

