/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, PB0_ON, TRANS_PB1_ON, PB1_ON, TRANS_PB0_ON}state;

void Tick() {
	switch(state) {
		case Start:
			PORTB = 0x01;
			state = PB0_ON;
			break;
		case PB0_ON:
			if ((PINA & 0x01) == 0x01) { state = TRANS_PB1_ON; }
			else { state = PB0_ON; }
			break;
		case TRANS_PB1_ON:
			if ((PINA & 0x01) == 0x01) { state = TRANS_PB1_ON; }
			else { state = PB1_ON; }
			break;
		case PB1_ON:
			if ((PINA & 0x01) == 0x01) { state = TRANS_PB0_ON; }
			else { state = PB1_ON; }
			break;
		case TRANS_PB0_ON:
			if ((PINA & 0x01) == 0x01) { state = TRANS_PB0_ON; }
			else { state = PB0_ON; }
			break;
	}
	switch(state) {
		case Start:
			break;
		case PB0_ON:
			PORTB = 0x01;
			break;
		case TRANS_PB1_ON:
			PORTB = 0x02;
			break;
		case PB1_ON:
			PORTB = 0x02;
			break;
		case TRANS_PB0_ON:
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
