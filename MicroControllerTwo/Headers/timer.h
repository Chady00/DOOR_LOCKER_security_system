 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Header file for the TIMER AVR driver
 *
 * Author: Chady Achraf
 *
 *******************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum{
	NORMAL,COMPARE
}MODE;

typedef enum{
	NO_CLOCK,NO_PRESCALE,_8,_64,_256,_1024,EXTERNAL_FALL,EXTERNAL_RISE
}PRESCALER;

typedef struct{
	MODE mode;
	uint8 initial_value;
	uint8 compare_value;
	PRESCALER prescaler;
}TIMER0_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*Initialize Timer0*/
void Timer0_Init(TIMER0_ConfigType *Config_Ptr);
/*set call back function for Timer0*/
void Timer0_setCallBack(void(*a_ptr)(void));
/*Timer0 De-initialization*/
void Timer0_Deinit();

#endif /* TIMER_H_ */
