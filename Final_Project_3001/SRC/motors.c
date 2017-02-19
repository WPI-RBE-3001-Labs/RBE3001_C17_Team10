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
#define link2 12

#define deg2Rad 360/(2*M_PI)

/**
 * @brief Helper function to stop the motors on the arm.
 *
 * @todo Create way to stop the motors using the DAC.
 */
//
//double link1 = 15;
//double link2 = 12;
//double angle1 = 0;
//double angle2 = 0;
//float x = 0;
//float y = 0;
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
//void gotoAngles(int lowerTheta, int upperTheta) {
//
//	x = (15 * cos(lowerTheta)) + (12 * cos(lowerTheta + upperTheta));
//	y = 13 + (15 * sin(lowerTheta)) + (12 * sin(lowerTheta + upperTheta));
//
//}
/**
 * @brief Drive the end effector of the arm to a desired X and Y position in the workspace.
 *
 * @param x The desired x position for the end effector.
 * @param y The desired y position for the end effector.
 *
 * @todo Use kinematic equations to move the end effector to the desired position.
 */
void gotoXY(int x, int y) {
	//angle 1 is the low link angle
	//angle 2 is the high link angle

	float angle1, angle2 = 0;

	float xsq = x * x;
	float ysq = y * y;

	float l1sq = link1 * link1;
	float l2sq = link2 * link2;
//------------------------------------------------------------------------------------------
	angle1 = atan(y / x)
			+ acos((xsq + ysq + l1sq - l2sq) / (2 * link1 * sqrt(xsq + ysq)));

	angle1 = (int) (angle1 * deg2Rad) + 135;
//------------------------------------------------------------------------------------------
	angle2 = -acos(((l1sq + l2sq) - (xsq + ysq)) / (2 * link1 * link2));

	angle2 = (int) (deg2Rad * angle2) + 270;
//------------------------------------------------------------------------------------------

	printf("%f %f \n\r", angle1, angle2);

	goToBothLinks(angle1, angle2);
	//goToBothLinks(35, 80);

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
			setDAC(2, 0);
			setDAC(3, 0);
		} else {
			setDAC(0, -value);
			setDAC(1, 0);
			setDAC(2, 0);
			setDAC(3, 0);
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
