#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"


void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);


void InitTim2Cap(u16 arr,u16 psc);
void InitTim3Cap(u16 arr,u16 psc);

void GetVal(void);
void GetVal2(void);
void GetTouchRef(void);
void AutoSetRef(void);







#endif
