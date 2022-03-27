/**
  ******************************************************************************
  * @file    MLX90614.c
  * @author  xie
  * @version V1
  * @date    2021年11月2日
  * @brief   扩展端口模块红外测温传感器函数库
	
						端口定义：
						PC12:	MLX90614_IIC_SCL		MLX90614 IIC数据脚	
						PD2:	MLX90614_IIC_SDA		MLX90614 IIC时钟脚	
							
							
  ******************************************************************************
**/	
#include "MLX90614.h"
#include "delay.h"
#include <stdio.h>



/*******************************************************************************
* Function Name  : SMBus_StartBit
* Description    : Generate START condition on SMBus
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_StartBit(void)
{
    SMBUS_SDA_H();		// Set SDA line
    SMBus_Delay(5);	    // Wait a few microseconds
    SMBUS_SCK_H();		// Set SCL line
    SMBus_Delay(5);	    // Generate bus free time between Stop
    SMBUS_SDA_L();		// Clear SDA line
    SMBus_Delay(5);	    // Hold time after (Repeated) Start
    // Condition. After this period, the first clock is generated.
    //(Thd:sta=4.0us min)在SCK=1时，检测到SDA由1到0表示通信开始（下降沿）
    SMBUS_SCK_L();	    // Clear SCL line
    SMBus_Delay(5);	    // Wait a few microseconds
}

/*******************************************************************************
* Function Name  : SMBus_StopBit
* Description    : Generate STOP condition on SMBus
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_StopBit(void)
{
    SMBUS_SCK_L();		// Clear SCL line
    SMBus_Delay(5);	    // Wait a few microseconds
    SMBUS_SDA_L();		// Clear SDA line
    SMBus_Delay(5);	    // Wait a few microseconds
    SMBUS_SCK_H();		// Set SCL line
    SMBus_Delay(5);	    // Stop condition setup time(Tsu:sto=4.0us min)
    SMBUS_SDA_H();		// Set SDA line在SCK=1时，检测到SDA由0到1表示通信结束（上升沿）
}

/*******************************************************************************
* Function Name  : SMBus_SendByte
* Description    : Send a byte on SMBus
* Input          : Tx_buffer
* Output         : None
* Return         : None
*******************************************************************************/
u8 SMBus_SendByte(u8 Tx_buffer)
{
    u8	Bit_counter;
    u8	Ack_bit;
    u8	bit_out;

    for(Bit_counter=8; Bit_counter; Bit_counter--)
    {
        if (Tx_buffer&0x80)
        {
            bit_out=1;   // If the current bit of Tx_buffer is 1 set bit_out
        }
        else
        {
            bit_out=0;  // else clear bit_out
        }
        SMBus_SendBit(bit_out);		// Send the current bit on SDA
        Tx_buffer<<=1;				// Get next bit for checking
    }

    Ack_bit=SMBus_ReceiveBit();		// Get acknowledgment bit
    return	Ack_bit;
}

/*******************************************************************************
* Function Name  : SMBus_SendBit
* Description    : Send a bit on SMBus 82.5kHz
* Input          : bit_out
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_SendBit(u8 bit_out)
{
    if(bit_out==0)
    {
        SMBUS_SDA_L();
    }
    else
    {
        SMBUS_SDA_H();
    }
    SMBus_Delay(2);					// Tsu:dat = 250ns minimum
    SMBUS_SCK_H();					// Set SCL line
    SMBus_Delay(6);					// High Level of Clock Pulse
    SMBUS_SCK_L();					// Clear SCL line
    SMBus_Delay(3);					// Low Level of Clock Pulse
    //	SMBUS_SDA_H(1);				    // Master release SDA line ,
    return;
}

/*******************************************************************************
* Function Name  : SMBus_ReceiveBit
* Description    : Receive a bit on SMBus
* Input          : None
* Output         : None
* Return         : Ack_bit
*******************************************************************************/
u8 SMBus_ReceiveBit(void)
{
    u8 Ack_bit;

    SMBUS_SDA_H();          //引脚靠外部电阻上拉，当作输入
    SMBus_Delay(2);			// High Level of Clock Pulse
    SMBUS_SCK_H();			// Set SCL line
		MLX90614_SDA_IN();
    SMBus_Delay(5);			// High Level of Clock Pulse
    if (SMBUS_SDA_PIN())
    {
        Ack_bit=1;
    }
    else
    {
        Ack_bit=0;
    }
    SMBUS_SCK_L();			// Clear SCL line
    SMBus_Delay(3);			// Low Level of Clock Pulse
		MLX90614_SDA_OUT();

    return	Ack_bit;
}

