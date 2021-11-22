//basic SPI header

#ifndef _SPI_H_
#define _SPI_H_
unsigned char SPI_transfer(unsigned char data) {
        SPDR = data;
        asm("nop");
        while(!(SPSR & (1 << SPIF)));
        return SPDR;
}
void SPI_init_master() {
        SPCR = 0x00;
        SPCR |= (1 << SPE) | (1 << MSTR);
}
void SPI_init_slave() {
        SPCR = 0x00;
        SPCR |= (1 << SPE);
}
#endif
