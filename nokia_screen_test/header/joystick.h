#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

unsigned short js_hdef;
unsigned short js_vdef;
unsigned short js_hcurr;
unsigned short js_vcurr;

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADPS2);// | (1 << ADPS0);
}

void Joystick_init() {
	ADC_init();
	ADMUX = 0x01;
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	js_vdef = ADC;
	ADMUX = 0x00;
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	js_hdef = ADC;
}

char Joystick_poll() {
	ADMUX = 0x01;
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	js_vcurr = ADC;
	ADMUX = 0x00;
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	js_hcurr = ADC;
	if ( ((js_vcurr <= (js_vdef+2)) && (js_vcurr >= (js_vdef-2)))
	&&   ((js_hcurr <= (js_hdef+2)) && (js_hcurr >= (js_hdef-2))) )
       	{ return 0x00; }
	
	else if ( ((js_vcurr > (js_vdef+2)) )
	&&   ((js_hcurr <= (js_hdef+2)) && (js_hcurr >= (js_hdef-2))) )
       	{ return 0x01; }

	else if ( ((js_vcurr > (js_vdef+2)) )
	&&   ( (js_hcurr < (js_hdef-2))) )
       	{ return 0x02; }
	
	else if ( ((js_vcurr <= (js_vdef+2)) && (js_vcurr >= (js_vdef-2)))
	&&   ( (js_hcurr < (js_hdef-2))) )
       	{ return 0x03; }
	
	else if ( ((js_vcurr < (js_vdef-2)) )
	&&   ( (js_hcurr < (js_hdef-2))) )
       	{ return 0x04; }
	
	else if ( ((js_vcurr < (js_vdef-2)) )
	&&   ((js_hcurr <= (js_hdef+2)) && (js_hcurr >= (js_hdef-2))) )
       	{ return 0x05; }
	
	else if ( ((js_vcurr < (js_vdef-2)) )
	&&   ((js_hcurr > (js_hdef+2))) )
       	{ return 0x06; }
	
	else if ( ((js_vcurr <= (js_vdef+2)) && (js_vcurr >= (js_vdef-2)))
	&&   ((js_hcurr > (js_hdef+2))) )
       	{ return 0x07; }
	
	else if ( ((js_vcurr > (js_vdef+2)) )
	&&   ((js_hcurr > (js_hdef+2))) )
       	{ return 0x08; }

	
	else { return 0x04; }
}

#endif