/*******************************************************************************
* Function Name  : SMBus_ReceiveByte
* Description    : Receive a byte on SMBus
* Input          : ack_nack
* Output         : None
* Return         : RX_buffer
*******************************************************************************/
u8 SMBus_ReceiveByte(u8 ack_nack)
{
    u8 	RX_buffer;
    u8	Bit_Counter;

    for(Bit_Counter=8; Bit_Counter; Bit_Counter--)
    {
        if(SMBus_ReceiveBit())			// Get a bit from the SDA line
        {
            RX_buffer <<= 1;			// If the bit is HIGH save 1  in RX_buffer
            RX_buffer |=0x01;
        }
        else
        {
            RX_buffer <<= 1;			// If the bit is LOW save 0 in RX_buffer
            RX_buffer &=0xfe;
        }
    }
    SMBus_SendBit(ack_nack);			// Sends acknowledgment bit
    return RX_buffer;
}

/*******************************************************************************
* Function Name  : SMBus_Delay
* Description    : 延时  一次循环约1us
* Input          : time
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_Delay(u16 time)
{
    delay_us(time);
}

/*******************************************************************************
* Function Name  : SMBus_Init
* Description    : SMBus初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MLX90614_Init(void)
{
		GPIO_InitTypeDef GPIO_Initure;
     
    __HAL_RCC_GPIOC_CLK_ENABLE();           		//开启GPIOC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();           		//开启GPIOD时钟
		
		/*注：红外测温的IIC接口要配置成开漏输出*/
    GPIO_Initure.Pin=GPIO_PIN_12;								//PC12
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  		//开漏输出
    GPIO_Initure.Pull=GPIO_PULLUP;          		//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;   			//高速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);	

    GPIO_Initure.Pin=GPIO_PIN_2;								//PD2
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_OD;  		//开漏输出
    GPIO_Initure.Pull=GPIO_PULLUP;          		//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;    			//高速
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);		
    	
//    GPIO_InitTypeDef  GPIO_InitStructure;

//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);	// 使能PA端口时钟

//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;						// A6
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;			// 
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	// IO口速度为100MHz
//    GPIO_Init(GPIOH, &GPIO_InitStructure);							// 推挽输出 ，IO口速度为50MHz

//    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;				// PA4 5   Data  Clk
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;			// 设置成上拉输入
//    GPIO_Init(GPIOH, &GPIO_InitStructure);						// 初始化GPIOA4 5

    SMBUS_SCK_H();
    SMBUS_SDA_H();
}

