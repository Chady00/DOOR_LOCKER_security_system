/*
 ============================================================================
 Name        : MicroController2.c
 Author      : Chady Achraf
 Copyright   : Your copyright notice
 Description : MicroController2
 ============================================================================
 */

#include<avr/io.h>
#include<util/delay.h>
#include"motor.h"
#include"buzzer.h"
#include"uart.h"
#include"twi.h"
#include"external_eeprom.h"
#include"timer.h"
#include"MicroController2.h"

/*******************************************************************************
 *                     	    Global Variables                                   *
 *******************************************************************************/

uint8 pass_size = 0; /* Unified global variable to count the password size entered on keypad*/
uint16 pass[16];/* Global Array to store the password from EEPROM(in case compare match is needed)*/
uint16 match_pass[16];/*Global Array to store the password received from UART(to compare with EEPROM)*/
unsigned char counter=0;/*Global variable used to count the number of times handle_display is called*/
unsigned char seconds=0;/*Counts the number of seconds that actually passed*/
unsigned char fixed_pass_size=0;/*Contains the actual password size saved in EEPROM*/

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * callback function that increments counter until it's equal to number of overflows
 * Overflow mode: for each counter = number of overflows= 31, seconds++
 * Compare mode: for each counter = compare match, seconds++
 * For initial value = 0 , compare reg = 250 to make 32ms
 * for one complete second , counter must be equal to 31 approximately
 */
void Handle_action(void) {
	counter++;
	if (counter == 31) {
		seconds++;
		counter = 0;
	}
}

/*
 * Description :
 * Waits for a specific number of seconds
 * De-initizalizes the timer and sets seconds back to 0
 */
void wait_sec(unsigned char sec) {
	while (seconds != sec);
	Timer0_Deinit();
	seconds = 0;
}

/*
 * Description :
 * Compare function that compares:
 * 1. the output of EEPROM loaded in pass[16]
 * 2. the second password entered in match_pass[16]
 * if match return MATCH of value 1 , else return MISMATCH of value 0
 */
CHECK compare() {
	/*This condition prevents from entering a partial password or more than required*/
	if(fixed_pass_size!=pass_size){return MISMATCH;}

	/*This step prevents from entering "ENTER" button as a password*/
	if (pass_size == 0) {
			return MISMATCH;
		}
	for (int i = 0; i < fixed_pass_size; i++) {
		if (pass[i] != match_pass[i]) {
			return MISMATCH;
		}
	}
	return MATCH;
}


/*
 * Description :
 * Function used to store/load the values to/from the eeprom
 */
void store_read_pass(void) {
	for (int var = 0; var < pass_size; ++var) {
		EEPROM_writeByte((0x0311 + var), pass[var]);
		_delay_ms(100);
		EEPROM_readByte((0x0311 + var), &pass[var]);
	}
}

/*
 * Description :
 * Read the password from the eeprom and stores it in pass[16]
 */
void EEPROM_read_pass() {
	for (int i = 0; i < pass_size; i++) {
		EEPROM_readByte((0x0311 + i), &pass[i]);
	}
}

/*
 * Description :
 * Enter a password in match_pass[16], to match with the password in EEPROM
 */
void ENTER_pass_2() {
	uint8 key = 0;
	for (int i = 0; key != 13; i++) {
		key = UART_recieveByte();
		if (key != 13) {
			match_pass[i] = key;
			pass_size++;
		}
	}
}

/*
 * Description :
 * Enter the password for the first time and store in pass[16]
 */
void ENTER_first_time(){
	uint8 key = 0;
	for (int i = 0; key != 13; i++) {
		key = UART_recieveByte();
		if (key != 13) {
			pass[i] = key;
		}
	}
}

