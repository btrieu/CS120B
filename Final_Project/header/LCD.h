#ifndef __LCD_H__
#define __LCD_H__

void LCD_clear() {
	for (unsigned int i = 0; i < 504; ++i) {
		SPI_transfer(0x00);
	}
	PORTB = PINB & 0xFB; //enter command mode
	SPI_transfer(0x40);
	SPI_transfer(0x80);
	PORTB = PINB | 0x04; // enter data mode
}

void LCD_fill() {
	for (unsigned int i = 0; i < 504; ++i) {
		SPI_transfer(0xFF);
	}
}

void LCD_printBitmap(unsigned char bitmap[]) {
	for (unsigned int i = 0; i < 504; ++i) {
                SPI_transfer(bitmap[i]);
        }
	PORTB = PINB & 0xFB; //enter command mode
	SPI_transfer(0x40);
	SPI_transfer(0x80);
	PORTB = PINB | 0x04; // enter data mode
}

void LCD_init() {
	PORTB = PINB & 0xF0; //enable reset (active low), write enable, command mode
	PORTB = PINB | 0x02; //disable reset
	SPI_transfer(0x21); //power up, enable extended instruction set
	SPI_transfer(0xC8); //set operating voltage to 0xD0 for 3.3V, 0x9A for 5V
	SPI_transfer(0x20); //enable regular instruction set
	SPI_transfer(0x0C); //normal display mode
	PORTB = PINB | 0x04; //enter data mode
	LCD_clear();
}

#endif
