/*
 * Ports.c
 *
 *  Created on: Jan 25, 2017
 *      Author: Tom Hagen
 */
#include <avr/io.h>
#include <RBELib/RBELib.h>
void setPinsDir(char port, int dir, char numPins, ...) {
	switch (port) {
	case 'A':
		while (numPins != 0) {
			if (dir)
				DDRA |= (dir << 1);
			else
				DDRA &= ~(dir << 0);

		}
		break;
	case 'B':
		while (numPins != 0) {
			if (dir)
			DDRB |= (dir << 1);
			else
			DDRB &= ~(dir << 0)

		}
		break;
	case 'C':
		while (numPins != 0) {
			if (dir)
			DDRC |= (dir << 1);
			else
			DDRC &= ~(dir << 0)

		}
		break;
	case 'D':
		while (numPins != 0) {
			if (dir)
			DDRD |= (dir << 1);
			else
			DDRD &= ~(dir << 0)

		}
		break;

	}
}
unsigned char getPinsVal(char port, int numPins, ...) {
	return 'a';
}
void setPinsVal(char port, int val, int numPins, ...) {
}
