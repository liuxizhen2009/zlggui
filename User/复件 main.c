

#include "main_include.h "

/**
*************************************************************************************
*@brief
*@author 刘西振  rundream@gmail.com
*@param
*@param
*@return
**************************************************************************************
*/
int main(void)
{
	volatile u8 os_err;
	
	//由于使用UCOS,一下的初始化虽然可以在OS运行之前运行,但注意别使能任何中断.
//	CLI(); //关闭总中断 
	ChipHalInit();			//片内硬件初始化	 
	init_printf(printf_buf,uart1_putc);
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

void TaskStart(void *p_arg)
{
	volatile u8 os_err;
	INT8U TskIndex=0;
	
	
	OS_CPU_SysTickInit();	//使能SYSTICK中断
	
#if (OS_TASK_STAT_EN > 0)
    OSStatInit();       /*这东西可以测量CPU使用量 */
#endif
	

	pUSART1MsgQ=OSQCreate(&UsartQue[0],N_Message);
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


