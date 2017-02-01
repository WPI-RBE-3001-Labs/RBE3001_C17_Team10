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
double dErrHIGH = 0;
double errSumHIGH = 0;
double lastErrHIGH = 0;

int errorLOW = 0;
double dErrLOW = 0;
double errSumLOW = 0;
double lastErrLOW = 0;

signed int outputLOW = 0;
signed int outputHIGH = 0;

void setConst(char link, float Kp, float Ki, float Kd) {
switch(link){
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
	long error = actPos - setPoint;

	if (link == 'H') {
		dErrHIGH = (error - lastErrHIGH) / timePassed;
		errSumHIGH += pidConsts.Ki_H * error; //running sum of the errors multiplied by the Ki constant
		outputHIGH = pidConsts.Ki_H * errSumHIGH + pidConsts.Kd_H * dErrHIGH
				+ pidConsts.Kp_H * error; //Output of calculated PID
		lastErrHIGH = error;  				//things needed for the re-run
		return outputHIGH;
	}
	else {
		dErrLOW = (error- lastErrLOW) / timePassed;
		errSumLOW += pidConsts.Ki_H * error; //running sum of the errors multiplied by the Ki constant
		outputLOW = pidConsts.Ki_L * errSumLOW + pidConsts.Kd_L * dErrLOW
				+ pidConsts.Kp_L * error; //Output of calculated PID
		lastErrLOW = error;  				//things needed for the re-run
		return outputLOW;
	}


}

ISR(TIMER0_COMPA_vect) {
	//Some arbitrary set point for now
	//calcPID('L', 0, getADC(2));

}

