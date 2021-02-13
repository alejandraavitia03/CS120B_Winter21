/*	Author: Alejandra Avitia-Davila
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #7  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO: https://youtu.be/MKXUHhX__4M
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned short my_short = 0xABCD;

void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	//ADEN: setting this bit enables analog to digitial conversion
	//ADSC: setting this bit starts the first conversion
	//ADATE: setthing this bit enable auto triggering. since we are
	//		in Free Running Mode, a new conversion will trigger
	//		whenever the previous conversion completes
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    
    /* Insert your solution below */
    ADC_init();
    while (1) {
	    my_short = ADC;
	    unsigned char char1 = (my_short);
	    unsigned char char2 = (my_short >> 8);
	    PORTB = char1;
	    PORTD = char2;

    }
    return 1;
}
