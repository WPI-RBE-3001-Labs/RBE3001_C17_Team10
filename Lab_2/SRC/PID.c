/*
 * PID.c
 *
 *  Created on: Jan 29, 2017
 *      Author: ADMIN
 */

#include <RBELib/RBELib.h>
#include <math.h>

#define KPPOT 7
#define KIPOT 6
#define KDPOT 5

pidConst pidConsts;

volatile int timePassed = 1;
volatile int tempCount = 0;

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
//	printf("%f \n\r", error);
	if (link == 'H') {
		dErrHIGH = (error - lastErrHIGH) / .01;
		errSumHIGH += pidConsts.Ki_H * error; //running sum of the errors multiplied by the Ki constant
		outputHIGH = pidConsts.Ki_H * errSumHIGH + pidConsts.Kd_H * dErrHIGH
				+ pidConsts.Kp_H * error; //Output of calculated PID
		lastErrHIGH = error; //things needed for the re-run
		return outputHIGH;
	} else {
		dErrLOW = (error - lastErrLOW) / .01;
		//	printf("%f \n\r", dErrLOW);

		errSumLOW += (.01 * error);
		//running sum of the errors multiplied by the Ki constant

		outputLOW = pidConsts.Ki_L * errSumLOW + pidConsts.Kd_L * dErrLOW
				+ pidConsts.Kp_L * error;

		//outputLOW = pidConsts.Kp_L * error;
		//printf("%f \n\r", outputLOW);
		return outputLOW;

		lastErrLOW = error;			//things needed for the re-run
	}

}
void getConsts() {
	changeADC(7);
	//0<Kp<5 ,0<Ki<1 ,0<Kd<0.5
	float p = (float) getADC(7) * 0.004888;	//0.004887585532746823069403714565;
	pidConsts.Kp_L = p;

	changeADC(6);
	float i = (float) getADC(6) / 1023;
	pidConsts.Ki_L = i;

	changeADC(5);
	float d = (float) getADC(5) * 0.0004888;// 4.8875855327468230694037145650049e-4;
	pidConsts.Kd_L = d;

	changeADC(2);

}

