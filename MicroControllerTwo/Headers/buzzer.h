/*
 * buzzer.h
 *
 *  Created on: Oct 23, 2021
 *      Author: lenovo
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include"std_types.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PORTA_MY_ID               0
#define PORTB_MY_ID               1
#define PORTC_MY_ID               2
#define PORTD_MY_ID               3

#define PIN0_MY_ID                0
#define PIN1_MY_ID                1
#define PIN2_MY_ID                2
#define PIN3_MY_ID                3
#define PIN4_MY_ID                4
#define PIN5_MY_ID                5
#define PIN6_MY_ID                6
#define PIN7_MY_ID                7

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void buzzer_init(uint8,uint8);
void buzzer_on();
void buzzer_off();


#endif /* BUZZER_H_ */
