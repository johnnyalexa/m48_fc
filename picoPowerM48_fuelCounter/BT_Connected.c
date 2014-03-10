/*
 * BT_Connected.c
 *
 * Created: 9/23/2013 10:36:56 PM
 *  Author: John
 */ 
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include "picoPowerM48_fuelCounter.h"

#include <util/delay.h>

/*-------------------- Download_Function   -------------------------
*    Function:    Download_Function
*    Purpose:    Handles BT communication
*
*    Parameters:        none
*    Returns:    none
*------------------------------------------------------------*/
void Download_Function(void){
    
    uint8_t adcv;
    uint8_t uart_data = 0;
    Timer2_Stop();
    
    _delay_ms(500);
    ADC_Init();
    
    
                    
        while(0 == USART_Receive(&uart_data))
            ;
        do{
            uart_data = 0;
             USART_Receive(&uart_data);
        }while(uart_data!=0x0A);
        
    while(0 != (PIND&0x08)){            
    
        adcv=ADC_GetVal();
        adcv=ADC_GetVal();
                
        USART_Transmit(0x30+(adcv/100));
        USART_Transmit(0x30+((adcv%100)/10));
        USART_Transmit(0x30+(adcv%10));
        
        USART_Transmit(0x0D);
        USART_Transmit(0x0A);
            
        _delay_ms(500);
        
        while(0 == USART_Receive(&uart_data))
            ;
        do{
            uart_data = 0;
            USART_Receive(&uart_data);
        }while(uart_data!=0x0A);
    }
    
    power_adc_disable();
    sleep_cpu();
    
} 

/**********************************
    BT Connection protocol 
        (PIN 1234) - find : ATP?

Wait 2 seconds

Send AT
Read OK

Wait for  "CONNECT  'C488-E5-3DF4FA' "

Send data with no flow control 
(try to put data in a long string format)

Test BT pin to see if BT is still ON

***********************************/