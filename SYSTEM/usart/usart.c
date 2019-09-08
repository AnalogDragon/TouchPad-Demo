#include "usart.h"

struct USART1_REG USART1_DATA;
  
void uart_init(u32 bound){
	
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//RCC Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);
	
	//GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	//NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//USART
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE); 

}

//To send AA BB 00 Close
//To send AA BB 01 Open
void USART1_IRQHandler(void){                
	u8 Res;
	u8 USART_RX_STATE;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		USART1_DATA.USART_RX_NUM = USART_ReceiveData(USART1);
		if(USART_RX_STATE==0 || USART_RX_STATE==0xff){
			if(Res==0xAA)USART_RX_STATE=1;
		}
		else if(USART_RX_STATE==1){
			if(Res==0xBB)
				USART_RX_STATE=2;
			else
				USART_RX_STATE=0;
		}
		else if(USART_RX_STATE==2){
			USART_RX_STATE = 0xFF;
			if(USART1_DATA.USART_RX_NUM);
			else;
		}
	}
}


void send_once(void){
	u8 i;
	
	USART1_DATA.USART_TX_BUF[0] = 0xBB;
	USART1_DATA.USART_TX_BUF[1] = 0xAA;
	USART1_DATA.USART_TX_BUF[2] = 0x65;
	
	for(i=0;i<64;i++){
		USART1_DATA.USART_TX_BUF[i*2+2] = 0;
		USART1_DATA.USART_TX_BUF[i*2+3] = 0;
	}
	
	USART1_DATA.USART_TX_BUF[129] = 0;
	for(i=0;i<(3+64*2);i++){
		USART1_DATA.USART_TX_BUF[129] += USART1_DATA.USART_TX_BUF[i];
	}
	DMA_USART1_SendData(200);
}

void printtime(void){		//调试用
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
		USART_SendData(USART1,((u8)(DispData.Pos/10)%10)+0x30);//向串口1发送数据
	
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
		USART_SendData(USART1,((u8)DispData.Pos%10)+0x30);//向串口1发送数据
	
	
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
		USART_SendData(USART1,'\r');//向串口1发送数据
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
		USART_SendData(USART1,'\n');//向串口1发送数据
	
}


void DMA_USART1_init(void) 
{ 
    DMA_InitTypeDef t_dma;
		NVIC_InitTypeDef t_nvic;
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
    t_dma.DMA_PeripheralBaseAddr = USART1_DR_BASE;
    t_dma.DMA_MemoryBaseAddr = (u32)(&USART1_DATA.USART_TX_BUF);
    t_dma.DMA_DIR = DMA_DIR_PeripheralDST;
    t_dma.DMA_BufferSize = 0;
    t_dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    t_dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    t_dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    t_dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    t_dma.DMA_Mode = DMA_Mode_Normal;
    t_dma.DMA_Priority = DMA_Priority_Medium;
    t_dma.DMA_M2M = DMA_M2M_Disable;
	
    DMA_Init(DMA1_Channel4,&t_dma);
    DMA_Cmd(DMA1_Channel4,ENABLE);
    DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);    
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		
    t_nvic.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    t_nvic.NVIC_IRQChannelPreemptionPriority = 1;
    t_nvic.NVIC_IRQChannelSubPriority = 1;
    t_nvic.NVIC_IRQChannelCmd = ENABLE;
		
    NVIC_Init(&t_nvic);
} 


void DMA1_Channel4_IRQHandler(void){
    if(DMA_GetFlagStatus(DMA1_FLAG_TC4) == SET){
        DMA_ClearFlag(DMA1_FLAG_TC4);
        DMA_Cmd(DMA1_Channel4,DISABLE);
    }
}

void DMA_USART1_SendData(u16 size){
		DMA1_Channel4->CNDTR = size;
		DMA_Cmd(DMA1_Channel4,ENABLE);
}



