#ifndef COMMON_H
#define COMMON_H
#include "..//UCOSII//ucos_ii.h"
#include "..//STM32Lib//stm32f10x.h"
#include "..//Logger//Debug.h"
#include "..//User//TypeDef.h"
#include "..//User//AppOpts.h"
//#include "memory.h"
#include "string.h"
////////////////////////////////////////////////////////////////////
//±‡“Îøÿ÷∆
#define DEBUG
//extern void tfp_printf(char* s,char *fmt, ...);

#define INT8U_INT16U(H,L)  (((INT16U) H <<8)|( L ))

////////////////////////////////////////////////////////////////////
extern void DelayUS(u16 Time);  //Millisecond = 1-->ms;= 0 --> us
extern void DelayMS(u16 Time);

#define CR 0x0d
#define LF 0x0a
#endif
