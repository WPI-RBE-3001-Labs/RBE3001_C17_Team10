/*
 * SPI.c
 *
 *  Created on: Jan 25, 2017
 *      Author: Tom Hagen
 */
#include <avr/io.h>
#include <RBELib/RBELib.h>
void initSPI() {
	// Set MOSI and SCK output
	DDRB = (OUTPUT << PB7) | 		//Set SCK to output
			(INPUT << PB6) | 		//Set MISO to input
			(OUTPUT << PB5) | 		//Set MOSI to output
			(OUTPUT << PB4);		//Set SS to output

	PORTB = (HIGH << PB4);			//Drive the slave select pin high

	/*SPI Control Register 0	 */
	SPCR = (0 << SPIE) | 		// SPI interrupt enable, leave this off for now
			(1 << SPE) | 		//SPI enable, written to one is enabled
			(1 << DORD) |  		//Most significant bit first
			(1 << MSTR) | 		//Master SPI mode
			(0 << CPOL) | 		//0 SCK is low when idle, 1 is high when idle
			(1 << CPHA) |  	//Leading edge is setup, trailing edge is sample
			(1 << SPR1) |  		//Fosc/64
			(1 << SPR0);		//Fosc/64

	/*SPI Status Register 0		*/
	SPSR = (0 << SPI2X) |		//Double SPI Speed, disable for now
			(0 << WCOL) |//Initialize the write collision flag to 0 just in case
			(0 << SPIF);		//Clear any preexisting SPI interrupt flags
}

unsigned char spiTransceive(BYTE data) {
	SPDR = data;

	// Wait for transmission to complete
	while (!(SPSR & (1 << SPIF)))
		;
	//Read any received data
	return SPDR;
	return 'a';
}
