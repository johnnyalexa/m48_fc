/*
 * picoPowerM48_fuelCounter.h
 *
 * Created: 7/6/2013 9:27:40 PM
 *  Author: John
 */ 


#ifndef PICOPOWERM48_FUELCOUNTER_H_
#define PICOPOWERM48_FUELCOUNTER_H_

// Define CPU speed
// QTAL hz/4
#define F_CPU					921600UL

//No interrupts pending
#define		IDLE				0x00U
//Fuel counter recorded one impulse
#define		IMPULSE				0x01U
//Timer elapsed interrupt flag
#define		ONE_SECOND_INT		0x02U
//Timer elapsed interrupt and fuel counter
//interrupt occurred almost simultaneously
#define		BOTH_INTERRUPTS		0x03U
//Interrupt when BT module is connected
#define		BT_CONNECTED		0x04U



#if 0 //we don't use timer 1
// time intervals for timer 1
#define     ONE_SECOND_TICKS    14400
#define		INTERVAL_1_LOW		0x0000UL // 0 
#define		INTERVAL_1_HIGH		0x0F86UL // 3974	
#define		INTERVAL_2_HIGH		0x10FCUL // 4348
#define		INTERVAL_3_HIGH		0x1B0EUL // 6926
#define		INTERVAL_4_HIGH		0x2799UL // 10137

#else

//timer intervals for timer 2
#define     ONE_SECOND_TICKS    900
#define		INTERVAL_1_LOW		0x0000UL // 0
#define		INTERVAL_1_HIGH		0x00F8UL // 248
#define		INTERVAL_2_HIGH		0x0110UL // 272
#define		INTERVAL_3_HIGH		0x01B1UL // 433
#define		INTERVAL_4_HIGH		0x027AUL // 634
#endif

// fuel consumption per impulse in microliters
#define		CONS_1_UL			14425
#define		CONS_2_UL			14257
#define		CONS_3_UL			14210
#define		CONS_4_UL			14320
#define		CONS_5_UL			15780

#define		STATUS_BUFFER_START_ADDRESS  228U
#define		FIRST_USE_LOCATION			 255U


typedef struct {
	uint16_t Hours;
	uint8_t  Minutes;
	uint8_t  Seconds;
	}runningTime_t;
	
typedef struct {
	uint32_t Liters;
	uint32_t MicroLiters;
	}fuelConsumption_t;		
	
typedef struct {
	runningTime_t		Duration;    
	fuelConsumption_t	Consumption;
	}consumptionInfo_t;	

///m48Eeprom functions
void EEPROM_GetCurrentPosition(void);
void EEPROM_saveData(consumptionInfo_t * data);
void EEPROM_loadData(consumptionInfo_t * data);

///m48Functions Functions
void USART_Transmit(uint8_t data);
uint8_t USART_Receive(uint8_t * data);
void USART_print(char * text);
uint8_t ADC_GetVal(void);

///m48Init Functions
void Mcu_Init(void);
void Timer1_Start(void);
void Timer1_Stop(void);
void Timer2_Start(void);
void Timer2_Stop(void);
void ADC_Init(void);


// BT connected
void Download_Function(void);

// efuse = 0x01
// hfuse = 0xDF
// lfuse = 0x6B

#endif /* PICOPOWERM48_FUELCOUNTER_H_ */