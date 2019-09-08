#include "timer.h"
			
			

//arr：自动重装值。
//psc：时钟预分频数
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		//
		}
}


//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射   
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	
	TIM3->CCR1=50;

}


void TIM4_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	TIM_Cmd(TIM4, ENABLE);
}


void TIM4_IRQHandler(void) //用于系统时钟
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		SysTimeInt();
	}
}


//Center Tim2Ch4 PA3
//Right	Tim3Ch1	PA6
//Down	Tim3Ch2	PA7
//Up		Tim3Ch3	PB0
//Left	Tim3Ch4	PB1

void InitTim2Cap(u16 arr,u16 psc){
	GPIO_InitTypeDef  GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	InitStructure.TIM_Channel = TIM_Channel_4;
	InitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	InitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	InitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	InitStructure.TIM_ICFilter = 0x03;
	TIM_ICInit(TIM2, &InitStructure);

	TIM_Cmd(TIM2,ENABLE);
	GPIOA->CRL &= 0xFFFF0FFF;
	GPIOA->CRL |= 0x00003000;
	GPIOA->ODR &= ~(1<<3);
}

void InitTim3Cap(u16 arr,u16 psc){
	GPIO_InitTypeDef  GPIO_InitStructure; 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler =psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	InitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	InitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	InitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	InitStructure.TIM_ICFilter = 0x03;
	
	InitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInit(TIM3, &InitStructure);
	InitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM3, &InitStructure);
	InitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInit(TIM3, &InitStructure);
	InitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInit(TIM3, &InitStructure);

	TIM_Cmd(TIM3,ENABLE);
	
	GPIOA->CRL &= 0x00FFFFFF;
	GPIOA->CRL |= 0x33000000;
	GPIOB->CRL &= 0xFFFFFF00;
	GPIOB->CRL |= 0x00000033;
	
	GPIOA->ODR &= ~(1<<6);
	GPIOA->ODR &= ~(1<<7);
	GPIOB->ODR &= ~(1<<0);
	GPIOB->ODR &= ~(1<<1);
}


void GetVal(void){//1ms
	static u8 Timer = 0;
	static u16 CenterBuf[5];
	static u16 RightBuf[5];
	static u16 DownBuf[5];
	static u16 LeftBuf[5];
	static u16 UpBuf[5];
	
	/*Center*/
	TIM2->CNT = 0;
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC4|TIM_IT_Update);
	GPIOA->CRL &= 0xFFFF0FFF;
	GPIOA->CRL |= 0x00004000;	//FLoting
	delay_us(80);
	while(TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET){
		if(TIM2->CNT > 0xF000){
			CenterBuf[Timer] = TIM2->CNT;
			goto L1;
		}
	}
	CenterBuf[Timer] = TIM2->CCR4;
L1:
	GPIOA->CRL &= 0xFFFF0FFF;
	GPIOA->CRL |= 0x00003000;	//Output PP
	
	/*Right	Tim3Ch1	PA6*/
	TIM3->CNT = 0;
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update);
	GPIOA->CRL &= 0xF0FFFFFF;
	GPIOA->CRL |= 0x04000000;	//FLoting
	delay_us(80);
	while(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET){
		if(TIM3->CNT > 0xF000){
			RightBuf[Timer] = TIM3->CNT;
			goto L2;
		}
	}
	RightBuf[Timer] = TIM3->CCR1;
L2:
	GPIOA->CRL &= 0xF0FFFFFF;
	GPIOA->CRL |= 0x03000000;	//Output PP
	
	/*Down	Tim3Ch2	PA7*/
	TIM3->CNT = 0;
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC2|TIM_IT_Update);
	GPIOA->CRL &= 0x0FFFFFFF;
	GPIOA->CRL |= 0x40000000;	//FLoting
	delay_us(80);
	while(TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET){
		if(TIM3->CNT > 0xF000){
			DownBuf[Timer] = TIM3->CNT;
			goto L3;
		}
	}
	DownBuf[Timer] = TIM3->CCR2;
L3:
	GPIOA->CRL &= 0x0FFFFFFF;
	GPIOA->CRL |= 0x30000000;	//Output PP
	
	/*Up Tim3Ch3	PB0*/
	TIM3->CNT = 0;
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC3|TIM_IT_Update);
	GPIOB->CRL &= 0xFFFFFFF0;
	GPIOB->CRL |= 0x00000004;	//FLoting
	delay_us(80);
	while(TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET){
		if(TIM3->CNT > 0xF000){
			UpBuf[Timer] = TIM3->CNT;
			goto L4;
		}
	}
	UpBuf[Timer] = TIM3->CCR3;
L4:
	GPIOB->CRL &= 0xFFFFFFF0;
	GPIOB->CRL |= 0x00000003;	//Output PP
	
	/*Left	Tim3Ch4	PB1*/
	TIM3->CNT = 0;
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC4|TIM_IT_Update);
	GPIOB->CRL &= 0xFFFFFF0F;
	GPIOB->CRL |= 0x00000040;	//FLoting
	delay_us(80);
	while(TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET){
		if(TIM3->CNT > 0xF000){
			LeftBuf[Timer] = TIM3->CNT;
			goto L5;
		}
	}
	LeftBuf[Timer] = TIM3->CCR4;
L5:
	GPIOB->CRL &= 0xFFFFFF0F;
	GPIOB->CRL |= 0x00000030;	//Output PP
	
	Timer++;
	if(Timer >= 5){
		TouchValue.Center = ((CenterBuf[0]+CenterBuf[1]+CenterBuf[2]+CenterBuf[3]+CenterBuf[4])/5) + 0.5;
		TouchValue.Right = ((RightBuf[0]+RightBuf[1]+RightBuf[2]+RightBuf[3]+RightBuf[4])/5) + 0.5;
		TouchValue.Down = ((DownBuf[0]+DownBuf[1]+DownBuf[2]+DownBuf[3]+DownBuf[4])/5) + 0.5;
		TouchValue.Left = ((LeftBuf[0]+LeftBuf[1]+LeftBuf[2]+LeftBuf[3]+LeftBuf[4])/5) + 0.5;
		TouchValue.Up = ((UpBuf[0]+UpBuf[1]+UpBuf[2]+UpBuf[3]+UpBuf[4])/5) + 0.5;
		Timer = 0;
	}
	
}       



void GetTouchRef(void){
	u8 i,j;
	u32 a,b,c,d,e;
	a=b=c=d=e=0;
	for(i=0;i<50;i++){
		for(j=0;j<5;j++){
			delay_ms(1);
			GetVal();
		}
		a += TouchValue.Center;
		b += TouchValue.Right;
		c += TouchValue.Down;
		d += TouchValue.Left;
		e += TouchValue.Up;
		DispPos(i);
	}
	TouchValue.CenterRef = a/50+0.5;
	TouchValue.RightRef = b/50+0.5;
	TouchValue.DownRef = c/50+0.5;
	TouchValue.LeftRef = d/50+0.5;
	TouchValue.UpRef = e/50+0.5;
}









