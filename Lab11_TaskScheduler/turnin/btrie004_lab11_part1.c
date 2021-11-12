/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: Task Scheduler Keypad
 *	Demo Link: N/A
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
#include "io.h"
#include "keypad.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

const unsigned long timerPeriod = 1; //in milliseconds

typedef struct task {
   int state;                  // Task's current state
   unsigned long period;       // Task period
   unsigned long elapsedTime;  // Time elapsed since last task tick
   int (*TickFct)(int);        // Task tick function
} task;

task tasks[1];
const unsigned short tasksNum = 1;

void TimerISR() {
	unsigned char i;
	for (i = 0; i < tasksNum; ++i) {
		if (tasks[i].elapsedTime >= tasks[i].period) {
			tasks[i].state = tasks[i].TickFct(tasks[i].state);
			tasks[i].elapsedTime = 0;
		}
		tasks[i].elapsedTime += timerPeriod;
	}
}

unsigned char keypad;

enum KeypadSM_States {KPSM_Start, KPSM_Run};
int TickFct_KeypadSM(int state){
	switch(state) {
		case KPSM_Start:
			state = KPSM_Run;
			keypad = '\0';
			break;
		case KPSM_Run:
			state = KPSM_Run;
			break;
	}
	switch(state) {
		case KPSM_Start:
			break;
		case KPSM_Run:
			keypad = GetKeypadKey();
			break;
	}
	return state;
}

int main(void) {
    	/* Insert DDR and PORT initializations */
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;
	/* Insert your solution below */
	unsigned char i = 0;
	tasks[i].state = KPSM_Start;
	tasks[i].period = 500;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_KeypadSM;
	
	LCD_init();
	TimerSet(timerPeriod);
	TimerOn();
    	while (1) { }
    	return 1;
}
