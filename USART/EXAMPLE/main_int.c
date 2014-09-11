/*
	UART1 - interrupt (receive) mode.
	PA9 - USART1_TX
	PA10 - USART1_RX
*/ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l1xx_conf.h" //insert includes

//print WITHOUT new line and carriage return
//void printString(char *data);	//send/print string overserial port USART1.
//void printNumber(int32_t number, uint8_t base);	//send/print SINGED/UNSIGNED int32_t number over serial port USART1.

////print WITH new line and carriage return
//void printStringLn(char *data);	//send/print string overserial port USART1.
//void printNumberLn(int32_t number, uint8_t base);	//send/print SINGED/UNSIGNED number over serial port USART1.
//void printLn(void);	//print new line and carriage return

//send raw data, any type.
//void writeData(void *data, uint8_t dataSize);

uint16_t received_data = 0;

void clockCorrection(void);
void gpio_init(void);

//STM32L100RCT6-Discovery - clock correction function - SYSCLK = 32MHz, HSI. 
void clockCorrection()
{
	RCC_DeInit();
	RCC_HSICmd(ENABLE);
	
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) != SET)	// Wait until HSI is ready
	{
  	}
	 
	FLASH->ACR |= FLASH_ACR_ACC64;		// Enable 64-bit access 
  	FLASH->ACR |= FLASH_ACR_PRFTEN;		// Enable Prefetch Buffer 
	FLASH->ACR |= FLASH_ACR_LATENCY;	// Flash 1 wait state 
    
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;	// Power enable
  	PWR->CR = PWR_CR_VOS_0;				// Select the Voltage Range 1 (1.8 V) 
  	while((PWR->CSR & PWR_CSR_VOSF) != RESET) // Wait until the Voltage Regulator is ready 
  	{
	}
	  
	RCC_PLLConfig(RCC_PLLSource_HSI, RCC_PLLMul_4, RCC_PLLDiv_2);	// HSI = 16MHz; 16MHz * 4 / 2 = 32MHz
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != SET)	// Wait untill PLL is ready
	{
  	}
	RCC_PCLK1Config(RCC_HCLK_Div1);			// Configures the Low Speed APB(APB1) clock (PCLK1).
	RCC_PCLK2Config(RCC_HCLK_Div1);			// Configures the High Speed APB(APB2) clock (PCLK2).

	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);	// Configures the System Clock source to PLL
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)	// Wait till PLL is used as system clock source 
  	{
  	}
}

void gpio_init()
{
	GPIO_InitTypeDef GPIO_led1;
	GPIO_InitTypeDef GPIO_led2;
	
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
		
	/* Configure the GPIO_LED pin 8*/
	GPIO_led1.GPIO_Pin = GPIO_Pin_8;
	GPIO_led1.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_led1.GPIO_OType = GPIO_OType_PP;
	GPIO_led1.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_led1.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOC, &GPIO_led1);
	
	/* Configure the GPIO_LED pin 9*/
	GPIO_led2.GPIO_Pin = GPIO_Pin_9;
	GPIO_led2.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_led2.GPIO_OType = GPIO_OType_PP;
	GPIO_led2.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_led2.GPIO_Speed = GPIO_Speed_40MHz;
  	GPIO_Init(GPIOC, &GPIO_led2);

}

int main(void)
{
	clockCorrection();
	gpio_init();
	usart1_RXint_init();    //initialize USART1
	
	printStringLn("Ready!");    
	printStringLn("------------------------------------------------");
	
	while(1)
	{
	if(received_data == 'a')
		{			
			printStringLn("a was received");    
			received_data = 0;
		}
	}
}

void USART1_IRQHandler()
{
	while(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	// Received characters added to fifo
	{
		received_data = USART_ReceiveData(USART1); // Receive single/last character
	}
}
