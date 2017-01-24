/*
 * main.c
 *
 *  Created on: Jan 21, 2017
 *      Author: Tom Hagen
 */
#include <RBELib/RBELib.h>
#include <avr/io.h>

#define POTCHAN 7
#define button1 DDRBbits._P5
#define button2 DDRBbits._P6
#define button3 DDRBbits._P7
int timestamp = 0;

void printPotVal(int potVal) {
	//5000/1023
	unsigned int mV = potVal * 4.88758;		//in millivolts
	//300/1023
	unsigned int angle = potVal * .293255132;		//in degrees
	printf("%d %d %d %d \n\r", timestamp, potVal, mV, angle);
}

void readPotVal() {
	int val = getADC(POTCHAN);
	clearADC(POTCHAN);
	printPotVal(val);
}

void sampleADC() {
	while (timestamp < 60) {
		readPotVal(POTCHAN);
		_delay_ms(1000);
		timestamp++;
	}
}

setupPWMPin() {
	DDRAbits._P5 = OUTPUT;

}

int main() {
	initRBELib();
	debugUSARTInit(115200);
	initADC(7);

	initTimer(0, NORMAL, 0);
	setupPWMPin();
	initializeSquareWaveGenerator();

	while (1) {
		/*Part 2*/
		//sampleADC();
		/*Part 3*/
		pollButtons();

		//adjustDutyCycle();
		//putCharDebug('a');
	}

	return 0;
}

