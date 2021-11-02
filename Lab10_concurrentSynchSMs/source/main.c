/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: Concurrent SM Light Show 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

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

enum ThreeLEDsSM_States {TLSM_Start};
int TickFct_ThreeLEDsSM(int state){

}

enum BlinkingLEDSM_States {BLSM_Start};
int TickFct_BlinkingLEDSM(int state){

}

enum CombineLEDsSM_States {CLSM_Start};
int TickFct_CombineLEDsSM(int state){

}



int main(void) {
    	/* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	/* Insert your solution below */
	unsigned char i = 0;
	tasks[i].state = TLSM_Start;
	tasks[i].period = 1000;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_ThreeLEDsSM;
	++i;
	tasks[i].state = BLSM_Start;
	tasks[i].period = 1000;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_BlinkingLEDSM;
	++i;
	tasks[i].state = CLSM_Start;
	tasks[i].period = 1000;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_CombineLEDsSM;
	
	TimerSet(1000);
	TimerOn();
    	while (1) { }
    	return 1;
}
