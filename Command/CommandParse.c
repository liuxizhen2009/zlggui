
 #include "..//User//common.h"
 #include "..//Command//Command.h"

/**
	typedef struct AT_Commnd
{
	INT8U * pCommand;
	INT8U  Len;
	INT8U (* Judge)(INT8U *pBuf,INT8U* pCmd,INT8U CmdSize);//判断是不是该命令
	INT8U (* ParaseFun)(INT8U *pBuf,INT8U* pCmd);//如果是该命令下一步要解析
	INT8U * Comment;	
}AT_Commnd;
*/
#define MAX_COMMOND_NUM 0x1f

//extern INT8U TEST_Command(INT8U *pBuf,INT8U* pCmd);
INT8U MyStrCompare(INT8U *Str1,INT8U*Str2,INT8U Len) ;
INT8U JudgeCommand(INT8U *pBuf,INT8U*Str2,INT8U Len);

INT8U 	HelpCommand(INT8U *pBuf,INT8U* pCmd);
extern  INT8U  SimKeyCommand(INT8U *pBuf,INT8U* pCmd);

AT_Commnd CommandGroup[MAX_COMMOND_NUM]=
{
	
	{"#Help",sizeof("#Help"),JudgeCommand,HelpCommand,"#Help"},
	{"#SimKey",sizeof("#SimKey"),JudgeCommand,SimKeyCommand,"#SimKey"},

	{NULL,sizeof(NULL),NULL,NULL}

}	;
INT8U 	HelpCommand(INT8U *pBuf,INT8U* pCmd)
{
	INT8U SubIndex=0;
	while(CommandGroup[SubIndex].pCommand!=NULL)
	{
		TLOG("%s\n",CommandGroup[SubIndex].Comment);
		SubIndex++;
	};
	return 0;
}

INT8U MyStrCompare(INT8U *Str1,INT8U*Str2,INT8U Len)
{
        while(Len-->1)
        {
            if(*Str1++!=*Str2++)
            {
                   return 0;
            }
        }
        return 1;
};	
 /**
 *************************************************************
 	@brief
	@param
	@param
	@return 
**************************************************************
*/

INT8U JudgeCommand(INT8U *pBuf,INT8U*Str2,INT8U Len)
{
	TskComMsg *	p_msg=(TskComMsg *)pBuf;
	return MyStrCompare(p_msg->Data.Ptr,Str2,Len);
}


INT8U CommandParse(INT8U *pBuf,AT_Commnd AT_Group[])
 {
	INT8U SubIndex=0;
	INT8U State=0;
	while(AT_Group[SubIndex].pCommand!=NULL)
	{
		//UDLOG("parase command\n");
//		UDLOG("%s\n",AT_Group[SubIndex].Comment);
		if(AT_Group[SubIndex].Judge!=NULL)
		{
			State=AT_Group[SubIndex].Judge(pBuf,AT_Group[SubIndex].pCommand,AT_Group[SubIndex].Len);	
		}
		switch(State)
		{
            case(0):
				break;
            case(1)://匹配对了
				if(AT_Group[SubIndex].ParaseFun!=NULL)
                 	AT_Group[SubIndex].ParaseFun(pBuf,AT_Group[SubIndex].pCommand);
				break;
			case(2):
				break;
			default: break;
		}

		SubIndex++;

	};
	return 0;	 		
 }



