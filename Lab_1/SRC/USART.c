/*
 * USARTDebug.c
 *
 *  Created on: Jan 21, 2017
 *      Author: Tom Hagen
 */
#include <RBELib/RBELib.h>
#include <avr/io.h>

void debugUSARTInit(unsigned long baudrate) {
	// Set baud rate
	UBRR1H = (F_CPU / (16 * baudrate) - 1) >> 8;
	UBRR1L = (F_CPU / (16 * baudrate) - 1);

	//Enable receiver and transmitter
	UCSR1B = (1 << RXEN1) | (1 << TXEN1);

}

void putCharDebug(char byteToSend) {
	// Wait for the buffer to empty
	while (!( UCSR1A & (1 << UDRE1)))
		;

	// Send the data
	UDR1 = byteToSend;
}

unsigned char getCharDebug(void) {
	//Wait for the data to get there
	while (!(UCSR1A & (1 << RXC1)))
		;
	// Return the data
	return UDR1;
}
