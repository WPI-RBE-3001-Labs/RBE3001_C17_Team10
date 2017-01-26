/*
 * timestamp.c
 *
 *  Created on: Jan 22, 2017
 *      Author: Tom Hagen
 */

#include <RBELib/RBELib.h>
#include <avr/io.h>

//Set up some variables for timestamps
int seconds = 0;
int minutes = 0;
int hours = 0;

void initializeTimer() {
	//initialize a timer to count for 1 second each time
	initTimer(0, NORMAL, 0);

}
void resetTimeStamp(){
	seconds = 0;
	minutes = 0;
	hours = 0;
}

void incrementCounts() {
	if (seconds >= 60) {
		minutes++;
		seconds = 0;
	}

	else if (minutes >= 60) {
		hours++;
		minutes = 0;
	}
	putCharDebug(seconds);
}

//ISR(TIMER0_OVF_vect) {
//	seconds++;
//	incrementCounts();
//
//}
