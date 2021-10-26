/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #8  Exercise #2
 *	Exercise Description: Display ADC from photoresistor
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

short maxADC = 0;

void Tick() {
	short currADC = ADC;
	PORTB = currADC;
	PORTD = currADC >> 8;
	if (currADC > maxADC) { maxADC = currADC; }
}

int main(void) {
	//DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0x03; PORTD = 0xFC;
	ADC_init();
	while (1) {
		Tick();
    	}	
    	return 1;
}
