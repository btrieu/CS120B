/*	Author: Brandon Trieu btrie004@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #2  Exercise #3
 *	Exercise Description: Exercise #2 Expansion
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
	unsigned char tempA = 0x00;
while(1) {
	unsigned char cntavail = 4;
	tempA = (PINA & 0x0F);
	if ((tempA & 0x01) == 0x01) {
		cntavail--;
	}
       	if ((tempA & 0x02) == 0x02) {
		cntavail--;
	}
	if ((tempA & 0x04) == 0x04) {
		cntavail--;
	}
	if ((tempA & 0x08) == 0x08) {
		cntavail--;
	}		
	PORTC = PINC | cntavail;
	if (cntavail == 0) {
		PORTC = PINC | 0x80;
	}
}
	return 0;

}

