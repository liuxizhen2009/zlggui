

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
#include "..\\Hal\\Hal.h"

/**
***********************************************************************
*@brief	  LED的gpio的初始化在GPIO_confiurationZ中
*@author   刘西振 rundream1314@gmail.com
*@param
*@param
*@return   led 状态
************************************************************************
*/

BOOLEAN LedOperate(LedType Led,LedOptType Opt)
{
	switch(Led)
	{
		case(LED1):
			switch(Opt)
			{
				case(LedOff):
				LED1_OFF;					
				break;
				case(LedOn):
				LED1_ON	;					
				break;
				case(LedToggle):
					if(LED1_POLLING){
							LED1_ON;
						}else{
						
						   LED1_OFF;
						}					
				break;
				case(LedPolling):
					LED1_POLLING;					
				break;
				default:break;
			};			
		break;
		case(LED2):  
				switch(Opt)
				{
					case(LedOff):
					LED2_OFF;					
					break;
					case(LedOn):
					LED2_ON;					
					break;
					case(LedToggle):
						if(LED2_POLLING){
							LED2_ON;
						}else{						
						   LED2_OFF;
						}			
					break;
					case(LedPolling):
						LED2_POLLING;					
					break;
					default:break;
				};			
		break;
		default:break;
	};
	return TRUE;	
}
/**
***********************************************************************
*@brief	  测试用的可以删除
*@author   刘西振 rundream1314@gmail.com
*@param
*@param
*@return
************************************************************************
*/

void Led2Toggle(void)
{
	LedOperate(LED2,LedToggle);
}
/************************************************************************
*/
void Led1Toggle(void)
{
	LedOperate(LED1,LedToggle);
}









/*
************************************************************************
*/





