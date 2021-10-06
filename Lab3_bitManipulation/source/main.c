/*	Author: Brandon Trieu btrie004@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #3  Exercise #2
 *	Exercise Description: Basic car fuel sensor
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0x01; PORTB = 0x01;
	DDRD = 0xFF; PORTD = 0xFF;
	unsigned short weight;
    /* Insert your solution below */
    while (1) {
    	//PORTB = PINB & 0x01;
	weight = 0;
	weight = ((weight + PIND) << 1) | (PINB & 0x01);
	if (weight >= 70) {
		PORTB = PINB | 0x02;
	}
	if (weight > 5 && weight < 70) {
		PORTB = PINB | 0x04;
	}
    }
    return 1;
}
