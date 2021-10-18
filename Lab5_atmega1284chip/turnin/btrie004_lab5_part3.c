/*	Author: Brandon Trieu
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #5  Exercise #3
 *	Exercise Description: Lightshow - change on press
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Lights1, Wait1, Lights2, Wait2, Lights3, Wait3, Lights4, Wait4, Lights5, Wait5, Lights6, Wait6} state;

void Tick() {
	switch(state){
		case Start:
			state = Lights1;
			break;
		case Lights1:
			if ((~PINA & 0x01) == 0x01) { state = Wait1; }
			else { state = Lights1; }
			break;
		case Wait1:
			if ((~PINA & 0x01) == 0x01) { state = Wait1; }
			else { state = Lights2; }	
			break;
		case Lights2:
			if ((~PINA & 0x01) == 0x01) { state = Wait2; }
			else { state = Lights2; }
			break;
		case Wait2:
			if ((~PINA & 0x01) == 0x01) { state = Wait2; }
			else { state = Lights3; }	
			break;
		case Lights3:
			if ((~PINA & 0x01) == 0x01) { state = Wait3; }
			else { state = Lights3; }
			break;
		case Wait3:
			if ((~PINA & 0x01) == 0x01) { state = Wait3; }
			else { state = Lights4; }	
			break;
		case Lights4:
			if ((~PINA & 0x01) == 0x01) { state = Wait4; }
			else { state = Lights4; }
			break;
		case Wait4:
			if ((~PINA & 0x01) == 0x01) { state = Wait4; }
			else { state = Lights5; }	
			break;
		case Lights5:
			if ((~PINA & 0x01) == 0x01) { state = Wait5; }
			else { state = Lights5; }
			break;
		case Wait5:
			if ((~PINA & 0x01) == 0x01) { state = Wait5; }
			else { state = Lights6; }	
			break;
		case Lights6:
			if ((~PINA & 0x01) == 0x01) { state = Wait6; }
			else { state = Lights6; }
			break;
		case Wait6:
			if ((~PINA & 0x01) == 0x01) { state = Wait6; }
			else { state = Lights1; }	
			break;
	}
	switch(state){
		case Start:
			break;
		case Lights1:
			PORTB = 0x15;
			break;
		case Wait1:
			PORTB = 0x2A;
			break;
		case Lights2:
			PORTB = 0x2A;
			break;
		case Wait2:
			PORTB = 0xFF;
			break;
		case Lights3:
			PORTB = 0xFF;
			break;
		case Wait3:
			PORTB = 0x00;
			break;
		case Lights4:
			PORTB = 0x00;
			break;
		case Wait4:
			PORTB = 0xFF;
			break;
		case Lights5:
			PORTB = 0xFF;
			break;
		case Wait5:
			PORTB = 0x00;
			break;
		case Lights6:
			PORTB = 0x00;
			break;
		case Wait6:
			PORTB = 0x15;
			break;
	}
}


int main(void) {
/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	/* Insert your solution below */
	PORTB = 0x00;
	state = Start;
    while (1) {
	Tick();
    }
    return 1;
}
