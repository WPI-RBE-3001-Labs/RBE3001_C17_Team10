/*
 * timer.c
 *
 *  Created on: Jan 22, 2017
 *      Author: Tom Hagen
 */
#include <RBELib/RBELib.h>
#include <avr/io.h>

//void initTimer(int timer, int mode, unsigned int comp) {
//	cli();
//
//	switch (timer) {
//	case 0:
//		//Initialize timer to 0
//		TCNT0 = 0;
//
//		TCCR0B = (1 << CS02) | 	//prescaler 1024
//				(0 << CS01) | 	//prescaler 1024
//				(1 << CS00); 	//prescaler 1024
//
//		//enable Timer overflow interrupt:
//		TIMSK0 = (1 << TOIE0);
//
//		//Enable Timer interrupt flag
//		TIFR0 = (1 << TOV0);
//		break;
//	case 1:
//		//Initialize timer to 0
//		TCNT1 = 0;
//
//		TCCR1B = (1 << CS02);// | 	//prescaler 1024
//				//(1 << CS01); | 	//prescaler 1024
//						//(1 << CS00); 	//prescaler 1024
//
//		//enable Timer overflow interrupt:
//		TIMSK1 = (1 << TOIE1);
//
//		//Enable Timer interrupt flag
//		TIFR1 = (1 << TOV1);
//		break;
//	case 2:
//		//Initialize timer to 0
//		TCNT2 = 0;
//
//		TCCR2B = (1 << CS02) |	//prescaler 1024
//				(0 << CS01) |	//prescaler 1024
//				(1 << CS00);	//prescaler 1024
//
//		//enable Timer overflow interrupt:
//		TIMSK2 = (1 << TOIE2);
//
//		//Enable Timer interrupt flag
//		TIFR2 = (1 << TOV2);
//		break;
//	}
//
//	if (mode == CTC) {
//		//Enable CTC mode
//		switch (timer) {
//		case 0:
//
//			TCCR0A = (1 << WGM01) |	//CTC mode
//					(0 << WGM00);	//CTC mode
//
//			TCCR0B = (0 << WGM02);	//CTC mode
//
//			OCR0A = comp; //Set comparison value
//
//			//Enable timer interrupts for CTC mode
//			TIMSK0 = (1 << OCIE0A);
//
////			//Clear timer compare match flag
//			TIFR0 = (1 << OCF0A);
//			break;
//		case 1:
//			TCCR1A = (0 >> WGM11) | 	//Set mode to CTC
//					(0 >> WGM10); 		//Set mode to CTC
//			TCCR1B = (0 >> WGM13) | //Set mode to CTC
//					(1 >> WGM12);
//
//			OCR1A = comp;		//Set comparison value
//
//			TIMSK1 = (1 << OCIE1A); //Enable timer interrupts for CTC mode
//
//			TIFR1 = (1 << OCF1A);	//Clear timer compare match flag
//
//			break;
//		case 2:
//
//			TCCR2A = (1 >> WGM21) |	//Set mode to CTC
//					(0 >> WGM20);	//Set mode to CTC
//			TCCR2B = (0 >> WGM12);	//Set mode to CTC
//
//			OCR2A = comp;		//Set comparison value
//
//			//Enable timer interrupts for CTC mode
//			TIMSK2 = (1 << OCIE2A);
//
//			//Clear timer compare match flag
//			TIFR2 = (1 << OCF2A);
//			break;
//		}
//	}
////enable global interrupts
//	sei();
//}
void initTimer(int timer, int mode, unsigned int comp) {
	//if (timer == 0)
//	{
	//Enables Timer0 Interrupt overflow mode
	//TIMSK0 = 0x01;
	TCCR1A = 0x00;
	TCCR1B = 0x01;
	TIMSK1 = 0x01;
	SREG |= 0x80;
	//TCNT0=0x00;
	//Sets 256 prescalar
//	TCCR0B |= (1 << CS01);		//|(1 << CS00);

	//Sets all bits LOW
	//OCR0A = 0x00;

	//Sets Clock with no prescaling
	//TCCR0B = 0x01;
	// prescale 256
	//TCCR0B = (1 << CS01)|(1 << CS00);

//	}
}


void setCompValue(unsigned char timer, unsigned short int comp) {
	OCR0A = comp; //Set comparison value
}

