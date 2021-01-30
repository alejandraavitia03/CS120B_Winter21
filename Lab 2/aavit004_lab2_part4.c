/*	Author: Pita
 *  Partner(s) Name: Alejandra Avitia-Davila
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <stdlib.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif	

int main(void) {
		
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF; // Configure port B's 8 pins as outputs, initialize to 0s
	DDRD = 0xFF; PORTD = 0x00;

	unsigned char tmpA = 0x00, tmpB = 0x00, tmpC = 0x00, tmpD = 0x00; // Temporary variable to hold the value of A
	unsigned char totalW = 0x00;
	while(1) {
		// 1) Read input
		tmpA = PINA;
		tmpB = PINB;
		tmpC = PINC;
		//tmpD = 0x00;//RESTRT
		//tmpI = 0x00;
		totalW = tmpA + tmpB+ tmpC;
        tmpD = totalW / 15;
        tmpD = tmpD << 2; //shift lft two bits which will go to tmpI
		// 2) Permoft the computation 
		if(totalW > 140) {
			tmpD = tmpD + 0x01;
		}
        if(((tmpA - tmpC) > 0x50) || ((tmpC - tmpA) > 0x50)){
            tmpD = tmpD + 0x02;
		}
		// 3) Write to the outputs
        PORTD = tmpD;
        tmpD = 0x00; //reset
	}
	return 0;
}
