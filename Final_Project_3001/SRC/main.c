/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: Joest
 */

#include <RBELib/RBELib.h>
//For use of abs()
#include <stdlib.h>
#include <avr/io.h>

#define LOWLINK 2
#define HIGHLINK 3
#define CURRSENSOR 0
//100 Hz = 45
//Going down takes 1s, need to wait 8 seconds before going down
#define goDownMaxCount 8*45*600
//Some time to grab weight when under the gripper, AKA Trial and error
#define bringUpMaxCount 4*500
//Goes down for 1s then only drop the weight when we need to
#define openGripMaxCount 45*500*1.1
#define closeGripMaxCount 40*50*1.025
#define weighMaxCount 40*500*1.025

#define WEIGHTTHRESHOLD 1500

int timestamp = 0;
volatile int count = 0;

int homeFlag = 0;
volatile int sampleFlag = 0;

//value to get 100Hz
int maxCount = 45;
int armState = 0;

//next line for triangle
int nextLine = 0;

////////////////////////////////
//Variables and Flags
int pickupFlag = 1;
int state;
int xIR = 0;
int timerFlag;
volatile int goDownFlag = 0;
volatile int bringUpFlag = 0;
volatile int openGripFlag = 0;
volatile int getIRFlag = 0;
int IRCount = 0;
volatile int closeGripFlag = 0;
int flatCase = 0;
volatile int weighFlag = 0;
int currents[100];
long currVal = 0;
int reRunFlag = 0;
int previousCurrent = 0;
////////////////////////////

int linkAngle(int angle) {
	return (angle + 85) / .26;
}
//This Function controls the lower link
void goToLowLink(int setPos) {
	int potVal = linkAngle(setPos);

	changeADC(LOWLINK);

	int actPos = getADC(LOWLINK);

	int value = calcLowPID(potVal, actPos);

	driveLink('L', value);
}
//Controls the higher Link
void goToHighLink(int setPos) {
	int potVal = linkAngle(setPos);

	changeADC(HIGHLINK);

	int actPos = getADC(HIGHLINK);

	int value = calcHighPID(potVal, actPos);

	driveLink('H', value);
}
//Helper function to determine if the arm is in the range of angles it needs to be
int inRange(int angle1, int angle2) {
	int val = 0;
	changeADC(2);

	int angle1Deg = (getADC(2) * .26) - 85;

	changeADC(3);

	int angle2Deg = ((getADC(3) * .26) - 85) - 90;
	//printf("%d %d \n\r", angle1Deg, angle2Deg);
	if ((angle1Deg <= (angle1 + 5)) && (angle1Deg >= (angle1 - 5))
			&& (angle2Deg <= (angle2 + 5)) && angle2Deg >= (angle2 - 5))
		val = 1;

	return val;
}
int inRangeToGrip(int angle1, int angle2) {
	int val = 0;
	changeADC(2);

	int angle1Deg = (getADC(2) * .26) - 85;

	changeADC(3);

	int angle2Deg = ((getADC(3) * .26) - 85) - 90;

	if ((angle1Deg <= (angle1 + 5)) && (angle1Deg >= (angle1 - 5))
			&& (angle2Deg <= (angle2 + 5)) && angle2Deg >= (angle2 - 5))
		val = 1;

	return val;
}
//Comnination of link movement, uses forward kinematics
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

//	if (inRange(theta1, theta2))
//		stopMotors();

}
//
void pickupWeight() {
	changeADC(2);

	int angle1Deg = (getADC(2) * .26) - 85;

	changeADC(3);

	int angle2Deg = ((getADC(3) * .26) - 85) - 90;

	goToBothLinks(90, 0);
	if ((angle1Deg <= 95 && angle1Deg >= 85)
			&& (angle2Deg <= 5 && angle2Deg >= -5))
		pickupFlag = 0;
}

//Determines the weight of the block using the current sensor for the higher link
//Takes the average  current when the block is held
long analyzeWeight() {
	changeADC(1);

	for (int i = 0; i < 100; i++)
		currents[i] = getADC(1);

	for (int i = 0; i < 100; i++)
		currVal += currents[i];

	currVal = currVal / 100;
	//printf("%f \n\r", calcCurrent(currVal));
	return currVal;

}
//Goes to the position to put the block back down on the belt
void goBackDown() {
	goToBothLinks(15, 60);
}
//Closes the gripper
void cg() {
	setServo(0, 180);
}
//Open Gripper
void og() {
	setServo(0, 0);
}
void bringWeightUp() {
	goToBothLinks(90, 0);
}