int main(void) {
	uint8 key; /* Value sent from UART in MC1*/
	uint8 write_first_enable = 1;/* If wfe = 1 --> write password for the first time*/
	uint8 menu_enable = 0;/* If menu=1 --> handle menu options*/
	uint8 count_mismatch = 0;/* stores the number of times the user enters a wrong password*/

	/* Initialize the UART driver with Baud-rate = 9600 bits/sec */
	UART_ConfigType UART_type = { EIGHT_BIT, DISABLED, ONE_BIT, _9600 };
	UART_init(&UART_type);

	/* Initialize the LCD Driver */
	LCD_init();

	/*Initialize the BUZZER driver*/
	buzzer_init(PORTB_MY_ID, PIN0_MY_ID);
	//buzzer_on();

	/*Initialize the TWI driver*/
	TWI_ConfigType TWI_type = { 400000, 0x01 };
	TWI_init(&TWI_type);

	/*Initialize the MOTOR driver*/
	DcMotor_Init();

	/*Enable global interrupts by setting the I-bit*/
	SREG |= (1 << 7);

	/*Setting configuration for TIMER0 driver*/;
	TIMER0_ConfigType TIMER0_type = { NORMAL, 0, 250, _1024 };
	Timer0_setCallBack(&Handle_action);
	_delay_ms(100);

	while (1) {

		/*----1---read first time the password until ENTER is pressed and save it in EEPROM*/
		if (write_first_enable) {

			/*Enter password for the first time*/
			ENTER_first_time();
			_delay_ms(10);

			/* Display Enter_first message on MC1*/
			UART_sendByte(ENTER_AGAIN);
			_delay_ms(10);

			/* Ask to enter password a second time
			 * Enter passwor second time and store the second password in match_pass[] */
			ENTER_pass_2();
			/*lock the fixed password size to use in comparing passwords*/
			fixed_pass_size=pass_size;

			/*if both passwords match, don't enter password again
			 * save password in EEPROM
			 * write_first_enable =0 , menu_enable=1
			 */
			if (compare()) {
				/* Display Enter_first message on MC1 LCD*/
				UART_sendByte(MENU);
				/*save password into EEPROM and read it back into array[16]*/
				store_read_pass();
				/*skip block---1--- and start block---2---*/
				write_first_enable = 0;
				menu_enable = 1;
			}

			/*if both passwords don't match, repeat step1*/
			else {

				UART_sendByte(WRONG_PASS);
				/*reset password size to prevent exceeding the previous stored values*/
				pass_size = 0;

			}
		}

		/*----2----show two options : either + unlock door or - change password
		 * write_first_enable = 0;(block will not be executed)
		 * menu_enable=1;
		 */
		if (menu_enable) {
			/*Skip this block if count_missmatch = 1*/
			if (count_mismatch == 0) {
				/*Enter character '+' or character '-' */
				key = UART_recieveByte();
				/* display ENTER_FIRST user reset count_mismatch or entered password correctly*/
				UART_sendByte(ENTER_FIRST);
				_delay_ms(10);
			}

			/*----3---- option '+' block*/
			if (key == '+') {
				/*Reset password size*/
				pass_size = 0;
				/* Ask to enter password a second time
				 * Enter password second time and store the second password in match_pass[]
				 */
				ENTER_pass_2();
				_delay_ms(30);

				/*Load the value from EEPROM to pass[16] array for compare*/
				EEPROM_read_pass();
				_delay_ms(10);

				/*if both passwords match, start motor actions*/
				if (compare()) {
					/*Send open door message*/
					UART_sendByte(DOOR_UNLOCKED);
					/*start timer0 */
					Timer0_Init(&TIMER0_type);
					/*Rotate Dc motor in CW direction for 15 seconds*/
					DcMotor_Rotate(CW);
					wait_sec(15);

					/*start timer0 */
					Timer0_Init(&TIMER0_type);
					/*Stop Dc motor for 3 seconds*/
					DcMotor_Rotate(STOP);
					wait_sec(3);

					/*start timer0 */
					Timer0_Init(&TIMER0_type);
					/*Rotate Dc motor in CW direction for 15 seconds*/
					DcMotor_Rotate(ACW);
					wait_sec(15);
					DcMotor_Rotate(STOP);
					/* Display MENU options message on MC1 LCD*/
					UART_sendByte(MENU);
					/*reset count_mismatch for the next iteration*/
					count_mismatch = 0;
				}

				/*if passwords don't match, ask user to enter password again up to 2 times*/
				else {
					/*the user entered a wrong password, increase count_mismatch*/
					count_mismatch++;

					if (count_mismatch == 3) {
						UART_sendByte(INTRUDER);
						/*start timer0 */
						Timer0_Init(&TIMER0_type);
						/*turn on buzzer for 1 minute*/
						buzzer_on();
						wait_sec(60);
						/*turn off buzzer*/
						buzzer_off();
						/*reset count_mismatch for the next iteration*/
						count_mismatch = 0;
						/* display main options*/
						UART_sendByte(MENU);
					}

					else {
						/* Display WRONG PASS message on MC1 LCD*/
						UART_sendByte(WRONG_PASS);
					}
				}
			}

			/*----4---- option '-' block*/
			else if (key == '-') {
				/*reset password size*/
				pass_size = 0;

				/* Ask to enter password a second time
				 * Enter password second time and store the second password in match_pass[]
				 */
				ENTER_pass_2();
				_delay_ms(30);

				/*Load the value from EEPROM to pass[16] array for compare*/
				EEPROM_read_pass();
				_delay_ms(10);

				/*if both passwords match:
				 * write_first_enable = 1;
				 * menu_enable=0;(block will not be executed)
				 * reset count_mismatch for the next iteration
				 */
				if (compare()) {
					UART_sendByte(PASS_MATCH);
					write_first_enable = 1;
					menu_enable = 0;
					/*reset pass_size*/
					pass_size = 0;
					count_mismatch = 0;
				}
				/*if passwords don't match, ask user to enter password again up to 2 times*/
				else {
					/*the user entered a wrong password, increase count_mismatch*/
					count_mismatch++;

					if (count_mismatch == 3) {
						UART_sendByte(INTRUDER);
						/*start timer0 */
						Timer0_Init(&TIMER0_type);
						/*turn on buzzer for 1 minute*/
						buzzer_on();
						wait_sec(60);
						/*turn off buzzer*/
						buzzer_off();
						/*reset count_mismatch for the next iteration*/
						count_mismatch = 0;
						/* display main options*/
						UART_sendByte(MENU);
					}

					else {
						/* Display WRONG PASS message on MC1 LCD*/
						UART_sendByte(WRONG_PASS);
					}
				}
			}
		}
		_delay_ms(100);
	}
}

