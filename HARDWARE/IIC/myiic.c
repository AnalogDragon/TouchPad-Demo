#include "myiic.h"

#define AS3935_A0 	0
#define AS3935_A1 	0
#define AS3935ADDR  ((AS3935_A1<<2)|(AS3935_A0<<1))

//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4|GPIO_Pin_5);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
 	delay_us(1);
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL=0;//ʱ�����0
	delay_us(2);
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
		SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
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




