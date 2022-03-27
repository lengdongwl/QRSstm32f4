
#ifndef __DS18B20_H
#define __DS18B20_H
#include "sys.h"

//IO��������
#define DHT11_IO_IN()  {GPIOA->MODER&=~(3<<(1*2));GPIOA->MODER|=0<<(1*2);}	//PG9����ģʽ
#define DHT11_IO_OUT() {GPIOA->MODER&=~(3<<(1*2));GPIOA->MODER|=1<<(1*2);} 	//PG9���ģʽ
 
////IO��������											   
#define	DHT11_DQ_OUT    PAout(1)//���ݶ˿�	PA1
#define	DHT11_DQ_IN     PAin(1) //���ݶ˿�	PA1
   	
u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(u16 *temp,u16 *humi);//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11  
#endif
