#include "timer.h"
			
			

//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		//
		}
}


//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��   
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	
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


void TIM4_IRQHandler(void) //����ϵͳʱ��
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


#define SetMaxCount 2000
#define FilterLength 10


void GetVal(void){
	static u8 Timer = 0;
	static u32 CenterBuf;
	static u32 RightBuf;
	static u32 DownBuf;
	static u32 LeftBuf;
	static u32 UpBuf;

	/*Center*/
	TIM2->CNT = 0;
	TIM_ClearFlag(TIM2, TIM_FLAG_CC4|TIM_FLAG_Update);
	GPIOA->CRL &= 0xFFFF0FFF;
	GPIOA->CRL |= 0x00004000;	//FLoting
	while(TIM_GetFlagStatus(TIM2, TIM_FLAG_CC4) == RESET){
		if(TIM2->CNT > SetMaxCount){
			CenterBuf += SetMaxCount;
			goto L1;
		}
	}
	CenterBuf += TIM2->CCR4;
L1:
	
	/*Right	Tim3Ch1	PA6*/
	TIM3->CNT = 0;
	TIM_ClearFlag(TIM3, TIM_FLAG_CC1|TIM_FLAG_Update);
	GPIOA->CRL &= 0xF0FFFFFF;
	GPIOA->CRL |= 0x04000000;	//FLoting
	while(TIM_GetFlagStatus(TIM3, TIM_FLAG_CC1) == RESET){
		if(TIM3->CNT > SetMaxCount){
			RightBuf += SetMaxCount;
			goto L2;
		}
	}
	RightBuf += TIM3->CCR1;
L2:
	
	/*Down	Tim3Ch2	PA7*/
	TIM3->CNT = 0;
	TIM_ClearFlag(TIM3, TIM_FLAG_CC2|TIM_FLAG_Update);
	GPIOA->CRL &= 0x0FFFFFFF;
	GPIOA->CRL |= 0x40000000;	//FLoting
	while(TIM_GetFlagStatus(TIM3, TIM_FLAG_CC2) == RESET){
		if(TIM3->CNT > SetMaxCount){
			DownBuf += SetMaxCount;
			goto L3;
		}
	}
	DownBuf += TIM3->CCR2;
L3:
	
	/*Up Tim3Ch3	PB0*/
	TIM3->CNT = 0;
	TIM_ClearFlag(TIM3, TIM_FLAG_CC3|TIM_FLAG_Update);
	GPIOB->CRL &= 0xFFFFFFF0;
	GPIOB->CRL |= 0x00000004;	//FLoting
	while(TIM_GetFlagStatus(TIM3, TIM_FLAG_CC3) == RESET){
		if(TIM3->CNT > SetMaxCount){
			UpBuf += SetMaxCount;
			goto L4;
		}
	}
	UpBuf += TIM3->CCR3;
L4:
	
	/*Left	Tim3Ch4	PB1*/
	TIM3->CNT = 0;
	TIM_ClearFlag(TIM3, TIM_FLAG_CC4|TIM_FLAG_Update);
	GPIOB->CRL &= 0xFFFFFF0F;
	GPIOB->CRL |= 0x00000040;	//FLoting
	while(TIM_GetFlagStatus(TIM3, TIM_FLAG_CC4) == RESET){
		if(TIM3->CNT > SetMaxCount){
			LeftBuf += SetMaxCount;
			goto L5;
		}
	}
	LeftBuf += TIM3->CCR4;
L5:
	
	GPIOA->CRL &= 0x00FF0FFF;
	GPIOA->CRL |= 0x33003000;
	GPIOB->CRL &= 0xFFFFFF00;
	GPIOB->CRL |= 0x00000033;	//Output PP
	
	Timer++;
	if(Timer >= FilterLength){
		TouchValue.Center = CenterBuf/FilterLength + 0.5;
		TouchValue.Right = RightBuf/FilterLength + 0.5;
		TouchValue.Down = DownBuf/FilterLength + 0.5;
		TouchValue.Left = LeftBuf/FilterLength + 0.5;
		TouchValue.Up = UpBuf/FilterLength + 0.5;
		CenterBuf=RightBuf=DownBuf=LeftBuf=UpBuf=Timer = 0;
	}
	
}       