/*******************************************************************************
 * Function Name  : SMBus_ReadMemory
 * Description    : READ DATA FROM RAM/EEPROM
 * Input          : slaveAddress, command
 * Output         : None
 * Return         : Data
*******************************************************************************/
u16 SMBus_ReadMemory(u8 slaveAddress, u8 command)
{
    u16 data;			// Data storage (DataH:DataL)
    u8 Pec;				// PEC byte storage
    u8 DataL=0;			// Low data byte storage
    u8 DataH=0;			// High data byte storage
    u8 arr[6];			// Buffer for the sent bytes
    u8 PecReg;			// Calculated PEC byte storage
    u8 ErrorCounter;	// Defines the number of the attempts for communication with MLX90614

    ErrorCounter=0x00;				// Initialising of ErrorCounter
    slaveAddress <<= 1;	//2-7位表示从机地址

    do
    {
repeat:
        SMBus_StopBit();			    //If slave send NACK stop comunication
        --ErrorCounter;				    //Pre-decrement ErrorCounter
        if(!ErrorCounter) 			    //ErrorCounter=0?
        {
            break;					    //Yes,go out from do-while{}
        }

        SMBus_StartBit();				//Start condition
        if(SMBus_SendByte(slaveAddress))//Send SlaveAddress 最低位Wr=0表示接下来写命令
        {
            goto	repeat;			    //Repeat comunication again
        }
        if(SMBus_SendByte(command))	    //Send command
        {
            goto	repeat;		    	//Repeat comunication again
        }

        SMBus_StartBit();					//Repeated Start condition
        if(SMBus_SendByte(slaveAddress+1))	//Send SlaveAddress 最低位Rd=1表示接下来读数据
        {
            goto	repeat;             	//Repeat comunication again
        }

        DataL = SMBus_ReceiveByte(ACK);	//Read low data,master must send ACK
        DataH = SMBus_ReceiveByte(ACK); //Read high data,master must send ACK
        Pec = SMBus_ReceiveByte(NACK);	//Read PEC byte, master must send NACK
        SMBus_StopBit();				//Stop condition

        arr[5] = slaveAddress;		//
        arr[4] = command;			//
        arr[3] = slaveAddress+1;	//Load array arr
        arr[2] = DataL;				//
        arr[1] = DataH;				//
        arr[0] = 0;					//
        PecReg=PEC_Calculation(arr);//Calculate CRC
    }
    while(PecReg != Pec);		//If received and calculated CRC are equal go out from do-while{}

    data = (DataH<<8) | DataL;	//data=DataH:DataL
    return data;
}

/*******************************************************************************
* Function Name  : PEC_calculation
* Description    : Calculates the PEC of received bytes
* Input          : pec[]
* Output         : None
* Return         : pec[0]-this byte contains calculated crc value
*******************************************************************************/
u8 PEC_Calculation(u8 pec[])
{
    u8 	crc[6];
    u8	BitPosition=47;
    u8	shift;
    u8	i;
    u8	j;
    u8	temp;

    do
    {
        /*Load pattern value 0x000000000107*/
        crc[5]=0;
        crc[4]=0;
        crc[3]=0;
        crc[2]=0;
        crc[1]=0x01;
        crc[0]=0x07;

        /*Set maximum bit position at 47 ( six bytes byte5...byte0,MSbit=47)*/
        BitPosition=47;

        /*Set shift position at 0*/
        shift=0;

        /*Find first "1" in the transmited message beginning from the MSByte byte5*/
        i=5;
        j=0;
        while((pec[i]&(0x80>>j))==0 && i>0)
        {
            BitPosition--;
            if(j<7)
            {
                j++;
            }
            else
            {
                j=0x00;
                i--;
            }
        }/*End of while */

        /*Get shift value for pattern value*/
        shift=BitPosition-8;

        /*Shift pattern value */
        while(shift)
        {
            for(i=5; i<0xFF; i--)
            {
                if((crc[i-1]&0x80) && (i>0))
                {
                    temp=1;
                }
                else
                {
                    temp=0;
                }
                crc[i]<<=1;
                crc[i]+=temp;
            }/*End of for*/
            shift--;
        }/*End of while*/

        /*Exclusive OR between pec and crc*/
        for(i=0; i<=5; i++)
        {
            pec[i] ^=crc[i];
        }/*End of for*/
    }
    while(BitPosition>8); /*End of do-while*/

    return pec[0];
}

/*******************************************************************************
* Function Name  : SMBus_ReadTemp
* Description    : Calculate and return the temperature
* Input          : None
* Output         : None
* Return         : SMBus_ReadMemory(0x00, 0x07)*0.02-273.15
*******************************************************************************/
float SMBus_ReadTemp(void)
{
    float temp;
    temp = SMBus_ReadMemory(SA, RAM_ACCESS|RAM_TOBJ1)*0.02-273.15;
    return temp;
}

/*********************************END OF FILE*********************************/


#if 0

/**
  * @brief  产生IIC起始信号
  * @param  None
  * @retval	None 
  * @note		None
  */
