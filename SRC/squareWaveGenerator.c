/*
 * squareWaveGenerator.c
 *
 *  Created on: Jan 23, 2017
 *      Author: Tom Hagen
 */
#include <RBELib/RBELib.h>
#include <avr/io.h>

int count = 0;
int ticksNeeded = 1;
int highTime = 0;
int period = 0;
float duty = 0;
int upFlag = 2;
int timerCount = 0;

void initializeSquareWaveGenerator() {
	DDRBbits._P0 = INPUT;
	DDRBbits._P1 = INPUT;
	DDRBbits._P2 = INPUT;

	//DDRAbits._P5 = OUTPUT;
//	PINAbits._P5 = HIGH;

	duty = .5;
	period = 206;
	highTime = 512;

//	adjustDutyCycle();
}
void setPinHIGH() {
	PORTA = (1 << PA5);
	//PINAbits._P5 = HIGH;
}
void setPinLOW() {

//	PINAbits._P5 = LOW;

	PORTA = (0 << PA5);

}

void setPin(int upFlag) {
	if (upFlag == 0) {
		setPinHIGH();
	} else if (upFlag == 1) {
		setPinLOW();
	}
}

void pollButtons() {
	//If the first button is pressed initialize
	//a timer for a frequency of 1Hz
//	if (PINBbits._P0 == HIGH) {
//		ticksNeeded = 512;
//	}
//
//	//If the second button is pressed initialize
//	//a timer for a frequency of 20Hz
//	else if (PINBbits._P1 == HIGH) {
//		ticksNeeded = 26;
//	}
//
//	//If the third button is pressed initialize
//	//a timer for a frequency of 100Hz
//	else if (PINBbits._P2 == HIGH) {
//		ticksNeeded = 5;
//
//	} else
//		ticksNeeded = 1;

	setPin(upFlag);
}
void adjustDutyCycle() {
	/*For now lets just get a duty cycle of .5*/
	//	int reading = getADC(7);
	//duty = (reading / 1024) * 100;
	period = F_CPU / ticksNeeded;	// in ticks
	highTime = period * duty;
}

//ISR
//Just flips a pin high to low based on the frequency of the timer
ISR(TIMER0_OVF_vect) {
	count++;
	if (count <= 562) {
		upFlag = 1;
	} else {
		upFlag = 0;
		count = 0;
	}

	if (count > 1125) {
		count = 0;
	}

}
