/**
  ******************************************************************************
  * @file    bh1750.c
  * @author  xie
  * @version V2
  * @date    2021��10��28��
  * @brief   bh1750��ǿ�ȴ�����
	
						�˿ڶ��壺
							PA8��IIC_SCL_BH1750 //BH1750 IIC����������
							PC9��IIC_SDA_BH1750 //BH1750 IIC��ʱ������	 
	******************************************************************************
**/	
#include "bh1750.h"
#include "delay.h"

#define	 SlaveAddress   0x46 			//����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
uint8_t  BUF[4];                  //�������ݻ�����
uint16_t illumination_data; 			//���ն�ȡֵ

/**
  * @brief  BH1750�˿ڳ�ʼ��
  * @param  None
  * @retval	None 
  * @note		None
  */
void BH1750_PortInit(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();           //����GPIOCʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_8;						//PA8
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);			//��ʼ���˿�
	
	  GPIO_Initure.Pin=GPIO_PIN_9;						//PC9
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);			//��ʼ���˿�

}

/**
  * @brief  SDA���ģʽ
  * @param  None
  * @retval	None 
  * @note		None
  */
void SDA_OUT(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();           //����GPIOCʱ��

		GPIO_Initure.Pin=GPIO_PIN_9;						//PC9
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);			//��ʼ���˿�
}

/**
  * @brief  SDA����ģʽ
  * @param  None
  * @retval	None 
  * @note		None
  */
void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();           //����GPIOCʱ��

		GPIO_Initure.Pin=GPIO_PIN_9;						//PC9
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  		//�������
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);			//��ʼ���˿�

}

/***************************************************************
** ���ܣ�     ����IIC��ʼ�ź�
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void BH1750_Start()
{
  SDA_OUT();     //sda�����
	IIC_SDA_BH1750=1;	  	  
	IIC_SCL_BH1750=1;
	delay_us(4);
 	IIC_SDA_BH1750=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_BH1750=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

/***************************************************************
** ���ܣ�     ����IICֹͣ�ź�
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void BH1750_Stop()
{
	SDA_OUT();//sda�����
	IIC_SDA_BH1750=0;//STOP:when CLK is high DATA change form low to high
	IIC_SCL_BH1750=1;
 	delay_us(4);
	IIC_SDA_BH1750=1;//����I2C���߽����ź�
	delay_us(4);
}

/***************************************************************
** ���ܣ�     ����IICӦ���ź�
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void BH1750_SendACK(uint8_t ack)
{
    SDA_OUT();          //sda�����
		if(ack)IIC_SDA_BH1750=1;   //дӦ���ź�
		else IIC_SDA_BH1750=0;          
    IIC_SCL_BH1750=1;          //����ʱ����
    delay_us(2);        //��ʱ
    IIC_SCL_BH1750=0;          //����ʱ����
    delay_us(2);        //��ʱ
}

/***************************************************************
** ���ܣ�     ����IIC�����ź�
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
uint8_t BH1750_RecvACK()
{
    uint8_t data;
	  SDA_IN();         //SDA����Ϊ����
    IIC_SCL_BH1750=1;        //����ʱ����
    delay_us(2);      //��ʱ
    data = READ_SDA_BH1750;  //��Ӧ���ź�
    IIC_SCL_BH1750=0;        //����ʱ����
    delay_us(2);      //��ʱ
    return data;
}

/***************************************************************
** ���ܣ�     ��IIC���߷���һ���ֽ�����
** ������	  dat��һ�ֽ�����
** ����ֵ��    ��
****************************************************************/
void BH1750_SendByte(uint8_t dat)
{
    uint8_t i,bit;
		SDA_OUT();               //sda�����
    for (i=0; i<8; i++)      //8λ������
    {
	    bit=dat&0x80;
		if(bit) IIC_SDA_BH1750=1;
		else IIC_SDA_BH1750=0;
        dat <<= 1;           //�Ƴ����ݵ����λ
        IIC_SCL_BH1750=1;           //����ʱ����
        delay_us(2);         //��ʱ
        IIC_SCL_BH1750=0;           //����ʱ����
        delay_us(2);         //��ʱ
    }
    BH1750_RecvACK();
}

