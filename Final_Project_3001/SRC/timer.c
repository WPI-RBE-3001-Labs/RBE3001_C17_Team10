/*
 * timer.c
 *
 *  Created on: Jan 22, 2017
 *      Author: Tom Hagen
 */
#include <RBELib.h>
#include <avr/io.h>

void initTimer(int number, int mode, unsigned int comp) {
	//disable global interrupts while we set up the timer
	cli();

	switch (number) {
	case 0:
		//Initialize timer to 0
		TCNT0 = 0;
		//disable prescaler
		//TCCR0B = (1 << CS00);
		TCCR0B = (1 << CS01) | (1 << CS00);
		//enable Timer overflow interrupt:
		TIMSK0 = (1 << TOIE0);
		//Enable Timer interrupt flag
		TIFR0 = (1 << TOV0);
		break;
	case 1:
		//Initialize timer to 0
		TCNT1 = 0;
		//disable prescaler
		TCCR1B = (1 << CS10);
		//enable Timer overflow interrupt:
		TIMSK1 = (1 << TOIE1);
		//Enable Timer interrupt flag
		TIFR1 = (1 << TOV1);
		break;
	case 2:
		//Initialize timer to 0
		TCNT2 = 0;
		//disable prescaler
		TCCR2B = (1 << CS20);
		//enable Timer overflow interrupt:
		TIMSK2 = (1 << TOIE2);
		//Enable Timer interrupt flag
		TIFR2 = (1 << TOV2);
		break;
	}
	if (mode == CTC) {
		//Enable CTC mode
		switch (number) {
		case 0:
			//Set mode to CTC
			TCCR0A = (1 << WGM01);
			TCCR0B = (0 << WGM02);
			OCR0A = comp; //Set comparison value
			//Enable timer interrupts for CTC mode
			TIMSK0 = (1 << OCIE0A);
			//Clear timer compare match flag
			TIFR0 = (1 << OCF0A);
			break;
		case 1:
			//Set mode to CTC
			TCCR1B = (1 << WGM12);

			OCR1A = comp;			//Set comparison value
			//Enable timer interrupts for CTC mode
			TIMSK1 = (1 << OCIE1A);
			//Clear timer compare match flag
			TIFR1 = (1 << OCF1A);
			break;
		case 2:
			//Set mode to CTC
			TCCR2A = (1 << WGM21);
			OCR2A = comp;			//Set comparison value
			//Enable timer interrupts for CTC mode
			TIMSK2 = (1 << OCIE2A);

			//Clear timer compare match flag
			TIFR2 = (1 << OCF2A);
			break;
		}
	}
	//enable global interrupts
	sei();
}

/*
 * Sets a timers comparison A value. Only has an effect for timers in CTC mode.
 */
void setCompValue(unsigned char number, unsigned short int comp) {
	switch (number) {
	case 0:
		OCR0A = comp; //Set comparison value
		break;
	case 1:
		OCR1A = comp; //Set comparison value
		break;
	case 2:
		OCR2A = comp; //Set comparison value
		break;
	}
}

