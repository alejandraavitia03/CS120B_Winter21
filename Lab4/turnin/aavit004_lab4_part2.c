/*	Author: Pita
 *  Partner(s) Name: none
 *	Lab Section:
 *	Assignment: Lab #4  Exercise #2
 *	Exercise Description:
 *
 *	Buttons are connected to PA0 and PA1. 
 *	Output for PORTC is initially 7. 
 *	Pressing PA0 increments PORTC once (stopping at 9). 
 *	Pressing PA1 decrements PORTC once (stopping at 0). 
 *	If both buttons are depressed (even if not initially simultaneously), 
 *		PORTC resets to 0. 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Init, Increment, Decrement, Reset, IncWait, DecWait}state;
unsigned char counter = 0x00; //Put the global variable back

void TickSM_Part2();
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
    /* Insert your solution below */
    state = Start;
    counter = 0x07;
    while (1) {
        TickSM_Part2();
    }
    return 1;
}

void TickSM_Part2(){
	switch(state){ 		//Transition Switch
		case Start:
			state = Init;
			break;
		case Init:
			if((PINA & 0x03) == 0x03){
				state = Reset;
			}
			else if ((PINA & 0x03) == 0x02){
				state = DecWait;
			}
			else if((PINA & 0x03) == 0x01){
				state = IncWait;
			}
			break;
		case Increment:
			if((PINA & 0x03) == 0x01){
				state = Increment;
			}
			else if((PINA & 0x03) == 0x03){
				state = Reset;
			}
            		else if((PINA & 0x03) == 0x02){
                		state = DecWait;
            		}
            		else{
                		state = Init;
            		}
			break;
		case IncWait:
			state = Increment;
			break;
		case Reset:
			if((PINA & 0x03) == 0x03){
				state = Reset;
			}
			else{
				state = Init;
			}
			break;
		case Decrement:
            		if((PINA & 0x03) == 0x02){
                		state = Decrement;
            		}
            		else if((PINA & 0x03) == 0x03){
                		state = Reset;
            		}
            		else if((PINA & 0x03) == 0x01){
                		state = IncWait;
            		}
            		else{
                		state = Init;
            		}
			break;
		case DecWait:
			state = Decrement;
			break;
		default:
			state = Start;
			break;
	
	}
	switch(state){		//State Switch Satement 
		case Start:
			counter = 0x07;
			break;
		case Init:
			break;
		case Increment:
			break;
		case IncWait:
			if(counter < 0x09){
				counter = counter + 1;
			}
			break;
		case Reset:
			counter = 0x00;
			break;
		case Decrement:
			break;
		case DecWait:
			if(counter > 0x00){
                counter = counter - 1;
			}
			break;
		default:
            counter = 0x07;;
			break;
	}
    PORTC = counter;
}

