/*
 * SPI.c
 *
 *  Created on: Jan 25, 2017
 *      Author: Tom Hagen
 */
#include <avr/io.h>
#include <RBELib/RBELib.h>
void initSPI() {
	DDRB = (OUTPUT << PB4) |		//set SS to output
			(OUTPUT << PB5) |	//set MOSI to output
			(INPUT << PB6) |		//set MISO to input
			(OUTPUT << PB7);	//set SCLK to output
	PORTB = (HIGH << PB4);	//Set the Master SS high, AKA the AVR is the master

	/* Power Reduction Register */
	PRR0 = (0 << PRSPI);//To enable the SPI module, Power Reduction Serial Peripheral Interface bit in the Power Reduction
						//Register (0.PRSPI0) must be written to '0'.

	/*SPI Control Register 0	 */
	SPCR = (1 << SPIE) | 		// SPI interrupt enable, leave this off for now
			(1 << SPE) | 		//SPI enable, written to one is enabled
			(0 << DORD) |  		//Most significant bit first
			(1 << MSTR) | 		//Master SPI mode
			(0 << CPOL) | 		//0 SCK is low when idle, 1 is high when idle
			(0 << CPHA) | 		//Leading edge is sample, trailing edge is setup
			(1 << SPR1) |  		//Fosc/16
			(0 << SPR0);		//Fosc/16

	/*SPI Status Register 0		*/
	//SPSR = (0 << SPI2X);		//Double SPI Speed, disable for now
	//	(0 << WCOL) |//Initialize the write collision flag to 0 just in case
	//(0 << SPIF);		//Clear any preexisting SPI interrupt flags
	//Deassert all the chip selects by setting them as outputs and set high
	DDRC = (OUTPUT << PC0) |			//Set the spare chip select as output
			(OUTPUT << PC1) |		//Set the CoProcessor chip select as output
			(OUTPUT << PC2) |			//Set the spare chip select as output
			(OUTPUT << PC3) |			//Set the spare chip select as output
			(OUTPUT << PC4) |			//Set the Enc 1 chip select as output
			(OUTPUT << PC5) |			//Set the Enc 0  chip select as output
			(OUTPUT << PC6);			//Set the spare chip select as output

	DDRD = (OUTPUT << PD4);			//Set the Lin_Dac chip select as output

	PORTC = (HIGH << PC0) |//Choose the spare chip select port as not a slave
			(HIGH << PC1) |//Choose the CoProcessor chip select port as not a slave
			(HIGH << PC2) |//Choose the spare chip select port as not a slave
			(HIGH << PC3) |//Choose the spare chip select port as not a slave
			(HIGH << PC4) |//Choose the Enc1 chip select port as not a slave
			(HIGH << PC5) |//Choose the Enc0 chip select port as not a slave
			(HIGH << PC6);	//Choose the spare chip select port as not a slave

	PORTD = (HIGH << PD4);	//Choose the Lin_DAC chip select port as not a slave

}

unsigned char spiTransceive(BYTE data) {
	SPDR = data;
	// Wait for transmission to complete
	while (!(SPSR & (1 << SPIF)))
		;

	//Read any received data
	return SPDR;

}
