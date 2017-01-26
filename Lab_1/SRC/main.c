/*
 * main.c
 *
 *  Created on: Jan 21, 2017
 *      Author: Tom Hagen
 */
#include <RBELib/RBELib.h>
#include <avr/io.h>

#define POTCHAN 7
double compare = 0;
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
setCompareValue(double desiredFrequency) {
	//desired frequncy in Hz
	//2048 is 2*Prescaler(in this case the precaler is 1024
	compare = (F_CPU / (2048 * desiredFrequency)) - 1;

}

int main() {
	initRBELib();
	int test = 0;
//
	debugUSARTInit(115200);
	//putCharDebug('a');
//
	//initADC(7);

	//setCompareValue(100);//Compare value to hit compare value at a 100Hz frequency
	//printf("%G", compare);
	initTimer(1, NORMAL, compare);

	initializeSquareWaveGenerator();

	//DDRB |= (1 << PIN7);	//set pin 7 to output
	while (1) {
		/*Part 2*/
		//sampleADC();
		/*Part 3*/
		//	adjustDutyCycle();
//		pollButtons();
//		PORTB = (test << PIN7);	//write portB pin 7 high
//		putCharDebug(test + '0');
//		_delay_ms(200);
//		test = !test;
	}

	return 0;
}

