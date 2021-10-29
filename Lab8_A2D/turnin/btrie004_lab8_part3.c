/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #8  Exercise #3 
 *	Exercise Description: Light when ADC/2>=maxADC
 *	Demo Link: https://drive.google.com/file/d/17L6tGMEE_Q2M8O73JfF7x_VW431VYuVU/view?usp=sharing
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
	if (currADC >= maxADC/2) { PORTB = 0x01; }
	else if (currADC < maxADC/2) { PORTB = 0x00; }
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
