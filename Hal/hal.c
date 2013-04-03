/***************************************************
**HAL.c
**主要用于芯片硬件的内部外围和外部外围的初始化，两大INIT函数
**在MAIN中调用，使MAIN函数中尽量与硬件库无关
***************************************************/

#include "..\\STM32Lib\\stm32f10x.h"

#include "..\\USER\\AppOpts.h"
//#include "..\\Logger\\Debug.h"
#include ".\\hal.h"


//各个内部硬件模块的配置函数
extern void GPIO_Configuration(void);			//GPIO
extern void RCC_Configuration(void);			//RCC
extern void USART_Configuration(void);					//串口


extern void NVIC_Configuration(void);			//NVIC
extern void OLED_Configuration(void);
extern void TIMER_Configuration(void);
extern char printf_buf[];
extern void init_printf(void* putp,void (*putf) (void*,char));
extern void tfp_printf(char *fmt, ...);
extern void uart1_putc(void * ptr,char c);

/*******************************
**函数名:ChipHalInit()
**功能:片内硬件初始化
*******************************/
void  ChipHalInit(void)
{
	//初始化时钟源
	RCC_Configuration();
	
	//初始化GPIO
	GPIO_Configuration();
	//初始化串口
	USART_Configuration();
	init_printf(printf_buf,uart1_putc);
	tfp_printf("tfp_printf Inital over\n");

	TIMER_Configuration();

	//初始化中断，在ucos启动后执行，其中包含两步，1，分配中断优先级，2，使能中断
	//NVIC_Configuration();

//	OLED_Configuration();
}


/*********************************
**函数名:ChipOutHalInit()
**功能:片外硬件初始化
*********************************/
extern u8 CML_FirmwareDownload(void);
void  ChipOutHalInit(void)
{
// 	if(CML_FirmwareDownload()){
// 		
// 		LWIP_ERROR(("CML_FirmwareDownload fail\n"));
// 	}else{
// 	   	LWIP_DEBUGF(CML_DEBUG,("CML_FirmwareDownload success\n"));
// //		do { if (((CML_DEBUG) & DBG_ON)&& ((CML_DEBUG) & DBG_TYPES_ON)
// //		&& ((u16)((CML_DEBUG)& DBG_MASK_LEVEL) >= DBG_MIN_LEVEL))
// //		{LWIP_PLATFORM_DIAG(("*******CML_FirmwareDownload success\n")); 
// //			if ((CML_DEBUG) & DBG_HALT) while(1); } } while(0);
// 	}	
}
