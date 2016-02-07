 /*
 ===============================================================================
											##### print numbers and strings #####
																	c file
 ===============================================================================
 * @date    6-Feb-2016
 * @author  Domen Jurkovic
 
 * edit and call UART_Init();
 */

#include "stm32l1xx_uart_print.h"

/****************************************************************************************/
/* HARDWARE INIT FUNCTIONS - change accordingly to your HW */
/****************************************************************************************/

/* UART INIT
Pins:   |USART1: (APB2)  |USART2(APB1)  |USART3(APB1)   |
TX:     |PA9,    PB6     |PA2           |PB10,  PC10    |TX
RX:     |PA10,   PB7     |PA3           |PB11,  PC11    |RX

BAUD RATE: 9600, 19200, 38400, 57600, 115200 ... check docs.

EDIT INDENTED sections
*/	 
void UART_Init(void){
	USART_InitTypeDef USART_InitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_StructInit(&USART_InitStructure);
	
					RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
					//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
					//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

					//TX
					gpio_pinSetup_AF(GPIOA, GPIO_Pin_9, GPIO_AF_USART1, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_40MHz);
					RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
					//RX
					gpio_pinSetup_AF(GPIOA, GPIO_Pin_10, GPIO_AF_USART1, GPIO_OType_PP, GPIO_PuPd_UP, GPIO_Speed_40MHz);
					RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

					USART_InitStructure.USART_BaudRate = 19200;
	
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
					USART_Init(USART1, &USART_InitStructure);
					//USART_Init(USART2, &USART_InitStructure);
					//USART_Init(USART3, &USART_InitStructure);
	
	// UNCOMMENT IF RX INTERRUPT IS NEEDED
				// set according NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
				//NVIC_SetPriority(USART1_IRQn, 3);		// ? don't know if it is neccessary
				NVIC_EnableIRQ(USART1_IRQn);
				
				NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
				//NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
				//NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
				
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//SET PRIORITY!!!
				NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//SET PRIORITY!!! AND PRIORITY GROUP!!!
				
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
				
				USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//enable USART1 interrupt - Receive Data register not empty interrupt.
				//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//enable USART1 interrupt - Receive Data register not empty interrupt.
				//USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//enable USART1 interrupt - Receive Data register not empty interrupt.
			
	
				USART_Cmd(USART1, ENABLE);
				//USART_Cmd(USART2, ENABLE);
				//USART_Cmd(USART3, ENABLE);
}

/*
	Modify this functions according to your hardware send protocol apd peripheral
*/
uint32_t _send_byte(uint8_t byte){
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//wait for cleared flag
	USART_SendData(USART1, byte);	
	return 0;
	
	/*while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	//wait for cleared flag
	USART_SendData(USART2, byte);	
	return 0;*/
	
	/*while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);	//wait for cleared flag
	USART_SendData(USART3, byte);	
	return 0;*/
}

uint8_t _receive_byte(void){
	
	return 0;
}


/****************************************************************************************/
/* PRINT/WRITE FUNCTIONS */
/****************************************************************************************/

/*
	Send/print character/string. 
	Printable data for viewing on terminal.
	Call this function:		printString("test data");
	Data must be string.
*/
void printString(char *data){
	uint16_t i;
	uint16_t str_length = strlen(data);
	for(i = 0; i < str_length; i++)
	{
		_send_byte(data[i]);
	}
}


/*
	Send/print unsigned or signed number. 
	Printable data for viewing on terminal.
	Call this function:		printNumber(number, DEC);		printNumber(2246, DEC);	
	Base: DEC, HEX, OCT, BIN
	Data must be number, int32_t.
*/
void printNumber(int32_t number, uint8_t base){
	if (number < 0) 
	{
		printString("-");
		number = -number;
		printUnsignedNumber(number, base);
	}
	else 
	{
		printUnsignedNumber(number, base);
	}
}
//print WITH new line and carriage return
void printStringLn(char *data){
	uint16_t i;
	uint16_t str_length = strlen(data);
	for(i = 0; i < str_length; i++)
	{
		_send_byte(data[i]);
	}
	printLn();
}

/*
	Send/print unsigned or signed number. 
	Printable data for viewing on terminal.
	Call this function:		printNumber(number, DEC);		printNumber(2246, DEC);	
	Base: DEC, HEX, OCT, BIN
	Data must be number, int32_t.
*/
void printNumberLn(int32_t number, uint8_t base){
	if (number < 0) 
	{
		printString("-");
		number = -number;
		printUnsignedNumber(number, base);
	}
	else 
	{
		printUnsignedNumber(number, base);
	}
	printLn();
}

void printLn(){
	printString("\n\r");
}

/*
	Send raw data. 
	Not "printable" data.
	Call this function:		writeData(&data, sizeof(data));
	Data can be any type.
*/
void writeData(void *data, uint8_t dataSize){
  uint8_t i, d;
  
  d = dataSize/2;
 
  for(i = 0; i < d; i++)
  {
    _send_byte(*( ((uint16_t *)data) + i ) );
  }
 
}

/*
	This is "private" function. It is used by other functions like: printNumber(int32_t number, uint8_t base). 
	However, it can be used by user.
	Send/print unsigned number. 
	Printable data for viewing on terminal.
	Call this function:		printUnsignedNumber(number, DEC);		printUnsignedNumber(2246, DEC);	
	Base: DEC, HEX, OCT, BIN
	Data must be number, int32_t.
*/
void printUnsignedNumber(uint32_t n, uint8_t base){
	char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
	char *str = &buf[sizeof(buf) - 1];
	unsigned long m;
	char c;
  *str = '\0';

  //prevent crash if called with base == 1
  if (base < 2) base = 10;

  do 
	{
    m = n;
    n /= base;
    c = m - base * n;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
	} while(n);

	printString(str); 
}

void printFloat(float number){
	int32_t integer_part = (int32_t) number;
	float decimal_part =  (number - (float)integer_part) * 10000;
	
	if((integer_part == 0) && (decimal_part < 0)){
		printString("-");
	}
	printNumber(integer_part, DEC);
	printString(".");
	
	if(decimal_part < 0){
		decimal_part = - decimal_part;
	}
	if(decimal_part < 1000){
			if(decimal_part < 100)	printNumber(0, DEC);
			if(decimal_part < 10)	printNumber(0, DEC);
			printNumber(0, DEC);
	}
	printNumber((uint32_t)decimal_part, DEC);
}

void printFloatLn(float number){
	printFloat(number);
	printLn();
}