#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"


//IO方向设置
#define SDA_IN()  {GPIOA->CRL&=0xFFF0FFFF;GPIOA->CRL|=8<<16;}
#define SDA_OUT() {GPIOA->CRL&=0xFFF0FFFF;GPIOA->CRL|=3<<16;}

//IO操作函数	 
#define IIC_SCL    PAout(5) //SCL
#define IIC_SDA    PAout(4) //SDA	 
#define READ_SDA   PAin(4)  //输入SDA 

#define IRQ				 PAin(0)

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节


u8 AS3935_Read(u8 ReadAddr,u8 Leng,u8* Data);
u8 AS3935_Write(u8 ReadAddr,u8 Leng,u8* Data);
void AS3935_Init(void);




struct SetReg_bit{
	//0x00
	u8 PWD						:1;	//电源 0:ON
	u8 AFE_GB					:5;	//增益
	u8 Reserved0			:2;
	
	//0x01
	u8 WDTH						:4;
	u8 NF_LEV					:3;
	u8 Reserved				:1;
	
	//0x02
	u8 SREJ						:4;
	u8 MIN_NUM_LIGH		:2;
	u8 CL_STAT				:1;
	u8 Reserved1			:1;
	
	//0x03
	u8 INT						:4;
	u8 Reserved2			:1;
	u8 MASK_DIST			:1;
	u8 LCO_FDIV				:2;
	
	//0x04
	u8 S_LIG_L				:8;
	
	//0x05
	u8 S_LIG_M				:8;
	
	//0x06
	u8 S_LIG_MM				:5;
	u8 Reserved3			:3;
	
	//0x07
	u8 DISTANCE				:6;
	u8 Reserved4			:2;
	
	//0x08
	u8 TUN_CAP				:4;
	u8 Reserved5			:1;
	u8 DISP_TRCO			:1;
	u8 DISP_SRCO			:1;
	u8 DISP_LCO				:1;
};


union SetReg_reg{
	struct SetReg_bit bit;
	u8 SetList[9];
};

extern union SetReg_reg SetReg;



#endif



