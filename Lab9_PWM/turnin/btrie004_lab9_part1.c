/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: A0 for C4, A1 for D4, A2 for E4
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
	static double current_frequency;
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; }
		else { TCCR3B |= 0x03; }
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		else if (frequency > 31250) { OCR3A = 0x0000; }
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}


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
