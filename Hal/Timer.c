/**
*****************************************************************************************
*@file 
*@brief
*@author
*@date
*@version
*****************************************************************************************
*/
/**
*************************************************************************************
*@brief
*@author
*@param
*@param
*@return
**************************************************************************************
*/
#include "..\\User\\common.h"
#include "stm32f10x.h"
#include ".\\Hal.h"

u16 Time3_Counter = 0;
u16 nTime3 = 0;
u16 Time4_Counter = 0;
u16 nTime4 = 0;
#define MAX_100US_TIMER 8
sys_timeout time100us[MAX_100US_TIMER];

/*
void TIMER4_US_MS_Config(bool Magnitude)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	if(Magnitude == MS)
	{
		TIM_DeInit(TIM4);
		TIM_TimeBaseStructure.TIM_Period = 8000;//   
		TIM_TimeBaseStructure.TIM_Prescaler = SystemFrequency/8000000 - 1;    	//预分频,此值+1为分频的除数
		TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数	
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	//	使能预装载
		TIM_ARRPreloadConfig(TIM4, ENABLE);
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4|TIM_IT_Update);
	}
	else if(Magnitude == US)
	{
		TIM_DeInit(TIM4);
		TIM_TimeBaseStructure.TIM_Period = 8;
		TIM_TimeBaseStructure.TIM_Prescaler = SystemFrequency/8000000 - 1; //预分频,此值+1为分频的除数
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;  //
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数	
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);			
		TIM_ARRPreloadConfig(TIM4, ENABLE);
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4|TIM_IT_Update);
	}
}
*/

void TIMER_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);


	/* TIM2 configuration */
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period =60000/100;//
	TIM_TimeBaseStructure.TIM_Prescaler =SystemFrequency/60000; //8000000
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/*使能预装载*/
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4|TIM_IT_Update);
    /* Enable TIM2 Update interrupt [TIM2溢出中断允许]*/
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 
    /* TIM2 enable counter [允许tim2计数]*/
    TIM_Cmd(TIM2, ENABLE);   

}



void Start_CountingKey(void)
{
	/*溢出都配置中断*/
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);	
	/* 允许TIM2开始计数 */
	TIM_Cmd(TIM3, ENABLE);
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

void TIM2_IRQ(void)
{
	
}

void Stop_CountingKey(void)
{
	/*溢出都配置中断*/
	TIM_ITConfig(TIM3,TIM_IT_Update, DISABLE);	
	/* 允许TIM2开始计数 */
	TIM_Cmd(TIM3, DISABLE);
}

/**
***********************************************************************
*@brief
*@author   刘西振 rundream1314@gmail.com
*@param
*@param
*@return
************************************************************************
*/
bool ms_Timer_EN = FALSE;
void DelayUS(u16 Time)  //Millisecond = 1-->ms;= 0 --> us
{
	u16 i=10;
	while(Time--){
		i=10;
		while(i--);
	}
	

}

void DelayMS(u16 Time)
{
	u16 i=1200;
	while(Time--){
		i=1200;
		while(i--);
	}	
}


/**
***********************************************************************
*@brief	声明了10个自定义软件定时器的结构体空间
typedef struct sys_timeout {
	u8 time_type;	   			//定时类型，一次行的，还是周期的
  	struct sys_timeout *next;	//下一定时器。	
	sys_timeout_handler handler;//回调函数		
  	void *arg;			   		//回调参数	
	u32 ticks_reload;			//周期定时器重载值	
  	u32 ticks_counter;			//计数值	
}sys_timeout;
	一旦creat了可以stop但是不支持del，del后空间也是没有回收，不能在用来create

*@author   刘西振 rundream1314@gmail.com
*@param
*@param
*@return
************************************************************************
*/

SysTimeOut AppTimer[10];

void TimerWheel(SysTimeOut AppTimer[])
{
	u8 i=0;
//	while(AppTimer[i].next!=NULL)
for(i=0;i<MAX_TIMER_NUM;i++)
	{
		 //if(AppTimer[i].ticks_counter>0){}
		if(AppTimer[i].time_type==Preiod)
		{
			if(AppTimer[i].ticks_counter==0){

				AppTimer[i].handler(AppTimer[i].arg);//执行函数
				AppTimer[i].ticks_counter=AppTimer[i].ticks_reload;
			}
		   	AppTimer[i].ticks_counter--;
				
		}else if(AppTimer[i].time_type==Once){
			
			if(AppTimer[i].ticks_counter==1){//为了避免==0的时候重复执行，这里设置==1
				AppTimer[i].handler(AppTimer[i].arg);//执行函数			
			}
			if(AppTimer[i].ticks_counter){//只有在大于0的时候才--，==0的时候就不动了
				AppTimer[i].ticks_counter--;		
			}
			
		}else if(AppTimer[i].time_type==Paulse){//这个用来暂停定时器，
				
		};	
	}	
};

SysTimeOut * AppTimerCreat(enum TimerType time_type,sys_timeout_handler handler,void *arg,u32 ticks_reload)
{
	u8 i;
	for(i=0;(AppTimer[i].time_type!=UnUsed)&&(i<MAX_TIMER_NUM);i++);//寻找空闲的第一个位置
	if(i>=MAX_TIMER_NUM){
		ELOG("timer creat err\n");
		return NULL;
	}else{
		//AppTimer[i]
		if(i>0){
			AppTimer[i-1].next=&AppTimer[i];
		}
		AppTimer[i].time_type=time_type;
		AppTimer[i].handler=handler;
		AppTimer[i].arg=arg;
		AppTimer[i].ticks_reload=ticks_reload;
		AppTimer[i].ticks_counter=ticks_reload;
		return &AppTimer[i];
	}
			
}
/**
***********************************************************************
*@brief	 启动once类型定时器。
*@author   刘西振 rundream1314@gmail.com
*@param
*@param
*@return
************************************************************************
*/
void ResetOnce(SysTimeOut * pTimer,u32 Ticks){

	if(pTimer!=NULL){	
		pTimer->time_type=Once;
		pTimer->ticks_counter=Ticks;
	}
}



/************************************************************************
*/

