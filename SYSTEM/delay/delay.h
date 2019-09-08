#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"




void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

void SysTimeInt(void);
u16 GetDtTime(u16 timebuf,u16 timecnt);

#endif





























