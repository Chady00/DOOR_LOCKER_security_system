/******************************************************************************
 *
 * Module: MOTOR
 *
 * File Name: motor.c
 *
 * Description: Header file for the motor driver
 *
 * Author: Chady Achraf
 *
 *******************************************************************************/
#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum{
 STOP,CW,ACW
}DcMotor_State;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Initialize the motor.
 * 1. Setup the motor pins Direction as output pins.
 * 2. Set the motor initial state to Stopped
 */
void DcMotor_Init(void);
/*
 * Description :
 * Rotate the motor: CW / A-CW / STOP
 * 1. Rotate the motor according to the motor state
 * 2. Send the speed to the PWM driver to control it
 */
void DcMotor_Rotate(DcMotor_State state);

#endif /* MOTOR_H_ */
