/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #11  Exercise #5
 *	Exercise Description: Obstacle Dodging Game
 *	Demo Link: https://drive.google.com/file/d/1CrKSDeyRUjSw2VD24bkcCGMHZS4wU7lf/view?usp=sharing
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

task tasks[3];
const unsigned short tasksNum = 3;

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

unsigned char button = 0x00;
unsigned char pause = 0x00;
unsigned char cursorPos;
unsigned char lose = 0x00;

enum ReadInputSM_States {RISM_Start, RISM_Wait, RISM_Up, RISM_Down, RISM_Pause, RISM_Pause1, RISM_Pause2, RISM_Release};
int TickFct_ReadInputSM(int state){
	switch(state) {
		case RISM_Start:
			state = RISM_Wait;
			button = 0x00;
			LCD_Cursor(2);
			cursorPos = 2;
			break;
		case RISM_Wait:
			if (lose == 0x01) { state = RISM_Wait; }
			else if ((~PINC & 0x07) == 0x01) { state = RISM_Pause; }
			else if ((~PINC & 0x07) == 0x02) { state = RISM_Up; }
			else if ((~PINC & 0x07) == 0x04) { state = RISM_Down; }
			else { state = RISM_Wait; }
			break;
		case RISM_Up:
			state = RISM_Release;
			break;
		case RISM_Down:
			state = RISM_Release;
			break;
		case RISM_Pause:
			state = RISM_Pause1;
			break;
		case RISM_Pause1:
			if ((~PINC & 0x01) == 0x01) { state = RISM_Pause1; }
			else { state = RISM_Pause2; }
			break;
		case RISM_Pause2:
			if ((~PINC & 0x01) == 0x01) { 
				pause = 0x00;
				LCD_ClearScreen();
				LCD_Cursor(cursorPos);
				state = RISM_Release;
		       	}
			else { state = RISM_Pause2; }
			break;
		case RISM_Release:
			if ((~PINC & 0x07) == 0x00) { state = RISM_Wait; }
			else { state = RISM_Release; }
			break;
	}
	switch(state) {
		case RISM_Start:
			break;
		case RISM_Wait:
			button = 0x00;
			break;
		case RISM_Up:
			LCD_Cursor(2);
			cursorPos = 2;
			break;
		case RISM_Down:
			LCD_Cursor(18);
			cursorPos = 18;
			break;
		case RISM_Pause:
			pause = 0x01;
			if (lose != 0x01) {
				LCD_DisplayString(1, "Paused!");
			}
			break;
		case RISM_Pause1:
			break;
		case RISM_Pause2:
			break;
		case RISM_Release:
			break;
	}
	return state;
}

unsigned char k = 16;
unsigned char j = 32;
unsigned char l = 0;
unsigned char obstaclePos[2];

enum ObstacleSM_States {OSM_Start, OSM_Run, OSM_Lose, OSM_Lose1, OSM_Pause};
int TickFct_ObstacleSM(int state){
	switch(state) {
		case OSM_Start:
			state = OSM_Run;
			break;
		case OSM_Run:
			if (lose == 0x01) { state = OSM_Lose; }
			else if (pause == 0x01) { state = OSM_Pause; }
			else { state = OSM_Run; }
			break;
		case OSM_Lose:
			state = OSM_Lose1;
			break;
		case OSM_Lose1:
			if ((~PINC & 0x01) == 0x01) {
			       	lose = 0x00;
				k = 16;
				j = 32;
				l = 0;
				pause = 0x00;	
			       	state = OSM_Run;
		       	}
			else { state = OSM_Lose1; }
			break;
		case OSM_Pause:
			if (pause == 0x00) { state = OSM_Run; }
			else { state = OSM_Pause; }
			break;
	}
	switch(state) {
		case OSM_Start:
			break;
		case OSM_Run:
			if (k == 0) { 
				++l;
				if (l == 6) {
					k = 16;
					l = 0;
				}
		       	}
			if (j == 11) { j = 32; }
			LCD_ClearScreen();	
			if (k > 0) {
				LCD_Cursor(k);
				if (k != 0) {
					LCD_WriteData('#');
					obstaclePos[0] = k;
				}
				LCD_Cursor(cursorPos);
				--k;
			}
			if (j > 11) {
				LCD_Cursor(j+5);
				LCD_WriteData('#');
				obstaclePos[1] = j+5;
				LCD_Cursor(cursorPos);
				--j;
			}
			break;
		case OSM_Lose:
			LCD_DisplayString(1, "You Lose!");
			break;
		case OSM_Pause:
			break;
	}
	return state;
}


enum CollisonSM_States {CSM_Start, CSM_Run};
int TickFct_CollisionSM(int state){
	switch(state) {
		case CSM_Start:
			state = CSM_Run;
			break;
		case CSM_Run:
			state = CSM_Run;
			break;
	}
	switch(state) {
		case CSM_Start:
			break;
		case CSM_Run:
			if ((cursorPos == obstaclePos[0]) || (cursorPos == obstaclePos[1])) {
				lose = 0x01;
			}
			else { lose = 0x00; }
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
	tasks[i].state = RISM_Start;
	tasks[i].period = 100;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_ReadInputSM;
	++i;
	tasks[i].state = OSM_Start;
	tasks[i].period = 200;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_ObstacleSM;
	++i;
	tasks[i].state = CSM_Start;
	tasks[i].period = 1;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_CollisionSM;

	LCD_init();
	TimerSet(timerPeriod);
	TimerOn();
    	while (1) { }
    	return 1;
}
