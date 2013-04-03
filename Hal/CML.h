#ifndef CML_H
#define CML_H

//void CML_Call_Analog(void);
//void CML_Call_Digital(call_out * pInfo);
//void CML_Coming_Call_Digital(void);
//void CML_Coming_Call_Analog(void);
//void CML_Power_Contr(void);
//void cml_reset(void);

#define delay55ns	  __nop();__nop();__nop();__nop()
#define delay110ns    __nop();__nop();__nop();__nop();__nop();__nop(); __nop();__nop()
#define delay210ns    __nop();__nop();__nop();__nop();__nop();__nop(); __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop()


/**
6.16  C-BUS Register Summary 
Table 9  C-BUS Registers 
ADDR. 	(hex)   REGISTER   Word Size 
(bits) 
$01          W  	C-BUS RESET  							0 
$A7          W      AuxADC Configuration  					16 
$A8          W      AuxDAC Data and Control  16 
$A9          R       AuxADC1 Data and Status/Checksum 2 hi  16 
$AA          R       AuxADC2 Data and Status/Checksum 2 lo  16 
$AB          W      SYSCLK 1 PLL Data  						16 
$AC          W      SYSCLK 1 Ref  16 
$AD          W      SYSCLK 2 PLL Data  						16 
$AE          W      SYSCLK 2 Ref  							16 
$AF                   reserved   
$B0          W      Analogue Output Gain  16 
$B1          W      Input Gain and Signal Routing  16 
$B2          W      RF Synthesiser Data (CMX7131 only)  16 
$B3          W      RF Synthesiser Control (CMX7131 only)  16 
$B4          R       RF Synthesiser Status (CMX7131 only)  8 
$B5          W      TxData 0  16 
$B6          W      TxData 1  16 
$B7          W      TxData 2  16 
$B8          R       RxData 0/Checksum 1 hi  16 
$B9          R       RxData 1/Checksum 1 lo  16 
$BA          R       RxData 2   16 
$BB          R       RxData 3   16 
$BC                   reserved   
$BD                   reserved   
$BE                   reserved   
$BF                   reserved   
$C0          W      Power Down Control  16 
$C1          W      Modem Control  16 
$C2          W      AuxData Write  16 
$C3          W      CMX6x8 Analogue Gain  16 
$C4                   reserved   
$C5          R       Rx Data 4  16 
$C6          R       IRQ Status  16 
$C7          W      Modem Configuration  16 
$C8          W      Programming Register  16 
$C9          R       Modem Status  16 
$CA          W      Tx Data 3  16 
$CB          W      Tx Data 4  16 
$CC          R       AuxData Read  16 
$CD         W      AuxADC Threshold Data  16 
$CE          W      Interrupt Mask   16 
$CF                   reserved   
All other C-BUS addresses (including those not listed above) are either reserved for future use or 
allocated for production testing and must not be accessed in normal operation.

*/
/////////刘西振添加了一部分////////////////////////////////////////////////////////////////////////
//Programming Register 
///////////////////////////////////////////////////////////////////////////////////////
//$A8          W      AuxDAC Data and Control  16 
#define AUXDAX_DATA_CTL		0xA8

#define AuxADC1 0xA9
#define AuxADC2 0xAA


//CMX7141寄存器定义
//$B0          W      Analogue Output Gain  16 
#define ANALOG_OUT_GAIN 0xB0
//$B1          W      Input Gain and Signal Routing  16 
#define INPUT_GAIN_SIG_ROUT	0xB1

//$B2          W      RF Synthesiser Data (CMX7131 only)  16 
//$B3          W      RF Synthesiser Control (CMX7131 only)  16 
//$B4          R       RF Synthesiser Status (CMX7131 only)  8 
//$B5          W      TxData 0  16 
#define TXDATA0	0xB5   //发送数据,TXDATA0包括事务计数、blockID、事务字节数
//$B6          W      TxData 1  16 
#define TXDATA1	0xB6
//$B7          W      TxData 2  16 
#define TXDATA2	0xB7
//$B8          R       RxData 0/Checksum 1 hi  16 
#define RXDATA0 0xB8    //接收数据,RXDATA0包括事务计数、blockID、事务字节数
//$B9          R       RxData 1/Checksum 1 lo  16 
#define RXDATA1 0xB9
//$BA          R       RxData 2   16
#define RXDATA2 0xBA 
//$BB          R       RxData 3   16 
#define RXDATA3 0xBB





#define PWD     	0xC0   //          W      Power Down Control  16 
#define MODE 		0xC1   //模式寄存器
//$C2          W      AuxData Write  16 
#define AUX_DATA 	0xC2
#define ACONFIG 	0xC3   //mic与speaker增益寄存器



#define RXDATA4 	0xC5
#define STAT 		0xC6   //状态寄存器
//$C7          W      Modem Configuration  16 
#define MODEM_CFG	0xC7

#define PROG_REG    0xC8
#define MODEMSTAT	0xC9   //modem状态

#define TXDATA3		0xCA
#define TXDATA4		0xCB 
/*
$CA          W      Tx Data 3  16 
$CB          W      Tx Data 4  16 
$CC          R       AuxData Read  16 
$CD          W      AuxADC Threshold Data  16 
$CE          W      Interrupt Mask   16 
*/
#define 	AUXDATA 			0xCC   //色码、ID号、慢速数据
#define 	AUX_ADC_THRESHOLD  	0xCD
#define 	IRQ_MASK			0xCE




















//STAT指示位
#define PROGBIT 	0x0001
#define	IRQ_SYNC	0x0010
#define IRQ_DATA	0x0020
#define IRQ_TXDONE	0x0040
#define IRQ_CALLED	0x0080
#define IRQ_AUXADC1 0x0100
#define IRQ_AUXADC2 0x0200
#define IRQ_ANALOG	0x0800
#define IRQ_EVENT	0x4000

//MODEM指示位
#define FIELD_MODEMSTATE	0x00f0
#define IDLE				0x0000
#define	RX_SEARCHING		0x0010
#define RX_RECEIVING		0x0020
#define TX_RAMPING			0x0040
#define TX_SENDING			0x0050
#define BAD_COLOURCODE		0x0070
#define ONE_BAD_CRC_HI		0x0090
#define TWO_BAD_CRC_HI		0x00A0
#define ONE_BAD_CRC_EI		0x00B0
#define TWO_BAD_CRC_EI		0x00C0
#define TX_XEER_ERROR		0x00D0

#define FIELD_FS			0xC000
#define FS1					0x0000
#define FS2					0x4000
#define FS3					0x8000
#define FS4					0xC000
//色码与ID号指示位
#define FIELD_CCIDX			0x00FF
#define FIELD_IDTYPE		0xE000
#define GROUP_IDINDEX		0x0F00
#define NO_MATCH			0x0000
#define ID1_MATCH			0x2000
#define ID1_GROUP			0x4000
#define ID2_MATCH			0x6000
#define ID2_GROUP			0x8000
#define GROUP_ID_MATCH		0xA000
#define Common_All_Call		0xC000
#define BROADCAST			0xE000
//帧信息提取位
#define FIELD_BYTES			0x000F
#define FIELD_XFERCOUNT		0x00C0
#define INCR_XFERCOUNT		0x0040
#define FIELD_XFERTYPE		0x0030
#define HEADER_INFO			0x0000
#define PAYLOAD_DATA		0x0010
#define PAYLOAD_SLOW		0x0020
#define END_INFO			0x0030

#endif // CML_H
