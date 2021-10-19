/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Lights}state;

void Tick() {
	switch(state) {
		case Start:
			PORTB = 0x00;
			state = Lights;
			break;
		case Lights:
			state = Lights;
			break;
	}
	switch(state) {
		case Start:
			break;
		case Lights:
			if (PINB == 0x00) { PORTB = 0x01; }
			else if (PINB == 0x01) { PORTB = 0x02; }
			else if (PINB == 0x02) { PORTB = 0x04; }
			else if (PINB == 0x04) { PORTB = 0x01; }
			else { PORTD == 0xFF; }
			break;
	}
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
	while(1) {
		Tick();
		while (!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}
