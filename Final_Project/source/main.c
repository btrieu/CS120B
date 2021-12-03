#include <avr/io.h>
#include "SPI.h"
#include "LCD.h"
#include "bitmaps.h"
#include "joystick.h"
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

unsigned char spawnOrder[10] = {4,2,3,1,3,4,1,2,1,3};

unsigned char tempBitmap[504];

unsigned char* combineBitmap(unsigned char* a, unsigned char* b) {
	for (unsigned short i = 0; i < 504; ++i) {
		tempBitmap[i] = a[i] | b[i];
	}
	return tempBitmap;
}
unsigned char charPos[504];
unsigned char currPos = 0;

enum PlayerPositionSM_States {PPSM_Start, PPSM_Wait, PPSM_DL, PPSM_DR, PPSM_UL, PPSM_UR};
int TickFct_PlayerPositionSM(int state){
	unsigned short i;
	switch(state) {
		case PPSM_Start:
			currPos = 0x01;
			state = PPSM_Wait;
			break;
		case PPSM_Wait:
			if (currPos == 1) {
				if ((Joystick_poll() & 0x0F) == 0x01) { state = PPSM_UL; }
				else if ((Joystick_poll() & 0x0F) == 0x02) { state = PPSM_UR; }
				else if ((Joystick_poll() & 0x0F) == 0x03) { state = PPSM_DR; }
				else if ((Joystick_poll() & 0x0F) == 0x04) { state = PPSM_DR; }
				else if ((Joystick_poll() & 0x0F) == 0x08) { state = PPSM_UL; }
			}
			else if (currPos == 2) {
				if ((Joystick_poll() & 0x0F) == 0x01) { state = PPSM_UR; }
				else if ((Joystick_poll() & 0x0F) == 0x02) { state = PPSM_UR; }
				else if ((Joystick_poll() & 0x0F) == 0x06) { state = PPSM_DL; }
				else if ((Joystick_poll() & 0x0F) == 0x07) { state = PPSM_DL; }
				else if ((Joystick_poll() & 0x0F) == 0x08) { state = PPSM_UR; }
			}
			else if (currPos == 3) {
				if ((Joystick_poll() & 0x0F) == 0x04) { state = PPSM_DR; }
				else if ((Joystick_poll() & 0x0F) == 0x05) { state = PPSM_DR; }
				else if ((Joystick_poll() & 0x0F) == 0x06) { state = PPSM_DL; }
				else if ((Joystick_poll() & 0x0F) == 0x07) { state = PPSM_UL; }
				else if ((Joystick_poll() & 0x0F) == 0x08) { state = PPSM_UL; }
			}
			else if (currPos == 4) {
				if ((Joystick_poll() & 0x0F) == 0x02) { state = PPSM_UR; }
				else if ((Joystick_poll() & 0x0F) == 0x03) { state = PPSM_UR; }
				else if ((Joystick_poll() & 0x0F) == 0x04) { state = PPSM_DR; }
				else if ((Joystick_poll() & 0x0F) == 0x05) { state = PPSM_DL; }
				else if ((Joystick_poll() & 0x0F) == 0x06) { state = PPSM_DL; }
			}
			else { PORTC = 0x01; }
			break;
		case PPSM_DL:
			state = PPSM_Wait;
			break;
		case PPSM_DR:
			state = PPSM_Wait;
			break;
		case PPSM_UL:
			state = PPSM_Wait;
			break;
		case PPSM_UR:
			state = PPSM_Wait;
			break;
	}
	switch(state) {
		case PPSM_Start:
			break;
		case PPSM_Wait:
			if (currPos == 1) {
				for (i = 0; i < 504; ++i) {
					charPos[i] = downleft_1p[i];
				}
			}
			else if (currPos == 2) {
				for (i = 0; i < 504; ++i) {
					charPos[i] = downright_1p[i];
				}
			}
			else if (currPos == 3) {
				for (i = 0; i < 504; ++i) {
					charPos[i] = upright_1p[i];
				}
			}
			else if (currPos == 4) {
				for (i = 0; i < 504; ++i) {
					charPos[i] = upleft_1p[i];
				}
			}
			break;
		case PPSM_DL:
			currPos = 1;
			break;
		case PPSM_DR:
			currPos = 2;
			break;
		case PPSM_UL:
			currPos = 4;
			break;
		case PPSM_UR:
			currPos = 3;
			break;
	}
	return state;
}

