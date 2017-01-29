/*
 * DAC.c
 *
 *  Created on: Jan 25, 2017
 *      Author: Tom Hagen
 */
#include <avr/io.h>
#include <RBELib/RBELib.h>

void setDAC(int DACn, int SPIVal) {

	//we want sure that SPI is valid number  for the DAC AKA 12 bits
	if (SPIVal < 0)
		SPIVal = 0;
	else if (SPIVal > 4095)
		SPIVal = 4095;

	//FOR 12 bit data
	//Sends 24 bits, last 4 dont care
	//first 4 are command bits
	//next 4 are address bits
	//last 12+4 is data and the 4 dont care bits
	//command = 0b0011 (write to and update DAC)
	//address = 0b0000 (DAC A, might need to change this later)

	BYTE data[3];

	data[0] = 0x30 | DACn;		//write and update DAC and the DAC address
	data[1] = (BYTE) (SPIVal >> 4);	//sends the first 8 bits, mask for it to be 8 bits
	data[2] = (BYTE) (SPIVal << 4);	//sends the last 4 bits with the rest we dont care about, mask for it to be 8 bits

	//Select the DAC as an SPI Slave by driving the chip select low
	PORTD = (LOW << PD4);

	for (int i = 0; i < 3; i++) {
		spiTransceive(data[i]);
		//printf("%d \n\r", data[i]);

	}

	//Turn off the chip select for the DAC
	PORTD = (HIGH << PD4);

}
