/*	Author: Alejandra Avitia-Davila
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #7  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO: https://youtu.be/oBoiLKfkp98
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned short my_short = 0x00;
//unsigned char my_char1 = 0x00;
//unsigned char my_char2 = 0x00;

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
	    if(my_short >= 0x3F){
		    PORTB = 0x01;
	    }
	    else{
		    PORTB = 0x00;
	    }
	    		    
    }
    return 1;
}
