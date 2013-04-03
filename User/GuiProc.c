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
//#include "..\\User\\AppTask.h"
void 	TskGuiProc(void *p_arg)//30
{

	volatile	INT8U err;
		TLOG("TskGuiProc start Prio=%02d\n",OSTCBCur->OSTCBPrio);
		GUI_Initialize();				// 初始化LCM
 		GUI_SetColor(1, 0);//设置前景色和背景色的
		DispLog();						// 显示LOG
		RunMenuDemo();					// 显示菜单
		for(;;)
	{	   

		OSTimeDly(OS_TICKS_PER_SEC);//
		
	}
};
