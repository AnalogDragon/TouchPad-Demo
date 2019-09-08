#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define SPI_SHCP        PBout(6)	//Riseλ��
#define SPI_STCP        PBout(5)	//1ˢ�� 0����
#define SPI_DS          PBout(7)	//����

void LED_Init(void);//��ʼ��
void LED_DAT(u32 num);
void LedTask(void);
void DispPos(u8 num);

#endif
