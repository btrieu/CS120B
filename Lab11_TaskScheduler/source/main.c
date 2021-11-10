/*	Author: Brandon Trieu btrie004@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #7  Exercise #1
 *	Exercise Description: PA0 increments up to 9, PA1 decrements down to 0 (start at 7), output on C. Now with timers!! And with an LCD screen!!
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
#include "io.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char ctr = 0;

void Tick() {	
	if (ctr == 0){
		LCD_DisplayString(1,"lid?");
		++ctr;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	TimerSet(1);
	TimerOn();
	LCD_init();
	
    while (1) {
    	Tick();
	while (!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
