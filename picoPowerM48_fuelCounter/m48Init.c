/*
 * m48Init.c
 *
 * Created: 6/28/2013 9:00:26 PM
 *  Author: John
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/sleep.h>
#include <avr/power.h>


/*-------------------- Init_Timer1   -------------------------
*    Function:    Init_Timer1
*    Purpose:    Initialize timer 1
*
*    Parameters:        none
*    Returns:    none
*------------------------------------------------------------*/
void Init_Timer1(void){
    
    // set up timer with prescaler = 8
    TCCR1B = (1 << WGM12);
    //set the top value for 1 sec interrupt
    OCR1A = 14400;
    // initialize counter
    //TCNT1 = 0;
    // enable compare match interrupt
    TIMSK1 = (1 << OCIE1A);// | (1<<TOIE1);
}

/*-------------------- Init_Timer2   -------------------------
*    Function:    Init_Timer2 
*    Purpose:    Initialize timer 2 to overflow 4 times 
*                a second, in async mode. Enabled in sleep
*                mode
*
*    Parameters:    none
*    Returns: none
*------------------------------------------------------------*/
void Init_Timer2(void){
    TCCR2B = 0;
    //Enable timer2 overflow interrupt
    TIMSK2 = (1<<TOIE2);         
    //count 225 for 4Hz       
    TCNT2 = 30;        
    //Enable async operation                    
    ASSR = (1<<AS2);                    
}


/*-------------------- Timer1_Start  -------------------------
*    Function:    Timer1_Start
*    Purpose:    Start timer 1. After Init the timer is stopped
*
*    Parameters:    none
*    Returns:    none
*------------------------------------------------------------*/
void Timer1_Start(void){
    // clkI/O/64 (from prescaler)
    TCCR1B |= (1<<CS11) | (1<<CS10);
}

/*-------------------- Timer1_Stop   -------------------------
*    Function:    Timer1_Stop
*    Purpose:    Stop timer 1
*
*    Parameters:    none
*    Returns:    none
*------------------------------------------------------------*/
void Timer1_Stop(void){
    //stop timer with prescaler 64
    TCCR1B &= ~((1<<CS11) | (1<<CS10));
}

/*-------------------- Timer2_Start     -------------------------
*    Function:    Timer2_Start
*    Purpose:    Start timer 2 
*
*    Parameters:    none
*    Returns:    none
*------------------------------------------------------------*/
void Timer2_Start(void){
    //count 225 for 4Hz
    TCNT2 = 30;        
    //Clock prescaler set to 1024, overflow 4 timer each sec                    
    TCCR2B = (1<<CS20) | (1<<CS21) | (1<<CS22);         
}

/*-------------------- Timer2_Stop   -------------------------
*    Function:    Timer2_Stop
*    Purpose:    Stop timer 2 when for example, the BT is
*                connected.
*
*    Parameters:    none
*    Returns:    none
*------------------------------------------------------------*/
void Timer2_Stop(void){
    TCCR2B = 0;    
}

/*-------------------- EXT_Int_Init -------------------------
*    Function:    EXT_Int_Init
*    Purpose:    Initialize externam Interrupts
*
*    Parameters:    none
*    Returns:    none
*------------------------------------------------------------*/
void EXT_Int_Init(void){
            //Rising edge INT1  //  Falling edge INT0
    EICRA = (1<<ISC11) | (1<<ISC10) | (1<<ISC01) | (0<<ISC00); 
    // set enable flags in int mask
    EIMSK = (1<<INT1) | (1<<INT0);                               
    
}

/*-------------------- Init_Port     -------------------------
*    Function:    Init_Port
*    Purpose:    Initialize IO ports
*    
*    Parameters:    none
*    Returns:    none
*------------------------------------------------------------*/
void Init_Port(void){
    //All pins are input
    DDRB = 0x00; 
    DDRC = 0x00;
    DDRD = 0x00;
    //All ports are state low
    PORTB = 0x00;   
    PORTC = 0x00;   // The UART tx pin will still work
    PORTD = 0x00;
}

/*-------------------- Init_Uart      -------------------------
*    Function:    Init_Uart
*    Purpose:    Initialize serial connectivity @115200 baud.
*
*    Parameters:    none
*    Returns:    none
*------------------------------------------------------------*/
void Init_Uart(void){
    // Enable U2Xn to get a baud rate with less error margin
    UCSR0A = (1<<U2X0);       
     // Transmitter enable and Receiver enable                  
    UCSR0B = (1<<TXEN0)|(1<<RXEN0);           
    // Asynchronous USART | No parity | 1 stopbit | CH size 8-bit    
    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01) | (1<<USBS0);
    // 115200 Baudrate @ 0.9216 Mhz
    UBRR0L = 0;                                        
}

/*-------------------- Enable_PRR    -------------------------
*    Function:    Enable_PRR
*    Purpose:    Enable power reduction.
*
*    Parameters:    none
*    Returns:    none
*------------------------------------------------------------*/
void Enable_PRR(void){
    //cut power for spi
    power_spi_disable();
    //cut power for timer 0
    power_timer0_disable();
    //cut power for timer 1
    power_timer1_disable();
    //cut power for timer 2
    //    power_timer2_disable();
    //cut power for twi
    power_twi_disable();
    //cut power for adc
    power_adc_disable();
    //cut power for usart 0
    //power_usart0_disable();
}

/*-------------------- ADC_Init       -------------------------
*    Function:    ADC_Init
*    Purpose:    Initialize ADC unit.
*
*    Parameters:    none
*    Returns:    none
*------------------------------------------------------------*/
void ADC_Init(void){
    power_adc_enable();
    //Set the Band Gap voltage as the ADC input
    ADMUX = (1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<REFS0);
    // ADC en, 128 division factor
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS0);
}

/*-------------------- Mcu_Init     -------------------------
*    Function:    Mcu_Init
*    Purpose:    Initialize the microcontroller.
*
*    Parameters:    none
*    Returns:    none
*------------------------------------------------------------*/
void Mcu_Init(void){
    //disable interrupts
    cli(); 
    
    //    clock_prescale_set(clock_div_4);

    Init_Port();
    //    Init_Timer();
    Init_Uart();
    EXT_Int_Init();
    Init_Timer2();
    
    Enable_PRR();
    
    //    ADC_Init();
    
    //    set_sleep_mode(SLEEP_MODE_IDLE); 
    
    // Set sleep mode to Power-save
    set_sleep_mode(SLEEP_MODE_PWR_SAVE); 
    // Enable sleep (possible to put the device into sleep mode
    // when executing the sleep function)
    sleep_enable();                      
    //    clock_prescale_set(clock_div_4);
    //enable interrupts
    sei();
}




