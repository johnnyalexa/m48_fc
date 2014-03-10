/*
 * m48Eeprom.c
 *
 * Created: 6/30/2013 3:17:23 PM
 *  Author: John
 */ 
#include <avr/io.h>
#include "picoPowerM48_fuelCounter.h"

/* Shows current buffer position */
static uint8_t BufferPtr;
static uint8_t StatusBufferValue;
static uint8_t FirstWrite;
uint8_t stat;


/*-------------------- EEPROM_write   -------------------------
*    Function:    EEPROM_write
*    Purpose:    Writes a values into EEPROM
*
*    Parameters:        
*        uiAddress - The EEPROM address
*        ucData      - Data to be writen
*    Returns:    none
*------------------------------------------------------------*/
void EEPROM_write(unsigned char uiAddress, unsigned char ucData){
    /* Wait for completion of previous write */
    while(EECR & (1<<EEPE))
        ;
    /* Set up address and Data Registers */
    EEARL = uiAddress;
    EEDR = ucData;
    /* Write logical one to EEMPE */
    EECR |= (1<<EEMPE);
    /* Start eeprom write by setting EEPE */
    EECR |= (1<<EEPE);
}

/*-------------------- EEPROM_read   -------------------------
*    Function:    EEPROM_read
*    Purpose:    Read data from EEPROM
*
*    Parameters:        
*        uiAddress - The EEPROM address
*    Returns:    
*        Data read from EEPROM
*------------------------------------------------------------*/
unsigned char EEPROM_read(unsigned char uiAddress){
    /* Wait for completion of previous write */
    while(EECR & (1<<EEPE))
        ;
    /* Set up address register */
    EEARL = uiAddress;
    /* Start eeprom read by writing EERE */
    EECR |= (1<<EERE);
    /* Return data from Data Register */
    return EEDR;
}

/*-------------------- EEPROM_saveData   -------------------------
*    Function:    EEPROM_saveData
*    Purpose:    Write data structure to EEPROM
*
*    Parameters:
*        data - Data structure to be written
*    Returns: none
*------------------------------------------------------------*/
void EEPROM_saveData(consumptionInfo_t * data){
    uint8_t * dataW =(uint8_t *) data;
    uint8_t baseAddress = BufferPtr * sizeof(consumptionInfo_t);
    uint8_t i;
    
    //disable interrupts    
    for(i=0;i<sizeof(consumptionInfo_t);i++){
        EEPROM_write(baseAddress+i,dataW[i]);
        }
    
    BufferPtr++;
    if(BufferPtr > 18){
        BufferPtr = 0;    
    }
    StatusBufferValue = (uint8_t)(StatusBufferValue+1);        
    EEPROM_write((STATUS_BUFFER_START_ADDRESS + BufferPtr),(uint8_t)StatusBufferValue);
    stat = BufferPtr;
    if(0 != FirstWrite)
        EEPROM_write(FIRST_USE_LOCATION, 0x00);
    //enable interrupts
}

/*-------------------- EEPROM_loadData   -------------------------
*    Function:    EEPROM_loadData
*    Purpose:    Loads data structure from EEPROM
*
*    Parameters:
*        data - Data structure pointer to be written
*    Returns: none
*------------------------------------------------------------*/
void EEPROM_loadData(consumptionInfo_t * data){
    if(0 != FirstWrite){// If first write, init with zero;
        //data->Consumption.Liters = 0;
        //data->Consumption.MicroLiters = 0;
        //data->Duration.Hours = 0;
        //data->Duration.Minutes = 0;
        //data->Duration.Seconds = 0;
        USART_Transmit('F');
    }else{
        USART_Transmit('S');
        uint8_t i;
        uint8_t * dataR =(uint8_t *) data;
        uint8_t baseAddress = ((BufferPtr+18)%19) * sizeof(consumptionInfo_t);
        for(i=0;i<sizeof(consumptionInfo_t);i++){
            dataR[i] = EEPROM_read(baseAddress+i);
        }
    }
}

/*-------------------- EEPROM_GetCurrentPosition   -------------------------
*    Function:    EEPROM_GetCurrentPosition
*    Purpose:    Loads Current position in EEPROM.
*
*    Parameters: none
*    Returns: none
*------------------------------------------------------------*/
void EEPROM_GetCurrentPosition(void){
    uint8_t U8_value1,U8_value2;
    uint8_t U8_position;
    
    U8_value1 = EEPROM_read(STATUS_BUFFER_START_ADDRESS);
    
    for(U8_position=0;U8_position<19;U8_position++){
        
        U8_value2 = EEPROM_read(STATUS_BUFFER_START_ADDRESS + ((U8_position+1)%19));
        
        if(U8_value2 == (uint8_t)(U8_value1+1)){
            U8_value1 = U8_value2;
            
        }else{
            BufferPtr = U8_position;
            StatusBufferValue = U8_value1;
            stat = BufferPtr;
            break;
        }
    }
    if((0 == BufferPtr)&&(0xFF == EEPROM_read(FIRST_USE_LOCATION))){
        FirstWrite = 1;
    }    
}