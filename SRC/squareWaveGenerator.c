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
int lowTime = 0;
int periodCount = 0;
float duty = 0;
volatile int upFlag = 0;
int timerCount = 0;

void initializeSquareWaveGenerator() {
	DDRB = (0 << PB2) |		//setup Port B pins 2,1,0 as inputs
			(0 << PB1) | 	//setup Port B pins 2,1,0 as inputs
			(0 << PB0); 	//setup Port B pins 2,1,0 as inputs

	//setup Port A pin 5 as an output, keeps the ADC analog pin as an input
	DDRA |= (1 << PA4);

	duty = .5;
	periodCount = 100;
	highTime = 20;
	//adjustDutyCycle();
}
void setPinHIGH() {
	//Changes only what's on Port A Pin 4 to high
	PORTA |= (1 << PA4);

}
void setPinLOW() {
	//Changes only what's on Port A Pin 4 to low
	PORTA &= ~(1 << PA4);
}

void setPin(int upFlag) {
	if (upFlag == 0) {
		setPinHIGH();
		TCNT1 = 32768;
	} else if (upFlag == 1) {
		setPinLOW();
		TCNT1 = 32768;
	}

}

void pollButtons() {
//	If the first button is pressed initialize
//	a timer for a frequency of 1Hz

	if (PINB0 == HIGH) {
		//If the ISR goes off at a rate of 100Hz, then it would take 100 interrupts to
		//count up to the period count to toggle the output
		periodCount = 100;

	}

	//If the second button is pressed initialize
	//a timer for a frequency of 20Hz
	else if (PINB1 == HIGH) {
		//If the ISR goes off at a rate of 100Hz, then it would take 20 interrupts to
		//count up to the period count to toggle the output
		periodCount = 20;
	}

	//If the third button is pressed initialize
	//a timer for a frequency of 100Hz
	else if (PINB2 == HIGH) {
		//If the ISR goes off at a rate of 100Hz, then it would take 1 interrupts to
		//count up to the period count to toggle the output
		periodCount = 1;
	}

}
void adjustDutyCycle() {
	/*For now lets just get a duty cycle of .5*/
	int reading = getADC(7);
	duty = (reading / 1024);
	//printf("%f \n\r", duty);
	highTime = 256 * duty;
	lowTime = 256 * (1 - duty);

}

//ISR
//Just flips a pin high to low based on the frequency of the timer

ISR(TIMER1_OVF_vect) {
	//putCharDebug('a');
	count++;
	if (count <= 1) {
		//putCharDebug('a');
		upFlag = !upFlag;
		setPin(upFlag);
	} else if (count > 1 && count < 3) {
		upFlag = !upFlag;
		setPin(upFlag);
		count = 0;
	} else
		count = 0;

}
