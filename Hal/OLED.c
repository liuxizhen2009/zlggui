#include "../STM32Lib/stm32f10x.h"
#include "hal.h"

#define delay210ns    __nop();__nop();__nop();__nop();__nop();__nop(); __nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop()

/*******************************************************************************
* Function Name  : OLED_Configuration
* 设置OLED
* 设置为输出LED灯
*******************************************************************************/

//OLED

#define RSTTFT_H	GPIOC->BSRR = GPIO_Pin_12
#define RSTTFT_L	GPIOC->BRR=GPIO_Pin_12

#define CSTFT_H		GPIOD->BSRR=GPIO_Pin_11
#define CSTFT_L 	GPIOD->BRR=GPIO_Pin_11

#define A0_H		GPIOC->BSRR=GPIO_Pin_11
#define A0_L        GPIOC->BRR=GPIO_Pin_11

#define WRTFT_H		GPIOD->BSRR=GPIO_Pin_12
#define WRTFT_L     GPIOD->BRR=GPIO_Pin_12



#define V74_ON     GPIOD->BSRR=GPIO_Pin_9;
#define V74_OFF    GPIOD->BRR=GPIO_Pin_9;


void Delay11(u16 speed)
{
	u16 i;
	while(speed!=0)
	{
		speed--;
		for(i=0;i<400;i++);
	}
}

#if 0
void wr_c(u8 cmd)
{
	CSTFT_L;//cs=0;	   
	__nop();__nop();__nop();__nop();__nop();__nop(); __nop();__nop();
	
	A0_L;   //a0=0;
	__nop();__nop();__nop();__nop();__nop();__nop(); __nop();__nop();
	
	WRTFT_L;//wr=0;

	GPIOE->ODR=(cmd<<8);	

	delay210ns;
	WRTFT_H;//wr=1;
}



void wr_d(u8 dat)
{
	CSTFT_L;//cs=0;
	__nop();__nop();__nop();__nop();__nop();__nop(); __nop();__nop();
	A0_H;//a0=1;
	__nop();__nop();__nop();__nop();__nop();__nop(); __nop();__nop();
	WRTFT_L;//wr=0;
	delay210ns;

	GPIOE->ODR=(dat<<8);
	delay210ns;
	WRTFT_H;//wr=1;
}
#else
void wr_c(u8 cmd)
{
	u16 temp;
	
	CSTFT_L;//cs=0;	   
//	__nop();__nop();__nop();__nop();__nop();__nop(); __nop();__nop();
	
	A0_L;   //a0=0;
//	__nop();__nop();__nop();__nop();__nop();__nop(); __nop();__nop();
	
	WRTFT_L;//wr=0;
	temp = (((uint16_t)GPIOE->IDR) & 0x00FF);
	temp = temp | (cmd<<8);
	GPIOE->ODR= temp;
//	delay210ns;
	WRTFT_H;//wr=1;
}

void wr_d(u8 dat)
{
	u16 temp;

	CSTFT_L;//cs=0;
//	__nop();__nop();__nop();__nop();__nop();__nop(); __nop();__nop();
	A0_H;//a0=1;
//	__nop();__nop();__nop();__nop();__nop();__nop(); __nop();__nop();
	WRTFT_L;//wr=0;
//	delay210ns;
	temp = (((uint16_t)GPIOE->IDR) & 0x00FF);
	temp = temp | (dat<<8);
	GPIOE->ODR= temp;
//	delay210ns;
	WRTFT_H;//wr=1;
}




void OLED_Configuration(void)
{
	V74_ON;
 //while(1);

  LED1_ON;
  LED2_ON;

	CSTFT_L;        //     cs=0;
	RSTTFT_L;       //     res=0;
	Delay11(10000);//SysTickDelay(100); //shortdelay(10);	 ???????
	RSTTFT_H;      //res=1; 
    	Delay11(10000);//		SysTickDelay(1000); 

		
	wr_c(0xe2);    //write_c(0xe2);     	

 	wr_c(0xfd);    //write_c(0xfd);    
	wr_d(0x12);    //write_d(0x12);


	wr_c(0xae);    //write_c(0xae);     //Display OFF

    wr_c(0xa8);         //Set Multiplex
	wr_d(0x7f);          

	wr_c(0x15);         //set coluum address
	wr_d(0x00);    
	wr_d(0x9f);    	 	

	wr_c(0x75);         //set row address
	wr_d(0x00);    
	wr_d(0x7f);    

    wr_c(0xa1);         //Set Start Line
	wr_d(0x00);   
	   	
    wr_c(0xa2);         //Set Display offset
	wr_d(0x00);  

	wr_c(0x81);         //set contrast of R
	wr_d(0xf5);    

	wr_c(0x82);         //set contrast of G
	wr_d(0xd0);      

	wr_c(0x83);         //set contrast of B
	wr_d(0xa9);     

    wr_c(0xbe);         //Set VCOMH
	wr_d(0x3c);      

    wr_c(0x87);          //Master Current Control
	wr_d(0x0b);


    wr_c(0xa0);           //Set driver remap and color depth
	wr_d(0xb0);           //normal mode 		     				   							     
/*   
    write_c(0xab);      //Dim Mode setting
	write_d(0x7f);      
    write_d(0x7f);  
	write_d(0x7f);  
	write_d(0x7f);  
	write_d(0x7f); 
*/  
    wr_c(0xb1);    // write_c(0xb1);      //Phase 1 and 2 Period adjustment
    wr_d(0x21);	   //write_d(0x21);      

    wr_c(0xb3);   //write_c(0xb3);      //Display Clock Driver /oscillator Frequency
    wr_d(0xc0);	   //write_d(0xc0);   
	   
    wr_c(0x8a);   //write_c(0x8a);      //Set Second Pre-charge speed
	wr_d(0x02);    //write_d(0x02);      //normal mode

    wr_c(0x64);   //write_c(0xb4);      //Set Second Precharge period
	wr_d(0x07);   //write_d(0x07);     
						   
    wr_c(0xbb);   //write_c(0xbb);      //Set Pre-charge level	
	wr_d(0x3e);   //write_d(0x3e);    
	  
    wr_c(0xb9);   //write_c(0xb9);      //Enable Linear Gray Scale Table

	wr_c(0xa4);	  //write_c(0xa4);      //Set Normal Display 
 
	wr_c(0xaf);    //write_c(0xaf);     //Display ON	

}


#endif
