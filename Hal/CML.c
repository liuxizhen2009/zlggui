/**
*****************************************************************************************
*@file 	  CML.c
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
#include "..\\User\\Common.h"
#include ".\\CML.h"
#include ".\\FunImage5.h"
#include ".\\Hal.h"
void CBUS_Send16U(u8 addr,u16 data);

INT8U  Chack_Prog_flag(INT16U TimeOutCounter);

static IsfCsfType CsfOrIsf;//默认的是CSF
static Switch JianTing;
//static IsfCsfType CsfOrIsf;//默认的是CSF

IsfCsfType CsfIsfOperate(GetOrSet Oper ,IsfCsfType Param)
{
   if(Oper==GET_S){
		return CsfOrIsf;
	}else if(Oper==SET_S){
		CsfOrIsf=Param;
		
		if (CsfOrIsf== ISF){
			LWIP_DEBUGF(CML_DEBUG,("SET ISF MODE\n"));
			CBUS_Send16U(MODEM_CFG,0x0000);
		}else if(CsfOrIsf== CSF){
			LWIP_DEBUGF(CML_DEBUG,("SET CSF MODE\n"));
		 	CBUS_Send16U(MODEM_CFG,0x0200);
		}else{ELOG("err\n");}
		return Param;
	}
	ELOG("Param err\n");
	return 0;
};

Switch JianTingOperate(GetOrSet Oper ,Switch Param)
{
 	if(Oper==GET_S){
		return JianTing;
	}else if(Oper==SET_S){
	 	JianTing=Param;
		if (JianTing==OFF){
			LWIP_DEBUGF(CML_DEBUG,("SET ISF JianTing OFF \n"));
			if (CsfOrIsf== ISF){
				LWIP_DEBUGF(CML_DEBUG,("ISF Jianting Off\n"));
				CBUS_Send16U(MODEM_CFG,0x0000);
			}else if(CsfOrIsf== CSF){
				LWIP_DEBUGF(CML_DEBUG,("CSF Jianting Off\n"));
			 	CBUS_Send16U(MODEM_CFG,0x0200);
			}

		}else if(JianTing==ON){
			if (CsfOrIsf== ISF){
				LWIP_DEBUGF(CML_DEBUG,("SET ISF JianTing ON\n"));
				CBUS_Send16U(MODEM_CFG,0x1000);
			}else if(CsfOrIsf== CSF){
				LWIP_DEBUGF(CML_DEBUG,("SET CSF JianTing On\n"));
		 		CBUS_Send16U(MODEM_CFG,0x1200);
			}
		}else{ELOG("err\n");}
		return JianTing;
	}
	ELOG("Param err\n");
	return 0;
}
extern INT8U SetOwnIdCommand(INT8U *pBuf,INT8U* pCmd);
void CML_OwnID_Set(u32 Binary_ID1, u32 Binary_ID2 , u8 Page);
Switch JianTingOperate(GetOrSet Oper ,Switch Param);
extern RxTxType RfStateOperate(GetOrSet Oper,RxTxType Param);
extern BOOLEAN IsRfInstallOperate(GetOrSet Oper,BOOLEAN Param);
//
void CML7141Initial(void)
{
//	u32 ISFRxID1=0x1ffff,ISFRxID2=0x2ffff;	
//	LWIP_DEBUGF(CML_DEBUG,("Set Rec Mode\n")); 
//	CML_OwnID_Set(ISFRxID1, ISFRxID2, 0);
		 
	CsfIsfOperate(SET_S,CSF);
	IsRfInstallOperate(SET_S,TRUE);
	SetOwnIdCommand(NULL,NULL);
	JianTingOperate(SET_S,ON);

	CBUS_Send16U(0xC0,0xff40);	//CBUS_SendTwoByte(0xC0,0x30,0x40);	  //Powerdown Control: Enabled: DISC amp, IP1, bias
	CBUS_Send16U(0xB1,0x0010);    //CBUS_SendTwoByte(0xB1,0x0b,0x58);//Input Gain: Input1=0dB, DISC->Input1
	CBUS_Send16U(0xC1,0x0000);	 //idle

    Chack_Prog_flag(2*CML_TIME_OUT);//
	CBUS_Send16U(0xC1,0x0011);	 //set Rx 4FSK formatted, Rx mode	
}


/*--------------------通过SPIRF发送数据------------------------*/
 void SPI_RF_Senddata(u32 data)
{
	 u8 i;
	 SPI_RF_CS_L;
	for(i=0;i<22;i++)
	{
		SPI_RF_CLK_L;
		delay55ns;
		if(data&0x00200000)
		{
			SPI_RF_data_H;
			delay55ns;					
		}
		else
		{
			SPI_RF_data_L;
			delay55ns;
		}
		SPI_RF_CLK_H;
		delay55ns;
		data=data<<1;
	}
	SPI_RF_CS_H;
}

