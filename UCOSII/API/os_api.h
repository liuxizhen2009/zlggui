//*------------------------------------------------------------------------------------------------
//* 文件名			   : os_api.h
//* 功能描述		   : os_api.c的头文件
//* 作者    		   : 焦海波
//* 版本			   : 0.1
//* 建立日期、时间	   : 2006/06/10 10:48
//* 最近修改日期、时间 : 
//* 修改原因		   : 
//*------------------------------------------------------------------------------------------------

#ifndef	__os_api_h
#define	__os_api_h

//*------------------------------- 宏、常量、变量、类型及结构体定义 ---------------------------------
typedef	OS_EVENT*	HANDLER;

//* 创建一个新的信号量
#define		OSAPISemNew(uwCnt)				OSSemCreate(uwCnt)
//* 0等待时间查看指定的信号量是否可用，无论是否可用，函数都将立即返回，适合用于ISR
#define		OSAPISemWaitWith0Delay(hSem)	OSSemAccept(hSem)
//* 发送一个信号
#define		OSAPISemSend(hSem)				OSSemPost(hSem)
//* 退出阻塞资源
#define		OSAPIBlockExit(hBlock)			OSMutexPost(hBlock)
//*--------------------------------------- 函数原型声明 -------------------------------------------
extern INT8U OSAPISemWait(HANDLER hSem, INT16U uwWaitMS);
extern void OSAPISemFree(HANDLER hSem);
extern void OSAPISemFreeExt(HANDLER hSem);
extern HANDLER OSAPIBlockNew(INT8U ubPIP);
extern INT8U OSAPIBlockEnter(HANDLER hBlock, INT16U uwWaitMS);
extern INT8U OSAPIQPost(HANDLER hQueue, void *pvMsg);
extern INT8U OSAPIQReceive(HANDLER hQueue, PP ppMsg, INT16U uwTimeout);

#endif

