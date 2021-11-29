#include <avr/io.h>
#include "timer.h"
#include "SPI.h"
#include "LCD.h"
#include "bitmaps.h"
#include "joystick.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
unsigned char garbage;
unsigned long long  ctr = 0;

unsigned char currMap[504];
void Tick() {
	for (unsigned short i = 1; i < 504; ++i) {
		attack[i-1] = attack[i];
	}
	attack[503] = 0x00;
	for (unsigned int k = 0; k < 504; ++k) {
		currMap[k] = attack[k] | left[k];
	}
	LCD_printBitmap(currMap);
	PORTC = Joystick_poll();
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xBF;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
    /* Insert your solution below */
	TimerSet(10);
	TimerOn();
	SPI_init_master();
	LCD_init();
	Joystick_init();
    while (1) {
    	Tick();
	while (!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
