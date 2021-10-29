/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #8  Exercise #4 
 *	Exercise Description: More ADC, More Lights
 *	Demo Link: https://drive.google.com/file/d/17MzF9HnkWeqVf_PLDmdSyiLgrz2zcUyH/view?usp=sharing 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

short maxADC = 0;

void Tick() {
	short currADC = ADC;
	if (currADC > maxADC) { maxADC = currADC; }
	if (currADC < maxADC/8) { PORTB = 0x01; }
	else if (currADC < (maxADC/8)*2) { PORTB = 0x03; }
	else if (currADC < (maxADC/8)*3) { PORTB = 0x07; }
	else if (currADC < (maxADC/8)*4) { PORTB = 0x0F; }
	else if (currADC < (maxADC/8)*5) { PORTB = 0x1F; }
	else if (currADC < (maxADC/8)*6) { PORTB = 0x3F; }
	else if (currADC < (maxADC/8)*7) { PORTB = 0x7F; }
	else if (currADC < (maxADC/8)*8) { PORTB = 0xFF; }
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0x03; PORTD = 0xFC;
	ADC_init();
	while (1) {
		Tick();
    	}	
    	return 1;
}
