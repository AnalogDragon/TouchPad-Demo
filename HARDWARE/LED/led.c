#include "led.h"



//LED IO初始化 74hc595
void LED_Init(void){
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
	SPI_SHCP = 0;
	SPI_STCP = 0;
	SPI_DS = 0;
}
 




void LED_DAT(u32 num){
  u8 i;
  SPI_STCP = 0;
  for(i=0;i<32;i++){//400khz
    SPI_SHCP = 0;
    SPI_DS = (num>>31)&1;
    SPI_SHCP = 1;
    num <<= 1;
  }
  SPI_STCP = 1;
}



void LedTask(void){
	u8 i;
	static u8 Step = 0;
	u32 temp = 0,temp0 = 0;
	if(DispData.EN){
		if(DispData.All){
			temp = 0xFFFFFFFF;
			Step = 14;
		}
		else if(Step){
			Step --;
			temp0 = temp = 0x80000000>>(DispData.Pos%32);	//中心点
			for(i=0;i<Step;i++){
				temp0 |= (temp>>i) | (temp<<(31-i));
				temp0 |= (temp<<i) | (temp>>(31-i));
			}
			temp = temp0;
		}
		else{
			temp = 0x80000000>>(DispData.Pos%32);
		}
	}
	
	LED_DAT(temp);
	
}



void DispPos(u8 num){
	u32 temp;
	temp = 0x80000000>>(num%32);
	LED_DAT(temp);
	
}






