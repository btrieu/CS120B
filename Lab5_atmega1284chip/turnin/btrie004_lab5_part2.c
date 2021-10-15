/*	Author: Brandon Trieu btrie004@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #5  Exercise #2
 *	Exercise Description: PA0 increments up to 9, PA1 decrements down to 0 (start at 7), output on C
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Wait, Increment, IncrementWait, Decrement, DecrementWait, Reset, ResetWait}state;

void Tick() {
	switch(state) {
		case Start:
			PORTC = 0x07;
			state = Wait;
			break;
		case Wait:
			if ((~PINA & 0x03) == 0x01) { state = Increment; }
			else if ((~PINA & 0x03) == 0x02) { state = Decrement; }
			else if ((~PINA & 0x03) == 0x03) { state = Reset; }
			else { state = Wait; }
			break;
		case Increment:
			state = IncrementWait;
			break;
		case IncrementWait:
			if ((~PINA & 0x01) == 0x01) { state = IncrementWait; }
			else if ((~PINA & 0x03) == 0x03) { state = Reset; }
			else { state = Wait; }
			break;
		case Decrement:
			state = DecrementWait;
			break;
		case DecrementWait:
			if ((~PINA & 0x02) == 0x02) { state = DecrementWait; }
			else if ((~PINA & 0x03) == 0x03) { state = Reset; }
			else { state = Wait; }
			break;
		case Reset:
			state = ResetWait;
			break;
		case ResetWait:
			if ((~PINA & 0x03) == 0x00) { state = Wait; }
			else { state = ResetWait; }
			break;
		default:
			PORTD = 0xFF;
			break;
	}
	switch(state) {
		case Start:

			break;
		case Wait:

			break;
		case Increment:
			if (PINC < 9) { PORTC = PINC + 1; }
			break;
		case IncrementWait:

			break;
		case Decrement:
			if (PINC > 0) { PORTC = PINC - 1; }
			break;
		case DecrementWait:

			break;
		case Reset:
			PORTC = 0x00;
			break;
		case ResetWait:

			break;
		default:
			PORTD = 0xFF;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    	state = Start;
    /* Insert your solution below */
    while (1) {
    	Tick();
    }
    return 1;
}
