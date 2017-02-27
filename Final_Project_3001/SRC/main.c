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
int linkAngle(int angle) {
	return (angle + 85) / .26;
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

int inRange(int angle1, int angle2) {
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

	if (inRange(theta1, theta2))
		stopMotors();

}

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

float analyzeWeight() {
	changeADC(1);
	int currVal = getADC(1);
	float curr = calcCurrent(currVal);
	return curr;
	printf("%f \n\r", curr);

}
void cg() {
	setServo(0, 180);
}
void og() {
	setServo(0, 0);
}
void bringWeightUp() {
	goToBothLinks(90, 0);
}
void ggEZ() {
	goToBothLinks(90, 90);
}

enum state {
	start,
	getXD,
	goToXD,
	pickUPWeight,
	ANALyzeWeight,
	heavyWeight,
	lightWeight,
	celebrate
};
enum timerFlag {
	goDown, closeGrip, bringUp, openGrip, none
};
int main(void) {
//Enable printf() and setServo()
	initRBELib();

//setup buttons as inputs
	DDRB &= ~(1 << PIN0);

	debugUSARTInit(115200);
	initSPI();

//	initTimer(0, NORMAL, 0);

//printf("Encoder Counts\n\r");

	initADC(LOWLINK);
	initADC(HIGHLINK);
	initADC(4);
	initADC(1);
	initTimer(0, NORMAL, 0);
	timerFlag = none;
	setConst('L', 40, .5, .005);
	setConst('H', 40, 0, 0);

	state = start;
	setServo(5, 90);
	og();
	while (1) {
		/*stopMotors();
		 og();
		 */
		//setServo(5, 114);
		printf("%d %d \n\r", count, closeGripMaxCount);
		switch (state) {
		case start:
			gotoXY(0, 31);
			if (!PINBbits._P0) {

				setServo(5, 114);

				state = getXD;

			}
			break;
		case getXD:
			stopMotors();
			setServo(0, 0);
			changeADC(4);
			if (IRDist(4) > 6 && IRDist(4) < 14) {

				xIR = IRDist(4) + 20;
				//printf("%d %d \n\r", IRDist(4), xIR);
				state = goToXD;
				//printf("GO TO XD");
			}

			break;
		case goToXD:
			timerFlag = goDown;

			if (goDownFlag) {
				//printf("%d \n\r", xIR);
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
			//printf("Pick up weight");

			break;

		case pickUPWeight:
			timerFlag = bringUp;
			cg();

			if (bringUpFlag)
				bringWeightUp();
			//			if (closeGripFlag) {
			//
			//			} else {
			//				//	bringWeightUp();
			//			}

			break;

		case ANALyzeWeight:
			if (analyzeWeight() > WEIGHTTHRESHOLD)
				state = heavyWeight;
			else

				state = lightWeight;
			break;

		case heavyWeight:
			//goBackDown();
			timerFlag = openGrip;
			if (openGripFlag) {
				stopMotors();
				og();
				state = celebrate;
			}

			break;

		case lightWeight:
			stopMotors();
			og();
			state = celebrate;
			break;

		case celebrate:
			ggEZ();
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
			//printf("in\n\r");
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
			//printf("in");
			bringUpFlag = 1;
			timerFlag = none;

		}
		break;
	case openGrip:
		if (count >= openGripMaxCount) {
			//printf("in");
			openGripFlag = 1;
			timerFlag = none;
		}
		break;
	case none:
		count = 0;
		break;
	}
}

