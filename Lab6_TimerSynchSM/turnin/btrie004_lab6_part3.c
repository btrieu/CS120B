/*	Author: Brandon Trieu btrie004@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #6  Exercise #3
 *	Exercise Description: PA0 increments up to 9, PA1 decrements down to 0 (start at 7), output on C. Now with timers!!
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Wait, Increment, IncrementWait, Decrement, DecrementWait, Reset, ResetWait}state;

unsigned char holdCtr = 0;

void Tick() {
	switch(state) {
		case Start:
			PORTB = 0x07;
			state = Wait;
			break;
		case Wait:
			if ((~PINA & 0x03) == 0x01) { state = Increment; }
			else if ((~PINA & 0x03) == 0x02) { state = Decrement; }
			else if ((~PINA & 0x03) == 0x03) { state = Reset; }
			else { state = Wait; }
			break;
		case Increment:
			holdCtr = 0;
			state = IncrementWait;
			break;
		case IncrementWait:
			++holdCtr;
			if ((~PINA & 0x03) == 0x00) { state = Wait; }
			else if (((~PINA & 0x03) == 0x01) && holdCtr > 10) { state = Increment; }
			else if ((~PINA & 0x03) == 0x03) { state = Reset; }
			else { state = IncrementWait; }
			break;
		case Decrement:
			holdCtr = 0;
			state = DecrementWait;
			break;
		case DecrementWait:
			++holdCtr;
			if ((~PINA & 0x03) == 0x00) { state = Wait; }
			else if (((~PINA & 0x03) == 0x02) && holdCtr > 10) { state = Decrement; }
			else if ((~PINA & 0x03) == 0x03) { state = Reset; }
			else { state = DecrementWait; }
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
			if (PINB < 9) { PORTB = PINB + 1; }
			break;
		case IncrementWait:

			break;
		case Decrement:
			if (PINB > 0) { PORTB = PINB - 1; }
			break;
		case DecrementWait:

			break;
		case Reset:
			PORTB = 0x00;
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
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    	state = Start;
    /* Insert your solution below */
	TimerSet(100);
	TimerOn();
    while (1) {
    	Tick();
	while (!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