/*-------------------------通过CBUS发送数据---------------------------*/
static void CBUS_Senddata(u8 data)
{
    u8 i;
	for(i=0;i<8;i++)
	{
	 	CBus_CLK_L;
	 	delay110ns;
	 	if(data&0x80)
			CBus_data_H;
		else
		    CBus_data_L;
		CBus_CLK_H;
		delay110ns;
		data=data<<1;
	}
}

/*--------------------通过CBUS接收数据------------------------*/
static u8 CBUS_Recdata(void)
{
    u8 recdata=0,i;
	CBus_CLK_L;
	delay110ns;
	for(i=0;i<8;++i)
	{
		recdata=recdata<<1;
	 	CBus_CLK_H;
		delay55ns;
	 	if(CBus_Revdata)
			recdata|=0x01;
		CBus_CLK_L; 
		delay210ns;
	}
	return (recdata);
}

/*---------------------------CBUS复位-----------------------------*/
static void CBUS_REST(void)
{ 
	u8 i;
	CBus_CS_L;
	delay110ns;
	CBUS_Senddata(0x01);
	CBus_CS_H;
	for(i=0;i<10;i++)
	delay110ns;
}

/*-----通过CBUS发送两个字节:addr为地址，data_H_8为高8位数据，data2为低8位数据-----*/
void CBUS_SendTwoByte(u8 addr,u8 data_H_8,u8 data_L_8)
{	
	 u8 i;
	 CBus_CS_L;
	 delay110ns;
	 CBUS_Senddata(addr);
	 delay210ns;
	 CBUS_Senddata(data_H_8);
	 delay210ns;
	 CBUS_Senddata(data_L_8);
	 CBus_CS_H;
	 for(i=0;i<10;i++)
	   delay110ns;	
}
/*-----通过CBUS发送两个字节:addr为地址，data_H_8为高8位数据，data2为低8位数据-----*/
static void CBUS_Send8U(u8 addr,u8 data)
{	
	 u8 i;
	 CBus_CS_L;
	 delay110ns;
	 CBUS_Senddata(addr);
	 delay210ns;
	 CBus_CS_H;
	 for(i=0;i<10;i++)
	   delay110ns;	
}
/*-----通过CBUS发送两个字节:addr为地址，data_H_8为高8位数据，data2为低8位数据-----*/
/*先发送高位字节*/
void CBUS_Send16U(u8 addr,u16 data)
{	
	 u8 i;
	 CBus_CS_L;
	 delay110ns;
	 CBUS_Senddata(addr);
	 delay210ns;
	 CBUS_Senddata((u8)(data>>8));
	 delay210ns;
	 CBUS_Senddata((u8)(data));
	 CBus_CS_H;
	 for(i=0;i<10;i++)
	   delay110ns;	
}
/*---------------------------通过CBUS读两个字节-------------------------*/
u16 CBUS_RecTwoByte(u8 addr)
{	
	 u16 recdata=0;
	 u8  i;
	 CBus_CS_L;
	 delay110ns;
	 CBUS_Senddata(addr);
	 delay210ns;
	 recdata=CBUS_Recdata();
	 recdata<<=8;
	 delay210ns;
	 recdata|=CBUS_Recdata();
	 delay110ns;
	 CBus_CS_H;
	 for(i=0;i<10;i++)
	   delay110ns;
	 return (recdata);	
}
/**
*************************************************************************************
*@brief
const FirmInfo Firm5
*@author
*@param
*@param
*@return
**************************************************************************************
*/
//#define CML_TIME_OUT 0x800


