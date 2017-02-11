/*
 * forwardKinematics.c
 *
 *  Created on: Feb 5, 2017
 *      Author: thagen
 */
#include <math.h>

//float x = 0;
//float y = 0;
float degToRad(int degree) {
	return ((float) degree * (2 * M_PI / 360));
}
//void printXY() {
//	changeADC(2);
//
//	int angle1Deg = (getADC(2) * .26) - 85;
//	float angle1 = degToRad(angle1Deg);
//
//	changeADC(3);
//
//	int angle2Deg = ((getADC(3) * .26) - 85) - 90;
//	float angle2 = degToRad(angle2Deg);
//
//	x = (15 * cos(angle1)) + (12 * cos(angle1 + angle2));
//	y = 13 + (15 * sin(angle1)) + (12 * sin(angle1 + angle2));	//cm
//
//	//printf("%f %f \n\r", angle1, angle2);
//
//	printf("%d %d %f %f \n\r", angle1Deg, angle2Deg, x, y);
//	//printf("%f %f \n\r", x, y);
//}

