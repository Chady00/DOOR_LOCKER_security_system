/*
 ============================================================================
 Name        : buzzer.c
 Author      : Chady Achraf
 Copyright   : Your copyright notice
 Description : Buzzer
 ============================================================================
 */
#include"buzzer.h"
#include"avr/io.h"
#include"common_macros.h"
#include"gpio.h"
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
/*Stores the port and pin values to turn ON/OFF without the need of parameters*/
uint8 Port_used;
uint8 Pin_used;
/*******************************************************************************
 *                           Functions Definitions                             *
 *******************************************************************************/

void buzzer_init(uint8 PORT,uint8 PIN) {
/*setup PIN_ID as output pin*/
	switch (PORT) {
	case PORTA_MY_ID:
		SET_BIT(DDRA, PIN);
		break;

	case PORTB_MY_ID:
		SET_BIT(DDRB, PIN);
		break;

	case PORTC_MY_ID:
		SET_BIT(DDRC, PIN);
		break;

	case PORTD_MY_ID:
		SET_BIT(DDRD, PIN);
		break;
	}
	Port_used=PORT;
	Pin_used=PIN;

}

void buzzer_off() {
	GPIO_writePin(Port_used, Pin_used, LOGIC_LOW);
}

void buzzer_on(){
	GPIO_writePin(Port_used, Pin_used, LOGIC_HIGH);
}
