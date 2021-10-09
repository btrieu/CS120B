/*	Author: Brandon Trieu btrie004@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #4  Exercise #2
 *	Exercise Description: Counter with buttons to increment/decrement
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Init, Increment, Decrement, Reset}state;

void Tick() {
	switch(state) {
		case Start:
			PORTC = 0x07;
			state = Init;
			break;
		case Init:
			if ((PINA & 0x03) == 0x03) { state = Reset; }
			else if (PINA == 0x00) { state = Init; }
			else if ((PINA & 0x01) == 0x01) { state = Increment; }
			else if ((PINA & 0x02) == 0x02) { state = Decrement; }
			else { PORTA = 0xFF; }
			break;
		case Increment:
			if ((PINA & 0x03) == 0x03) { state = Reset; }
			else if (!((PINA & 0x03) == 0x03)) { state = Init; }
			else { PORTA = 0xFF; }
			break;
		case Decrement:
			if ((PINA & 0x03) == 0x03) { state = Reset; }
			else if (!((PINA & 0x03) == 0x03)) { state = Init; }
			else { PORTA = 0xFF; }
			break;
		case Reset:
			if ((PINA & 0x03) == 0x03) { state = Reset; }
			else { state = Init; }
			break;
	}
	switch(state) {
		case Start:
			break;
		case Init:
			break;
		case Increment:
			if (PINC < 9) {	
				PORTC = PINC+1;
			}
			break;
		case Decrement:
			if (PINC > 0) {
				PORTC = PINC-1;
			}
			break;
		case Reset:
			PORTC = 0x00;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	state = Start;
    /* Insert your solution below */
    while (1) {
	Tick();
    }
    return 1;
}
