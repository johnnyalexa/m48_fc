/*
 * picoPowerM48_fuelCounter.c
 *
 * Created: 6/28/2013 8:56:17 PM
 *  Author: John
 */ 
/* #############################################
#    Included files
#  ############################################# */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <stdio.h>

//Include Project header
#include "picoPowerM48_fuelCounter.h"
//Delay functions
#include <util/delay.h>
#include <avr/wdt.h>



#define LOOP_CNT                        5U
#define POWER_DOWN_TIMEOUT              10U

extern unsigned char state;
extern uint16_t ticks;
extern uint8_t seconds;
extern uint8_t stat;

static uint16_t timerTicks;
static uint8_t    timeToSave;


int main(void)
{
    //int i=0;
    consumptionInfo_t            CurrentData;
    runningTime_t                * CurrentDuration;
    fuelConsumption_t            * CurrentConsumption;
    CurrentDuration              = &CurrentData.Duration;
    CurrentDuration->Seconds  = 0;
    CurrentConsumption          = &CurrentData.Consumption;
    uint16_t                    ml_consumption;

    Mcu_Init();
    USART_print("main");
    // Set sleep mode to Power-save
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
    //If reset occurs during BT connection
    Download_Function();
    
    // Execute sleep instruction
    sleep_cpu();
    
//    Timer2_Start();
    
//sei();
    //EEPROM_GetCurrentPosition();
    //EEPROM_loadData(&test);
    while(1)
    {
    switch(state){
    

        case BOTH_INTERRUPTS:{
            //USART_Transmit('b');
        }
        case IMPULSE:{
        //    USART_Transmit('x');
        
        Timer2_Start();
    // Set sleep mode to Power-save
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    
            timerTicks += ticks;
            
            if(0 == seconds){

                if(ticks < INTERVAL_1_HIGH){
                    ml_consumption = CONS_1_UL;
                    USART_Transmit('1');
                }else if((ticks > INTERVAL_1_HIGH)&&(ticks < INTERVAL_2_HIGH)){
                    ml_consumption = CONS_2_UL;
                    USART_Transmit('2');
                }else if((ticks > INTERVAL_2_HIGH)&&(ticks < INTERVAL_3_HIGH)){
                    ml_consumption = CONS_3_UL;
                    USART_Transmit('3');
                }else if((ticks > INTERVAL_3_HIGH)&&(ticks < INTERVAL_4_HIGH)){
                    ml_consumption = CONS_4_UL;
                    USART_Transmit('4');                    
                }else{
                    ml_consumption = CONS_5_UL;
                    USART_Transmit('5');
                }
            }else{
                USART_Transmit('6');
                ml_consumption = CONS_5_UL;
                CurrentDuration->Seconds += seconds;
            //    USART_Transmit(0x30 + seconds);
                seconds = 0;
            }
            ticks= 0 ;
            
            CurrentDuration->Seconds += timerTicks / ONE_SECOND_TICKS;
            timerTicks = timerTicks % ONE_SECOND_TICKS;
            CurrentDuration->Minutes += CurrentDuration->Seconds / 60;
            timeToSave               += CurrentDuration->Seconds / 60;
            CurrentDuration->Seconds  = CurrentDuration->Seconds % 60;
            CurrentDuration->Hours   += CurrentDuration->Minutes / 60;
            CurrentDuration->Minutes  = CurrentDuration->Minutes % 60;
                        
            CurrentConsumption->MicroLiters += ml_consumption / 10;
            CurrentConsumption->Liters +=CurrentConsumption->MicroLiters / 1000000;
            CurrentConsumption->MicroLiters = CurrentConsumption->MicroLiters % 1000000;                        
                        
            if(timeToSave > 5){
                //save data
                timeToSave = 0;
            }
            
            state &= ~IMPULSE;
#if 0 /// measure battery voltage
            uint8_t adcv1;
                adcv1=ADC_GetVal();
                adcv1=ADC_GetVal();
                
                USART_Transmit(0x30+(adcv1/100));
                USART_Transmit(0x30+((adcv1%100)/10));
                USART_Transmit(0x30+(adcv1%10));
                //power_adc_disable();
                USART_Transmit(0x0D);
                USART_Transmit(0x0A);
#endif
                                
            break;
        } 
        case ONE_SECOND_INT:{
            
            if(seconds > POWER_DOWN_TIMEOUT){
                Timer2_Stop();
                //save data

                set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode to Power-save
            }
            state &= ~ONE_SECOND_INT;
            USART_Transmit('s');
            break;
        }

        case BT_CONNECTED:{
        //    uint8_t adcv=0;
        //    uint8_t idx=5;
            state &= ~BT_CONNECTED;
            USART_Transmit('a');
            
            Download_Function();
            
            break;
        }
        case IDLE:{
        //    USART_Transmit('i');
            break;
        }
        default:{
            state = IDLE;
            break;
        }
    
    }
    

        
        
        if(IDLE == state){
           // PORTC &=0xFE;
        // Execute sleep instruction
        sleep_cpu();                    
        //    PORTC |=0x01;
        }        


    }
}