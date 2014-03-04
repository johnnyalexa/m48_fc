/*
 * m48Functions.c
 *
 * Created: 6/28/2013 10:15:24 PM
 *  Author: John
 */ 
#include<avr/io.h>
#include <string.h>
#include "picoPowerM48_fuelCounter.h"


/*-------------------- USART_Transmit   -------------------------
*	Function:	USART_Transmit
*	Purpose:	Transmit a byte through UART.
*
*	Parameters: 
*		data - data to be sent
*	Returns: none
*------------------------------------------------------------*/
void USART_Transmit(uint8_t data){
	/* Wait for empty transmit buffer */
//	while(!(UCSR0A & (1<<UDRE0)));  // Make sure that the data register is empty before putting the device to sleep
//		;
	/* Put data into buffer, sends the data */
	UDR0 = data;
	/* Wait for empty transmit buffer */
	while(!(UCSR0A & (1<<UDRE0)));  // Make sure that the data register is empty before putting the device to sleep
		;	
}

/*-------------------- USART_Receive   -------------------------
*	Function:	USART_Receive
*	Purpose:	Receive a byte from UART.
*
*	Parameters: 
*		data - data pointer to save data to
*	Returns: none
*------------------------------------------------------------*/
uint8_t USART_Receive(uint8_t * data)
{
	/* Wait for data to be received */
	if(UCSR0A & (1<<RXC0)){
	/* Get and return received data from buffer */
	*data = UDR0;
	return 1;
	}else
		return 0;	
}

/*-------------------- USART_print   -------------------------
*	Function:	USART_print
*	Purpose:	Print a string to UART.
*
*	Parameters: 
*		text - pointer to byte array (string)
*	Returns: none
*------------------------------------------------------------*/
void USART_print(char * text){
	
	uint8_t text_size=strlen(text);	
	uint8_t i;
	
	for(i=0;i<text_size;i++){
		USART_Transmit(text[i]);		
	}
	USART_Transmit(0x0D);
	USART_Transmit(0x0A);
}

/*-------------------- ADC_GetVal   -------------------------
*	Function:	ADC_GetVal
*	Purpose:	Reads ADC value, Battery voltage.
*
*	Parameters: none
*	Returns: 
*		ADC value.
*------------------------------------------------------------*/
uint8_t ADC_GetVal(void){
	uint8_t return_val=0;
	// Start an ADC conversion
	ADCSRA|=(1<<ADSC);
	// Wait the conversion to be completed
	while((1<<ADIF)&ADCSRA)
		;
	return_val = (uint8_t)(ADCW>>2);
	return return_val;
}
