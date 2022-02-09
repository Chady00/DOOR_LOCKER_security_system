/*
 ============================================================================
 Name        : Final_Project_MC1.c
 Author      : Chady Achraf
 Copyright   : Your copyright notice
 Description : Final_Project_MC1
 ============================================================================
 */
#include<avr/io.h>
#include "keypad.h"
#include "uart.h"
#include "lcd.h"
#include "timer.h"
#include <util/delay.h>

/*******************************************************************************
 *                     	    Global Variables                                   *
 *******************************************************************************/
unsigned char counter=0;/*Global variable used to count the number of times handle_display is called*/
unsigned char seconds=0;/*Counts the number of seconds that actually passed*/

/********************************************************d***********************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * callback function that increments counter until it's equal to number of overflows
 * Overflow mode: for each counter = number of overflows= 31, seconds++
 * Compare mode: for each counter = compare match, seconds++
 * For initial value = 0 , compare reg = 249 to make 32ms
 * for one complete second , counter must be equal to 31 approximately
 */
void handle_display() {
	counter++;
	if (counter == 31) {
		seconds++;
		counter = 0;
	}
}

int main(void) {
	DDRA = (1 << 0);
	uint8 key_num;/*Get keypad value and pass it to MC2 through UART */

	/*skip is used if you want to receive two messages consecutively using UART driver
	 *if skip =1 , MC1 will skill the keypad process and directly receives message
	 */
	uint8 skip = 0;

	/* Variable message used to distinguish the display message
	 * It should follow the enum defined in MC2 accordingly
	 */
	uint8 message = 0;

	/* Initialize the LCD driver*/
	LCD_init();

	/*Enable global interrupts by setting the I-bit*/
	SREG |= (1 << 7);

	/* Initialize the UART driver with Baud-rate = 9600 bits/sec */
	UART_ConfigType UART_config = { EIGHT_BIT, DISABLED, ONE_BIT, _9600 };
	UART_init(&UART_config);

	/*Setting configuration for TIMER0 driver*/;
	TIMER0_ConfigType TIMER0_config = {NORMAL, 0, 0, _1024 };
	Timer0_setCallBack(&handle_display);
	_delay_ms(100);

	/*Display String and Move Cursor*/
	LCD_displayString("Plz enter pass: ");
	LCD_moveCursor(1, 0);

	while (1) {
		while (key_num != 13 && key_num != '+' && key_num != '-' && skip != 1) {
			/*Read keypad button pressed and send it to MC2 through UART*/
			key_num = KEYPAD_getPressedKey();

			/*For each pressed button display an Asterisk unless it's enter or an option*/
			if (key_num != 13 && key_num != '+' && key_num != '-')
				LCD_displayCharacter('*');
			_delay_ms(20);

			/* Send the pressed key to MC2 through UART */
			UART_sendByte(key_num);

			/* Wait 500msec before getting a new press from the keypad buttons, Press time is 500msec */
			_delay_ms(2500);
		}

		/*reset the key_num to any value for the next iteration*/
		key_num = 0;
		/*reset the skip to any value for the next iteration*/
		skip = 0;
		/*Read which message to display*/
		message = UART_recieveByte();

		/* The ENTER password message is displayed*/
		if (message == 0) {
			LCD_clearScreen();
			/*Display message*/
			LCD_displayString("Plz enter pass: ");

			/*Move back the pointer to (1,0)*/
			LCD_moveCursor(1, 0);
			_delay_ms(20);
		}

		/* The RE-ENTER PASS message is displayed*/
		else if (message == 1) {
			LCD_clearScreen();
			/*Display message*/
			LCD_displayString("Re-enter pass: ");

			/*Move back the pointer to (1,0)*/
			LCD_moveCursor(1, 0);
			_delay_ms(20);
		}

		/* The MENU is displayed*/
		else if (message == 2) {
			LCD_clearScreen();
			/*Display message*/
			LCD_displayString("+ : open door");

			/*Move back the pointer to (1,0)*/
			LCD_moveCursor(1, 0);
			LCD_displayString("- : change pass");
			_delay_ms(20);
		}

		/* The WRONG PASSWORD message is displayed*/
		else if (message == 3) {
			LCD_clearScreen();
			/*Display MISMATCH message*/
			LCD_displayStringRowColumn(0, 1, "WRONG PASSWORD");
			/*Start Timer0*/
			Timer0_Init(&TIMER0_config);

			/* The WRONG PASSWORD message is displayed for 2 seconds*/
			while (seconds != 2)
				;
			/*reset the seconds back to 0*/
			seconds = 0;
			/*Stop Timer0*/
			Timer0_Deinit();
			/*display Enter password message for the next iteration*/
			LCD_clearScreen();
			LCD_displayString("Plz enter pass:");
			LCD_moveCursor(1, 0);
			_delay_ms(20);
		}

		/* The DOOR UNLOCKED message is displayed*/
		else if (message == 4) {
			/* Display door is unlocked*/
			LCD_clearScreen();
			/*Display DOOR UNLOCKED message*/
			LCD_displayStringRowColumn(0, 2, "DOOR UNLOCKED");
			/*set skip to 1 so you skip the get_keypad requirement*/
			skip = 1;
		}
		/* The ACCESS GRANTED message is displayed*/
		else if (message == 5) {
			LCD_clearScreen();

			/*Display message*/
			LCD_displayStringRowColumn(0, 1, "ACCESS GRANTED");

			/*Start Timer0*/
			Timer0_Init(&TIMER0_config);
			/* The ACCESS GRANTED message is displayed for 2 seconds*/
			while (seconds != 2)
				;
			/*reset the seconds back to 0*/
			seconds = 0;
			/*Stop Timer0*/
			Timer0_Deinit();
			/*display Enter password message for the next iteration*/
			LCD_clearScreen();
			LCD_displayString("Plz enter pass:");
			LCD_moveCursor(1, 0);
			_delay_ms(20);
		} else if (message == 6) {
			LCD_clearScreen();
			/*Display MISMATCH message*/
			LCD_displayStringRowColumn(0, 4, "INTRUDER");
			LCD_displayStringRowColumn(1, 4, "DETECTED");
			skip = 1;
		}

	}

}