/***************************************************************
** ���ܣ�     ��IIC���߽���һ���ֽ�����
** ������	  �޲���
** ����ֵ��   dat������һ�ֽ�����
****************************************************************/
uint8_t BH1750_RecvByte()
{
    uint8_t i;
    uint8_t dat = 0;
		SDA_IN();                 //SDA����Ϊ����
    IIC_SDA_BH1750=1;              //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)     //8λ������
    {
        dat <<= 1;
        IIC_SCL_BH1750=1;          //����ʱ����
        delay_us(2);        //��ʱ
		if(READ_SDA_BH1750) dat+=1;               
        IIC_SCL_BH1750=0;          //����ʱ����
        delay_us(2);        //��ʱ
    }
    return dat;
}

/***************************************************************
** ���ܣ�     ��bh1750д������
** ������	  �޲���
** ����ֵ��   ��
****************************************************************/
void Single_Write_BH1750(uint8_t REG_Address)
{
    BH1750_Start();                  //��ʼ�ź�
    BH1750_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    BH1750_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
    BH1750_Stop();                   //����ֹͣ�ź�
}

/***************************************************************
** ���ܣ�     ��������BH1750�ڲ����� 
** ������	  �޲���
** ����ֵ��   ��
****************************************************************/
void Multiple_Read_BH1750(void)
{   
		uint8_t i;	
    BH1750_Start();                          //��ʼ�ź�
    BH1750_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
	
		for (i=0; i<3; i++)                      //������ȡ2����ַ���ݣ��洢��BUF
			{
					BUF[i] = BH1750_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
					if (i == 3)
					{
						 BH1750_SendACK(1);                //���һ��������Ҫ��NOACK
					}
					else
					{		
						BH1750_SendACK(0);                //��ӦACK
				 }
		 }
    BH1750_Stop();                          //ֹͣ�ź�
}

/***************************************************************
** ���ܣ�     ��ʼ��BH1750
** ������	  �޲���
** ����ֵ��   ��
****************************************************************/
void BH1750_Init(void)
{
   BH1750_PortInit();
   Single_Write_BH1750(0x01);  
//   ADDR = 0;  						//��ADDRλ��ʼ������
}
void BH1750_DeInit(void)
{
  HAL_GPIO_DeInit(GPIOA,GPIO_PIN_8);
  HAL_GPIO_DeInit(GPIOC,GPIO_PIN_9);
}

/***************************************************************
** ���ܣ�     ��ȡ���ն�
** ������	  �޲���
** ����ֵ��   data�����ع��ն�ֵ
****************************************************************/
uint16_t Get_Bh_Value(void)
{  
	float temp;
	int dis_data;
	BUF[0]=0;BUF[1]=0;BUF[2]=0;BUF[3]=0;
	illumination_data=0;
    Single_Write_BH1750(0x01);			// power on
    Single_Write_BH1750(0x10);			// H- resolution mode
    Multiple_Read_BH1750();				// �����������ݣ��洢��BUF��
    dis_data=BUF[0];
    dis_data=(dis_data<<8)+BUF[1];		// �ϳ����ݣ�����������
    temp=(float)(dis_data/1.2);
		illumination_data=(int)temp;
    return illumination_data;//���ն�ȡֵ
}
/**
 * @description: ��ȡ���ն�
 * @param {*}
 * @return {*}����ֵ
 */
uint16_t TaskBoard_BH(void)
{
    int i, j, b;
    int buf[10];
    for (i = 0; i < 10; i++)
    {
        buf[i] = Get_Bh_Value();
        delay_ms(1);
    }
    for (i = 0; i < 10; i++) //����
    {
        for (j = 0; j < 10 - i - 1; j++)
        {
            if (buf[j] > buf[j + 1])
            {
                b = buf[j];
                buf[j] = buf[j + 1];
                buf[j + 1] = b;
            }
        }
    }
    //ȡ�м���+ƫ��
    return (buf[5] + buf[6]) / 2;
}