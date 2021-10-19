/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: "Stop the Light" Game
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Lights, Stop, StopRelease}state;

unsigned short ctr;
unsigned short currState;

void Tick() {
	switch(state) {
		case Start:
			PORTB = 0x00;
			state = Lights;
			break;
		case Lights:
			if ((~PINA & 0x01) == 0x01) { state = Stop; }
			else { state = Lights; }
			break;
		case Stop:
			if ((~PINA & 0x01) == 0x01) { state = Stop; }
			else { state = StopRelease; }
			break;
		case StopRelease:
			if ((~PINA & 0x01) == 0x01) { state = Lights; }
			else { state = StopRelease; }
			break;
	}		
	switch(state) {
		case Start:
			break;
		case Lights:
			if (ctr >= 4) { ctr = 0; }
			++ctr;
			if (ctr == 1) { PORTB = 0x01; }
			if (ctr == 2) { PORTB = 0x02; }
			if (ctr == 3) { PORTB = 0x04; }
			if (ctr == 4) { PORTB = 0x02; }
			break;
		case Stop:
			currState = ctr;
			if (ctr == 1) { PORTB = 0x01; }
			if (ctr == 2) { PORTB = 0x02; }
			if (ctr == 3) { PORTB = 0x04; }
			if (ctr == 4) { PORTB = 0x02; }
			break;
		case StopRelease:
			if (currState == 1) { PORTB = 0x01; }
			if (currState == 2) { PORTB = 0x02; }
			if (currState == 3) { PORTB = 0x04; }
			if (currState == 4) { PORTB = 0x02; }
			ctr = 0;
			break;
	}
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	TimerSet(300);
	TimerOn();
	while(1) {
		Tick();
		while (!TimerFlag);
		TimerFlag = 0;
	}
	return 1;
}