u8 CML_FirmwareDownload(void)
{
    u16* ptr; //const unsigned int *ptr ;	
    u16 i,cnt,chk1, chk2, chk3, chk4;
	s16 TimeOutCounter;    
		LWIP_DEBUGF(CML_DEBUG,("%s\n",Firm5.Tag));
		CBUS_REST();       //cmx7041reset();
		DelayMS(500);		
	
	/*----------------------------DB1 DownLoad Begin-----------------------------------*/	
		CBUS_Send16U(TXDATA1,DB1_PTR);//Set24BitWrite( 0xb6, DB1_PTR ) ;
		CBUS_Send16U(TXDATA2,DB1_LEN); //Set24BitWrite( 0xb7, DB1_LEN ) ;	

		TimeOutCounter=CML_TIME_OUT;
		do { 
			DelayUS(10);	 //本次修改参照原文本
			if(TimeOutCounter--<0){
				ELOG("time out\n");
				return 1;
			}
		}while(!(CBUS_RecTwoByte(STAT)&0x0001));			
		
		CBUS_Send16U(PROG_REG,0x0001);//Set24BitWrite( 0xc8, 0x0001 ) ;	
		TimeOutCounter=CML_TIME_OUT;
		do { 
			DelayUS(10);	 //本次修改参照原文本
			if(TimeOutCounter--<0){
				ELOG("time out\n");
				return 1;
			}
		}while(!(CBUS_RecTwoByte(STAT)&0x0001));
	
		cnt = DB1_LEN ;
		ptr = (u16*)db1 ;
		do 
		{

		    CBUS_Send16U(PROG_REG,*ptr);//Set24BitWrite(0xc8, *ptr) ;
		    do
			{
				DelayUS(1);	 //本次修改参照原文本
			}while(!(CBUS_RecTwoByte(STAT)&0x0001));
			ptr++ ;
		}while ( cnt-- ) ;
	/*----------------------------DB1 DownLoad End-----------------------------------*/
	/*----------------------------DB2 DownLoad Begin---------------------------------*/	
		ptr = (u16*)db2 ;
		CBUS_Send16U(TXDATA1,DB2_PTR);//Set24BitWrite(0xb6, DB2_PTR ) ;
		CBUS_Send16U(TXDATA2,DB2_LEN);//Set24BitWrite(0xb7, DB2_LEN ) ;
		do
		{
			DelayUS(10);	 //本次修改参照原文本
		}while(!(CBUS_RecTwoByte(STAT)&0x0001));
	
		CBUS_Send16U(PROG_REG,0x0001);
		TimeOutCounter=CML_TIME_OUT;
		do { 
			DelayUS(10);	 //本次修改参照原文本
			if(TimeOutCounter--<0){
				ELOG("time out\n");
				return 1;
			}
		}while(!(CBUS_RecTwoByte(STAT)&0x0001));
		
		cnt = DB2_LEN ;
		ptr = (u16*)db2 ;
		do 
		{

		    CBUS_Send16U(PROG_REG,*ptr);//Set24BitWrite( 0xc8, *ptr ) ;
			do
			{
				DelayUS(10);	 //本次修改参照原文本
			}while(!(CBUS_RecTwoByte(STAT)&0x0001));
		    ptr++ ;
		} while ( cnt-- ) ;
		
		CBUS_Send16U(TXDATA1,ACTIVATE_PTR); //Set24BitWrite(0xb6, ACTIVATE_PTR) ;
		CBUS_Send16U(TXDATA2,ACTIVATE_LEN); //Set24BitWrite(0xb7, ACTIVATE_LEN) ;
		CBUS_Send16U(PROG_REG,0x0001);//Set24BitWrite( 0xc8, 0x0001 ) ;
		TimeOutCounter=CML_TIME_OUT;
		do { 
			DelayUS(1);	 //本次修改参照原文本
			if(TimeOutCounter--<0){
				ELOG("time out\n");
				return 1;
			}
		}while(!(CBUS_RecTwoByte(STAT)&0x0001));
	/*----------------------------DB2 DownLoad End---------------------------------*/	
	/*----------------------------Check---------------------------*/	
		chk1 = CBUS_RecTwoByte( 0xa9 ) ; // read checksum 这几个是复用的寄存器，check不是主要功能
		chk2 = CBUS_RecTwoByte( 0xaa ) ;
		chk3 = CBUS_RecTwoByte( 0xb8 ) ;
		chk4 = CBUS_RecTwoByte( 0xb9 ) ;
		
	if(!((chk3 == DB1_CHK_HI) && (chk4 == DB1_CHK_LO) &&(chk1 ==DB2_CHK_HI) && (chk2 ==DB2_CHK_LO)))
		{
			ELOG("Check fail\n");
			return 1;
		};

	/*----------------------------ACTIVE_CODE---------------------------*/
	
	DelayUS(10);
	CBUS_Send16U(PROG_REG,(u16)(ACTIVE_CODE>>16)); //Set24BitWrite( 0xc8, ACTIVE_CODE/0X10000) ;
		TimeOutCounter=CML_TIME_OUT;
		do { 
			DelayUS(10);	 //本次修改参照原文本
			if(TimeOutCounter--<0){
				ELOG("time out\n");
				return 1;
			}
		}while(!(CBUS_RecTwoByte(STAT)&0x0001));
	
	DelayUS(10);
	CBUS_Send16U(PROG_REG,(u16)(ACTIVE_CODE));; //Set24BitWrite(0xc8,(unsigned int)ACTIVE_CODE) ;
		TimeOutCounter=CML_TIME_OUT;
		do { 
			DelayUS(10);	 //本次修改参照原文本
			if(TimeOutCounter--<0){
				ELOG("time out\n");
				return 1;
			}
		}while(!(CBUS_RecTwoByte(STAT)&0x0001));
	/*----------------------------ACTIVE_CODE---------------------------*/
	return CML_NO_ERR;
}

/*-----------------完成AT010的的发射链路设置----------------------*/
void CML_VCO_Config(u32 data)
{
	SPI_RF_Senddata(data);
}

/**
*************************************************************************************
*@brief
*@author
*@param
*@param
*@return
**************************************************************************************
*/

/*----------------------CBUS高层驱动函数------------------------*/