void Start_Bit(void)
{
	MLX90614_SDA_OUT();     //SDA线输出	
  delay_us(5); 
  MLX90614_IIC_SDA=1; 
	MLX90614_IIC_SCL=1; 
  
  delay_us(30); 
  MLX90614_IIC_SDA=0; 
  delay_us(30); 
	MLX90614_IIC_SCL=0; 
  delay_us(30);  
}

/**
  * @brief  产生IIC停止信号
  * @param  None
  * @retval	None 
  * @note		None
  */
void Stop_Bit()
{
	MLX90614_SDA_OUT();     //SDA线输出	
  delay_us(1); 
  MLX90614_IIC_SDA=0; 
  delay_us(2); 
	MLX90614_IIC_SCL=1; 
  delay_us(2); 
  MLX90614_IIC_SDA=1; 
}

/**
  * @brief  发送一个位
  * @param  None
  * @retval	None 
  * @note		None
  */
void Send_Bit(uint8_t Bit_out)
{
	MLX90614_SDA_OUT();     //SDA线输出	
  delay_us(5); 
  if(Bit_out==0) {
		MLX90614_IIC_SDA=0; 
	}else{
		MLX90614_IIC_SDA=1; 
	}
  delay_us(5); 
	MLX90614_IIC_SCL=1; 
  delay_us(30); 
	MLX90614_IIC_SCL=0; 
  delay_us(30); 
}

/**
  * @brief  读一个位
  * @param  None
  * @retval	None 
  * @note		None
  */
uint8_t Receive_Bit()
{
  uint8_t Bit_in;
  MLX90614_SDA_IN(); //SDA设置为输入
  delay_us(5); 
	MLX90614_IIC_SCL=1; 
  delay_us(5); 
  if(MLX90614_READ_SDA==1){
		Bit_in=1;
	}else{
		Bit_in=0;
	}
  delay_us(10); 
	MLX90614_IIC_SCL=0; 
  delay_us(30); 
  return Bit_in;
}

/**
  * @brief  等待应答ACK应答
  * @param  None
  * @retval	1 - ACK 
						0 - NACK
  * @note		None
  */
uint8_t Slave_Ack()
{
  uint8_t ACK;
  ACK=0;
  MLX90614_SDA_IN(); //SDA设置为输入
  delay_us(5); 
	MLX90614_IIC_SCL=1; 
  delay_us(10); 
  if(MLX90614_READ_SDA==1){
		ACK=0;
	}else{
		ACK=1;
	}
  delay_us(10); 
	MLX90614_IIC_SCL=0; 
  delay_us(30); 
  return ACK;
}

//----------------------------------------------------------------------------------------------------------------------------------------//
//Name: TX_byte
//----------------------------------------------------------------------------------------------------------------------------------------//

/**
	* @brief  发送一个字节
  * @param  None
  * @retval	1 - ACK 
						0 - NACK
  * @note		None
  */
void TX_Byte(uint8_t TX_buffer)
{
  uint8_t Bit_Out;
  for(uint8_t Bit_Counter=8;Bit_Counter;Bit_Counter--)
  {
    if(TX_buffer&0x80){
			Bit_Out=1;
		}else{
			Bit_Out=0;
		} 
    Send_Bit(Bit_Out); //Send the current bit on SMBus
    TX_buffer<<=1; //Get next bit to check
  }
 }

