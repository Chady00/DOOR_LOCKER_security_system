/******************************************************************************
 *
 * Module: MOTOR
 *
 * File Name: motor.c
 *
 * Description: Source file for the motor driver
 *
 * Author: Chady Achraf
 *
 *******************************************************************************/
#include"motor.h"
#include"gpio.h"
#include "avr/io.h" /* To use the IO Ports Registers */
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * Initialize the motor.
 * 1. Setup the motor pins Direction as output pins.
 * 2. Set the motor initial state to Stopped
 */
void DcMotor_Init(void) {
	/*setting PinD4 and PinD5 as output pins--> input to the H-Bridge*/
	GPIO_setupPinDirection(PORTD_ID, PIN4_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT);
	/* Motor is Stopped at the beginning */
	PORTD &= ~(1 << PIN4_ID) & ~(1 << PIN5_ID);
}
/*
 * Description :
 * Rotate the motor: CW / A-CW / STOP
 * 1. Rotate the motor according to the motor state
 * 2. Send the speed to the PWM driver to control it
 */
void DcMotor_Rotate(DcMotor_State state) {
	/*Set the current motor state*/
	if (state == STOP) {
		GPIO_writePin(PORTD_ID, PIN5_ID, LOGIC_LOW);
		GPIO_writePin(PORTD_ID, PIN4_ID, LOGIC_LOW);
	}
	if (state == CW) {
		GPIO_writePin(PORTD_ID, PIN5_ID, LOGIC_LOW);
		GPIO_writePin(PORTD_ID, PIN4_ID, LOGIC_HIGH);
	}
	if(state == ACW){
		GPIO_writePin(PORTD_ID, PIN5_ID, LOGIC_HIGH);
		GPIO_writePin(PORTD_ID, PIN4_ID, LOGIC_LOW);
	}

}