//States for the state machine
enum state {
	start,
	getXD,
	goToXD,
	pickUPWeight,
	weighWeight,
	garbageCurrent,
	heavyWeight,
	lightWeight
};
//Timer flags to determine what to do in the isr
enum timerFlag {
	goDown, closeGrip, bringUp, openGrip, weigh, none
};
int main(void) {
//Enable printf() and setServo()
	initRBELib();

//setup buttons as inputs
	DDRB &= ~(1 << PIN0);

	debugUSARTInit(115200);
	initSPI();

	initADC(LOWLINK);
	initADC(HIGHLINK);
	initADC(4);
	initADC(1);
	initADC(0);

	initTimer(0, 0, 0);
	timerFlag = none;
	setConst('L', 40, .5, .005);
	setConst('H', 40, 0, 0);

	state = start;
	setServo(5, 90);
	og();
	while (1) {

		//Start of State Machine
		switch (state) {
		case start:
			gotoXY(0, 31);
			setServo(5, 113);
			if (inRange(90, 0)) {
				state = getXD;
			}
			break;

		case getXD:
			stopMotors();
			setServo(0, 0);
			changeADC(4);
			if (IRDist(4) > 6 && IRDist(4) < 14) {
				xIR = IRDist(4) + 20;
				state = goToXD;
			}

			break;
		case goToXD:
			timerFlag = goDown;
			printf("%d %d \n\r", IRDist(4), xIR);
			if (goDownFlag) {
				if (xIR >= 32) {
					goToBothLinks(0, 90);
					flatCase = 1;
				} else if (xIR == 31) {
					goToBothLinks(16, 61);
					timerFlag = closeGrip;
				} else {
					gotoXY(xIR, 2);
					timerFlag = closeGrip;
				}
			}

			if (closeGripFlag) {
				printf("here \n\r");
				state = pickUPWeight;
			}
			if (inRangeToGrip(getTheta1(xIR, 2), getTheta2(xIR, 2))
					&& flatCase) {
				stopMotors();
				state = pickUPWeight;
			}
			break;

		case pickUPWeight:
			timerFlag = bringUp;
			cg();
			if (bringUpFlag) {
				bringWeightUp();
				if (inRange(90, -90)) {
					state = weighWeight;
				}
			}
			break;

		case weighWeight:
			bringWeightUp();
			printf("%ld \n\r", analyzeWeight());
			timerFlag = weigh;
			if (weighFlag) {
				int currentReading = analyzeWeight();
				if ((previousCurrent >= 480 && previousCurrent <= 510)
						|| previousCurrent == 0)
					state = garbageCurrent;
				else if (previousCurrent < 480)
					state = heavyWeight;
				else
					state = lightWeight;

				previousCurrent = currentReading;
			}

			break;

		case garbageCurrent:
			state = weighWeight;
			weighFlag = 1;
			break;

		case heavyWeight:
			goBackDown();
			printf("heavy \n\r");
			if (inRange(15, -30)) {
				stopMotors();
				og();
				reRunFlag = 1;
				goDownFlag = 0;
				bringUpFlag = 0;
				openGripFlag = 0;
				getIRFlag = 0;
				closeGripFlag = 0;
				weighFlag = 0;
				state = start;
			}

			break;

		case lightWeight:
			printf("Light \n\r");
			og();
			reRunFlag = 1;

			goDownFlag = 0;
			bringUpFlag = 0;
			openGripFlag = 0;
			getIRFlag = 0;
			closeGripFlag = 0;
			weighFlag = 0;
			count = 0;
			state = start;

			break;

		}

	}
	return 0;

}

ISR(TIMER0_OVF_vect) {
	count++;
	switch (timerFlag) {
	case goDown:
		if (count >= goDownMaxCount) {
			goDownFlag = 1;
			timerFlag = none;
		}
		break;
	case closeGrip:
		if (count >= closeGripMaxCount) {
			closeGripFlag = 1;
			putCharDebug('a');
			timerFlag = none;
		}
		break;

	case bringUp:
		if (count >= bringUpMaxCount) {
			bringUpFlag = 1;
			timerFlag = none;

		}
		break;
	case openGrip:
		if (count >= openGripMaxCount) {
			openGripFlag = 1;
			timerFlag = none;
		}
		break;
	case weigh:
		if (count >= weighMaxCount) {
			weighFlag = 1;
			timerFlag = none;
		}
		break;
	case none:
		count = 0;
		break;
	}
}

