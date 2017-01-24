/*
 * timer.c
 *
 *  Created on: Jan 22, 2017
 *      Author: Tom Hagen
 */
#include <RBELib/RBELib.h>
#include <avr/io.h>
void initTimer(int timer, int mode, unsigned int comp) {
	//CLK SOURCE = 32,768 Hz
	//clear all interrupts while setting up the clock

	cli();

	//Initialize timer to 0
	TCNT0 = 0;

	//CLOCK PRESCALER of 1024
	//TCCR0B = (1 << CS02) | (1 << CS00);

	if (mode == NORMAL) {

		/* THIS ONLY SETS UP TIMER 0 FOR THE LAB, LATER NEED TO IMPLEMENT ALL TIMERS*/
		//Initialize timer to 0
		TCNT0 = 0;
		//disable prescaler
		//Prescaler of 64
		TCCR0B = (1 << CS01) | (1 << CS00);
		//enable Timer overflow interrupt:
		TIMSK0 = (1 << TOIE0);
//		//Enable Timer interrupt flag
//		TIFR0 = (1 << TOV0);

	} else if (mode == CTC) {

		//Set mode to CTC and clear on compare match
		TCCR0A = (1 << COM0A1) | (1 << WGM01);

		//calculation for frequency for compare value
		// Fcomp = Fclk/(2*Prescaler(1+ compare value))

		OCR0A = comp; //Set comparison value

		//Enable timer interrupts for CTC mode
		TIMSK0 = (1 << OCIE0A);
		//enable overflow interrupt
		TIMSK0 = (1 << TOIE0);
		//Clear timer compare match flag and enable timer interrupt flag
		//TIFR0 = (1 << OCF0A) | (1 << TOV0);
	}
	//enable global interrupts
	sei();

}

void setCompValue(unsigned char timer, unsigned short int comp) {
	OCR0A = comp; //Set comparison value
}

