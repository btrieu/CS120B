/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: A0 for C4, A1 for D4, A2 for E4
 *	Demo Link: https://drive.google.com/file/d/17FA_8P7o5Gzts5mJCDVBHuXr4LihzGL1/view?usp=sharing
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "pwm.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Wait, C4, D4, E4}state;

void Tick() {
	switch(state) {
		case Start:
			state = Wait;
			break;
		case Wait:
			if ((~PINA & 0x07) == 0x01) { state = C4; }
			else if ((~PINA & 0x07) == 0x02) { state = D4; }
			else if ((~PINA & 0x07) == 0x04) { state = E4; }
			else { state = Wait; }
			break;
		case C4:
 			if ((~PINA & 0x07) == 0x01) { state = C4; }
			else { state = Wait; }
			break;
		case D4:
			if ((~PINA & 0x07) == 0x02) { state = D4; }
			else { state = Wait; }
			break;
		case E4:
			if ((~PINA & 0x07) == 0x04) { state = E4; }
			else { state = Wait; }
			break;
	}
	switch(state) {
		case Start:

			break;
		case Wait:
			set_PWM(0);
			break;
		case C4:
			set_PWM(261.63);
			break;
		case D4:
			set_PWM(293.66);
			break;
		case E4:
			set_PWM(329.63);
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = Start;

	PWM_on();

	while (1) {
		Tick();
	}
	return 1;
}
