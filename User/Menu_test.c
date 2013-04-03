
/****************************************************************************************
* 文件名：TEST.C
* 功能：GUI测试及演示程序。
* 作者：黄绍斌
* 日期：2003.09.09
****************************************************************************************/
// #include "..//STM32lib//stm32f10x.h"
// #include "..//STM32lib//stm32f10x_gpio.h"
// #include "stdio.h"
#include "..//User//common.h"
#include "..//User//KeyValDefine.h"
#include "..//ZLG_GUI//config.h""

/****************************************************************************
* 名称：DelayNS()
* 功能：长软件延时
* 入口参数：dly		延时参数，值越大，延时越久
* 出口参数：无
****************************************************************************/

void  DelayNS(uint32  dly)
{  uint32  i;

   for(; dly>0; dly--) 
      for(i=0; i<5000; i++);
}


/****************************************************************************
* 名称：WaitKey()
* 功能：等待一个有效按键，并返回按键值。本函数有去抖功能。
* 入口参数：无
* 出口参数：返回按键盘值。
****************************************************************************/

extern INT8U SimKeyPend(void);
uint8  WaitKey(void)
{  uint32  i;
   uint8   key;
   uint8   key_bak;
#if 0  
   key = 0;							// 接收按键
   while(0==key)
   {  key = GetKey();
      for(i=0; i<1000; i++);
   }
   
   key_bak = 1;
   while(0!=key_bak)				// 等待按键放开
   {  key_bak =GetKey();
      for(i=0; i<1000; i++);
   }
#else
	 
	key=SimKeyPend();
#endif	 
   return(key);
}

/****************************************************************************
* 名称：DispLog()
* 功能：显示LOG，用坚线填充屏幕(填充0xAA)。
* 入口参数：无
* 出口参数：无
****************************************************************************/
void  DispLog(void)
{  GUI_FillSCR(0xaa);				// 填充0xAA
   DelayNS(30);
}


/****************************************************************************
* 名称：RunMenuDemo()
* 功能：进行菜单的演示操作。
* 入口参数：无
* 出口参数：无
****************************************************************************/

/*
typedef  struct
{  WINDOWS  *win;					// 所属窗口
		
	uint8    no;						// 主菜单个数
	uint8    state;				//	刘西振，当前选中的猪菜单
	uint8   start_disp;			//起始显示索引
	uint8   end_disp;			//接收显示的索引
	uint8   disp_num;			//可显示的菜单的个数，
	MENU_ITEM  item[MMENU_NO];
} MENU;
*/

MENU  * pActiveMenu=NULL;//指向一个全局的当前活动menu的指针。

MENU * SetActiveMenu(MENU  * pMenu){
	pActiveMenu=pMenu;
	return pActiveMenu;
}
MENU * GetActiveMenu(void){
	return pActiveMenu;
}


