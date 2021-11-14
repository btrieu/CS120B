/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #11  Exercise #4
 *	Exercise Description: Display Button Press Over Original Message
 *	Demo Link: https://drive.google.com/file/d/1Csya3BG8xbkAJm_CLLxUDsv7uQH0XfhH/view?usp=sharing
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

task tasks[2];
const unsigned short tasksNum = 2;

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
const unsigned char string[16] = "Congratulations!";
const unsigned char* stringptr = string;
unsigned char i = 0;


enum DisplayCharSM_States {DCSM_Start, DCSM_Run};
int TickFct_DisplayCharSM(int state){
	switch(state) {
		case DCSM_Start:
			state = DCSM_Run;
			LCD_DisplayString(1,stringptr);
			LCD_Cursor(1);
			break;
		case DCSM_Run:
			state = DCSM_Run;
			break;
	}
	switch(state) {
		case DCSM_Start:
			break;
		case DCSM_Run:
			if (i == 33) { LCD_Cursor(1); i = 0; }
			else if (i == 16) { LCD_Cursor(17); }
			if (keypad != '\0') {
				LCD_WriteData(keypad);
				++i;
			}
	}
	return state;
}

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
	tasks[i].period = 300;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_KeypadSM;
	++i;
	tasks[i].state = DCSM_Start;
	tasks[i].period = 300;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_DisplayCharSM;

	LCD_init();
	TimerSet(timerPeriod);
	TimerOn();
    	while (1) { }
    	return 1;
}
