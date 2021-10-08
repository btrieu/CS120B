/*	Author: Brandon Trieu btrie004@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #4  Exercise #3
 *	Exercise Description: Household digital combination lock 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Lock, ReadInput, Unlock}state;

unsigned char passcodeDigits = 2;
char passcode[2] = {'#', 'Y'};
char userInput[2];
unsigned char passcodeCounter = 0;

void Tick() {
	switch(state) {
		case Start:
			PORTB = 0x00;
			state = Lock;
			break;
		case Lock:
			state = ReadInput;
			break;
		case ReadInput:
			if ((PINA&0x01) == 0x01 || (PINA&0x02) == 0x02 || (PINA&0x04) == 0x04) {
			       	passcodeCounter++;
				if (passcodeCounter < passcodeDigits) {
					if ((PINA&0x07) == 0x01) { userInput[passcodeCounter-1] = 'X'; }
					if ((PINA&0x07) == 0x02) { userInput[passcodeCounter-1] = 'Y'; }
					if ((PINA&0x07) == 0x04) { userInput[passcodeCounter-1] = '#'; }
					state = ReadInput;
				}
				else {
					if ((PINA&0x07) == 0x01) { userInput[passcodeCounter-1] = 'X'; }
					if ((PINA&0x07) == 0x02) { userInput[passcodeCounter-1] = 'Y'; }
					if ((PINA&0x07) == 0x04) { userInput[passcodeCounter-1] = '#'; }
					if (memcmp(passcode, userInput, passcodeDigits)) { state = Unlock; }
					else { state = Lock; }
				}
		       	}
			else { state = ReadInput; }
			break;
		case Unlock:
			if ((PINA & 0x80) == 0x80) { state = Lock; }
			else { state = Unlock; }
			break;
	}
	switch(state) {
		case Start:
			break;
		case Lock:
			passcodeCounter = 0;
			PORTB = 0x00;
			break;
		case ReadInput:
			break;
		case Unlock:
			PORTB = 0x01;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = Start;
    /* Insert your solution below */
    while (1) {
	Tick();
    }
    return 1;
}
