/*	Author: Alejandra Avitia-Davila
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #13  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
	*
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO: https://youtu.be/ZmpJlnwygKQ
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void A2D_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	A2D_init();
	unsigned short input = ADC;
    while (1) {
	    input  = ADC;
	    PORTB = (char)input;
	    PORTD = (char)(input >> 8);

    }
    return 1;
}
