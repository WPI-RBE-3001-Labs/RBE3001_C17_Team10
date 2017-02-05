/*
 * forwardKinematics.c
 *
 *  Created on: Feb 5, 2017
 *      Author: thagen
 */
#include <math.h>

float x = 0;
float y = 0;
float degToRad(int degree) {
	return (degree * (3.14159 / 180));
}
void printXY() {
	changeADC(2);
	float angle1 = degToRad((getADC(2) * .26) - 85);

	changeADC(3);
	float angle2 = degToRad(((getADC(3) * .26) - 85) - 90);

	x = 25 * cos(angle1) + 22 * cos(angle1 + angle2);
	y = 25 * sin(angle1) + 22 * sin(angle1 + angle2) + 19;	//mm

	printf("%f %f \n\r", x, y);
}

