/*
 * PID.c
 *
 *  Created on: Jan 29, 2017
 *      Author: ADMIN
 */

#include <RBELib/RBELib.h>
#include <math.h>

#define ABS_MAX_OUTPUT 4095 // As we can't go higher in our DAC

pidConst pidConsts;

int timePassed = 0;

int errorHIGH = 0;
float dErrHIGH = 0;
float errSumHIGH = 0;
float lastErrHIGH = 0;

int errorLOW = 0;
float dErrLOW = 0;
float errSumLOW = 0;
float lastErrLOW = 0;

float outputLOW = 0;
float outputHIGH = 0;

int maxCount = 0;
int tempCount = 0;

void setConst(char link, float Kp, float Ki, float Kd) {
	switch (link) {
	case 'H': //Sets PID constants for higher link
		pidConsts.Kp_H = Kp;
		pidConsts.Ki_H = Ki;
		pidConsts.Kd_H = Kd;
		break;

	case 'L':
		//Sets PID constants for Lower link
		pidConsts.Kp_L = Kp;
		pidConsts.Ki_L = Ki;
		pidConsts.Kd_L = Kd;
		break;
	}
}
signed int calcPID(char link, int setPoint, int actPos) {
	float error = actPos - setPoint;
	//printf("%f \n\r", error);
	if (link == 'H') {
		dErrHIGH = (error - lastErrHIGH) / timePassed;
		errSumHIGH += pidConsts.Ki_H * error; //running sum of the errors multiplied by the Ki constant
		outputHIGH = pidConsts.Ki_H * errSumHIGH + pidConsts.Kd_H * dErrHIGH
				+ pidConsts.Kp_H * error; //Output of calculated PID
		lastErrHIGH = error; //things needed for the re-run
		return outputHIGH;
	} else {
		//dErrLOW = (error - lastErrLOW) / timePassed;
		//	printf("%f \n\r", dErrLOW);
		errSumLOW += pidConsts.Ki_H * error;//running sum of the errors multiplied by the Ki constant
		outputLOW = pidConsts.Ki_L * errSumLOW + pidConsts.Kd_L * dErrLOW
				+ pidConsts.Kp_L * error;			//Output of calculated PID
				//	printf("%f \n\r", outputLOW);

		return outputLOW;
		lastErrLOW = error;			//things needed for the re-run
	}

}

void initPIDSampling() {
	maxCount = (F_CPU / 36864) * ((float) 1 / 100);
	tempCount = 0;
	initTimer(1, NORMAL, 36864);
}

ISR( TIMER1_OVF_vect) {
	printf("we in");
	if (tempCount >= maxCount) {

		//calculate the pid output for the arm according to the actual position
		//driveLink(1, calcPID('L', 0, getADC(2)));
		tempCount = 0;
	} else {
		tempCount++;
	}
}

