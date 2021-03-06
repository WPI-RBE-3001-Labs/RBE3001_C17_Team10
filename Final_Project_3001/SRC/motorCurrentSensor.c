/*
 * motorCurrentSensor.c
 *
 *  Created on: Feb 1, 2017
 *      Author: Tom
 */
float offset = 2.65;
float gain = 20.0;
float resistorVal = .05;
float VCC = 5.0;
float ADCMAX = 1023;
//takes an input from an adc, need to calculate the current from the motor

float calcCurrent(int val) {
	//First convert the reading to voltage
	//
	//The resistor across the sensor is .5Ohms
	float curr = -200.0
			+ ((float) 100.0 * (val * VCC / ADCMAX) - offset) / gain
					/ resistorVal;
	//printf("%f \n\r", curr);
	return curr;
}
