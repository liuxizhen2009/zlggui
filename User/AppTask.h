/**
**********************************************************************
*@file 
*@brief
*@author   刘西振 rundream1314@gmail.com
*@date
*@version
***********************************************************************
*/



extern OS_EVENT * pUSART1MsgQ;//定义一个串口的邮箱
extern OS_EVENT * pAppMsgQ;//定义一个串口的邮箱;//定义一个串口的邮箱

extern AT_Commnd CommandGroup[];

extern INT8U CommandParse(INT8U *pBuf,AT_Commnd AT_Group[]);



//CallOutType CurrentCall,OldCall;