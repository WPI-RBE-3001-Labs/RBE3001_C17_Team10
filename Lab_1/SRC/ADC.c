/*
 * ADC.c
 *
 *  Created on: Jan 21, 2017
 *      Author: Tom Hagen
 */
#include <RBELib/RBELib.h>
#include <avr/io.h>

void initADC(int channel) {
	//Set Port A register for the corresponding channel to input
	DDRA &= ~(1 << channel);

	//Disable digital input on ADC channel
	DIDR0 |= (1 << channel);

	ADMUX = (0 << REFS1) |	//Sets Reference voltage to Vcc
			(1 << REFS0);	//Sets Reference voltage to Vcc

	ADCSRA = (1 << ADEN) |	//Enable the ADC
			(1 << ADPS2) |	//Select the ADC prescaler to 128
			(1 << ADPS1) | //Select the ADC prescaler to 128
			(1 << ADPS0);	//Select the ADC prescaler to 128

}

void clearADC(int channel) {
	//Set Port A register for the corresponding channel to input
	DDRA &= ~(1 << channel);

	//Clear the data registers
	ADCH = 0;
	ADCL = 0;
	ADC = 0;
}

unsigned short getADC(int channel) {
	//Select the channel
	ADMUX |= channel;

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