void  RunMenuDemo(void)
{ 
	WINDOWS  mainwindows;
	MENU    mainmenu;
	MENU    submenu1[5];
	MENU  * p;
	void (*pFunction)(void);
	
	//   uint8    mselect;
	uint8    select=0, bak;
	uint8    key;
	  
	/* 设置主窗口并显示输出 */
	mainwindows.x = 0;									
	mainwindows.y = 0;
	mainwindows.with = 128;
	mainwindows.hight = 64;
	mainwindows.title = (uint8 *) "liuxizhen";
	mainwindows.state = (uint8 *) "OK";
	GUI_RecDataFill(&mainwindows);	
	GUI_WindowsDraw(&mainwindows);						// 绘制主窗口
	
	mainmenu.win=&mainwindows;
	mainmenu.parent_menu=NULL;
	mainmenu.name="main";
//	mainmenu.sub_menu=(struct MENU *)submenu1;//(MENU *)(&submenu1[0]);
	mainmenu.no=6;
	mainmenu.state=0;
	mainmenu.start_disp=0;
//	mainmenu.end_disp=mainwindows.hight/MENU_HIGHT;
	mainmenu.disp_num=mainwindows.RecClient.hight/MENU_HIGHT;
	mainmenu.item[0].str="1Contacts";
	mainmenu.item[0].Function=NULL;
	mainmenu.item[0].sub_menu=&submenu1[0];
	
	mainmenu.item[1].str="2Message";
	mainmenu.item[1].Function=NULL;
	mainmenu.item[1].sub_menu=&submenu1[1];

	mainmenu.item[2].str="3Setings";
	mainmenu.item[2].Function=NULL;
	mainmenu.item[2].sub_menu=&submenu1[2];
	
	mainmenu.item[3].str="4ProductInfo";
	mainmenu.item[3].Function=NULL;
	mainmenu.item[3].sub_menu=NULL;
	
	mainmenu.item[4].str="5Test";
	mainmenu.item[4].Function=NULL;
	mainmenu.item[4].sub_menu=NULL;
	
	mainmenu.item[5].str="6Debug";
	mainmenu.item[5].Function=NULL;
	mainmenu.item[5].sub_menu=NULL;
	
	submenu1[0].win=&mainwindows;
	submenu1[0].parent_menu=&mainmenu;
	submenu1[0].name="contacts";
	submenu1[0].no=2;
	submenu1[0].state=0;
	submenu1[0].start_disp=0;
//	submenu1[0].end_disp=mainwindows.hight/MENU_HIGHT;
	submenu1[0].disp_num=(submenu1[0].no<mainwindows.RecClient.hight/MENU_HIGHT)\
		?submenu1[0].no:mainwindows.RecClient.hight/MENU_HIGHT;
	submenu1[0].item[0].str="1Contacters";
	submenu1[0].item[0].Function=NULL;
	submenu1[0].item[0].sub_menu=NULL;
	submenu1[0].item[1].str="2Add New";
	submenu1[0].item[1].Function=NULL;
	submenu1[0].item[0].sub_menu=NULL;
	
	submenu1[1].win=&mainwindows;
	submenu1[1].parent_menu=&mainmenu;
	submenu1[1].name="Message";
	submenu1[1].no=2;
	submenu1[1].state=0;
	submenu1[1].start_disp=0;
	submenu1[1].disp_num=(submenu1[0].no<mainwindows.RecClient.hight/MENU_HIGHT)\
		?submenu1[0].no:mainwindows.RecClient.hight/MENU_HIGHT;
	submenu1[1].item[0].str="1NewMessage";
	submenu1[1].item[0].Function=NULL;
	submenu1[1].item[0].sub_menu=NULL;
	
	submenu1[1].item[1].str="2Receve";
	submenu1[1].item[1].Function=NULL;
	submenu1[1].item[1].sub_menu=NULL;
	
	submenu1[2].win=&mainwindows;
	submenu1[2].parent_menu=&mainmenu;
	submenu1[2].name="Setings";
	submenu1[2].no=2;
	submenu1[2].state=0;
	submenu1[2].start_disp=0;
	submenu1[2].disp_num=(submenu1[0].no<mainwindows.RecClient.hight/MENU_HIGHT)\
		?submenu1[0].no:mainwindows.RecClient.hight/MENU_HIGHT;
	submenu1[2].item[0].str="1Frenq";
	submenu1[2].item[0].Function=NULL;
	submenu1[2].item[0].sub_menu=NULL;
	
	submenu1[2].item[1].str="2Led";
	submenu1[2].item[1].Function=NULL;
	submenu1[2].item[1].sub_menu=NULL;

	
	SetActiveMenu(&mainmenu);
	GUI_MenuDraw(pActiveMenu);	
	while(1){	
		key  = WaitKey();

		switch(key){
		case(KEY_ENT):
			LWIP_DEBUGF(GUI_DEBUG|DBG_ON,("KEY_ENT:pActiveMenu->state=%d\n",pActiveMenu->state));
			if(pActiveMenu!=NULL){
				//这里就限制了submenu和function只能有其一，去掉else就可以两个都要了
				if(pActiveMenu->item[pActiveMenu->state].sub_menu!=NULL){//有submenu
					SetActiveMenu((pActiveMenu->item[pActiveMenu->state].sub_menu));
					GUI_MenuDraw(pActiveMenu);
				}else if(pActiveMenu->item[pActiveMenu->state].Function!=NULL)//没有子菜单，有功能函数，就要执行功能函数了
				{
					pFunction=pActiveMenu->item[pActiveMenu->state].Function;
					//(* )(void);
					(*pFunction)();//如果有函数执行函数。
				}
			}
		break;
		case(KEY_BACK):
			LWIP_DEBUGF(GUI_DEBUG|DBG_ON,("KEY_BACK:pActiveMenu->state=%d\n",pActiveMenu->state));
			if(pActiveMenu!=NULL){
				if(pActiveMenu->parent_menu!=NULL){//有submenu
					SetActiveMenu(pActiveMenu->parent_menu);
					GUI_MenuDraw(pActiveMenu);
				}
			}
		break;
		case(KEY_DOWN):
		{  
			
			LWIP_DEBUGF(GUI_DEBUG|DBG_ON,("KEY_UP:pActiveMenu->state=%d\n",pActiveMenu->state));			
			if(pActiveMenu->state<pActiveMenu->no-1){
					//pActiveMenu->state=0;
				pActiveMenu->state++;
				pActiveMenu->start_disp=pActiveMenu->state>(3)?(pActiveMenu->state-3):0;
				GUI_MenuDraw(pActiveMenu);
			}
		}	
		break;
		case(KEY_UP):
			LWIP_DEBUGF(GUI_DEBUG|DBG_ON,("KEY_DOWN:pActiveMenu->state=%d\n",pActiveMenu->state));
			if(pActiveMenu->state!=0){
				pActiveMenu->state--;
				pActiveMenu->start_disp=pActiveMenu->state>(3)?(pActiveMenu->state-3):0;
				GUI_MenuDraw(pActiveMenu);
			}
		break;
		default:break;
			
		}  
	}
		
	
	
}