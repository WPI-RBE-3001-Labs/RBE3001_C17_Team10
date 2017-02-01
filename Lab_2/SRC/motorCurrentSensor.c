/*
 * motorCurrentSensor.c
 *
 *  Created on: Feb 1, 2017
 *      Author: Tom
 */
float offset = 0;
float gain = 0;
//takes an input from an adc, need to calculate the current from the motor


float calcCurrent(int val) {
	//First convert the reading to voltage
	//
	//The resistor across the sensor is .5Ohms
	float curr = (val * (5 / 1023) + offset) / gain / (.5);
	return curr;
}
