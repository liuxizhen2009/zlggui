/**
**********************************************************************
*@file 
*@brief
*@author   ÁõÎ÷Õñ rundream1314@gmail.com
*@date
*@version
***********************************************************************
*/

#include "..\\User\\Common.h"
extern OS_EVENT * pSimKeyMsgQ;

//pUsartGetMsg=(TskComMsg *)OSQPend(pUSART1MsgQ,0,&os_err);



INT8U SimKeyPend(void){
	INT8U os_err;
	TskComMsg * pSimKeyGetMsg;
	pSimKeyGetMsg=(TskComMsg *)OSQPend(pSimKeyMsgQ,0,&os_err);
	if(os_err==OS_ERR_NONE){
		
			return (pSimKeyGetMsg->Data.Val);
	}
	return 0;
}

void SimKeyPost(INT8U KeyVal){
	INT8U os_err;
	static TskComMsg SimKeyMsg;
	
	SimKeyMsg.MsgType=KeyType;
	SimKeyMsg.Data.Val=KeyVal;
	
	if(OS_ERR_NONE!=OSQPost(pSimKeyMsgQ,&SimKeyMsg)){
		LWIP_ERROR(("msg Post err"));
	};
}







/***********************************************************************
*/

