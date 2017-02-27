/*
 * forwardKinematics.c
 *
 *  Created on: Feb 5, 2017
 *      Author: thagen
 */
#include <math.h>

#define link1 15
#define link2 16

#define deg2Rad 180/(M_PI)
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
int getTheta1(int x, int y) {

	float gamma = 0;
	float beta = 0;

	float theta2 = 0;
	float theta1[2];
	float point1[2];
	float point2[2];
	float actTheta1 = 0;

	float xsq = x * x;
	float ysq = y * y;
	float l1sq = link1 * link1;
	float l2sq = link2 * link2;
	float l12 = link1 * link2;
	float a = xsq + ysq - l1sq - l2sq;
	float b = 2 * l12;
	float ab = (a / b);

	theta2 = -acos(ab);

	beta = atan2(y, x);
	gamma = acos((l1sq + xsq + ysq - l2sq) / (2 * link1 * sqrt(xsq + ysq)));

	theta1[0] = (beta + gamma);
	theta1[1] = (beta - gamma);

	point1[0] = round(
	link1 * cos(theta1[0]) + link2 * cos(theta1[0] + theta2));
	point1[1] = round(
	link1 * sin(theta1[0]) + link2 * sin(theta1[0] + theta2));

	point2[0] = round(link1 * cos(theta1[1]) + link2 * cos(theta1[1] + theta2));
	point2[1] = round(link1 * sin(theta1[1]) + link2 * sin(theta1[1] + theta2));

	if (point1[0] == x && point1[1] == y)
		actTheta1 = theta1[0] * deg2Rad;
	else if ((point2[0] == x) && (point2[1] == y))
		actTheta1 = theta1[1] * deg2Rad;
	else
		printf("No Angles to be made");
	theta2 *= deg2Rad;
	//printf("%d \n\r", (int) actTheta1);
	return (int) actTheta1;

}

int getTheta2(int x, int y) {

	float gamma = 0;
	float beta = 0;

	float theta2 = 0;
	float theta1[2];
	float point1[2];
	float point2[2];
	float actTheta1 = 0;

	float xsq = x * x;
	float ysq = y * y;
	float l1sq = link1 * link1;
	float l2sq = link2 * link2;
	float l12 = link1 * link2;
	float a = xsq + ysq - l1sq - l2sq;
	float b = 2 * l12;
	float ab = (a / b);

	theta2 = -acos(ab);

	beta = atan2(y, x);
	gamma = acos((l1sq + xsq + ysq - l2sq) / (2 * link1 * sqrt(xsq + ysq)));

	theta1[0] = (beta + gamma);
	theta1[1] = (beta - gamma);

	point1[0] = round(
	link1 * cos(theta1[0]) + link2 * cos(theta1[0] + theta2));
	point1[1] = round(
	link1 * sin(theta1[0]) + link2 * sin(theta1[0] + theta2));

	point2[0] = round(link1 * cos(theta1[1]) + link2 * cos(theta1[1] + theta2));
	point2[1] = round(link1 * sin(theta1[1]) + link2 * sin(theta1[1] + theta2));

	if (point1[0] == x && point1[1] == y)
		actTheta1 = theta1[0] * deg2Rad;
	else if ((point2[0] == x) && (point2[1] == y))
		actTheta1 = theta1[1] * deg2Rad;
	else
		printf("No Angles to be made");
	theta2 *= deg2Rad;

	if (theta2 >= 0) {
		theta2 += 90;
	} else {
		theta2 = 90 + theta2;
	}

	return (int) theta2;
}
