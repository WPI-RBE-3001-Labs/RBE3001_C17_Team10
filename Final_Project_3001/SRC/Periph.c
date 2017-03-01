/*
 * Periph.c
 *
 *  Created on: Feb 11, 2017
 *      Author: thagen
 */
#include <RBELib/RBELib.h>
#include <avr/io.h>
#include <encoderMacros.h>
#include <math.h>



float vref = 5;
float GfCnv = 0.22;
float Cnt2Mv = 0.80586;

signed int getAccel(int axis) {
	signed int result = 0;
	char channel = axis;

	//assert the spare cs low (enable SPI comm to and from accelerometer)
	PORTD &= ~(1 << PIN7);
	//send config bits
	spiTransceive(0b00000110);

	//Get the information from the desired axis
	result = spiTransceive(channel << 6);
	result = (result & 0x0F) << 8;
	result |= spiTransceive(0x00);

	//deassert the spare cs
	PORTD |= (1 << PIN7);
	return result;
}


float convertAccelmV(int accelValue) {
	float mv = 0;
	mv = pow(accelValue, Cnt2Mv);
	printf("milliVolts: %f \n\r", mv);
	return mv;
}

float convertAccelGforce(int accelValue) {
	float gForce = 0;

	if (accelValue >= vref) {
		gForce = pow(accelValue - vref, GfCnv);
	} else {
		gForce = -pow(vref - accelValue, GfCnv);

	}
	printf("GForce: %f\n\r", gForce);
	return gForce;
}

int IRDist(int chan) {

	 int IrVal = 0;
	 IrVal = getADC(chan);


	int CmDist = 0;
	CmDist = ((IrVal - 905.05) / -47.286) + 2.3;
	return CmDist;

}

void encInit(int chan) {
	switch (chan) {
	case 0:
		//assert the channel 0 encoder cs
		PORTC &= ~(1 << PIN5);

		// Send configuration bytes

		//Write to mode register 0
		spiTransceive(0x88);
		//X4 quadrature mode
		spiTransceive(0x03);

		//deassert the encoder cs
		PORTC |= (1 << PIN5);

		break;
	case 1:
		//assert the channel 1 encoder cs
		PORTC &= ~(1 << PIN4);

		// Send configuration bytes

		//Write to mode register 0
		spiTransceive(0x88);
		//X4 quadrature mode
		spiTransceive(0x03);

		//deassert the encoder cs
		PORTC |= (1 << PIN4);

		break;

	}
}

void resetEncCount(int chan) {
	switch (chan) {
	case 0:
		PORTC &= ~(1 << PIN5);
		spiTransceive(0b00100000);	// Clear the counter
		//deassert the encoder cs
		PORTC |= (1 << PIN5);
		break;
	case 1:
		PORTC &= ~(1 << PIN4);
		spiTransceive(0b00100000);	// Clear the counter
		//deassert the encoder cs
		PORTC |= (1 << PIN4);
		break;
	}

}
signed long encCount(int chan) {
	signed long ticks = 0;

	switch (chan) {
//Returns the number of encoder counts on channel 0
	case 0:
		//assert the channel 0 encoder cs
		PORTC &= ~(1 << PIN5);

		// Send read-start byte
		spiTransceive(0b01100000);

		//Get the number of ticks in 32 bits
		ticks |= spiTransceive(0b00000000) << 24;
		ticks |= spiTransceive(0b00000000) << 16;
		ticks |= spiTransceive(0b00000000) << 8;
		ticks |= spiTransceive(0b00000000) << 0;

		//deassert the encoder cs
		PORTC |= (1 << PIN5);

		return ticks;
		break;
		//Returns the number of encoder counts on channel 1
	case 1:

		//assert the channel 0 encoder cs
		PORTC &= ~(1 << PIN4);

		// Send read-start byte
		spiTransceive(0b01100000);

		//Get the number of ticks in 32 bits
		ticks |= spiTransceive(0b00000000) >> 24;
		ticks |= spiTransceive(0b00000000) >> 16;
		ticks |= spiTransceive(0b00000000) >> 8;
		ticks |= spiTransceive(0b00000000) >> 0;

		//deassert the encoder cs
		PORTC |= (1 << PIN4);

		return ticks;
		break;

	default:
		return -1;
		break;
	}
}
