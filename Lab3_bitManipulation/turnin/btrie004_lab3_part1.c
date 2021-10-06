/*	Author: Brandon Trieu btrie004@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #3  Exercise #1
 *	Exercise Description: Count the number of 1s on ports A and B and output that number on port C.
 *
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
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char counter = 0x00;
	unsigned char tempA = 0x00;
	unsigned char tempB = 0x00;
	unsigned int i;
    /* Insert your solution below */
    while (1) {
	tempA = PINA;
	tempB = PINB;
	counter = 0;
	for (i = 0; i < 8; ++i) {
		if (tempA & 0x01) {
			counter++;
		}
		if (tempB & 0x01) {
			counter++;
		}
		tempA = tempA >> 1;
		tempB = tempB >> 1;
	}
	PORTC = counter;
    }
    return 1;
}
