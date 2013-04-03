#ifndef HAL_H
#define HAL_H

#define CLI()      __set_PRIMASK(1)  
#define SEI()      __set_PRIMASK(0)  


//硬件初始化
extern void  ChipHalInit(void);
extern void  ChipOutHalInit(void);

//输出宏定义
//输出宏定义
#define LED1_ON					GPIOA->BRR = GPIO_Pin_0;//GPIO_ResetBits(GPIOA, GPIO_Pin_0)
#define LED1_OFF				GPIOA->BSRR = GPIO_Pin_0;//GPIO_SetBits(GPIOA, GPIO_Pin_0)
//#define LED1_POLLING			GPIOA->ODR^=GPIO_Pin_0
#define LED1_POLLING			GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_0)

#define LED2_ON					GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define LED2_OFF				GPIO_SetBits(GPIOA, GPIO_Pin_1)
//#define LED2_POLLING			GPIOA->ODR^=GPIO_Pin_1
#define LED2_POLLING			GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_1)

#define CBus_CS_H		 GPIOA->BSRR=GPIO_Pin_7
#define CBus_CS_L		 GPIOA->BRR=GPIO_Pin_7
#define CBus_CLK_L		 GPIOA->BRR=GPIO_Pin_6 
#define CBus_CLK_H		 GPIOA->BSRR=GPIO_Pin_6
#define CBus_data_H		 GPIOA->BSRR=GPIO_Pin_3
#define CBus_data_L		 GPIOA->BRR=GPIO_Pin_3
#define CBus_Revdata 	 ((GPIOA->IDR) & GPIO_Pin_5)

#define SPI_RF_data_L		 GPIOC->BRR=GPIO_Pin_0
#define SPI_RF_data_H		 GPIOC->BSRR=GPIO_Pin_0
#define SPI_RF_CLK_L		 GPIOC->BRR=GPIO_Pin_1
#define SPI_RF_CLK_H		 GPIOC->BSRR=GPIO_Pin_1
#define SPI_RF_CS_L		 	 GPIOC->BRR=GPIO_Pin_2
#define SPI_RF_CS_H		 	 GPIOC->BSRR=GPIO_Pin_2


#define Start_Timer4	do{TIM4->CR1 |= 0x0001;TIM4->DIER |= 0x0001;}while(0)//TIM_Cmd(TIM4, ENABLE);TIM_ITConfig(TIM4,TIM_IT_Update, ENABLE); 
#define Start_Timer3	do{TIM3->CR1 |= 0x0001;TIM3->DIER |= 0x0001;}while(0)
#define Stop_Timer4 	do{TIM4->CR1 &= 0x03FE;}while(0)
#define Stop_Timer3 	do{TIM3->CR1 &= 0x03FE;}while(0)


#define Audio_Amp_OFF			GPIO_ResetBits(GPIOC, GPIO_Pin_5)
#define Audio_Amp_ON     	GPIO_SetBits(GPIOC, GPIO_Pin_5)


#define BWSW_A		GPIO_ResetBits(GPIOE, GPIO_Pin_2)
#define BWSW_D     	GPIO_SetBits(GPIOE, GPIO_Pin_2)
#define EXT3V_ON	GPIO_SetBits(GPIOE, GPIO_Pin_3)
#define EXT3V_OFF	GPIO_ResetBits(GPIOE, GPIO_Pin_3)
#define RXEN_ON		GPIO_ResetBits(GPIOE, GPIO_Pin_4)
#define RXEN_OFF    GPIO_SetBits(GPIOE, GPIO_Pin_4)
#define TXEN_ON		GPIO_SetBits(GPIOE, GPIO_Pin_5)
#define TXEN_OFF    GPIO_ResetBits(GPIOE, GPIO_Pin_5)
#define TXVCOEN_ON		 GPIO_ResetBits(GPIOE, GPIO_Pin_6)
#define TXVCOEN_OFF		 GPIO_SetBits(GPIOE, GPIO_Pin_6)




#endif