unsigned char slimes[504];
unsigned char spawnIndex = 0;
unsigned char spawnDelay = 0;
unsigned char maxSlimes = 2;
unsigned char currSlimes = 0;
unsigned char slimeArr[504][2];

enum SlimeSpawnSM_States {SSSM_Start, SSSM_Run};
int TickFct_SlimeSpawnSM(int state){
	switch(state) {
		case SSSM_Start:
			state = SSSM_Run;
			break;
		case SSSM_Run:
			state = SSSM_Run;
			break;
	}
	switch(state) {
		case SSSM_Start:
			break;
		case SSSM_Run:
			if (spawnIndex > 9) { spawnIndex = 0; }
			if (spawnDelay > 5) { spawnDelay = 0; }
			if (currSlimes < maxSlimes && spawnDelay == 5) {
				++currSlimes;
				if (spawnOrder[spawnIndex] == 1) {
					for (unsigned short i = 0; i < 504; ++i) {
						slimeArr[i][currSlimes-1] = lslime[i];
					}
				}
				else if (spawnOrder[spawnIndex] == 2) {
					for (unsigned short i = 0; i < 504; ++i) {
						slimeArr[i][currSlimes-1] = rslime[i];
					}
				}
				else if (spawnOrder[spawnIndex] == 3) {
					unsigned short k = 63;
					for (unsigned short i = 399; i < 504; ++i) {
						slimeArr[k][currSlimes-1] = rslime[i];
						++k;
					}
					for (k = k; k < 504; ++k) {
						slimeArr[k][currSlimes-1] = 0x00;
					}
					for (unsigned char j = 0; j < 64; ++j) {
						slimeArr[j][currSlimes-1] = 0x00;
					}
				}
				else if (spawnOrder[spawnIndex] == 4) {
					unsigned short k = 0;
					for (unsigned short i = 336; i < 441; ++i) {
						slimeArr[k][currSlimes-1] = lslime[i];
						++k;
					}
					for (k = k; k < 504; ++k) {
						slimeArr[k][currSlimes-1] = 0x00;
					}
				}
				++spawnIndex;
			}
			for (unsigned short z = 0; z < 504; ++z) {
				slimes[z] = slimeArr[z][0] | slimeArr[z][1];
			}
			++spawnDelay;
			break;
	}
	return state;
}
unsigned char ctr = 0;

enum DisplaySM_States {DSM_Start, DSM_Run};
int TickFct_DisplaySM(int state){
	switch(state) {
		case DSM_Start:
			state = DSM_Run;
			break;
		case DSM_Run:
			state = DSM_Run;
			break;
	}
	switch(state) {
		case DSM_Start:
			break;
		case DSM_Run:
			LCD_printBitmap(combineBitmap(charPos, slimes));
			break;
	}
	return state;
}


int main(void) {
    	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xBF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	unsigned char i = 0;
	tasks[i].state = PPSM_Start;
	tasks[i].period = 100;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_PlayerPositionSM;
	++i;
	tasks[i].state = SSSM_Start;
	tasks[i].period = 1000;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_SlimeSpawnSM;
	++i;
	tasks[i].state = DSM_Start;
	tasks[i].period = 500;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_DisplaySM;
	++i;
	TimerSet(timerPeriod);
	TimerOn();
	SPI_init_master();
	LCD_init();
	Joystick_init();
    	while (1) { }
    	return 1;
}

