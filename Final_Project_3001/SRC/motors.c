/*
 * motors.c
 *
 *  Created on: Feb 1, 2017
 *      Author: Tom
 */
#include <RBELib/RBELib.h>
#include <avr/io.h>
#include <math.h>

#define link1 15
#define link2 16

#define deg2Rad 180/(M_PI)

void stopMotors() {
	setDAC(0, 0);
	setDAC(1, 0);
	setDAC(2, 0);
	setDAC(3, 0);
}

/**
 * @brief Drive the arm to a desired angle
 *
 * @param lowerTheta The desired angle for the lower link.
 * @param upperTheta The desired angle for the upper link.
 *
 * @todo Make a way to drive the links to a desired angle.
 */
void gotoAngles(int lowerTheta, int upperTheta) {

//	x = (15 * cos(lowerTheta)) + (12 * cos(lowerTheta + upperTheta));
	//y = 13 + (15 * sin(lowerTheta)) + (12 * sin(lowerTheta + upperTheta));

}
/**
 * @brief Drive the end effector of the arm to a desired X and Y position in the workspace.
 *
 * @param x The desired x position for the end effector.
 * @param y The desired y position for the end effector.
 *
 * @todo Use kinematic equations to move the end effector to the desired position.
 */
void gotoXY(int x, int y) {

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
	theta2 *= deg2Rad;

	if (theta2 >= 0) {
		theta2 += 90;
	} else {
		theta2 = 90 + theta2;
	}

	goToBothLinks((int) actTheta1, (int) theta2);

}
/**
 * @brief Drive a link (upper or lower) in a desired direction.
 *
 * @param link Which link to control.
 * @param dir Which way to drive the link.
 *
 * @todo Create a way to drive either link in any direction.
 */

//0 is Low link
//1 is high link
void driveLink(char link, int value) {
	if (value > 4095) {
		value = 4095;
	} else if (value < -4095) {
		value = -4095;
	}

	switch (link) {

	case 'L':
		if (value >= 0) {
			setDAC(0, 0);
			setDAC(1, value);
		} else {
			setDAC(0, -value);
			setDAC(1, 0);
		}
		break;

	case 'H':
		if (value >= 0) {
			setDAC(2, value);
			setDAC(3, 0);
		} else {
			setDAC(2, 0);
			setDAC(3, -value);
		}
		break;

	}
}

/**
 * @brief Drive the arm to a "home" position using the potentiometers.  This
 * should be called before using the encoders and just goes to a default position.
 * Once this has been called once, you can initialize/clear the encoders.
 *
 * @todo Drive the arm to a known position using the potentiometers.
 */
void homePos() {

}
