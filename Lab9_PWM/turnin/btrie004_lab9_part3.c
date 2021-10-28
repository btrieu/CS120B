/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #9  Exercise #3
 *	Exercise Description: Mary Had A Little Lamb
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <timer.h>
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


enum States{Start, Off, On, WaitRelease}state;

double songArr[26] = {329.63,293.66, 261.63, 293.66, 329.63,329.63,329.63,293.66, 293.66, 293.66, 329.63, 392.99, 392.99, 329.63, 293.66, 261.63, 293.66, 329.63, 329.63, 329.63, 329.63, 293.66, 293.66, 329.63, 293.66, 261.63};

int beatCount[26] = {6,6,6,6,6,6,12,6,6,12,6,6,12,6,6,6,6,6,6,6,6,6,6,6,6,24};

int tempCount[26];

unsigned char i = 0;
unsigned char j = 0;

void Tick() {
	switch(state) {
		case Start:
			state = Off;
			set_PWM(0);
			break;
		case Off:
			for(unsigned char k = 0; k < 26; ++k) {
				tempCount[k] = beatCount[k];
			}
			if ((~PINA & 0x01) == 0x01) { state = On; }
			else { state = Off; }
			break;
		case On:
			if (i >= 26) { state = WaitRelease; }
			else { state = On; }
			break;
		case WaitRelease:
			if ((~PINA & 0x01) == 0x00) { state = Off; }
			else { state = WaitRelease; }
	}
	switch(state) {
		case Start:
			break;
		case Off:
			i = 0;
			j = 0;
			break;
		case On:
			if (j % 2 == 0) {
				--tempCount[i];
				set_PWM(songArr[i]);
				if(tempCount[i] == 0) {
					++j;
					++i;
				}
			}
			else { set_PWM(0); ++j; }
			break;
		case WaitRelease:
			set_PWM(0);
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = Start;
	PWM_on();
	TimerSet(50);
	TimerOn();
	while (1) {
		Tick();
		while (!TimerFlag) { }
		TimerFlag = 0;
	}
	return 1;
}
