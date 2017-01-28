/*
 * DAC.c
 *
 *  Created on: Jan 25, 2017
 *      Author: Tom Hagen
 */
#include <avr/io.h>
#include <RBELib/RBELib.h>

void DACinit() {
//Select the DAC as an SPI Slave by driving the chip select low
	PORTD = (LOW << PD4);


}
void setDAC(int DACn, int SPIVal) {

}
