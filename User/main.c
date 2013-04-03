/**
**********************************************************************
*@file 
*@brief
*@author   刘西振 rundream1314@gmail.com
*@date
*@version
***********************************************************************
*/

#include "main_include.h "

/**
*************************************************************************
*@brief
*@author 刘西振  rundream@gmail.com
*@param
*@param
*@return
*************************************************************************
*/
int main(void)
{
	volatile INT8U os_err;
	
	//由于使用UCOS,一下的初始化虽然可以在OS运行之前运行,但注意别使能任何中断.
	CLI(); //关闭总中断 
	ChipHalInit();			//片内硬件初始化	 

	ChipOutHalInit();		//片外硬件初始化
	OSInit();				//UCOS INIT~

	os_err = OSTaskCreate( TaskStart,									//第一个任务0
                          (void          * ) 0,							//不带参数
                          (OS_STK        * )&TaskStartStk[128-1],			//堆栈指针
                          (INT8U           ) START_TSK_PRIO				//优先级
                         );
	OSStart(); 
	
	while(1);
}
/**
*************************************************************************************
*@brief
*@author 刘西振  rundream@gmail.com
*@param
*@param
*@return
**************************************************************************************
*/
void OS_EventInital(void);

void TaskStart(void *p_arg)
{
	volatile INT8U os_err;
	INT8U TskIndex=0;
	
	
	OS_CPU_SysTickInit();	//使能SYSTICK中断
	
#if (OS_TASK_STAT_EN > 0)
    OSStatInit();       /*这东西可以测量CPU使用量 */
#endif
	

	//初始化需要的信号量，os 定时器等内容。
	OS_EventInital();
	
	while(TskFunArray[TskIndex]!=NULL)
	{
	 	os_err=OSTaskCreate(TskFunArray[TskIndex],
						(void *)0,
						(OS_STK*)&TaskStk[TskIndex][TSK_STK_DEPTH-1],
						(INT8U)(APP_BASE_TSK_PRIO+TskIndex*2+1)
						);
		switch(os_err)
		{
			case(OS_ERR_NONE):	break; 
			default:
				ELOG("error:TskIndex==%d\n",TskIndex);
				while(1);
					break;					 		
		}
		TskIndex++;		
	}
	
		//使能其他板上中断
	NVIC_Configuration();
	SEI(); //开启总中断
	
	for(;;)
	{
		OSTimeDly(OS_TICKS_PER_SEC);
		LedOperate(LED1,LedOn);
		OSTimeDly(OS_TICKS_PER_SEC);
		LedOperate(LED1,LedOff);
	}

}
/**
***********************************************************************
*@brief
*@author   刘西振 rundream1314@gmail.com
*@param
*@param
*@return
************************************************************************
*/
void OS_EventInital(void)
{
		INT8U os_err;
		pUSART1MsgQ=OSQCreate(&UsartQue[0],N_Message);
		pAppMsgQ=OSQCreate(&AppQue[0],N_Message);
	  pSimKeyMsgQ=OSQCreate(&SimKeyQue[0],N_Message);
		/*OS_TMR  *OSTmrCreate (
					  INT32U           dly,
                      INT32U           period,
                      INT8U            opt,
                      OS_TMR_CALLBACK  callback,
                      void            *callback_arg,
                      INT8U           *pname,
                      INT8U           *perr)*/
		pSoftTmr1=OSTmrCreate(
						2,
						1,
						OS_TMR_OPT_PERIODIC,
						NULL,
						NULL,
						"SoftTmr1",
						&os_err
						);
		pCMX7141ScanTimer=AppTimerCreat(Preiod,Led1Toggle,NULL,10);	
		
}

/**************************************************************************************
*/

