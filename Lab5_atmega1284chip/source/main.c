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
	DDRA = 0x00; PORTA = 0xFF;
//	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
//	unsigned char counter = 0x00;
	unsigned char tempA = 0x00;
//	unsigned char tempB = 0x00;
	unsigned char tempC = 0x00;
//	unsigned int i;
    /* Insert your solution below */
    while (1) {
	tempA = (~PINA) & 0x0F;
	tempC = 0;
	
	//fuel level
	if (tempA > 0) {
		tempC = tempC | 0x01;	
	}	
	tempC = tempC << 1;
	if (tempA > 2) {
		tempC = tempC | 0x01;
	}
	tempC = tempC << 1;
	if (tempA > 4) {
		tempC = tempC | 0x01;
	}
	tempC = tempC << 1;
	if (tempA > 6) {
		tempC = tempC | 0x01;
	}
	tempC = tempC << 1;
	if (tempA > 9) {
		tempC = tempC | 0x01;
	}
	tempC = tempC << 1;
	if (tempA > 12) {
		tempC = tempC | 0x01;
	}

	//low fuel light
	if (tempA <= 4) {
		tempC = tempC | 0x40;
	}

	PORTC = tempC;
    }
    return 1;
}
