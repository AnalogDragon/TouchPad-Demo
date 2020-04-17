#include "delay.h"
 
static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数

//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init()	 
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
// 	SysTick_Config(72000);
	
	fac_us=SystemCoreClock/8000000;	//为系统时钟的1/8  
	fac_ms=(u16)fac_us*1000;//非ucos下,代表每个ms需要的systick时钟数   

}								    

//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
} 


//1ms系统时钟
void SysTimeInt(void){
	
	SysTime.SysTimeCNT1ms++;
	SysTime.SysTimeFLG1ms = 1;
	
	if((SysTime.SysTimeCNT1ms%10) == 0){
		if(SysTime.SysTimeCNT1ms>=60000)SysTime.SysTimeCNT1ms = 0;
		SysTime.SysTimeCNT10ms++;
		SysTime.SysTimeFLG10ms = 1;
		
		if((SysTime.SysTimeCNT10ms%10) == 0){
			if(SysTime.SysTimeCNT10ms>=60000)SysTime.SysTimeCNT10ms = 0;
			SysTime.SysTimeCNT100ms++;
			SysTime.SysTimeFLG100ms = 1;
			
			if((SysTime.SysTimeCNT100ms%10) == 0){
				if(SysTime.SysTimeCNT100ms>=60000)SysTime.SysTimeCNT100ms = 0;
				SysTime.SysTimeCNT1s++;
				SysTime.SysTimeFLG1s = 1;
				
				if((SysTime.SysTimeCNT1s%60) == 0){
					if(SysTime.SysTimeCNT1s>=60000)SysTime.SysTimeCNT1s = 0;
					SysTime.SysTimeCNT1min++;
					SysTime.SysTimeFLG1min = 1;
					
					if((SysTime.SysTimeCNT1min%60) == 0){
						if(SysTime.SysTimeCNT1min>=60000)SysTime.SysTimeCNT1min = 0;
						SysTime.SysTimeCNT1h++;
						SysTime.SysTimeFLG1h = 1;
					}
				}
			}
		}
	}
	
}




u16 GetDtTime(u16 timebuf,u16 timecnt){
	if(timebuf > timecnt)
		return (60000 - timebuf + timecnt);
	else
		return (timecnt - timebuf);
}


