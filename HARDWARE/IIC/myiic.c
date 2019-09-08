#include "myiic.h"

#define AS3935_A0 	0
#define AS3935_A1 	0
#define AS3935ADDR  ((AS3935_A1<<2)|(AS3935_A0<<1))

//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
 	delay_us(1);
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0
	delay_us(2);
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}


u8 AS3935_Read(u8 ReadAddr,u8 Leng,u8* Data){				
  u8 i;
	if(!Leng)return 1;
	
	IIC_Start();  
	
	IIC_Send_Byte(AS3935ADDR);
	if(IIC_Wait_Ack())return 1;
	
	IIC_Send_Byte(ReadAddr);
	if(IIC_Wait_Ack())return 1;
	
	IIC_Start();  	 	   
	
	IIC_Send_Byte(AS3935ADDR+1);
	if(IIC_Wait_Ack())return 1;
	
	for(i=0;i<Leng-1;i++){
		*Data = IIC_Read_Byte(1);
		Data++;
	}	
	*Data = IIC_Read_Byte(0);
	IIC_Stop();
	return 0;
}


u8 AS3935_Write(u8 ReadAddr,u8 Leng,u8* Data){				
  u8 i;
	
	IIC_Start();  
	
	IIC_Send_Byte(AS3935ADDR);
	if(IIC_Wait_Ack())return 1;
	
	IIC_Send_Byte(ReadAddr);
	if(IIC_Wait_Ack())return 1;
	
	for(i=0;i<Leng;i++){
		IIC_Send_Byte(*Data);
		if(IIC_Wait_Ack())return 1;
		Data++;
	}		
	IIC_Stop();
	return 0;
}

union SetReg_reg SetReg;

void AS3935_Init(void){
	IIC_Start();  
	IIC_Send_Byte(AS3935ADDR);
	if(IIC_Wait_Ack())return;
	IIC_Send_Byte(0x3C);
	if(IIC_Wait_Ack())return;
	IIC_Send_Byte(0x96);
	if(IIC_Wait_Ack())return;
	IIC_Send_Byte(0x96);
	if(IIC_Wait_Ack())return;
	IIC_Stop();
	delay_ms(10);
	AS3935_Read(0,9,SetReg.SetList);
	
	SetReg.bit.PWD 					= 0x00;
	SetReg.bit.AFE_GB 			= 0x12;
	SetReg.bit.NF_LEV 			= 0x02;
	SetReg.bit.WDTH 				= 0x02;
	SetReg.bit.CL_STAT 			= 0x01;
	SetReg.bit.MIN_NUM_LIGH = 0x00;
	SetReg.bit.SREJ 				= 0x02;
	SetReg.bit.LCO_FDIV 		= 0x00;
	SetReg.bit.MASK_DIST 		= 0x00;
	SetReg.bit.DISP_LCO 		= 0x00;
	SetReg.bit.DISP_SRCO 		= 0x00;
	SetReg.bit.DISP_TRCO		= 0x00;
	SetReg.bit.TUN_CAP			= 0x00;
	
	
	AS3935_Write(0,9,SetReg.SetList);
	delay_ms(10);
// 	IIC_Start();  
// 	IIC_Send_Byte(AS3935ADDR);
// 	if(IIC_Wait_Ack())return;
// 	IIC_Send_Byte(0x00);
// 	if(IIC_Wait_Ack())return;
// 	IIC_Send_Byte(0x80);
// 	if(IIC_Wait_Ack())return;
// 	IIC_Send_Byte(0x80);
// 	if(IIC_Wait_Ack())return;
// 	IIC_Send_Byte(0x80);
// 	if(IIC_Wait_Ack())return;
// 	IIC_Send_Byte(0x80);
// 	if(IIC_Wait_Ack())return;
// 	IIC_Stop();
// 	delay_ms(10);
// 	
// 	IIC_Start();  
// 	IIC_Send_Byte(AS3935ADDR);
// 	if(IIC_Wait_Ack())return;
// 	IIC_Send_Byte(0x08);
// 	if(IIC_Wait_Ack())return;
// 	IIC_Send_Byte(0x80);
// 	if(IIC_Wait_Ack())return;
// 	IIC_Stop();
// 	delay_ms(10);
	
}




