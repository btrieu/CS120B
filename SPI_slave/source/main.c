#include <avr/io.h>
#include "timer.h"
#include "SPI.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void Tick() {
	if (SPDR == 0xFF) { PORTA = 0x01; }
	else if (SPDR == 0x01) { PORTA = 0x02; }
	else if (SPDR == 0x02) { PORTA = 0x04; }
	else if (SPDR == 0x03) { PORTA = 0x08; }
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0x00; // use 0x40 to enable MISO outputs
	DDRA = 0xFF; PORTA = 0x00;
    /* Insert your solution below */
	TimerSet(1000);
	TimerOn();
	SPI_init_slave();
    while (1) {
    	Tick();
	while (!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
