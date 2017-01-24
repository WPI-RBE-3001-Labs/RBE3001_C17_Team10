/*
 * ADC.c
 *
 *  Created on: Jan 21, 2017
 *      Author: Tom Hagen
 */
#include <RBELib/RBELib.h>
#include <avr/io.h>

void initADC(int channel) {
	//Set everything to inputs
	//bit of a hack for now will need to implement how to do it for a specific channel later on
	DDRA &= 0b00000000;

	//Reference voltages of VCC and ground
	ADMUX = (0 << REFS1) | (1 << REFS0);

	//Enable the ADC
	//Select the ADC prescaler to division factor of 128
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void clearADC(int channel) {
	//Set everything to inputs
	//bit of a hack for now will need to implement how to do it for a specific channel later on
	DDRA &= 0b00000000;

	//Clear the data registers
	ADCH = 0;
	ADCL = 0;
	ADC = 0;
}

unsigned short getADC(int channel) {
	//Select the channel
	ADMUX = (1 << REFS1) | (1 << REFS0) | channel;

	//start the conversion
	PRR = (0 << PRADC);
	ADCSRA |= (1 << ADSC);

	//wait for conversion to complete
	while (ADCSRA & (1 << ADSC))
		;

	return ADC;
}


void changeADC(int channel) {

}
