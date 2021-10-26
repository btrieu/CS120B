/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description: Display ADC from potentiometer
 *
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

void Tick() {
	short currADC = ADC;
	PORTB = currADC;
	PORTD = currADC >> 8;
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
