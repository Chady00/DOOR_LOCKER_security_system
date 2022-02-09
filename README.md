# DOOR_LOCKER_security_system
A door locking security system final project
1. The aim of this project is to design an interactive door lokcer security system using two ATmega16 microcontrollers, in which the password is entered and passed from one microcontroller to the other according to the UART-protocol. Use ATmega16 Microcontroller with frequency 8Mhz.
2. In this project, the user will enter a password(consisting of 1-> 16 keys), the password is sent to the second microcontroller through UART , the user will then enter the password again. If both passwords match, the password will be saved in using TWI protocol in EEPROM, if not, the user will keep entering a password.
3. After the password is entered, a menu will be displayed on LCD.
The user has two options : either to open the door, or change the password.
Whether the user chooses any of the options, they are required to enter the password once again. If it matches the password in EEPROM, access will be granted.
4. However, if they consecutively enter a wrong password 3 times in a row, the buzzer is enabled for 60 seconds.

Here are some of the tests done during the process of the project:

I connected an LCD to MC2 to test that the EEPROM can store and load the password correctly as expected.
![image](https://user-images.githubusercontent.com/84717550/153171522-14f26415-97a5-4b91-b652-f8dfaba150d2.png)

Testing Timer0 driver through incrementing count and displaying it on the LCD

![image](https://user-images.githubusercontent.com/84717550/153171960-73651e83-adbe-4c92-b7f2-717ca50a5471.png) ![image](https://user-images.githubusercontent.com/84717550/153171985-e2a3467c-66e8-454b-ae1f-f7db5075fc70.png) ![image](https://user-images.githubusercontent.com/84717550/153171997-94a3a3a0-0fa2-450a-bcd2-1f5ea9eb97a0.png)




