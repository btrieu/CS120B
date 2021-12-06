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

task tasks[6];
const unsigned short tasksNum = 6;

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
unsigned char gameover = 0;
unsigned char hit = 0;
unsigned char score = 0;
unsigned char multiplayer;

unsigned char* combineBitmap(unsigned char* a, unsigned char* b) {
	for (unsigned short i = 0; i < 504; ++i) {
		tempBitmap[i] = a[i] | b[i];
	}
	return tempBitmap;
}
unsigned char charPos[504];
unsigned char currPos = 0;

unsigned char js1;
unsigned char js2;

enum PlayerPositionSM_States {PPSM_Start, PPSM_Wait, PPSM_DL, PPSM_DR, PPSM_UL, PPSM_UR, PPSM_2p1, PPSM_2p2, PPSM_2p3, PPSM_2p4};
int TickFct_PlayerPositionSM(int state){
	unsigned short i;
	switch(state) {
		case PPSM_Start:
			currPos = 0x01;
			state = PPSM_Wait;
			break;
		case PPSM_Wait:
			js1 = Joystick_poll() & 0x0F;
			js2 = Joystick2_poll() & 0x0F;
			if (currPos == 1 && multiplayer == 0x00) {
				if (js1 == 0x01) { state = PPSM_UL; }
				else if (js1 == 0x02) { state = PPSM_UR; }
				else if (js1 == 0x03) { state = PPSM_DR; }
				else if (js1 == 0x04) { state = PPSM_DR; }
				else if (js1 == 0x08) { state = PPSM_UL; }
			}
			else if (currPos == 2 && multiplayer == 0x00) {
				if (js1 == 0x01) { state = PPSM_UR; }
				else if (js1 == 0x02) { state = PPSM_UR; }
				else if (js1 == 0x06) { state = PPSM_DL; }
				else if (js1 == 0x07) { state = PPSM_DL; }
				else if (js1 == 0x08) { state = PPSM_UR; }
			}
			else if (currPos == 3 && multiplayer == 0x00) {
				if (js1 == 0x04) { state = PPSM_DR; }
				else if (js1 == 0x05) { state = PPSM_DR; }
				else if (js1 == 0x06) { state = PPSM_DL; }
				else if (js1 == 0x07) { state = PPSM_UL; }
				else if (js1 == 0x08) { state = PPSM_UL; }
			}
			else if (currPos == 4 && multiplayer == 0x00) {
				if (js1 == 0x02) { state = PPSM_UR; }
				else if (js1 == 0x03) { state = PPSM_UR; }
				else if (js1 == 0x04) { state = PPSM_DR; }
				else if (js1 == 0x05) { state = PPSM_DL; }
				else if (js1 == 0x06) { state = PPSM_DL; }
			}
			else if (currPos == 1 && multiplayer == 0x01) {
				if ((js2 > 0x01 && js2 < 0x05)
				&& !(js1 > 0x01 && js1 < 0x05)) { state = PPSM_2p4; }
				else if ((js1 > 0x01 && js1 < 0x05)
				&& !(js2 > 0x01 && js2 < 0x05)) { state = PPSM_2p3; }
				else if ((js1 > 0x01 && js1 < 0x05)
				&& (js2 > 0x01 && js2 < 0x05)) { state = PPSM_2p2; }
			}
			else if (currPos == 2 && multiplayer == 0x01) {
				if ((js2 > 0x05 && js2 < 0x09)
				&& !(js1 > 0x05 && js1 < 0x09)) { state = PPSM_2p3; }
				else if ((js1 > 0x05 && js1 < 0x09)
				&& !(js2 > 0x05 && js2 < 0x09)) { state = PPSM_2p4; }
				else if ((js1 > 0x05 && js1 < 0x09)
				&& (js2 > 0x05 && js2 < 0x09)) { state = PPSM_2p1; }
			}
			else if (currPos == 3 && multiplayer == 0x01) {
				if ((js2 > 0x01 && js2 < 0x05)
				&& !(js1 > 0x01 && js1 < 0x05)) { state = PPSM_2p2; }
				else if ((js1 > 0x05 && js1 < 0x09)
				&& !(js2 > 0x05 && js2 < 0x09)) { state = PPSM_2p1; }
				else if ((js1 > 0x05 && js1 < 0x09)
				&& (js2 > 0x01 && js2 < 0x05)) { state = PPSM_2p4; }
			}
			else if (currPos == 4 && multiplayer == 0x01) {
				if ((js2 > 0x05 && js2 < 0x09)
				&& !(js1 > 0x05 && js1 < 0x09)) { state = PPSM_2p1; }
				else if ((js1 > 0x01 && js1 < 0x05)
				&& !(js2 > 0x01 && js2 < 0x05)) { state = PPSM_2p2; }
				else if ((js1 > 0x01 && js1 < 0x05)
				&& (js2 > 0x05 && js2 < 0x09)) { state = PPSM_2p3; }
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
		case PPSM_2p1:
			state = PPSM_Wait;
			break;
		case PPSM_2p2:
			state = PPSM_Wait;
			break;
		case PPSM_2p3:
			state = PPSM_Wait;
			break;
		case PPSM_2p4:
			state = PPSM_Wait;
			break;
	}
	switch(state) {
		case PPSM_Start:
			break;
		case PPSM_Wait:
			if (currPos == 1 && multiplayer == 0x00) {
				for (i = 0; i < 504; ++i) {
					charPos[i] = downleft_1p[i];
				}
			}
			else if (currPos == 2 && multiplayer == 0x00) {
				for (i = 0; i < 504; ++i) {
					charPos[i] = downright_1p[i];
				}
			}
			else if (currPos == 3 && multiplayer == 0x00) {
				for (i = 0; i < 504; ++i) {
					charPos[i] = upright_1p[i];
				}
			}
			else if (currPos == 4 && multiplayer == 0x00) {
				for (i = 0; i < 504; ++i) {
					charPos[i] = upleft_1p[i];
				}
			}
			else if (currPos == 1 && multiplayer == 0x01) {
				for (i = 0; i < 504; ++i) {
					charPos[i] = downleft_1p[i] | upleft_1p[i] | twoplayer[i];
				}
			}
			else if (currPos == 2 && multiplayer == 0x01) {
				for (i = 0; i < 504; ++i) {
					charPos[i] = downright_1p[i] | upright_1p[i] | twoplayer[i];
				}
			}
			else if (currPos == 3 && multiplayer == 0x01) {
				for (i = 0; i < 504; ++i) {
					charPos[i] = upright_1p[i] | downleft_1p[i] | twoplayer[i];
				}
			}
			else if (currPos == 4 && multiplayer == 0x01) {
				for (i = 0; i < 504; ++i) {
					charPos[i] = upleft_1p[i] | downright_1p[i] | twoplayer[i];
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
		case PPSM_2p1:
			currPos = 1;
			break;
		case PPSM_2p2:
			currPos = 2;
			break;
		case PPSM_2p3:
			currPos = 3;
			break;
		case PPSM_2p4:
			currPos = 4;
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
						slimeArr[0][y] = 0x00;
					}
				}
				else if (slimeLoc[y] == 2 || slimeLoc[y] == 3) {
					for (unsigned short x = 0; x < 504; ++x) {
						slimeArr[x][y] = slimeArr[x+1][y];
						slimeArr[503][y] = 0x00;
					}
				}
				--slimeMovesLeft[y];
			}
			if (slimeMovesLeft[0] == 0 && currSlimes > 0) {
				if (gameover == 0 && score > 0) {
					--score;
				}
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
unsigned char arrowLoc[5];
unsigned char arrowMovesLeft[5];

unsigned char jsPressed;

enum AttackSM_States {ASM_Start, ASM_Wait, ASM_Press, ASM_Press1, ASM_WaitRelease};
int TickFct_AttackSM(int state){
	switch(state) {
		case ASM_Start:
			state = ASM_Wait;
			break;
		case ASM_Wait:
			if ((~PINA & 0x04) == 0x04 && currAttacks < 5 && multiplayer == 0) { state = ASM_Press; }
			else if ((~PINA & 0x20) == 0x20 && currAttacks < 5 && multiplayer == 1) { jsPressed = 2; state = ASM_Press1; }
			else if ((~PINA & 0x04) == 0x04 && currAttacks < 5 && multiplayer == 1) { jsPressed = 1; state = ASM_Press1; }
			else { state = ASM_Wait; }
			break;
		case ASM_Press:
			state = ASM_WaitRelease;
			break;
		case ASM_Press1:
			state = ASM_WaitRelease;
			break;
		case ASM_WaitRelease:
			if (((~PINA & 0x04) == 0x04 || (~PINA & 0x20) == 0x20) && attackDelay > 10) { 
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
			for (unsigned char y = 0; y < currAttacks; ++y) {
				if (arrowLoc[y] == 2 || arrowLoc[y] == 3) {
					for (unsigned short x = 503; x > 0; --x) {
						attackArr[x][y] = attackArr[x-1][y];
						attackArr[0][y] = 0x00;
					}
				}
				else if (arrowLoc[y] == 1 || arrowLoc[y] == 4) {
					for (unsigned short x = 0; x < 504; ++x) {
						attackArr[x][y] = attackArr[x+1][y];
						attackArr[503][y] = 0x00;
					}
				}
				--arrowMovesLeft[y];
			}
			if (arrowMovesLeft[0] == 0 && currAttacks > 0) {
				for (unsigned char v = 1; v <= 4; ++v) {
					arrowMovesLeft[v-1] = arrowMovesLeft[v];
					arrowLoc[v-1] = arrowLoc[v];
					for (unsigned short w = 0; w < 504; ++w) {
						attackArr[w][v-1] = attackArr[w][v];
					}
				}
				for (unsigned char u = currAttacks; u < maxAttacks; ++u) {
					for (unsigned short t = 0; t < 504; ++t) {
						attackArr[t][u] = 0x00;
					}
				}
				currAttacks--;
			}
			for (unsigned short z = 0; z < 504; ++z) {
				attacks[z] = attackArr[z][0] | attackArr[z][1] | attackArr[z][2] | attackArr[z][3] | attackArr[z][4];
			}
			break;
		case ASM_Press:
			if (currAttacks < maxAttacks) {
				++currAttacks;
				arrowMovesLeft[currAttacks-1] = 31;
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
					arrowLoc[currAttacks-1] = 4;
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
					arrowLoc[currAttacks-1] = 3;
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
					arrowLoc[currAttacks-1] = 1;
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
					arrowLoc[currAttacks-1] = 2;
				}
			}
			break;
		case ASM_Press1:
			if (currAttacks < maxAttacks) {
				++currAttacks;
				arrowMovesLeft[currAttacks-1] = 31;
				if ((currPos == 4 || currPos == 1) && jsPressed == 1) { //top left
					jsPressed = 0;
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
					arrowLoc[currAttacks-1] = 4;
				}
				if ((currPos == 3 || currPos == 2) && jsPressed == 1) {//top right
					jsPressed = 0;
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
					arrowLoc[currAttacks-1] = 3;
				}
				if ((currPos == 1 || currPos == 3) && jsPressed == 2) {//bottom left
					jsPressed = 0;
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
					arrowLoc[currAttacks-1] = 1;
				}
				if ((currPos == 2 || currPos == 4) && jsPressed == 2) {//bottom right
					PORTC = 0xFF;
					jsPressed = 0;
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
					arrowLoc[currAttacks-1] = 2;
				}
			}
			break;
		case ASM_WaitRelease:
			break;
	}
	return state;
}

enum CollisionSM_States {CSM_Start, CSM_Run};
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
			hit = 0;
			for (unsigned short i = 0; i < 2; ++i) {
				for (unsigned short k = 0; k < 504; ++k) {
					if ((slimeArr[k][i] & charPos[k]) != 0x00) { /*gameover = 1;*/ }
					if ((slimeArr[k][i] & attacks[k]) != 0x00) {
						hit = 1;
						if (i == 0) { 
							slimeMovesLeft[0] = slimeMovesLeft[1];
							currSlimes--;
							slimeLoc[0] = slimeLoc[1];
							for (unsigned short w = 0; w < 504; ++w) {
								slimeArr[w][0] = slimeArr[w][1];
								slimeArr[w][1] = 0x00;
							}
						}
						else {
							currSlimes--;
							for (unsigned short w = 0; w < 504; ++w) {
								slimeArr[w][1] = 0x00;
							}
						}
					}
				}
			}
			break;
	}
	return state;
}

enum ScorekeeperSM_States {SKSM_Start, SKSM_Wait, SKSM_Hit, SKSM_Hit1};
int TickFct_ScorekeeperSM(int state){
	switch(state) {
		case SKSM_Start:
			state = SKSM_Wait;
			score = 0;
			break;
		case SKSM_Wait:
			if (hit == 0) { state = SKSM_Wait; }
			else if (hit == 1) { state = SKSM_Hit; }
			break;
		case SKSM_Hit:
			state = SKSM_Hit1;
			break;
		case SKSM_Hit1:
			if (hit == 1) { state = SKSM_Hit1; }
			else if (hit == 0) { state = SKSM_Wait; }
			break;
	}
	switch(state) {
		case SKSM_Start:
			break;
		case SKSM_Wait:
			PORTD = score;
			break;
		case SKSM_Hit:
			if (gameover == 0) {
				++score;
			}
			break;
		case SKSM_Hit1:
			break;
	}
	return state;
}

enum DisplaySM_States {DSM_Start, DSM_Run, DSM_Lose, DSM_Lose1};
int TickFct_DisplaySM(int state){
	switch(state) {
		case DSM_Start:
			state = DSM_Run;
			break;
		case DSM_Run:
			if (gameover == 0) { state = DSM_Run; }
			else if (gameover == 1) { state = DSM_Lose; }
			break;
		case DSM_Lose:
			state = DSM_Lose1;
			break;
		case DSM_Lose1:
			if ((~PINA & 0x04) == 0x04) {
				gameover = 0;
				for (unsigned short i = 0; i < 504; ++i) {
					slimes[i] = 0;
					slimeArr[i][0] = 0x00;
					slimeArr[i][1] = 0x00;
					attacks[i] = 0x00;
					attackArr[i][0] = 0x00;
					attackArr[i][1] = 0x00;
					attackArr[i][2] = 0x00;
					attackArr[i][3] = 0x00;
					attackArr[i][4] = 0x00;
				}
				currSlimes = 0;
				currAttacks = 0;
				score = 0;
				state = DSM_Run;
			}
			else { state = DSM_Lose1; }
			break;
	}
	switch(state) {
		case DSM_Start:
			break;
		case DSM_Run:
			LCD_printBitmap(combineBitmap(combineBitmap(charPos, slimes),attacks));
			break;
		case DSM_Lose:
			LCD_printBitmap(youLose);
			break;
		case DSM_Lose1:
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
	tasks[i].period = 100;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_AttackSM;
	++i;	
	tasks[i].state = CSM_Start;
	tasks[i].period = 50;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_CollisionSM;
	++i;	
	tasks[i].state = SKSM_Start;
	tasks[i].period = 5;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &TickFct_ScorekeeperSM;
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
	multiplayer = (PINA & 0x40) >> 6;
    	while (1) { }
    	return 1;
}