/*-----------完成CML的编程状态位查询，等待直到可以编程----------*/
INT8U  Chack_Prog_flag(INT16U TimeOutCounter)
{
	//INT16S TimeOutCounter;//=CML_TIME_OUT;
	if(TimeOutCounter==0){
		TimeOutCounter=TimeOutCounter-1;
	};
	while(!(CBUS_RecTwoByte(STAT)&0x0001)){
	 if(!(TimeOutCounter--)){
	 	ELOG("timeout err\n");
	 	return CML_TIME_OUT_ERR;
	 };

	};
	return 	 CML_NO_ERR; 
}

 
/*------------完成CML的的发射链路设置Config_for_Gain and Offsets---------------------*/
INT8U CML_Gain_Offsets_Config(void)
{
   u16 Program_Block4[6] = {0x8000,0x0000,0x0530,0x130e,0x0000,0x0000};   
   u8 i;//Upper_8_Bit,Lower_8_Bit,;
   
//   CBUS_SendTwoByte(0xc1,0x00,0x00);
//   Chack_Prog_flag();
//   CBUS_SendTwoByte(0xc1,0x02,0x30);
   for(i = 0;i < 6;i++)
   {
	   CBUS_Send16U(PROG_REG,Program_Block4[i]);
	   if(CML_NO_ERR!=Chack_Prog_flag(CML_TIME_OUT)){
	   	ELOG("timeout err\n");
		return CML_ERR;
	   };
	   DelayMS(10);// Delay #10

	}
	return CML_NO_ERR;
//	CBUS_SendTwoByte(0xB1,0x12,0xc0);//12,00   //Input Gain: Output1->MOD1, Output2->MOD2,DISC->Input1
}


void CML_Power_Contr(void)
{

	//u8 A_Ch_Power_Level_TX[16];
	//u8 D_Ch_Power_Level_TX[16];
    //2.1V  高   8290
	//1.54V 中	 81DF
	//1.05V 低	 8147
	//0.8V  极低 80F4

/*
    if(Tx_Status)
	{
		if (TX_Power = 0x01)
        	CBUS_SendTwoByte(0xA8,0x81,0x47); //A8[11:10] = 00 DAC1;A8[9:0] 
		else if (TX_Power = 0x02)
        	CBUS_SendTwoByte(0xA8,0x81,0xDF); //A8[11:10] = 00 DAC1;A8[9:0] 
		else if (TX_Power = 0x03)
        	CBUS_SendTwoByte(0xA8,0x82,0x90); //A8[11:10] = 00 DAC1;A8[9:0] 
		else
        	CBUS_SendTwoByte(0xA8,0x80,0xF4); //A8[11:10] = 00 DAC1;A8[9:0] 	
	}
	else
*/
        CBUS_Send16U(AUXDAX_DATA_CTL,0x8001); //dac1 0dbm 输出功率最小
    DelayMS(1);
}

void CML_Speaker_Gain(void)
{
    /*618 speaker 增益 CMX618 */
	/*Undo:Increase Speaker/Output Gain by +6dB */
//	u8 Speaker_Gain = 0xFF; 
	CBUS_Send16U(ACONFIG,0x01ff);
}
/**
*************************************************************************************
*@brief	  CBUS_Send16U(0xC1,0x0000);
	Chack_Prog_flag();
	CBUS_Send16U(0xC0,0xff40);	  //Turn on all analogue hardware
	CBUS_Send16U(0xB0,0x3100);	  //与RF最大不同，即MOD1同向
	CBUS_Send16U(0xB1,0x12C0);     	
	CML_Gain_Offsets_Config();

	CBUS_Send16U(0xCB,0x0fff); //load 5 TxDATA registers
	CBUS_Send16U(0xCA,0x0fff);	
	CBUS_Send16U(0xB7,0x100D);
	CBUS_Send16U(0xB6,0x0000);
	CBUS_Send16U(0xB5,0x1009);	
	CBUS_Send16U(0xC1,0x0012);		//set Tx burst mode
*@author
*@param
*@param
*@return
**************************************************************************************
*/
//extern RxTxType TxRxStatus;
//extern BOOLEAN  IsRfInstall;

extern RxTxType RfStateOperate(GetOrSet Oper,RxTxType Param);
extern void Ch_Freq_Config(RxTxType TorRm,u32 Freq);


