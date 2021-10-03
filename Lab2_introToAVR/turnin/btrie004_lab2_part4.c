/*	Author: Brandon Trieu btrie004@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #2  Exercise #4 Challenge
 *	Exercise Description: Amusement Park Weight Check
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned char tempD = 0x00;
while(1) {
    tempD = 0;
    tempD = PINA + PINB + PINC;
	tempD = tempD >> 2;
	if (PINA+PINB+PINC >= 141) {
		tempD = tempD | 0x01;
	}
	else {
		tempD = tempD & 0xFE;
	}
	if (PINA - PINC > 80 || PINC - PINA > 80) {
		tempD = tempD | 0x02;
	}
	else {
		tempD = tempD & 0xFD;
	}
    PORTD = tempD;
}
	return 0;

}

