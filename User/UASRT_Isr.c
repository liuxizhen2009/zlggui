/**
	@file USART_Isr.c
	@brief 串口中断的实际处理函数
	变量的声明放在了val.h中
	@author $liuxizhen
	@version v1.0
	@date 2011年12月23日9:06:19

*/
/**
	@brief 接收到字符的中断处理函数，
	如果不是为“CR”就放入缓冲区
	如果是CR，把缓冲区指针和长度赋值给消息的指针，长度 。
	然后发送消息。
*/
#include"..//User//Common.h"
#include "..//UCOSII//ucos_ii.h"
#include "..//Hal//hal.h"
#include "..//STM32Lib//stm32f10x.h"


#define CR 0x0d
#define LF 0x0a
#define USART_MAIN USART1
#define RCV_BUF_SIZE 0xff

extern OS_EVENT * pUSART1MsgQ;
extern OS_EVENT * pUSART2MsgQ;

TskComMsg UsartRcvDataMsge;

INT8U RcvDataCounter;
INT8U RcvDataBuf[RCV_BUF_SIZE];
INT8U RcvChar;

/**
	@brief 实际被调用的的串口2的中断函数
	接收到CR后才认为是结束
*/


void UsartMainIsrFun(void )
{	
	
	RcvChar=USART_ReceiveData(USART_MAIN);
	//USART_SendData(USART_DEBUG,RcvChar);
//	if((RcvChar!=LF)&&(RcvChar!=CR)&&(RcvDataCounter<RCV_BUF_SIZE))
	if((RcvChar!=LF)&&(RcvDataCounter<RCV_BUF_SIZE))
	{
		
		RcvDataBuf[RcvDataCounter]=RcvChar;
		RcvDataCounter++;
	}
//else if((RcvChar==LF)||(RcvChar==CR)||(RcvDataCounter>=RCV_BUF_SIZE))//命令是以CR+LF结尾的。
	else if((RcvChar==LF)||(RcvDataCounter>=RCV_BUF_SIZE)) //仅仅以LF判断结束吧
	{

		RcvDataBuf[RcvDataCounter]=RcvChar;//着一句是吧LF也copy进去
		RcvDataCounter++;//加这一个是吧LF也算入了。这样就是算入了CR和LF后的收到的个数
		UsartRcvDataMsge.MsgType=UsartMainISR;
		UsartRcvDataMsge.DataLen=RcvDataCounter;
		UsartRcvDataMsge.Data.Ptr=RcvDataBuf;
		//if((RcvChar==LF)||(RcvChar==CR))
		if(RcvDataCounter>RCV_BUF_SIZE)
		{
			ELOG("RcvBufFull, err\n");	
		}else if((RcvChar==LF)&&(UsartRcvDataMsge.DataLen>2))//仅仅以LF判断结束吧	后面的一个条件是为了避免仅仅收到CRLF的时候，也会发送消息的情况
		{		
			OSQPost(pUSART1MsgQ,&UsartRcvDataMsge) ;
		}else if((UsartRcvDataMsge.DataLen<=2)) //这里就是仅仅收到了CR LF的地方  当然可以细化这里，判定收到的内容不是CRLF
		{	
			
		};

		RcvDataCounter=0;


	}
	
}







