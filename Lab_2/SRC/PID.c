/*
 * PID.c
 *
 *  Created on: Jan 29, 2017
 *      Author: ADMIN
 */

#include <RBELib/RBELib.h>
#include <math.h>

#defne ABS_MAX_OUTPUT 4095 // As we can't go higher in our DAC

pidConst pidConsts;

Kp_H ; // HERE we set the PID
Ki_H ; // values for the motor
Kd_H ; // on the higher link

Kp_L ; // HERE we set the PID
Ki_L ; // values for the motor
Kd_L ; // on the lower link

int error = setPoint - actPos; // error calculation
double errSum
double lasterr

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


signed int calcPID(char link, int setPoint, int actPos);



double dErr = (error - lastErr) / timeChange; // D error
Output = kp * error + ki * errSum + kd * dErr; //Out put of calculated PID
lastErr = error;  //things needed for the re-run




