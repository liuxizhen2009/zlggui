/**
**********************************************************************
*@file 
*@brief
*@author   刘西振 rundream1314@gmail.com
*@date
*@version
***********************************************************************
*/

#include "..\\User\\Common.h"
#include "..\\User\\AppTask.h"

/**
***********************************************************************
*@brief	   
*@author   刘西振 rundream1314@gmail.com
*@param
*@param
*@return
************************************************************************
*/
void 	TskKeyDriver(void *p_arg)//32
{

	volatile	INT8U err;
	INT8U 	Scheld=0;
	TLOG("TaskKeyDriver start Prio=%02d\n",OSTCBCur->OSTCBPrio);
	for(;;)
	{	   
//		TLOG("11111111111111111111",Scheld++);
		//OSTimeDlyHMSM(0,0,10,0);//1分钟提示一次，当然也可以重启一次
		OSTimeDly(OS_TICKS_PER_SEC);
		
	}
};

/**
***********************************************************************
*@brief
*@author   刘西振 rundream1314@gmail.com
*@param
*@param
*@return
************************************************************************
*/
void 	TskUsartProc(void *p_arg) //pro31
{

	volatile INT8U os_err ;
	TskComMsg * pUsartGetMsg;//接收串口中断的信息的
	TLOG("TskUsartProc start Prio=%02d\n",OSTCBCur->OSTCBPrio);
	for(;;)
	{
	    
		pUsartGetMsg=(TskComMsg *)OSQPend(pUSART1MsgQ,0,&os_err);
	   switch(pUsartGetMsg->MsgType)
	   {
	   	case(UsartMainISR):
			CommandParse((INT8U *)pUsartGetMsg,CommandGroup);	
			//TLOG("hello\n");
			break;
		case(UsartDebugIsrType):
			
			break;
	   }
		//OSTimeDly(OS_TICKS_PER_SEC);

	}
};

/**
***********************************************************************
*@brief
*@author   刘西振 rundream1314@gmail.com
*@param
*@param
*@return
************************************************************************
*/
void 	TskAppProc(void *p_arg)
{
	volatile INT8U os_err ;
	TskComMsg * pAppMsg;//接收串口中断的信息的


	TLOG("TskAppProc start Prio=%02d\n",OSTCBCur->OSTCBPrio);
	for(;;)
	{
		
		pAppMsg=(TskComMsg *)OSQPend(pAppMsgQ,0,&os_err);
		OSTimeDly(OS_TICKS_PER_SEC);
	}		

}

/************************************************************************
*/