//----------------------------------------------------------------------------------------------------------------------------------------//
//Name: RX_byte
//Parameters: uint8_t ack_nack (acknowledgment bit)
//0 - Master device sends ACK
//1 - Master device sends NACK
//----------------------------------------------------------------------------------------------------------------------------------------//
/**
	* @brief  读1个字节
  * @param  Ack_Nack=1时，发送ACK
					  Ack_Nack=0时，发送NACK
  * @retval	None
  * @note		None
  */
 uint8_t RX_Byte(uint8_t Ack_Nack)
{
	uint8_t RX_Buffer;
	for(uint8_t Bit_Counter=8;Bit_Counter;Bit_Counter--)
	{
	 if(Receive_Bit()==1) //Read a bit from the SDA line
	  {
	   RX_Buffer<<=1; //If the bit is HIGH save 1 in RX_buffer
	   RX_Buffer|=0x01;
	  }
	 else //If the bit is LOW save 0 in RX_buffer
	  {
	   RX_Buffer<<=1;
	   RX_Buffer&=0xfe;
	  }
	}
	Send_Bit(Ack_Nack); //Sends acknowledgment bit
	return RX_Buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------//
//CALCULATE THE PEC PACKET
//----------------------------------------------------------------------------------------------------------------------------------------//
uint8_t PEC_cal(uint8_t pec[],int n)
{
    uint8_t crc[6];
    uint8_t Bitposition=47;
    uint8_t shift;
    uint8_t i;
    uint8_t j;
    uint8_t temp;
  do{
    crc[5]=0; //Load CRC value 0x000000000107
    crc[4]=0;
    crc[3]=0;
    crc[2]=0;
    crc[1]=0x01;
    crc[0]=0x07;
    Bitposition=47; //Set maximum bit position at 47
    shift=0;        //Find first 1 in the transmitted bytes
    i=5; //Set highest index (package byte index)
    j=0; //Byte bit index, from lowest
    while((pec[i]&(0x80>>j))==0 && (i>0))
    {
     Bitposition--;
     if(j<7){ j++;}
     else {j=0x00;i--;}
    }//the position of highest "1" bit in Bitposition is calculated
    shift=Bitposition-8; //Get shift value for CRC value
	
    while(shift)
    {
      for(i=5;i<0xFF;i--)
      {
       if((crc[i-1]&0x80) && (i>0)) //Check if the MSB of the byte lower is "1"
        { //Yes - current byte + 1
         temp=1; //No - current byte + 0
        } //So that "1" can shift between bytes
       else { temp=0;}
      crc[i]<<=1;
      crc[i]+=temp;
      }
    shift--;
    }
    //Exclusive OR between pec and crc
    for(i=0;i<=5;i++) { pec[i]^=crc[i]; }
    }
    while(Bitposition>8);
    return pec[0];
}

//----------------------------------------------------------------------------------------------------------------------------------------//
//READ DATA FROM RAM/EEPROM
//----------------------------------------------------------------------------------------------------------------------------------------//
unsigned long int MEM_READ(uint8_t slave_addR, uint8_t cmdR)
{
  uint8_t DataL; //
  uint8_t DataH; //Data packets from MLX90614
  uint8_t PEC; //
  unsigned long int Data; //Register value returned from MLX90614
  uint8_t Pecreg; //Calculated PEC byte storage
  uint8_t arr[6]; //Buffer for the sent bytes
  uint8_t ack_nack;
  uint8_t SLA;
  SLA=(slave_addR<<1);
begin:
  Start_Bit(); //Send start bit
  TX_Byte(SLA); //Send slave address, write
  if(Slave_Ack()==0){Stop_Bit();goto begin;} //Send command
  TX_Byte(cmdR);
  if(Slave_Ack()==0){Stop_Bit();goto begin;}//Send Repeated start bit
	Start_Bit(); //Send slave address, read
  TX_Byte(SLA+1);
  if(Slave_Ack()==0){Stop_Bit();goto begin;}
  DataL=RX_Byte(0); //
  //Read two bytes data
  DataH=RX_Byte(0); //
  PEC=RX_Byte(ack_nack); //Read PEC from MLX90614
  if(ack_nack==1) //Master sends ack or nack
  //This depends on the pec calculation, 
  //if the PEC is not correct, send nack and goto begin
  {Stop_Bit();goto begin;} //Send stop bit
  Stop_Bit();
  arr[5]=(SLA);
  arr[4]=cmdR;
  arr[3]=(SLA+1);
  arr[2]=DataL;
  arr[1]=DataH;
  arr[0]=0;
  Pecreg=PEC_cal(arr,6); //Calculate CRC
  if(PEC==Pecreg){ ack_nack=0;}
  else{ ack_nack=1;}
  Data=(DataH*256)+DataL;
  return Data;
}
#endif
