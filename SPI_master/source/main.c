#include <avr/io.h>
#include "timer.h"
#include "SPI.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char ctr = 0;

void Tick() {
	if ((ctr % 4) == 1) { SPI_transfer(0xFF); }
	else if ((ctr % 4) == 2) { SPI_transfer(0x01); }
	else if ((ctr % 4) == 3) { SPI_transfer(0x02); }
	else { SPI_transfer(0x03); }
	++ctr;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xBF;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	TimerSet(1000);
	TimerOn();
	SPI_init_master();
    while (1) {
    	Tick();
	while (!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
