/**
*************************************************************************
*@brief	 配置全集的编译选项
*@author 刘西振  rundream@gmail.com
*@param
*@param
*@return
*************************************************************************
*/


#ifndef APP_OPTS_H
#define APP_OPTS_H
#include "..\\Logger\\Debug.h"
/////////////////////
#define MAX_TIMER_NUM 10







///////////////



/* 来自于LWIP 的调试方法，Debugging options all default to off */


#ifndef DBG_TYPES_ON
#define DBG_TYPES_ON                    DBG_ON//这个跟别的是不同的，应该可以用的选项是DBG_TRACE一类的
#endif

#ifndef DBG_MIN_LEVEL
#define DBG_MIN_LEVEL                   DBG_LEVEL_OFF
#endif

#ifndef GUI_DEBUG
#define GUI_DEBUG                    	DBG_ON
#endif

#ifndef LCD_DEBUG
#define LCD_DEBUG                    	DBG_ON
#endif
#endif

/*************************************************************************
*/

