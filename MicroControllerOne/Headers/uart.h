 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum {
	FIVE_BIT,SIX_BIT,SEVEN_BIT,EIGHT_BIT,NINE_BIT=7
}FRAME_size; /* reference voltage indicates the conversion range for the ADC*/

typedef enum {
	DISABLED,EVEN=2,ODD
}PARITY_bit;

typedef enum {
	ONE_BIT,TWO_BIT
}STOP_bit;

typedef enum{
 _10=10,_300=300,_600=600,_1200=1200, _2400=2400,_4800=4800,_9600=9600,
 _14400=14400, _19200=19200,_38400=38400,_57600=57600,_115200=115200,
 _128000=128000,_256000
}BIT_RATE;

typedef struct {
	FRAME_size data_size;
	PARITY_bit parity;
	STOP_bit stop;
	BIT_RATE rate;
} UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
