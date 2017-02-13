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
#define HIGHLINK 3
#define CURRSENSOR 0

#define AXIS 2

int timestamp = 0;
volatile int count = 0;

int homeFlag = 0;
volatile int sampleFlag = 0;

//value to get 100Hz
int maxCount = 45;
int armState = 0;

//next line for triangle
int nextLine = 0;

int linkAngle(int angle) {
	return (angle + 85) / .26;
}

void printHighPotVal(int potVal) {
	//5000/1023
	unsigned int mV = potVal * 4.88758;		//in millivolts

	//300/1023
	unsigned int angle = ((potVal * .26) - 85) + 90;		//in degrees
////293255132
	printf("%d %d %d \n\r", potVal, mV, angle);
}

void printLowPotVal(int potVal) {
	//5000/1023
	//unsigned int mV = potVal * 4.88758;		//in millivolts

	//300/1023
	unsigned int angle = (potVal * .26) - 85;		//in degrees
//293255132
	printf("Arm Angle: %d \n\r", angle);
}

void readPotVal() {
	int val = getADC(LOWLINK);
	clearADC(LOWLINK);
	printLowPotVal(val);
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
void goToLowLink(int setPos) {
	int potVal = linkAngle(setPos);

	changeADC(LOWLINK);

	int actPos = getADC(LOWLINK);

	int value = calcLowPID(potVal, actPos);

	driveLink('L', value);
}

void goToHighLink(int setPos) {
	int potVal = linkAngle(setPos);

	changeADC(HIGHLINK);

	int actPos = getADC(HIGHLINK);

	int value = calcHighPID(potVal, actPos);

	driveLink('H', value);
}
void buttonGoTo() {

	if (!PINCbits._P7) {
		goToLowLink(0);
		armState = 0;
	} else if (!PINCbits._P6) {
		goToLowLink(30);
		armState = 30;

	} else if (!PINCbits._P5) {
		goToLowLink(60);
		armState = 60;
	} else if (!PINCbits._P4) {
		goToLowLink(90);
		armState = 90;
	} else {
		goToLowLink(0);
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

	printf("%d %d %f \n\r", armState, angle, mV, curr);
}

void buttonGoToXY() {

	if (!PINCbits._P7) {
		goToBothLinks(0, 90);
	} else if (!PINCbits._P6) {
		goToBothLinks(45, 0);
	} else if (!PINCbits._P5) {
		goToBothLinks(45, 90);
	} else if (!PINCbits._P4) {
		goToLowLink(20);
		goToHighLink(45);
	} else {
		goToLowLink(45);
		goToHighLink(10);

	}
}
void goToBothLinks(int theta1, int theta2) {
	if (theta1 > 90) {
		theta1 = 90;
	} else if (theta1 < 0) {
		theta1 = 0;
	}
	if (theta2 > 180) {
		theta2 = 180;
	} else if (theta2 < 0) {
		theta2 = 0;
	}

	goToLowLink(theta1);
	goToHighLink(theta2);

}
void makeTriangle() {
	changeADC(2);

	int angle1Deg = (getADC(2) * .26) - 85;

	changeADC(3);

	int angle2Deg = ((getADC(3) * .26) - 85) - 90;

	if (nextLine == 0) {
		goToBothLinks(0, 90);
		if ((angle1Deg <= 5 && angle1Deg >= -5)
				&& (angle2Deg <= 5 && angle2Deg >= -5))
			nextLine = 1;
	} else if (nextLine == 1) {
		goToBothLinks(45, 0);
		if ((angle1Deg <= 50 && angle1Deg >= 40) && (angle2Deg <= 5))
			nextLine = 2;
	} else if (nextLine == 2) {
		goToBothLinks(45, 90);
		if ((angle1Deg <= 50 && angle1Deg >= 40)
				&& (angle2Deg <= 5 && angle2Deg >= -5))
			nextLine = 3;
	} else if (nextLine == 3) {
		goToBothLinks(0, 90);
		if ((angle1Deg <= 5 && angle1Deg >= -5)
				&& (angle2Deg <= 95 && angle2Deg >= 85))
			nextLine = 4;
	} else if (nextLine == 4) {
		stopMotors();
	}
	//printf("%d %d \n\r", angle1Deg, angle2Deg);
}

void buttonSetVoltage() {
	if (!PINBbits._P0) {

		driveLink('L', 0);

	} else if (!PINBbits._P1) {
		driveLink('L', 1024);
	} else if (!PINBbits._P2) {
		driveLink('L', -2048);
	} else if (!PINBbits._P3) {
		driveLink('L', 4095);
	} else {
		resetEncCount(0);
		driveLink('L', 0);
	}
}

void streamArmData() {
	if (!PINBbits._P0)
		goToLowLink(85);
	else {
		goToLowLink(0);
		resetEncCount(0);
	}
}
void pollHomeButton() {
	if (!PINBbits._P1) {
		homeFlag = 1;
		resetEncCount(0);
	} else
		homeFlag = 0;
}

int main(void) {
	//Enable printf() and setServo()
	initRBELib();

	//setup buttons as inputs
	DDRB &= ~((1 << PIN0) | (1 << PIN1));
	DDRA |= (1 << PIN4);

	debugUSARTInit(115200);
	initSPI();

	initTimer(0, NORMAL, 0);

	//printf("Encoder Counts\n\r");

	encInit(0);
	initADC(LOWLINK);
	setConst('L', 40, .5, .005);

	while (1) {
//		printf("while \n\r");
		//	buttonSetVoltage();
		//driveLink('L', 2048);
		//printf("%d", sampleFlag);
		//driveLink('L', 4095);
		pollHomeButton();
		streamArmData();

		if (sampleFlag) {
			printLowPotVal(getADC(LOWLINK));
			printf("Encoder Counts: %d \n\r", encCount(0));
			printf("X Axis Acceleration: %d \n\r", getAccel(0));
			printf("Y Axis Acceleration: %d \n\r", getAccel(1));
			printf("Z Axis Acceleration: %d \n\r", getAccel(2));

			sampleFlag = 0;
		}

//		printf("%f \n\r", convertAccelmV(getAccel(2)));
////				convertAccelGforce(getAccel(2)));
	}
	return 0;

}

//POT VALUES
//720 = 90 deg
//340 = 0 deg

//}

ISR(TIMER0_OVF_vect) {
	count++;

	if (count >= maxCount) {
		//printf("in");
		sampleFlag = 1;
		count = 0;
	}

//	if (count > maxCount) {
//		sampleFlag = 1;
//
//		PINAbits._P7 = 1
//
//		count = 0;
//	}
}
