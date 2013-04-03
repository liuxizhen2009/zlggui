/*
*********************************************************************************************************
*                                    Embedded Systems Building Blocks
*                                 Complete and Ready-to-Use Modules in C
*
*                                           Master Include File
*
*                            (c) Copyright 1999, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
* Filename   : INCLUDES.H
* Programmer : Jean J. Labrosse
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

                                       /* MODULE ENABLED (1) or DISABLED (0)                           */
#define   MODULE_KEY_MN    1           /* Keyboard module                                              */
#define   MODULE_LED       0           /* Multiplexed LED module                                       */
#define   MODULE_LCD       0           /* LCD Character module                                         */
#define   MODULE_CLK       0           /* Clock/Calendar module                                        */
#define   MODULE_TMR       0           /* Timer Manager module                                         */
#define   MODULE_DIO       0           /* Discrete I/O module                                          */
#define   MODULE_AIO       0           /* Analog   I/O module                                          */
#define   MODULE_COMM_PC   0           /* Asynchronous Serial Communications module                    */
#define   MODULE_COMM_BGND 0           /*    Foreground/Background buffered serial I/O                 */
#define   MODULE_COMM_RTOS 0           /*    Real-Time Kernel      buffered serial I/O                 */

#define   CFG_C                       /* Indicate that application specific code is found in CFG.C    */
#define   CFG_H                        /* Indicate that configuration #defines is found in CFG.H       */

/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define  FALSE    0
#define  TRUE     1

/*$PAGE*/
/*
*********************************************************************************************************
*                                         Standard Libraries (DOS)
*********************************************************************************************************
*/

#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
//#include    <conio.h>
//#include    <dos.h>
//#include    <setjmp.h>

/*
*********************************************************************************************************
*                                            uC/OS Header Files
*********************************************************************************************************
*/

//#include    "\software\ucos-ii\ix86l-fp\bc45\os_cpu.h"
#include    "..\UCOSII\os_cfg.h"
#include    "..\UCOSII\ucos_ii.h"
//#include    "\software\blocks\pc\bc45\pc.h"

/*$PAGE*/
/*
*********************************************************************************************************
*                                      Building Blocks Header Files
*********************************************************************************************************
*/
#define 	CFG_H
#ifdef      CFG_H
#include    ".\cfg.h"
#endif

#if         MODULE_KEY_MN
#include    ".\KEY_MN\key.h"
#endif

#if         MODULE_LCD
#include    "\software\blocks\lcd\source\lcd.h"
#endif

#if         MODULE_LED
#include    "\software\blocks\led\source\led.h"
#endif

#if         MODULE_CLK
#include    "\software\blocks\clk\source\clk.h"
#endif

#if         MODULE_TMR
#include    "\software\blocks\tmr\source\tmr.h"
#endif

#if         MODULE_DIO
#include    "\software\blocks\dio\source\dio.h"
#endif

#if         MODULE_AIO
#include    "\software\blocks\aio\source\aio.h"
#endif

#if         MODULE_COMM_PC
#include    "\software\blocks\comm\source\comm_pc.h"
#endif

#if         MODULE_COMM_BGND
#include    "\software\blocks\comm\source\commbgnd.h"
#endif

#if         MODULE_COMM_RTOS
#include    "\software\blocks\comm\source\commrtos.h"
#endif
