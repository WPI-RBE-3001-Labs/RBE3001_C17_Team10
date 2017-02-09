/*
 * accelerometer.c
 *
 *  Created on: Feb 8, 2017
 *      Author: thagen
 */
#include <RBELib/RBELib.h>
#include <avr/io.h>

#define XChan 7
#define YChan 6
#define ZChan 5

volatile int sampleFlag = 0;

struct {
	int Ax;
	int Ay;
	int Az;
} accelValues;

void sampleAccelerometer() {
	//Spare CS pin set to output
	DDRC |= (OUTPUT >> PC0);

	//enable the chip select for the accelerometer to
	//	transmit the data over SPI
	//We need to send out 3 "packages" for configuring the DAC
	unsigned char package1 = 0;
	unsigned char package2 = 0;
	unsigned char package3 = 0;
	//Temp value for creating the packages from the SPI value
	unsigned long temp = 0;

	accelValues.Ax = getADC(XChan);
	accelValues.Ay = getADC(YChan);
	accelValues.Az = getADC(ZChan);

	//Package 1 is:
	//Write to and update (power up) DAC register n
	package1 = accelValues.Ax;

	//This is our second package
	package2 = accelValues.Ay;

	//This will be our final package
	//The bottom 4 bits are don't cares
	package3 = accelValues.Az;

	//Assert the accelerometer
	PORTC &= ~(HIGH >> PC0);
	spiTransceive(package1);
	spiTransceive(package2);
	spiTransceive(package3);

	//Toggle the SPI Line to send the data
	PORTC |= (HIGH >> PC0);
	PORTC &= ~(HIGH >> PC0);
	PORTC |= (HIGH >> PC0);
}

int main(void) {
	initTimer(0, NORMAL, 0);
	initADC(XChan);
	initADC(YChan);
	initADC(ZChan);
	while (1) {

		if (sampleFlag) {
			sampleAccelerometer();
			sampleFlag = 0;
		}

	}
	return 0;
}

ISR(TIMER0_OVF_vect) {
	count++;
	//If the count is above the threshold for 100Hz
	if (count >= 45) {
		sampleFlag = 1;
		count = 0;
	}
}
