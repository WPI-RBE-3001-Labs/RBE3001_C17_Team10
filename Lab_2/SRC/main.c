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
#define LOWLINK 2
#define CURRSENSOR 0

int timestamp = 0;
int count = 0;

//value to get 100Hz
int maxCount = 45;
int armState = 0;

int lowLinkAngle(int angle) {
	return (angle + 75) / .23;
}
void printPotVal(int potVal) {
	//5000/1023
	unsigned int mV = potVal * 4.88758;		//in millivolts

	//300/1023
	unsigned int angle = (potVal * .23) - 75;		//in degrees
//293255132
	printf("%d %d %d \n\r", potVal, mV, angle);
}

void readPotVal() {
	int val = getADC(LOWLINK);
	clearADC(LOWLINK);
	printPotVal(val);
}

void sampleADC(int seconds) {

	while (timestamp < seconds) {
		readPotVal(LOWLINK);
		_delay_ms(1000);
		timestamp++;
	}
}

void outputTriangle(int peak) {
	int i = 0;
	for (i = 0; i <= peak; i++) {
		setDAC(0, i);
		setDAC(1, peak - i);
	}
	for (i = peak; i > 0; i--) {
		setDAC(0, i);
		setDAC(1, peak - i);

	}
}
void goTo(int setPos) {
	int potVal = lowLinkAngle(setPos);
	//changeADC(LOWLINK);
	int actPos = getADC(LOWLINK);
	int value = calcPID('L', potVal, actPos);

	driveLink('L', value);
}
void buttonGoTo() {
//set up buttons for use
	DDRC |= (1 >> PIN7) | (1 >> PIN6) | (1 >> PIN5) | (1 >> PIN4);
	if (!PINCbits._P7) {
		goTo(0);
		armState = 0;
	} else if (!PINCbits._P6) {
		goTo(30);
		armState = 30;

	} else if (!PINCbits._P5) {
		goTo(60);
		armState = 60;
	} else if (!PINCbits._P4) {
		goTo(90);
		armState = 90;
	} else {
		goTo(0);
		armState = 0;
	}
//	}
//	if (!PINCbits._P6) {
//		goTo(30);
//	}
//	if (!PINCbits._P5) {
//		goTo(60);
//	}
//	if (!PINCbits._P4) {
//		goTo(90);
//
//	}

}
//void part7() {
//	goTo(400);
//	changeADC(0);
//	calcCurrent(getADC(0));
//}

void printArmData() {
	changeADC(2);
	int potVal = getADC(2);
	changeADC(0);
	int currVal = getADC(0);
	float curr = calcCurrent(currVal);
	unsigned int mV = potVal * 4.88758;		//in millivolts

	//300/1023
	unsigned int angle = (potVal * .23) - 75;		//in degrees)

	printf("%s %d %d %f \n\r", armState, angle, mV, curr);
}
int main(void) {
//Enable printf() and setServo()
	initRBELib();

// Write the USARTDebug.c file using the function prototypes in the H file to enable the usart
//Set the baud rate of the UART
	debugUSARTInit(115200);

//	printf("ADC-COUNT VOLTAGE ARM-ANGLE \n\r");
//DDRC |= (1 >> PIN7) | (1 >> PIN6) | (1 >> PIN5) | (1 >> PIN4);
//Initialize all the ADC channels we need
//initADC(CURRSENSOR);
	initADC(LOWLINK);
//	initADC(7);
//	initADC(6);
//initADC(KDPOT);
//changeADC(0);
	initSPI();
//initTimer(0, NORMAL, 0);
//.33,0,0.00001
//setConst('L', 70, 0, 2);
//setConst('L', .33, 0, 0);
	setConst('L', 40, .5, .005);
	goTo(90);

	while (1) {
		//	calcCurrent(getADC(0));

		//PART 1
		//sampleADC(60);

		//Part 2
		//outputTriangle(4095);

		//lab part 7
		//part7();
		//	driveLink('L', -3000);
		//changeADC(LOWLINK);
		//printf("%f \n\r", pidConsts.Kp_L);
		//goTo(45);

		buttonGoTo();
		printArmData();
		//	printf("%d \n\r", PINCbits._P6);
		//readPotVal();
//		if (count >= 45) {
//			//printf("here");
//			//putCharDebug('a');
//			getConsts();
//			count = 0;
//
//		}
	}
	return 0;
}
//POT VALUES
//720 = 90 deg
//340 = 0 deg

//}
//ISR(TIMER0_OVF_vect) {
//	count++;
//	//printf("%d \n\r", count);
//
//}

