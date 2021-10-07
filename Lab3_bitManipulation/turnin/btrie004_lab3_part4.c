/*	Author: Brandon Trieu btrie004@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #3  Exercise #4
 *	Exercise Description: Mapping lower nibbles to upper nibbles and vice versa
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
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char upperA = 0x00;
	unsigned char lowerA = 0x00;
    /* Insert your solution below */
    while (1) {
   	PORTB = 0;
	PORTC = 0;
	upperA = PINA & 0xF0;
	lowerA = PINA & 0x0F;
	PORTB = (upperA >> 4) | PINB;
	PORTC = (lowerA << 4) | PINC;
    }
    return 1;
}
