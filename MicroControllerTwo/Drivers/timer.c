 /******************************************************************************
 *
 * Module: TIMER
 *
 * File Name: timer.c
 *
 * Description: Source file for the TIMER AVR driver
 *
 * Author: Chady Achraf
 *
 *******************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "common_macros.h"
#include"timer.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
/* If Compare Mode is used, ISR handles the callback*/
ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/* If Overflow Mode is used, ISR handles the callback*/
ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*Initialize Timer0*/
void Timer0_Init(TIMER0_ConfigType *Config_Ptr)
{

	TCNT0 = Config_Ptr->initial_value;
	//LCD_intgerToString(Config_Ptr->initial_value);/*Set Timer initial value to 0*/
	OCR0=Config_Ptr->compare_value; /*Set Compare value, even if Compare Mode is not used*/
	//LCD_intgerToString(Config_Ptr->compare_value);
	/*Compare mode is pre-defined as enum member of value 1
	 * If compare mode is active , TOIE0 =0
	 * If overflow mode is active, TOIEO =1
	 * */

	TIMSK|= (1-(Config_Ptr->mode)<<TOIE0)|((Config_Ptr->mode)<<OCIE0);

	/* Configure the timer control register
	 * 1. Non PWM mode FOC0=1
	 * 2. Normal Mode WGM01=0 & WGM00=0 , Compare Mode WGM01=1 & WGM00 =0
	 * 3. Normal Mode COM00=0 & COM01=0, Compare Mode COM00=1 & COM01=0
	 * 4. clock = F_CPU/PRESCALER
	 */
	//Compare Mode =1 .. Normal Mode =0

	TCCR0 = (1<<FOC0) | (Config_Ptr->prescaler<<CS00)|(Config_Ptr->mode<<WGM01)|(Config_Ptr->mode<<COM00);
}

/*set call back function for Timer0*/
void Timer0_setCallBack(void(*a_ptr)(void)){
	/*Save the address of the Call back function in a global variable */
		g_callBackPtr = a_ptr;
}

/*Timer0 De-initialization*/
void Timer0_Deinit(){
	/* Clear All Timer0 Registers */
	TCCR0 = 0;
	TCNT0 = 0;
	OCR0 =0;
	/* Disable the Overflow Interrupt */
	TIMSK &= ~(1<<TICIE1);
}
