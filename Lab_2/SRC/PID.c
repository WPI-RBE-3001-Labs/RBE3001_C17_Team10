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

int error;
double dErr;
double errSum;
double lastErr;

void setConst(char link, float Kp, float Ki, float Kd) {
	if (link == 'High') {			//Sets PID constants for higher link
		pidConsts.Kp_H = Kp;
		pidConsts.Ki_H = Ki;
		pidConsts.Kd_H = Kd;
	} else {   						//Sets PID constants for Lower link
		pidConsts.Kp_L = Kp;
		pidConsts.Ki_L = Ki;
		pidConsts.Kd_L = Kd;
	}
}

//signed int calcPID(char link, int setPoint, int actPos) {
//	error = setPoint - actPos; // error calculation
//	dErr = (error - lastErr) / timeChange; 			// D error
//	output = kp * error + ki * errSum + kd * dErr; 	//Output of calculated PID
//	lastErr = error;  							//things needed for the re-run
//}