//Center Tim2Ch4 PA3
//Right	Tim3Ch1	PA6
//Down	Tim3Ch2	PA7
//Up		Tim3Ch3	PB0
//Left	Tim3Ch4	PB1
void GetVal2(void){//0.15ms max
	static u8 Timer = 0;
	static u32 CenterBuf;
	static u16 RightBuf;
	static u16 DownBuf;
	static u16 LeftBuf;
	static u16 UpBuf;
	u8 temp = 0x1F;

	/*Center*/
	TIM_ClearFlag(TIM2, TIM_FLAG_CC4|TIM_FLAG_Update);
	TIM_ClearFlag(TIM3, TIM_FLAG_CC1|TIM_FLAG_CC2|TIM_FLAG_CC3|TIM_FLAG_CC4|TIM_FLAG_Update);
	TIM2->CNT = TIM3->CNT = 0;
	GPIOA->CRL &= 0x00FF0FFF;
	GPIOB->CRL &= 0xFFFFFF00;
	GPIOA->CRL |= 0x44004000;
	GPIOB->CRL |= 0x00000044;	//Output PP	//FLoting
	while(1){
		if((temp&0x01) && TIM_GetFlagStatus(TIM2, TIM_FLAG_CC4) != RESET){//Center
			CenterBuf += TIM2->CCR4;
			temp &= 0xFE;
		}
		if((temp&0x02) && TIM_GetFlagStatus(TIM3, TIM_FLAG_CC1) != RESET){//Right
			RightBuf += TIM3->CCR1;
			temp &= 0xFD;
		}
		if((temp&0x04) && TIM_GetFlagStatus(TIM3, TIM_FLAG_CC2) != RESET){//Down
			DownBuf += TIM3->CCR2;
			temp &= 0xFB;
		}
		if((temp&0x08) && TIM_GetFlagStatus(TIM3, TIM_FLAG_CC3) != RESET){//Up
			UpBuf += TIM3->CCR3;
			temp &= 0xF7;
		}
		if((temp&0x10) && TIM_GetFlagStatus(TIM3, TIM_FLAG_CC4) != RESET){//Left
			LeftBuf += TIM3->CCR4;
			temp &= 0xEF;
		}
		if(temp == 0 || TIM2->CNT > SetMaxCount){
			if(temp&0x01)CenterBuf += SetMaxCount;
			if(temp&0x02)RightBuf += SetMaxCount;
			if(temp&0x04)DownBuf += SetMaxCount;
			if(temp&0x08)UpBuf += SetMaxCount;
			if(temp&0x10)LeftBuf += SetMaxCount;
			break;
		}
	}
	GPIOA->CRL &= 0x00FF0FFF;
	GPIOA->CRL |= 0x33003000;
	GPIOB->CRL &= 0xFFFFFF00;
	GPIOB->CRL |= 0x00000033;	//Output PP
	
	Timer++;
	if(Timer >= FilterLength){
		TouchValue.Center = CenterBuf/FilterLength + 4.5;	//GPIO��ʼ������ʱ�� = 4
		TouchValue.Right = RightBuf/FilterLength + 4.5;
		TouchValue.Down = DownBuf/FilterLength + 4.5;
		TouchValue.Left = LeftBuf/FilterLength + 0.5;
		TouchValue.Up = UpBuf/FilterLength + 0.5;
		CenterBuf=RightBuf=DownBuf=LeftBuf=UpBuf=Timer = 0;
	}
	
}       


#define InitFiltLen 32
void GetTouchRef(void){
	u8 i,j;
	u32 a,b,c,d,e;
	a=b=c=d=e=0;
	for(i=0;i<InitFiltLen;i++){
		for(j=0;j<FilterLength;j++){
			delay_ms(1);
			GetVal2();
		}
		a += TouchValue.Center;
		b += TouchValue.Right;
		c += TouchValue.Down;
		d += TouchValue.Left;
		e += TouchValue.Up;
		DispPos(i);
	}
	TouchValue.CenterRef = a/InitFiltLen+0.5;
	TouchValue.RightRef = b/InitFiltLen+0.5;
	TouchValue.DownRef = c/InitFiltLen+0.5;
	TouchValue.LeftRef = d/InitFiltLen+0.5;
	TouchValue.UpRef = e/InitFiltLen+0.5;
}









