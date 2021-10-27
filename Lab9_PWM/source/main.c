/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: Single Octave Scale
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


enum States{Start, Off, On, Up, Down, WaitRelease}state;

double currPWM = 0;

void Tick() {
	switch(state) {
		case Start:
			state = Off;
			set_PWM(0);
			currPWM = 0;
			break;
		case Off:
			if ((~PINA & 0x07) == 0x02) { state = On; }
			else { state = Off; }
			break;
		case On:
 			if ((~PINA & 0x07) == 0x02) { state = Off; }
			else if ((~PINA & 0x07) == 0x01) { state = Up; }
			else if ((~PINA & 0x07) == 0x04) { state = Down; }
			else { state = On; }
			break;
		case Up:
			state = WaitRelease;
			break;
		case Down:
			state = WaitRelease;
			break;
		case WaitRelease:
			if ((~PINA & 0x07) == 0x00) { state = On; }
			else { state = WaitRelease; }
			break;
	}
	switch(state) {
		case Start:
			break;
		case Off:
			set_PWM(0);
			//currPWM = 0;
			break;
		case On:
			if (currPWM == 0) { set_PWM(261.63); currPWM = 261.63; }
			else { set_PWM(currPWM); }
			break;
		case Up:
			if (currPWM == 261.63) { set_PWM(293.66); currPWM = 293.66; }
			else if (currPWM == 293.66) { set_PWM(329.63); currPWM = 329.63; }
			else if (currPWM == 329.63) { set_PWM(349.23); currPWM = 349.23; }
			else if (currPWM == 349.23) { set_PWM(392.00); currPWM = 392.00; }
			else if (currPWM == 392.00) { set_PWM(440.00); currPWM = 440.00; }
			else if (currPWM == 440.00) { set_PWM(493.88); currPWM = 493.88; }
			else if (currPWM == 493.88) { set_PWM(523.25); currPWM = 523.25; }
			break;
		case Down:
			if (currPWM == 523.25) { set_PWM(493.88); currPWM = 493.88; }
			else if (currPWM == 493.88) { set_PWM(440.00); currPWM = 440.00; }
			else if (currPWM == 440.00) { set_PWM(392.00); currPWM = 392.00; }
			else if (currPWM == 392.00) { set_PWM(349.23); currPWM = 349.23; }
			else if (currPWM == 349.23) { set_PWM(329.63); currPWM = 329.63; }
			else if (currPWM == 329.63) { set_PWM(293.66); currPWM = 293.66; }
			else if (currPWM == 293.66) { set_PWM(261.63); currPWM = 261.63; }
			break;
		case WaitRelease:
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = Start;
	PWM_on();
	TimerSet(200);
	TimerOn();
	while (1) {
		Tick();
		while (!TimerFlag) { }
		TimerFlag = 0;
	}
	return 1;
}
