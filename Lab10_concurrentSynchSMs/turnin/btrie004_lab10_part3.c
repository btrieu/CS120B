/*	Author: Brandon Trieu (btrie004)
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #10  Exercise #3
 *	Exercise Description: Concurrent SM Light Show with Switch-Operated Speaker
 *
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

task tasks[4];
const unsigned short tasksNum = 4;

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

unsigned char threeLEDs = 0x00;
unsigned char BlinkingLEDSM = 0x00;
unsigned char speaker = 0x00;

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

enum SpeakerSM_States {SPSM_Start, SPSM_Off, SPSM_On};
int TickFct_SpeakerSM(int state){
	switch(state) {
                case SPSM_Start:
                        state = SPSM_Off;
                        break;
                case SPSM_Off:
                        if (((PINA & 0x04) == 0x04)) { state = SPSM_On; }
			else { state = SPSM_Off; } 
                        break;
		case SPSM_On:
                        if (((PINA & 0x04) == 0x04)) { state = SPSM_On; }
			else { state = SPSM_Off; } 
                        break;
        }
        switch(state) {
                case SPSM_Start:
                        break;
                case SPSM_Off:
			speaker = 0x00;
			break;
		case SPSM_On:
			if (speaker == 0x00) { speaker = 0x10; }
			else if (speaker == 0x10) { speaker = 0x00; }
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
                        PORTB = BlinkingLEDSM | threeLEDs | speaker;
			break;
        }
        return state;
}

int main(void) {
    	/* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	/* Insert your solution below */
	unsigned char i = 0;
	tasks[i].state = TLSM_Start;
	tasks[i].period = 1000;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_ThreeLEDsSM;
	++i;
	tasks[i].state = BLSM_Start;
	tasks[i].period = 300;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_BlinkingLEDSM;
	++i;
	tasks[i].state = SPSM_Start;
	tasks[i].period = 2;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_SpeakerSM;
	++i;
	tasks[i].state = CLSM_Start;
	tasks[i].period = 1;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_CombineLEDsSM;
	
	TimerSet(timerPeriod);
	TimerOn();
    	while (1) { }
    	return 1;
}
