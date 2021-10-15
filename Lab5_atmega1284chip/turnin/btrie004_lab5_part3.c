/*	Author: Brandon Trieu btrie004@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #5  Exercise #3
 *	Exercise Description: lightshow on B, change mode w/ A0
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Lights1, Lights2, Lights3, Wait1, Wait2, Wait3}state;

void Tick() {
	PORTB = 0x00;
	switch(state) {
		case Start:
			PORTB = 0x00;
			state = Lights1;
			break;
		case Lights1:
			if ((PINA & 0x01) == 0x01) { state = Wait1; }
			else { state = Lights1; }
			break;
		case Wait1:
			state = Lights1;
			break;
		case Lights2:
		
			break;
		case Wait2:
		
			break;
		case Lights3:
		
			break;
		case Wait3:

			break;
	}
	switch(state) {
		case Start:

			break;
		case Lights1:
			PORTB = 0x15;
			}
			break;
		case Wait1:
		
			break;
		case Lights2:
		
			break;
		case Wait2:
		
			break;
		case Lights3:
		
			break;
		case Wait3:

			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	state = Start;
	while(1) {
		Tick();
	}
	return 1;
}
