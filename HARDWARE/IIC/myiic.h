#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"


//IO��������
#define SDA_IN()  {GPIOA->CRL&=0xFFF0FFFF;GPIOA->CRL|=8<<16;}
#define SDA_OUT() {GPIOA->CRL&=0xFFF0FFFF;GPIOA->CRL|=3<<16;}

//IO��������	 
#define IIC_SCL    PAout(5) //SCL
#define IIC_SDA    PAout(4) //SDA	 
#define READ_SDA   PAin(4)  //����SDA 

#define IRQ				 PAin(0)

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�


u8 AS3935_Read(u8 ReadAddr,u8 Leng,u8* Data);
u8 AS3935_Write(u8 ReadAddr,u8 Leng,u8* Data);
void AS3935_Init(void);




struct SetReg_bit{
	//0x00
	u8 PWD						:1;	//��Դ 0:ON
	u8 AFE_GB					:5;	//����
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



