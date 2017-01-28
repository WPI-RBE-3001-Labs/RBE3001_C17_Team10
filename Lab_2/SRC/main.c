/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: Joest
 */

#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>

#define POTCHAN 2
void printPotVal(int potVal) {
	//5000/1023
	unsigned int mV = potVal * 4.88758;		//in millivolts

	//300/1023
	unsigned int angle = potVal * .293255132;		//in degrees

	printf("%d %d %d \n\r", potVal, mV, angle);
}

void readPotVal() {
	int val = getADC(POTCHAN);
	clearADC(POTCHAN);
	printPotVal(val);
}

void sampleADC(int seconds) {
	int timestamp = 0;
	while (timestamp < seconds) {
		readPotVal(POTCHAN);
		_delay_ms(1000);
		timestamp++;
	}
}

int main(void) {
	//Enable printf() and setServo()
	initRBELib();

	// Write the USARTDebug.c file using the function prototypes in the H file to enable the usart
	//Set the baud rate of the UART
	debugUSARTInit(115200);
	// printf uses a fair amount of memory to be included but makes serial printing much easier
//	initADC(2);
//
//	sampleADC(2, 60);
//
//

	//POT VALUES
	//720 = 90 deg
	//340 = 0 deg
}

