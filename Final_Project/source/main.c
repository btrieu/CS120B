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
unsigned char slimeLoc[2];
unsigned char slimeMovesLeft[2];

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
				slimeMovesLeft[currSlimes-1] = 26;
				if (spawnOrder[spawnIndex] == 1) {
					for (unsigned short i = 0; i < 504; ++i) {
						slimeArr[i][currSlimes-1] = lslime[i];
					}
					slimeLoc[currSlimes-1] = 1;
				}
				else if (spawnOrder[spawnIndex] == 2) {
					for (unsigned short i = 0; i < 504; ++i) {
						slimeArr[i][currSlimes-1] = rslime[i];
					}
					slimeLoc[currSlimes-1] = 2;
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
					slimeLoc[currSlimes-1] = 3;
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
					slimeLoc[currSlimes-1] = 4;
				}
				++spawnIndex;
			}
			for (unsigned char y = 0; y < currSlimes; ++y) {
				if (slimeLoc[y] == 1 || slimeLoc[y] == 4) {
					for (unsigned short x = 503; x > 0; --x) {
						slimeArr[x][y] = slimeArr[x-1][y];
					}
				}
				else if (slimeLoc[y] == 2 || slimeLoc[y] == 3) {
					for (unsigned short x = 0; x < 504; ++x) {
						slimeArr[x][y] = slimeArr[x+1][y];
					}
				}
				--slimeMovesLeft[y];
			}
			if (slimeMovesLeft[0] == 0 && currSlimes > 0) {
				slimeMovesLeft[0] = slimeMovesLeft[1];
				currSlimes--;
				slimeLoc[0] = slimeLoc[1];
				for (unsigned short w = 0; w < 504; ++w) {
					slimeArr[w][0] = slimeArr[w][1];
					slimeArr[w][1] = 0x00;
				}
			}
			for (unsigned short z = 0; z < 504; ++z) {
				slimes[z] = slimeArr[z][0] | slimeArr[z][1];
			}
			++spawnDelay;
			break;
	}
	return state;
}

unsigned short attackDelay = 0;
unsigned char attacks[504];
unsigned char maxAttacks = 5;
unsigned char currAttacks = 0;
unsigned char attackArr[504][5];

enum AttackSM_States {ASM_Start, ASM_Wait, ASM_Press, ASM_WaitRelease};
int TickFct_AttackSM(int state){
	switch(state) {
		case ASM_Start:
			state = ASM_Wait;
			break;
		case ASM_Wait:
			if ((~PINA & 0x04) == 0x04) { state = ASM_Press; }
			else { state = ASM_Wait; }
			break;
		case ASM_Press:
			state = ASM_WaitRelease;
			break;
		case ASM_WaitRelease:
			if ((~PINA & 0x04) == 0x04 && attackDelay > 10) { 
				attackDelay = 0;
				state = ASM_WaitRelease;
		       	}
			else { state = ASM_Wait; }
			++attackDelay;
			break;
	}
	switch(state) {
		case ASM_Start:
			break;
		case ASM_Wait:
			break;
		case ASM_Press:
			if (currAttacks < maxAttacks) {
				++currAttacks;
				if (currPos == 4) {
					unsigned short k = 115;
					for (unsigned short i = 115; i <= 122; ++i) {
						attackArr[k][currAttacks-1] = attack[i];
						++k;
					}
					for (k = k; k < 504; ++k) {
						attackArr[k][currAttacks-1] = 0x00;
					}
					for (unsigned short j = 0; j < 115; ++j) {
						attackArr[j][currAttacks-1] = 0x00;
					}
				}
				if (currPos == 3) {
					unsigned short k = 131;
					for (unsigned short i = 131; i <= 138; ++i) {
						attackArr[k][currAttacks-1] = attack[i];
						++k;
					}
					for (k = k; k < 504; ++k) {
						attackArr[k][currAttacks-1] = 0x00;
					}
					for (unsigned short j = 0; j < 131; ++j) {
						attackArr[j][currAttacks-1] = 0x00;
					}
				}
				if (currPos == 1) {
					unsigned short k = 367;
					for (unsigned short i = 367; i <= 374; ++i) {
						attackArr[k][currAttacks-1] = attack[i];
						++k;
					}
					for (k = k; k < 504; ++k) {
						attackArr[k][currAttacks-1] = 0x00;
					}
					for (unsigned short j = 0; j < 367; ++j) {
						attackArr[j][currAttacks-1] = 0x00;
					}
				}
				if (currPos == 2) {
					unsigned short k = 383;
					for (unsigned short i = 383; i <= 390; ++i) {
						attackArr[k][currAttacks-1] = attack[i];
						++k;
					}
					for (k = k; k < 504; ++k) {
						attackArr[k][currAttacks-1] = 0x00;
					}
					for (unsigned short j = 0; j < 383; ++j) {
						attackArr[j][currAttacks-1] = 0x00;
					}
				}
			}
			for (unsigned short z = 0; z < 504; ++z) {
				attacks[z] = attackArr[z][0] | attackArr[z][1] | attackArr[z][2] | attackArr[z][3] | attackArr[z][4];
			}
			break;
		case ASM_WaitRelease:
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
			LCD_printBitmap(combineBitmap(combineBitmap(charPos, slimes),attacks));
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
	tasks[i].period = 500;//1000;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_SlimeSpawnSM;
	++i;
	tasks[i].state = ASM_Start;
	tasks[i].period = 50;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_AttackSM;
	++i;	
	tasks[i].state = DSM_Start;
	tasks[i].period = 100;
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

