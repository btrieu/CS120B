/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description: Concurrent SM Light Show 
 *	Demo Link: https://drive.google.com/file/d/19D2psZZ-BUntDuz_STfLbTIgzJZvPI_w/view?usp=sharing
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
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

unsigned char threeLEDs = 0;
unsigned char BlinkingLEDSM = 0;

enum ThreeLEDsSM_States {TLSM_Start, TLSM_Run};
int TickFct_ThreeLEDsSM(int state){
	switch(state) {
		case TLSM_Start:
			state = TLSM_Run;
			threeLEDs = 0;
			break;
		case TLSM_Run:
			state = TLSM_Run;
			break;
	}
	switch(state) {
		case TLSM_Start:
			break;
		case TLSM_Run:
			if (threeLEDs == 0x00) { threeLEDs = 0x01; }
			else if (threeLEDs == 0x01) { threeLEDs = 0x02; }
			else if (threeLEDs == 0x02) { threeLEDs = 0x04; }
			else if (threeLEDs == 0x04) { threeLEDs = 0x01; }
			break;
	}
	return state;
}

enum BlinkingLEDSM_States {BLSM_Start, BLSM_Run};
int TickFct_BlinkingLEDSM(int state){
	switch(state) {
		case BLSM_Start:
			state = BLSM_Run;
			BlinkingLEDSM = 0;
			break;
		case BLSM_Run:
			state = BLSM_Run;
			break;
	}
	switch(state) {
		case BLSM_Start:
			break;
		case BLSM_Run:
			if (BlinkingLEDSM == 0x00) { BlinkingLEDSM = 0x08; }
			else if (BlinkingLEDSM == 0x08) { BlinkingLEDSM = 0x00; }
			break;
	}
	return state;
}

enum CombineLEDsSM_States {CLSM_Start, CLSM_Run};
int TickFct_CombineLEDsSM(int state){
switch(state) {
                case CLSM_Start:
                        state = CLSM_Run;
                        break;
                case CLSM_Run:
                        state = CLSM_Run;
                        break;
        }
        switch(state) {
                case CLSM_Start:
                        break;
                case CLSM_Run:
                        PORTB = BlinkingLEDSM | threeLEDs;
			break;
        }
        return state;
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
	
	TimerSet(timerPeriod);
	TimerOn();
    	while (1) { }
    	return 1;
}
