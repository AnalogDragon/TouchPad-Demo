#include "sys.h"


void init_all(void){
	delay_init();
 	TIM4_Int_Init(1000-1,64-1);	//1000sps /1ms
	InitTim2Cap(0xFFFF,4-1);
	InitTim3Cap(0xFFFF,4-1);
	uart_init(115200);
	LED_Init();
}



int main(void){
	init_all();
	GPIOA->CRL &= 0xFF0FFFFF;	//Set Debug Output Pin
	GPIOA->CRL |= 0x00300000;
	PAout(5)=1;
	GetTouchRef();
	PAout(5)=0;

	while(1){
		
		if(SysTime.SysTimeFLG1ms){
			PAout(5)=1;
			GetVal2();
			if(TouchValue.KeySet){
				TouchValue.KeySet = 0;
				GetTouch();
			}
			TouchToDisp();
			LedTask();
			PAout(5)=0;
			SysTime.SysTimeFLG1ms = 0;
		}
		
		if(SysTime.SysTimeFLG10ms){
// 			AutoSetRef();
			send_once();
			SysTime.SysTimeFLG10ms = 0;
		}
		
		if(SysTime.SysTimeFLG100ms){
			SysTime.SysTimeFLG100ms = 0;
		}
		
		if(SysTime.SysTimeFLG1s){
			SysTime.SysTimeFLG1s = 0;
		}
		
		if(SysTime.SysTimeFLG1min){
			SysTime.SysTimeFLG1min = 0;
		}
		
		if(SysTime.SysTimeFLG1h){
			SysTime.SysTimeFLG1h = 0;
		}
		
	}
	
	
	
}

