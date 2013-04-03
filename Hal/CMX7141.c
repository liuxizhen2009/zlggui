

#include "..\\User\\Common.h"
#include ".\\CML.h"
//#include ".\\FunImage5.h"
#include ".\\Hal.h"
/**
*************************************************************************
*@brief
*@author 刘西振  rundream@gmail.com
*@param
*@param
*@return
*************************************************************************
*/
extern void CBUS_SendTwoByte(u8 addr,u8 data_H_8,u8 data_L_8);

void cmx7141_init_tx(void)
{
//	TLOG("cmx7141_init_tx\n\r");

#if with_RF	  //0924

	Tx_Status = 1;
	CML_RF_Config();
	CML_Power_Contr();
	Ch_Freq_Config();
	Delay(100,1);

	CBUS_SendTwoByte(0xC1,0x00,0x00);
	Chack_Prog_flag();
	CML_Gain_Offsets_Config();
	CBUS_SendTwoByte(0xC0,0xff,0x40);	  //Turn on all analogue hardware
	Delay(1,1);
	CBUS_SendTwoByte(0xB0,0xB1,0x00);	  //MOD1=8dB(反相因为MOD1比MOD2多一路反相1:1放大), MOD2=12dB
	Delay(1,1);
	CBUS_SendTwoByte(0xB1,0x12,0xC0);     //(12,C0)Input Gain: Output1->MOD1, Output2->MOD2,DISC->Input1	
	Delay(1,1);
	
#else

	CBUS_SendTwoByte(0xC1,0x00,0x00);
	while(!(CBUS_RecTwoByte(STAT)&0x0001));
 	CBUS_SendTwoByte(0xC0,0x0a,0x40);	  //  使能OP1,MOD1,BIAS
	CBUS_SendTwoByte(0xB1,0x02,0x00);     //  ROUTE OP1 TO MOD1
	CBUS_SendTwoByte(0xB0,0x10,0x00);	  //  设置增益 0db 

	#if CMX7141_DRIVER_SEND_IRQ_ENABLE
		CBUS_SendTwoByte(0xCE,0x80,0x60);
	#endif
#endif
}

void cmx7141_send_start(void)
{
	TLOG("cmx7141_send_start...\n\r");
// 	is_cmx7141_irq_send_status = 1;

//	need_to_switch_to_tx_mode = 1;
	cmx7141_init_tx();	
}

/*************************************************************************
*/