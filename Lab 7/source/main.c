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
void ADC_init(){
        ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
        //ADEN: setting this bit enables analog to digitial conversion
        //ADSC: setting this bit starts the first conversion
        //ADATE: setthing this bit enable auto triggering. since we are
        //              in Free Running Mode, a new conversion will trigger
        //              whenever the previous conversion completes
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
    ADC_init();
    while (1) { //max is 0x147
            my_short = ADC;
            if(my_short > 0)
                    PORTB = 0x01;
            if(my_short > 41)
                    PORTB = 0x02;
            if(my_short > 82)
                    PORTB = 0x04;
            if(my_short > 123)
                    PORTB = 0x08;
            if(my_short > 164)
                    PORTB = 0x10;
            if(my_short > 205)
                    PORTB = 0x20;
            if(my_short > 246)
                    PORTB = 0x40;
            if(my_short > 287)
                    PORTB = 0x80;
            }
    return 1;
}
