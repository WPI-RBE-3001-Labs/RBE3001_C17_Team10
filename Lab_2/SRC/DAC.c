/*
 * DAC.c
 *
 *  Created on: Jan 25, 2017
 *      Author: Tom Hagen
 */
#include <avr/io.h>
#include <RBELib/RBELib.h>

void DACinit() {
//Set the DAC CS pin as an output
	DDRD |= (OUTPUT << PD4);

	//Enable SPI to the DAC by sending a high CS
	PORTD |= (HIGH << PD4);

}
void setDAC(int DACn, int SPIVal) {

}
