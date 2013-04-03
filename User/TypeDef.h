/**
	@file TypeDef.h
	@brief SCDMA project 
	@author liuxizhen
	@date 2012年1月17日16:45:08
	@version V0.1
*/
/**
	@brief 这里面存放的是自己定义的一些信息类型。不含有参数的声明
	仅仅是类型的声明，宏定义也在这里

*/
#ifndef TYPE_DEF_H
	#define TYPE_DEF_H

#include "..//STM32Lib//stm32f10x.h"
#include "..//UCOSII//ucos_ii.h"

#define N_Message 32

#define NULL (void * ) 0
//定义了一个函数指针 2012年9月25日21:38:15 刘西振
typedef void (*TskFunPtr)(void * parg);

///////////////////////////
/**
	@brief 	任务间传递消息的结构体
*/
typedef struct
{
	INT8U		MsgType;
	INT8U 		SubMsgType;//这个参数主要是用来区分在串口传出的数据中，是哪个类型
						   //的，特别是需要传递数据块的时候，union是指针的时候，来辨别是那种数据
	union{
		INT8U *	Ptr;
		INT32U	Val;
		 }Data;
	
	INT8U		DataLen;	
	//INT8U_t		flag;
}TskComMsg;

typedef enum MessgeTypeId {	KeyType=1,
							UsartMainISR,
							TskStateType,
							UsartDebugIsrType,
							TskUsartType,
							StateRpType,
							EmergencyType}
							MessgeTypeId;//放在MessageType中的
typedef struct AT_Commnd
{
	INT8U * pCommand;
	INT8U  Len;
	INT8U (* Judge)(INT8U *pBuf,INT8U* pCmd,INT8U CmdSize);//判断是不是该命令
	INT8U (* ParaseFun)(INT8U *pBuf,INT8U* pCmd);//如果是该命令下一步要解析
	INT8U * Comment;	
}AT_Commnd;

typedef enum   MagnitudeType{
							US=0,
							MS	
}MagnitudeType;


typedef enum DigAnalogType
{
	AnalogMod=1,
	DigitalMod
	
}DigAnalogType;

typedef enum RxTxType
{
	RxMod=1,
	TxMod
}RxTxType;

typedef enum IsfCsfType
{
	ISF=1,
	CSF
}IsfCsfType;

typedef enum GetOrSet
{
	GET_S=1,
	SET_S
}GetOrSet;

typedef enum  e_comm_mode
{
	 comm_mode_voice = 0x0,				//000,            Voice Communication
	 comm_mode_voice_sld = 0x1,			//001,            Voice+SLD
	 comm_mode_t1_data = 0x2,			//010,            T1 Data Communication
	 comm_mode_t2_data = 0x3,			//011,            T2 Data Communicaiton
	 comm_mode_t3_data = 0x4,			//100,            T3 Data Communication
	 comm_mode_voice_append_data = 0x5	//101            //Voice+Appended Data(T2)
}e_comm_mode;

typedef struct 
{
     INT16U   caller_id[2];  //24 bits         
     INT16U   callee_id[2];  //24 bits
     e_comm_mode 	 comm_mode;  //3 bits
     INT8U   color_code[3]; //24 bits
}CallOutType;

//typedef call_out CallOutType;

//如何用一个结构体来描述一次呼叫？
//asc号码，频点，时间，生存时间（in s），tx rx类型，单呼组呼类型，dpmr号码，
typedef struct {
	INT8U AsicId[8];//"1234567" 7个号码，最后一个是‘\0’,号码是右对齐的，
	INT8U FreqPoint;
	INT8U HHMMSS[8];//asci的最后要带'\0'
	INT8U YYMMDD[8];
	IsfCsfType IsfCsf;
	INT8U LiveTicks;

	/////////////////////////以上是上层应该使用的数据
	//下面是底层使用的在上层进入下层的时候，根据上面的数据，
	//计算出下层需要的数据，
	//下层进入上层的时候，也是这样的，

	CallOutType Call;
	INT32U FreqParam1;
	INT32U FreqParam2;
	RxTxType TxRx;	
}CallInfoType;










typedef enum AudioSwithch{
	OFF,
	ON	
}Switch;


typedef struct FirmInfo
{
	const char Tag[64];
	const	u32 Len1;
	const	u16 * pData1;
	const	u32 Len2;
	const	u16 * pData2;
}FirmInfo;

/**
***********************************************************************
*@brief
 	借用了lwip里面的软件定时器的实现方式			
*@author   刘西振 rundream1314@gmail.com
*@param
*@param
*@return
************************************************************************
*/
	
typedef void (* sys_timeout_handler)(void *arg);//定时器的回调函数。

enum TimerType
{
	UnUsed=0,
	Preiod,
	Once,
	Paulse
};

typedef struct sys_timeout {
	enum TimerType time_type;	   			//定时类型，一次行的，还是周期的
  	struct sys_timeout *next;	//下一定时器。	
	sys_timeout_handler handler;//回调函数		
  	void *arg;			   		//回调参数	
	u32 ticks_reload;			//周期定时器重载值	
  	u32 ticks_counter;			//计数值	
}sys_timeout;
typedef sys_timeout SysTimeOut;

typedef enum LedType
{
	LED1=1,
	LED2,
	LED3
}LedType;
//对led的操作类型，
typedef enum LedOptType
{
	LedOff,
	LedOn,
	LedToggle,
	LedPolling
}LedOptType;

typedef struct FreqConfigType
{
	char FreqChannel;	//表示旋钮上的第几个频道(Asc)
	char FreqAsc[10];	//Asc形式的频率403000000\0
	///上面是写频软件应该发送的，下面是接收到写频软件的数据后应该填充的
	int 	FreqInt;		//FreqAsc转化后的频率
	int  	TxParam;		//发射需要写入寄存器的值
	int 	RXParam;	//接收
//	int 	RxParam2;
}FreqConfigType;




#endif

