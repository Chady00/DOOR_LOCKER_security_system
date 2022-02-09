/*
 ============================================================================
 Name        : MicroController1.h
 Author      : Chady Achraf
 Copyright   : Your copyright notice
 Description : MicroController1
 ============================================================================
 */
#ifndef MICROCONTROLLER1_H_
#define MICROCONTROLLER1_H_

#include "std_types.h"
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * callback function that increments counter until it's equal to number of overflows
 * Overflow mode: for each counter = number of overflows= 31, seconds++
 * Compare mode: for each counter = compare match, seconds++
 * For initial value = 0 , compare reg = 249 to make 32ms
 * for one complete second , counter must be equal to 31 approximately
 */
void handle_display();


#endif /* MICROCONTROLLER1_H_ */