void CML_Call_Digital(CallOutType * pInfo)
{
	u32 Freq=403;
//	TxRxStatus=TxMod;
	RfStateOperate(SET_S,TxMod);
	/*缺少RF*/
	//在发送前先将7141置为发送空闲
//	CBUS_SendTwoByte(0xC1,0x00,0x00);	 //刘西振2012年11月13日15:48:06添加
	CBUS_Send16U(MODE,0x0000);//CBUS_SendTwoByte(0xC1,0x00,0x00);
	Chack_Prog_flag(CML_TIME_OUT);
	CML_RF_Config(DigitalMod,TxMod);

		//CBUS_SendTwoByte(0xC0,0xff,0x40);	  //Turn on all analogue hardware
	CBUS_Send16U(PWD,0xff40);	  //Turn on all analogue hardware
	DelayMS(1);
	CML_Power_Contr();
		//这个里面既有直接对射频的控制，又有对7141的控制。
	Ch_Freq_Config(TxMod,Freq);//这个是我自己写死的 liuxizhen
	if(IsRfInstallOperate(GET_S,0)){
		LWIP_DEBUGF(CML_DEBUG,("\tRf Installed\n"));
		CBUS_Send16U(ANALOG_OUT_GAIN,0xB100);//MOD1=8dB(反相因为MOD1比MOD2多一路反相1:1放大), MOD2=12dB
	}else{
		LWIP_DEBUGF(CML_DEBUG,("\tRf NOT Installed\n"));
		CBUS_Send16U(ANALOG_OUT_GAIN,0x3100);//与RF最大不同，即MOD1同向
	}		
	DelayMS(1);
		//CBUS_SendTwoByte(0xB1,0x12,0xC0); 
	CBUS_Send16U(INPUT_GAIN_SIG_ROUT,0x12C0); //(12,C0)Input Gain: Output1->MOD1, Output2->MOD2,DISC->Input1	
	DelayMS(1);
 		//01_b9//	CBUS_SendTwoByte(0xCB,CB_H,CB_L);//load 5 TxDATA registers
 	LWIP_DEBUGF(CML_DEBUG,("\tpInfo->callee_id[0]=0x%02x\n",pInfo->callee_id[0]));
//	LWIP_DEBUGF(CML_DEBUG,("pInfo->callee_id[1]=0x%02x\n",pInfo->callee_id[1]));
	CBUS_Send16U(TXDATA4,(pInfo->callee_id[0])&0xfff);
		//	CBUS_Send16U(0xCB,0x00,0xBF);
	DelayMS(1);
		//0f_fd//	CBUS_SendTwoByte(0xCA,CA_H,CA_L);
 	LWIP_DEBUGF(CML_DEBUG,("\tpInfo->callee_id[1]=0x%02x\n",pInfo->callee_id[1]));
	CBUS_Send16U(TXDATA3,(pInfo->callee_id[1])&0xfff);	
	DelayMS(1);
//这个地方怎么处理那？liuxizhen
 //这个地方是跟color code相关的。2012年11月16日12:00:02
	if (CsfIsfOperate(GET_S,0)== ISF){
		LWIP_DEBUGF(CML_DEBUG,("\tISF MODE\n"));
		CBUS_Send16U(TXDATA2,0x100D);
	}else if(CsfIsfOperate(GET_S,0)== CSF){
		LWIP_DEBUGF(CML_DEBUG,("\tCSF MODE\n"));
	 	CBUS_Send16U(TXDATA2,0x1030);
	}else{ELOG("err\n");} 
	DelayMS(1);
		//	CBUS_SendTwoByte(0xB6,0x00,0x00);
	CBUS_Send16U(TXDATA1,0x0000);
	DelayMS(1);
		//	CBUS_SendTwoByte(0xB5,B5_H,0x09);
	CBUS_Send16U(TXDATA0,0x1009);
	DelayMS(1);
		//CBUS_SendTwoByte(0xC1,0x00,0x12);		//set Tx burst mode
	CBUS_Send16U(MODE,0x0012);					//set Tx burst mode
	DelayMS(1);
	CBUS_Send16U(ACONFIG,0x020F);//CML_MIC_Gain();	

}
/**
*************************************************************************************
*@brief
*@author
*@param
*@param
*@return
**************************************************************************************
*/
void CML_Coming_Call_Digital(void)
{

#if with_RF
/*
	Tx_Status = 0;
	CBUS_Send16U(0xC1,0x00,0x00);	 //idle
    Chack_Prog_flag();
	CML_RF_Config();	
	CBUS_Send16U(0xC0,0xff,0x40);	  //Powerdown Control: Enabled: DISC amp, IP1, bias
	CBUS_Send16U(0xB1,0x0b,0x58);    //CBUS_Send16U(0xB1,0x0b,0x58);//Input Gain: Input1=0dB, DISC->Input1
	CBUS_Send16U(0xB0,0x77,0x0f);
	CML_Power_Contr();
	Ch_Freq_Config();	
*/
	Tx_Status = 0;
	CML_RF_Config();	
	CBUS_Send16U(PWD,0xff40);	  //Powerdown Control: Enabled: DISC amp, IP1, bias
	CBUS_Send16U(INPUT_GAIN_SIG_ROUT,0x0b58);    //CBUS_Send16U(0xB1,0x0b,0x58);//Input Gain: Input1=0dB, DISC->Input1
	CBUS_Send16U(ANALOG_OUT_GAIN,0x770f);
	CML_Power_Contr();
	Ch_Freq_Config(430);//	
	CBUS_Send16U(MODE,0x0000);	 //idle
    Chack_Prog_flag();

	CBUS_Send16U(MODE,0x0011);	 //set Rx 4FSK formatted, Rx mode

#else

	CBUS_Send16U(PWD,0x3040);	  //Powerdown Control: Enabled: DISC amp, IP1, bias
	CBUS_Send16U(INPUT_GAIN_SIG_ROUT,0x0010);    //CBUS_Send16U(0xB1,0x0b,0x58);//Input Gain: Input1=0dB, DISC->Input1
	CBUS_Send16U(MODE,0x0000);	 //idle
    Chack_Prog_flag(CML_TIME_OUT);
	CBUS_Send16U(MODE,0x0011);	 //set Rx 4FSK formatted, Rx mode

#endif

	CML_Speaker_Gain();	 // 618喇叭增益	
//	CBUS_Send16U(0xC3,0x3F,0x00);    //调大
	
//	Switch_Audio_D;
//	Audio_Amp_ON;// 收到头帧时打开，更加合理

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
//extern u16 CBUS_RecTwoByte(u8 addr);
TskComMsg pStatReportMsg;		//接收串口中断的信息的
extern OS_EVENT * pAppMsgQ;//定义一个串口的邮箱
bool detect_net_input_event(void * data)
{
// 	message * msg = (message *)data;
	u16 Stat;

	Stat = 0x0000;  //得到最新的C6状态
	Stat = CBUS_RecTwoByte(STAT);
	if((0x0000 != Stat))// && (0x0010 != Stat))
	{
//	    msg->id = MSG_NET_INPUT;
//		msg->data.val = Stat;
		pStatReportMsg.MsgType=StateRpType;
		pStatReportMsg.Data.Val=Stat;
		LWIP_DEBUGF(CML_DEBUG,("sata %04x\n\n",Stat));
		OSQPost(pAppMsgQ,&pStatReportMsg);		
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
***********************************************************************
*@brief
	8.1.27   IRQ Status - $C6 read 

*@author   刘西振 rundream1314@gmail.com
*@param
*@param
*@return
************************************************************************
*/
//这几个参数的意义？
u16 xfercount,//
	bytecount,
	wordcount,
	RxData[5],//
	RxMsg[80];

void CML_IRQ_Event(u16 modemstat)
{
	 	u16 field;
		//field = modemstat;
		field = modemstat&FIELD_MODEMSTATE;
		 switch(field){
		 case(IDLE):
		 	LWIP_DEBUGF(CML_DEBUG,("**STATUS: IDLE \n"));//IDLE状态
		 break;
		 case(RX_SEARCHING):
		 	LWIP_DEBUGF(CML_DEBUG,("**STATUS: RX_SEARCHING \n"));//搜索状态
		 break;
		  case(RX_RECEIVING):
		 	LWIP_DEBUGF(CML_DEBUG,("**STATUS: RX_RECEIVING \n"));//接收状态
		 break;
		 case(BAD_COLOURCODE):
		 	LWIP_DEBUGF(CML_DEBUG,("**STATUS: BAD_COLOURCODE \n"));//色码错误
		 break;
		 case(ONE_BAD_CRC_HI):
		 	LWIP_DEBUGF(CML_DEBUG,("**STATUS: IDLE \n"));//IDLE状态
		 break;
		 case(TWO_BAD_CRC_HI):
		 	LWIP_DEBUGF(CML_DEBUG,("**STATUS: ONE_BAD_CRC_HI \n"));//头帧中有一个CRC错误
		 break;
		 case(ONE_BAD_CRC_EI):
		 	LWIP_DEBUGF(CML_DEBUG,("**STATUS: ONE_BAD_CRC_EI \n"));//尾帧中有一个CRC错误
		 break;
		 case(TWO_BAD_CRC_EI):
			LWIP_DEBUGF(CML_DEBUG,("**STATUS: TWO_BAD_CRC_EI \n"));//尾帧中有两个CRC错误
		 break;
		default:
			DLOG("**STATUS: Bad status : %04x\n",field);//非定义的错误状
			break;
		 };
}
void CML_IRQ_CALLED(u16 auxdata)
{
		u16 field;
		field = auxdata;
        field = FIELD_CCIDX & field;// Colour Code Index
        LWIP_DEBUGF(CML_DEBUG,("Colour Code Index : %02d \n", field));
        
        field = auxdata;
        field = FIELD_IDTYPE & field;// Match ID index and Type	
        if(field == NO_MATCH)
			LWIP_DEBUGF(CML_DEBUG,("NO_MATCH \n"));
		else if (field == ID1_MATCH)
            LWIP_DEBUGF(CML_DEBUG,("ID1 MATCH \n"));
        else if(field == ID1_GROUP)
            LWIP_DEBUGF(CML_DEBUG,("ID1 GROUP Call \n"));
        else if(field == ID2_MATCH)
            LWIP_DEBUGF(CML_DEBUG,("ID2 MATCH \n"));
        else if(field == ID2_GROUP)
            LWIP_DEBUGF(CML_DEBUG,("ID2 GROUP Call \n"));
		else if (field == GROUP_ID_MATCH){
			field = 0x0F00 & auxdata;
			LWIP_DEBUGF(CML_DEBUG,("GROUP_ID_MATCH %04x \n",field));
		}
		else if (field == Common_All_Call)
            LWIP_DEBUGF(CML_DEBUG,("Common_All_Call \n"));// 对方呼叫的是255
        else if(field == BROADCAST)
            LWIP_DEBUGF(CML_DEBUG,("System Broadcast \n")); 

           
        xfercount = 0;		
}

//这个时候传入的是RxData[0];

#define UpBit(x)   (((x)>>8)&&0xff)
#define LowBit(x)  ((x)&&0xff) 

void CML_IRQ_DATA(u16 data)
{
		u16 field;

		field = data& FIELD_XFERCOUNT;
        if(field != xfercount)
        {
            LWIP_DEBUGF(CML_DEBUG,("**XFER: bad xfer count: %04x\n", field));
            xfercount = field;
        }
        xfercount = xfercount + INCR_XFERCOUNT;
        xfercount = xfercount & FIELD_XFERCOUNT; 
        
        bytecount = RxData[0];
        bytecount = bytecount & FIELD_BYTES;
        
        field = RxData[0];
        field = field & FIELD_XFERTYPE;	
        if(field == HEADER_INFO)
        {
            LWIP_DEBUGF(CML_DEBUG,("TYPE: header info \n"));
            if(bytecount != 9){
                LWIP_DEBUGF(CML_DEBUG,("\tXFER: bad type count : %d\n", bytecount));
			}else{
                LWIP_DEBUGF(CML_DEBUG,("\tXFER: %d types\n", bytecount));
			}                
            RxData[1] = CBUS_RecTwoByte(RXDATA1);
            RxData[2] = CBUS_RecTwoByte(RXDATA2);
            RxData[3] = CBUS_RecTwoByte(RXDATA3);
            RxData[4] = CBUS_RecTwoByte(RXDATA4);
            LWIP_DEBUGF(CML_DEBUG,("\tRxData[0]:%04x\n",RxData[0]));
            LWIP_DEBUGF(CML_DEBUG,("\tRxData[1]:%04x\n",RxData[1]));
            LWIP_DEBUGF(CML_DEBUG,("\tRxData[2]:%04x\n",RxData[2]));
            LWIP_DEBUGF(CML_DEBUG,("\tRxData[3]:%04x\n",RxData[3]));
            LWIP_DEBUGF(CML_DEBUG,("\tRxData[4]:%04x\n",RxData[4]));
        }
        else if(field == PAYLOAD_SLOW)
        {
            LWIP_DEBUGF(CML_DEBUG,("TYPE: payload + slow data\n"));
//            auxdata = CBUS_RecTwoByte(AUXDATA);
//            My_Printf("SLOW: %04x\n", auxdata);
            LWIP_DEBUGF(CML_DEBUG,("XFER: %d bytes\n", bytecount));
            
            if(bytecount > 0)
            {
				RxData[1] = CBUS_RecTwoByte(RXDATA1);
                RxData[2] = CBUS_RecTwoByte(RXDATA2);
                RxData[3] = CBUS_RecTwoByte(RXDATA3);
                RxData[4] = CBUS_RecTwoByte(RXDATA4);
                LWIP_DEBUGF(CML_DEBUG,("\tRxData[0]:%04x\n",RxData[0]));
            	LWIP_DEBUGF(CML_DEBUG,("\tRxData[1]:%04x\n",RxData[1]));
            	LWIP_DEBUGF(CML_DEBUG,("\tRxData[2]:%04x\n",RxData[2]));
            	LWIP_DEBUGF(CML_DEBUG,("\tRxData[3]:%04x\n",RxData[3]));
            	LWIP_DEBUGF(CML_DEBUG,("\tRxData[4]:%04x\n",RxData[4]));
            }
        }				
        else if(field == PAYLOAD_DATA)
        {
            LWIP_DEBUGF(CML_DEBUG,("TYPE: payload\n"));
            LWIP_DEBUGF(CML_DEBUG,("XFER: %d bytes\n", bytecount));
            
            if(bytecount > 0)
            {
				RxData[1] = CBUS_RecTwoByte(RXDATA1);
                RxData[2] = CBUS_RecTwoByte(RXDATA2);
                RxData[3] = CBUS_RecTwoByte(RXDATA3);
                RxData[4] = CBUS_RecTwoByte(RXDATA4);
                LWIP_DEBUGF(CML_DEBUG,("\tRxData[0]:%04x\n",RxData[0]));
	            LWIP_DEBUGF(CML_DEBUG,("\tRxData[1]:%04x\n",RxData[1]));
	            LWIP_DEBUGF(CML_DEBUG,("\tRxData[2]:%04x\n",RxData[2]));
	            LWIP_DEBUGF(CML_DEBUG,("\tRxData[3]:%04x\n",RxData[3]));
	            LWIP_DEBUGF(CML_DEBUG,("\tRxData[4]:%04x\n",RxData[4]));				
            }
        }
        else if(field == END_INFO)
        {
            LWIP_DEBUGF(CML_DEBUG,("TYPE: end info\n"));
            if(bytecount != 3)
                LWIP_DEBUGF(CML_DEBUG,("XFER: bad type count : %d\n", bytecount));
            else
                LWIP_DEBUGF(CML_DEBUG,("XFER: %d types\n", bytecount));
            
			RxData[1] = CBUS_RecTwoByte(RXDATA1);
            RxData[2] = CBUS_RecTwoByte(RXDATA2);
            RxData[3] = CBUS_RecTwoByte(RXDATA3);
            RxData[4] = CBUS_RecTwoByte(RXDATA4);
			LWIP_DEBUGF(CML_DEBUG,("\tRxData[0]:%04x \n",RxData[0]));
			LWIP_DEBUGF(CML_DEBUG,("\tRxData[1]:%04x \n",RxData[1]));
			LWIP_DEBUGF(CML_DEBUG,("\tRxData[2]:%04x \n",RxData[2]));
			LWIP_DEBUGF(CML_DEBUG,("\tRxData[3]:%04x \n",RxData[3]));
			LWIP_DEBUGF(CML_DEBUG,("\tRxData[4]:%04x \n",RxData[4]));
        }
}


void CML_StateDetec(u16 CmlState)
{
    u16 modemstat,auxdata,i;
//    u16 upperbyte,bytecount,wordcount,RxData[5];
	u8 os_err=0;//ostimer 
	if((CmlState&IRQ_SYNC)==IRQ_SYNC){//帧头识别
	  	LWIP_DEBUGF(CML_DEBUG,("IRQ:SYNC\n"));
        CmlState= CmlState^ IRQ_SYNC;//清除这个状态位
        modemstat = CBUS_RecTwoByte(MODEMSTAT);//read Modem Status register
        modemstat = modemstat >> 14;	  //extract the framesync field
        modemstat = modemstat & 0x0003;
        modemstat = modemstat + 1;
        LWIP_DEBUGF(CML_DEBUG,("\tSYNC:FS%d\n",modemstat));
	}
	if((CmlState&IRQ_SYNC)==IRQ_EVENT){
	//错误发生
		DLOG("IRQ:EVENT \n");
		CmlState= CmlState^ IRQ_EVENT;
		modemstat = CBUS_RecTwoByte(MODEMSTAT);
		CML_IRQ_Event(modemstat);

	}
	if((CmlState&IRQ_CALLED)==IRQ_CALLED)
    {
        /*有呼入*/
        LWIP_DEBUGF(CML_DEBUG,("IRQ:CALLED\n"));			
        CmlState = CmlState ^ IRQ_CALLED;			
        auxdata = CBUS_RecTwoByte(AUXDATA);	
        CML_IRQ_CALLED(auxdata);
     }
	 if((CmlState& IRQ_DATA)==IRQ_DATA)//根据数据类型
    {
        /*有数据*/			
        LWIP_DEBUGF(CML_DEBUG,("IRQ:DATA\n"));
        CmlState= CmlState^ IRQ_DATA;
        RxData[0] = CBUS_RecTwoByte(RXDATA0);        
        //field = RxData[0];
	    CML_IRQ_DATA(RxData[0]);
        
    }
			
	
}

/**
*************************************************************************
*@brief
*@author 刘西振  rundream@gmail.com
*@param
*@param
*@return
*************************************************************************
*/

/*--------完成发射机的ID1/ID2配置,只支持Binary形式，不支持BCD码形式------*/
void CML_OwnID_Set(u32 Binary_ID1, u32 Binary_ID2 , u8 Page)
{
 	u16 Upper_12_Bit, Lower_12_Bit;
	u8 Upper_Byte, Lower_Byte, i;
	
	CBUS_SendTwoByte(0xC1,0x00,0x00);
	Chack_Prog_flag(CML_TIME_OUT);
//	CBUS_SendTwoByte(0xC1,0x02,0x30);
	for(i = 0;i < 2;i++)
	{
		/*-------Set ID-------*/
		if(0 == i)
		{
			Lower_12_Bit = Binary_ID1 & 0x00000FFF;
			Upper_12_Bit = (Binary_ID1 >> 12) & 0x00000FFF;
			Lower_Byte = (u8)Lower_12_Bit;
			Upper_Byte	= ((u8)(Lower_12_Bit >> 8)) | 0xD0;
		}
		else
		{
			Lower_12_Bit = Binary_ID2 & 0x00000FFF;
			Upper_12_Bit = (Binary_ID2 >> 12) & 0x00000FFF;
			Lower_Byte = (u8)Lower_12_Bit;
			Upper_Byte	= ((u8)(Lower_12_Bit >> 8)) | 0x50;
		}
		
		CBUS_SendTwoByte(0xC8,Upper_Byte,Lower_Byte);
		Chack_Prog_flag(CML_TIME_OUT);
	
		Lower_Byte = (u8)Upper_12_Bit;
		Upper_Byte	= ((u8)(Upper_12_Bit >> 8)) | 0x50;
	
		CBUS_SendTwoByte(0xC8,Upper_Byte,Lower_Byte);
		Chack_Prog_flag(CML_TIME_OUT);
	
		if(Page > 4)
		{
			Page = Page % 4;
		}
		Lower_Byte = Page << 4;
		Upper_Byte	= 0x50;	
	
		CBUS_SendTwoByte(0xC8,Upper_Byte,Lower_Byte);
		Chack_Prog_flag(CML_TIME_OUT);		
	}
}



/***************END OF FILE*****************************************/


