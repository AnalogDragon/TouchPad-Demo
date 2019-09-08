#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define SPI_SHCP        PBout(6)	//Rise位移
#define SPI_STCP        PBout(5)	//1刷新 0锁存
#define SPI_DS          PBout(7)	//数据

void LED_Init(void);//初始化
void LED_DAT(u32 num);
void LedTask(void);
void DispPos(u8 num);

#endif
