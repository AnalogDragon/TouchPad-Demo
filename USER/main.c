#include "sys.h"


void init_all(void){
	delay_init();	  //延时函数初始化	  
 	TIM4_Int_Init(1000-1,64-1);	//1000sps /1ms
	InitTim2Cap(0xFFFF,32-1);
	InitTim3Cap(0xFFFF,32-1);
	uart_init(115200);
	LED_Init();
}


int main(void){
	u8 Time2ms = 0;
	init_all();      //初始化系统
	GetTouchRef();

	while(1){
		
		if(SysTime.SysTimeFLG1ms){
			Time2ms = !Time2ms;
			if(Time2ms)
				GetVal();
			SysTime.SysTimeFLG1ms = 0;
		}
		
		if(SysTime.SysTimeFLG10ms){	
			GetTouch();
			TouchToDisp();
			LedTask();
			SysTime.SysTimeFLG10ms = 0;
		}
		
		if(SysTime.SysTimeFLG100ms){
			printtime();
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

