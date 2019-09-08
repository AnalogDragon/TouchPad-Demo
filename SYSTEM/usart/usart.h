#ifndef __USART_H
#define __USART_H
#include "sys.h" 

#define USART1_BUF_SIZE 200

struct USART1_REG{
	u8  USART_RX_NUM;
	
	u8  USART_RX_LEG;											//接收长度
	u8 	USART_RX_BUF[USART1_BUF_SIZE];		//接收缓存
	
	u8  USART_TX_LEG;											//发送长度
	u8 	USART_TX_BUF[USART1_BUF_SIZE];		//发送缓存
};

extern struct USART1_REG USART1_DATA;

void uart_init(u32 bound);
void send_once(void);
void printtime(void);
void DMA_USART1_init(void);
void DMA_USART1_SendData(u16 size);


#endif


