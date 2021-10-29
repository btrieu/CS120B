/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: Single Octave Scale
 *	Demo Link: https://drive.google.com/file/d/17DX5QbGj2GuULsciwhvr_xO-zQG9M0oN/view?usp=sharing
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
#include "pwm.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Off, On, Up, Down, WaitRelease}state;

double currPWM = 0;
unsigned char OnFlag = 0;

void Tick() {
	switch(state) {
		case Start:
			state = Off;
			set_PWM(0);
			currPWM = 0;
			break;
		case Off:
			if ((~PINA & 0x07) == 0x02) { state = On; }
			else if ((~PINA & 0x07) == 0x01) { state = Up; }
			else if ((~PINA & 0x07) == 0x04) { state = Down; }
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
			if ((~PINA & 0x07) == 0x00) {
				if (OnFlag) { state = On; }
				else { state = Off; }
			}
			else { state = WaitRelease; }
			break;
	}
	switch(state) {
		case Start:
			break;
		case Off:
			OnFlag = 0;
			set_PWM(0);
			break;
		case On:
			OnFlag = 1;
			if (currPWM == 0) { set_PWM(261.63); currPWM = 261.63; }
			else { set_PWM(currPWM); }
			break;
		case Up:
			if (currPWM == 261.63) { currPWM = 293.66; }
			else if (currPWM == 293.66) { currPWM = 329.63; }
			else if (currPWM == 329.63) { currPWM = 349.23; }
			else if (currPWM == 349.23) { currPWM = 392.00; }
			else if (currPWM == 392.00) { currPWM = 440.00; }
			else if (currPWM == 440.00) { currPWM = 493.88; }
			else if (currPWM == 493.88) { currPWM = 523.25; }
			if (OnFlag) { set_PWM(currPWM); }
			break;
		case Down:
			if (currPWM == 523.25) { currPWM = 493.88; }
			else if (currPWM == 493.88) { currPWM = 440.00; }
			else if (currPWM == 440.00) { currPWM = 392.00; }
			else if (currPWM == 392.00) { currPWM = 349.23; }
			else if (currPWM == 349.23) { currPWM = 329.63; }
			else if (currPWM == 329.63) { currPWM = 293.66; }
			else if (currPWM == 293.66) { currPWM = 261.63; }
			if (OnFlag) { set_PWM(currPWM); }
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
