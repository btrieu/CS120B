// Dual ADC w/ joystick
// demo link:

#include <avr/io.h>
#include "timer.h"
#include "io.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

short JS_hdefault;
short JS_vdefault;
short JS_hcurr;
short JS_vcurr;

void ADC_init() {
	ADCSRA |= (1 << ADEN);
}

void Joystick_init() {
	ADMUX = 0x00;
	ADCSRA |= 0x40;
	JS_vdefault = ADC;
	ADMUX = 0x01;
	ADCSRA |= 0x40;//(1 << ADSC);
	JS_hdefault = ADC;
	//JS_hcurr = JS_hdefault;
	//JS_vcurr = JS_vdefault;
}

unsigned char pollJoystick() {
	ADMUX = 0x00;
	ADCSRA |= 0x40;
	JS_vcurr = ADC;
	ADMUX = 0x01;
	ADCSRA |= 0x40;
	JS_hcurr = ADC;
/*	
	if (JS_vcurr/2 < JS_vdefault) { PORTB = 0x01; }
	else if (JS_vcurr/2 > JS_vdefault) { PORTB = 0x02; }
	else { PORTB = 0x00; }
	
	return '0';	*/
	/*if ( ((JS_vcurr <= (JS_vdefault*2)) && (JS_vcurr >= (JS_vdefault*0.5))) 
	&&   ((JS_hcurr <= (JS_hdefault*2)) && (JS_hcurr >= (JS_hdefault*0.5))) )
       	{ return '0'; }*/
	//else if ((JS_vcurr == JS_vdefault) && (JS_hcurr == JS_hdefault)) { return '1'; }
	//else { return '1'; }
}

short currADC;

enum States {Start, RunADC, RunADC1}state;

void Tick() {
	switch(state) {
		case Start:
			state = RunADC;
			break;
		case RunADC:
			if ((~PINA & 0x04) == 0x04) { state = RunADC1; }
			else { state = RunADC; }
			break;
		case RunADC1:
			if ((~PINA & 0x04) == 0x04) { state = RunADC; }
			else { state = RunADC1; }
			break;
	}
	switch(state) {
		case Start:
			break;
		case RunADC:
			ADMUX = 0x00;
			ADCSRA |= (1 << ADSC);
			currADC = ADC;
			PORTB = currADC;
			PORTD = currADC >> 8;
			break;
		case RunADC1:
			ADMUX = 0x01;
			ADCSRA |= (1 << ADSC);
			currADC = ADC;
			PORTB = currADC;
			PORTD = currADC >> 8;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    	state = Start;
    /* Insert your solution below */
	ADC_init();
	//LCD_init();
	//Joystick_init();
	TimerSet(500);
	TimerOn();
    while (1) {
    	Tick();
	while (!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}
