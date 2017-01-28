/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: Joest
 */

#include <RBELib/RBELib.h>
//For use of abs()
#include <stdlib.h>
#include <avr/io.h>
#include <RBELib/ADC.h>
#define POTCHAN 2

int timestamp = 0;

void printPotVal(int potVal) {
	//5000/1023
	unsigned int mV = potVal * 4.88758;		//in millivolts

	//300/1023
	unsigned int angle = (potVal * .23) - 75;		//in degrees
//293255132
	printf("%d %d %d \n\r", potVal, mV, angle);
}

void readPotVal() {
	int val = getADC(POTCHAN);
	clearADC(POTCHAN);
	printPotVal(val);
}

void sampleADC(int seconds) {

	while (timestamp < seconds) {
		readPotVal(POTCHAN);
		_delay_ms(1000);
		timestamp++;
	}
}

void outputTriangle(int peak) {
	int i = 0;
	for (i = 0; i <= peak; i++) {
		setDAC(0, i);
		_delay_us(100);
	}
	for (i = peak; i > 0; i--) {
			setDAC(0, i);
			_delay_us(100);
	}
}

int main(void) {
	//Enable printf() and setServo()
	initRBELib();

	// Write the USARTDebug.c file using the function prototypes in the H file to enable the usart
	//Set the baud rate of the UART
	debugUSARTInit(115200);
	// printf uses a fair amount of memory to be included but makes serial printing much easier
	printf("ADC-COUNT VOLTAGE ARM-ANGLE \n\r");
	initADC(2);
	initSPI();
//
	while (1) {
		//PART 1
//		sampleADC(60);

		//Part 2
		outputTriangle(4095);
	}
	//POT VALUES
	//720 = 90 deg
	//340 = 0 deg

}

