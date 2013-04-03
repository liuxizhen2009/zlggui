#include "..\\User\\Common.h"
#include "..\\Hal\\hal.h"


/******************************
**	任务堆栈
*******************************/
//任务要分散以便以后添加任务
//0-7保留  
//8-15
//16-23
//24-31
//32-40

#define MAX_TSK_NUM   8
#define TSK_STK_DEPTH 128			  
OS_STK   TaskStartStk[128];
#define START_TSK_PRIO  4
#define APP_BASE_TSK_PRIO  16
OS_STK   TaskStk[MAX_TSK_NUM][TSK_STK_DEPTH];
//
OS_STK	 Task31Stk[256];
OS_STK	 Task32Stk[256];





/*****************************
	任务函数
******************************/		   


void  TaskStart     (void *p_arg);
extern void 	TskKeyDriver(void *p_arg);	//32
extern void 	TskUsartProc(void *p_arg);	//31
extern void 	TskAppProc(void *p_arg);	//31
extern void 	TskGuiProc(void *p_arg);//30;
//extern void 	T_LwIPEntry(void * pvArg);

 TskFunPtr  TskFunArray[MAX_TSK_NUM]={
   TskUsartProc,
 TskKeyDriver,
 TskAppProc,
 TskGuiProc,
 NULL,
 NULL};///@NOTE：最后一个NULL不能去掉

						 

/****/
OS_EVENT * pUSART1MsgQ;//定义一个串口的邮箱
OS_EVENT * pAppMsgQ;//定义一个串口的邮箱
OS_EVENT * pSimKeyMsgQ;

OS_TMR * pSoftTmr1;

void * UsartQue[N_Message];
void * AppQue[N_Message];
void * SimKeyQue[N_Message];


/**
	printf buf
*/
char printf_buf[128];
extern void init_printf(void* putp,void (*putf) (void*,char));
extern void uart1_putc(void * ptr,char c);


extern void NVIC_Configuration(void);
extern BOOLEAN LedOperate(LedType Led,LedOptType Opt);

SysTimeOut * pCMX7141ScanTimer=NULL;
extern void Led1Toggle(void);
extern SysTimeOut * AppTimerCreat(u8 time_type,sys_timeout_handler handler,void *arg,u32 ticks_reload);


/*************************************************************************/


