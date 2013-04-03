#include "..\\STM32Lib\\stm32f10x.h"
#include "..\\Hal\\hal.h"
/*******************************************************************************
* Function Name  : GPIO_Configuration
* 设置PD3,PD4,PD5,PD6为键盘输入
* 设置PB0,5,8,9; PC5,7; PD7 ;PA8 为输出LED灯
*******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);			   //复用功能使能
    
    /*---------------LED------------------*/
    LED1_OFF;
    LED2_OFF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /*---------------LED------------------*/
    
	
	/* CBus Configuration ----- Begin*/
    /* PA7-CBUS_CS,PA6-CBUS_CLK,PA3-CBUS_Senddata*/
    //改相应的宏
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);	   	
    /* PA5-CBUS_Recdata*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //GPIO_Mode_IPU;//
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* CBus Configuration ----- END*/
	
	
	/* RF Config ----- Begin */
    GPIO_ResetBits(GPIOE, GPIO_Pin_3);//预置为高
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    /* RF Config ----- END */
	
	/* PLL写频 SPI Config ----- Begin */
    /* PC2-SPI_RF_CS, PC0-SPI_RF_DATA, PC1-SPI_RF_CLK */	   
    GPIO_SetBits(GPIOC, GPIO_Pin_2);//预置为高
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    /* PLL写频 SPI Config ----- END */
	
		
    
/*
RST-->PE1


CS-->PD4
RS(Register select)-->PD5
sclk--->PD7
sid-->pd11
*/		
    
   /*---------------LCD------------------*/    
   /* Pc8-cs,11-rst输出 -->PD11-CS   PC12--RST*/
//	GPIO_ResetBits(GPIOD, GPIO_Pin_9);		//预置高
//	GPIO_SetBits(GPIOC, GPIO_Pin_8);		//预置高
//GPIO_SetBits(GPIOD, GPIO_Pin_11);		//预置高	 PD11-CS
//GPIO_SetBits(GPIOD, GPIO_Pin_12);		//预置高     PD12-OLED_RW	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;	//推挽
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
   GPIO_Init(GPIOE, &GPIO_InitStructure);
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
   GPIO_Init(GPIOD, &GPIO_InitStructure);
	 /*KEY*/
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//PU;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
   GPIO_Init(GPIOB, &GPIO_InitStructure);
//    /*---------------OLED------------------*/



}


