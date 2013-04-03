/**
	@file USART.c
	@brief SCDMA project 
	@author liuxizhen
	@date 2012年1月17日16:45:08
	@version V0.1
*/

#include "..//STM32Lib//stm32f10x.h"
#include "..//UCOSII//os_cpu.h"			//借用了ucos的一些类型定义.实际没有使用ucos
#include "stdio.h"
//----------------------------------------------------------------------------------

#define    USART1_ENABLE 1
#define    USART2_ENABLE 0
#define    USART_DEBUG		USART1

/**********************************************
**串口配置函数,
**开始定义三个宏来控制，三个串口的使用
**宏开关在BoardCfg.h中


  硬件连接示意：
  usart1是和MPE158通信的串口，
  usart3的两个管脚PB10,PB11也是连接到了MEP上，但是中间有电阻，可以拆下来，断开连接
  将usart3作为调试串口来使用。 但是这里使用的是串口2作为了调试的输出串口。
  usart1：---------------------------------------模块通信串口
  PA9---->RXD0(MPE158.Pin11)
  PA10<---TXD0(MPE158.Pin16)
  uasrt2: ---------------------------------------调试串口
  PA2----TX
  PA3----PTT_E/RX
  @note 在不装配1R1和1R2的时候。
  usart3:
  PB10(TX)--->
  PB11(RX)<---
  
**********************************************/



void USART_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStructure;
	
	//使能串口1，PA，AFIO总线

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
            RCC_APB2Periph_AFIO |
            RCC_APB2Periph_USART1 , 
            ENABLE);
#if USART1_ENABLE 

    /* A9 USART1_Tx */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//推挽输出-TX
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* A10 USART1_Rx  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//浮空输入-RX
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

	USART_ClockInit(USART1, &USART_ClockInitStructure);
    USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); //$liu 这里要开接收中断

    /* Enable the USARTx */
    USART_Cmd(USART1, ENABLE);

//--------------------------------------------------------         
       //-----如下语句解决第1个字节无法正确发送出去的问题-----// 
       	USART_ClearFlag(USART1, USART_FLAG_TC);     // 清标志  
//--------------------------------------------------------
#endif

#if USART2_ENABLE

	//使能串口2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	// A2 做T2X
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // A3 做R2X 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate =115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

    USART_ClockInit(USART2, &USART_ClockInitStructure);
    USART_Init(USART2, &USART_InitStructure);
    
	 //串口2使用接收中断
//	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART2, ENABLE);

#endif
#if USART3_ENABLE
//使能串口3时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//pb10 做T2X
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // PB11 做R2X 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate =115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

    USART_ClockInit(USART3, &USART_ClockInitStructure);
    USART_Init(USART3, &USART_InitStructure);
    
    USART_Cmd(USART3, ENABLE);
	//串口3使用接收中断
//	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
#endif

}


void USART1_Putc(unsigned char c)
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

void USART1_Puts(char * str)
{
    while(*str)
    {
        //printf("%d\n",*str);
		USART_SendData(USART1, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}
void USART1_PutData(char * str,INT8U DataLen)
{
    unsigned char i;
	for(i=0;i<DataLen;i++)
	{
			USART_SendData(USART1, *str++);
			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
	}
}

void USART2_Putc(unsigned char c)
{
    USART_SendData(USART2, c);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}

void USART2_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART2, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }

}
void USART3_Putc(unsigned char c)
{
    USART_SendData(USART3, c);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET );
}

void USART3_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART3, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//

void	sys_write(char* ptr, int len){
		int i;
			for(i=0;i<len;i++){
		   		 USART_SendData(USART1, *(ptr++));
    			 while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
			}
		};

//////////////////////////////////////////////////////////////////////////////////////////////////////
///
/**

	这个是tiny_printf的标准输出
*/
void uart1_putc(void * ptr,char c)
{
	    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}
/*定义 fputc 此函数为printf所用*/
int fputc(int ch,FILE *f) 
{ 
    USART_SendData(USART_DEBUG, (INT8U) ch);  
    /* Loop until the end of transmission */ 
    while(USART_GetFlagStatus(USART_DEBUG, USART_FLAG_TXE) == RESET);
    return ch; 
} 
