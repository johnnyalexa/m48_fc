/*
 * M48Interrupts.c
 *
 * Created: 6/28/2013 9:44:30 PM
 *  Author: John
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include "picoPowerM48_fuelCounter.h"

unsigned char state = 0;
uint16_t ticks = 0;
uint8_t seconds = 0;

static int tmr2_cnt=0;
/*
 * Timer 1 Interrupt routine
 */
ISR(TIMER1_COMPA_vect){
    //OCR1A = 14400;
//    seconds++;
//    state |= ONE_SECOND_INT;    
}    

/*
 * Timer 1 Interrupt routine
 */
ISR(TIMER2_OVF_vect){
    TCNT2 = 30;
    tmr2_cnt += 225;
    if(900 == tmr2_cnt){
        seconds++;
        tmr2_cnt = 0;
        state |= ONE_SECOND_INT;    
    }        
}

/*
 * Fuel impulse signal interrupt routine
 */
ISR(INT0_vect){
    //ticks = TCNT1;
    //TCNT1 = 0;
    ticks = tmr2_cnt + TCNT2 - 30; //add counted timer minus tmr starting time (30)
    tmr2_cnt = 0;  // reset sw counter
    TCNT2 = 30;        //reset tmr2 counter to default for 4 hz
    state |= IMPULSE;    
}

/*
 * Button Interrupt routine
 * Open BT connection 
 */
ISR(INT1_vect){
    state = BT_CONNECTED;    
//    USART_Transmit('v');
}
