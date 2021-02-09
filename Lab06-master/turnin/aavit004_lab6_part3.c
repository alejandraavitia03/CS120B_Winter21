/*	Author: Alejandra Avitia-Davila (aavit004)
 *  Partner(s) Name: none
 *	Lab Section: 021
 *	Assignment: Lab #6  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO: https://youtu.be/vj5aUdswgBg
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum States{Start, Init, IncPress, DecPress, Inc, Dec, Reset}state;
unsigned char button,tmp;
void TickSM3(){
	switch(state){
		case Start:
			state = Init;
			break;
		case Init:
			if(button == 0x01){
				state = IncPress;
			}
			else if(button == 0x02){
				state = DecPress;
			}
			else if(button == 0x03){
				state = Reset;
			}
			else if(button == 0x00){
				state = Init;
			}
			break;
		case IncPress:
			if(button == 0x01){
				state = Inc;
			}
			else if(button == 0x02){
				state = DecPress;
			}
			else if(button == 0x03){
				state = Reset;
			}
			else if(button == 0x00){
				state = Init;
			}
			break;
		case DecPress:
			if(button == 0x01){
				state = IncPress;
			}
			else if(button == 0x02){
				state = Dec;
			}
			else if(button == 0x03){
				state = Reset;
			}
			else if(button == 0x00){
				state = Init;
			}
			break;
		case Inc:
			if(button == 0x01){
				state = Inc;
			}
			else if(button == 0x02){
				state = DecPress;
			}
			else if(button == 0x03){
				state = Reset;
			}
			else if(button == 0x00){
				state = Init;
			}
			break;
		case Dec:
			if(button == 0x01){
				state = IncPress;
			}
			else if(button == 0x02){
				state = Dec;
			}
			else if(button == 0x03){
				state = Reset;
			}
			else if(button == 0x00){
				state = Init;
			}
			break;
		case Reset:
			if(button == 0x01){
				state = IncPress;
			}
			else if(button == 0x02){
				state = DecPress;
			}
			else if(button == 0x03){
				state = Reset;
			}
			else if(button == 0x00){
				state = Init;
			}
			break;
	}
	switch(state){
		case Start:
			PORTB = 0x07;
			tmp = 0x00;
			break;
		case Init:
			tmp = 0x00;
			break;
		case IncPress:
			tmp = 0x00;
			if(PORTB < 0x09){
				PORTB += 0x01;
			}
			break;
		case DecPress:
			tmp = 0x00;
			if(PORTB > 0x00){
				PORTB -= 0x01;
			}
			break;
		case Inc:
			if((PORTB < 0x09) && (tmp >= 0x0A)){
					PORTB += 0x01;
					tmp = 0x00;
			}
			tmp += 0x01;
			break;
		case Dec:
			if((PORTB > 0x00) && (tmp >= 0x0A)){
                                        PORTB -= 0x01;
                                        tmp = 0x00;
                        }
                        tmp += 0x01;
                        break;
		case Reset:
			PORTB = 0x00;
			tmp = 0x00;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    
    TimerSet(100);
    TimerOn();
    state = Init;

    while(1){
	    button = ~PINA & 0x03;
            while(!TimerFlag);
            TimerFlag = 0;
	    TickSM3();
    }
    return 1;
}
