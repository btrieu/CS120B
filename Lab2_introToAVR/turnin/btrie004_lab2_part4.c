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
	unsigned char greaterThan140 = 0x00;
	unsigned char balanced = 0x00;
	tempD = PINA + PINB + PINC;
	if (tempD >= 140) {
		greaterThan140 = 0x01;
	}
	if (PINA - PINC <= 80 || PINC - PINA <= 80) {
		balanced = 0x02;
	}
	tempD = tempD | greaterThan140;
	tempD = tempD | balanced;
}
	return 0;

}

