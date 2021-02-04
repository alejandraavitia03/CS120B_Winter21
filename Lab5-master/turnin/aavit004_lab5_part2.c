/*	     Author: Pita
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	 Assignment: Lab #5  Exercise #2
 *	
 *	Buttons are connected to PA0 and PA1. Output for PORTC is initially 7. Pressing PA0 increments PORTC (stopping at 9). Pressing PA1 decrements PORTC (stopping at 0). If both buttons are depressed (even if not initially simultaneously), PORTC resets to 0. If a reset occurs, both buttons should be fully released before additional increments or decrements are allowed to happen. Use LEDs (and resistors) on PORTC. Use a state machine (not synchronous) captured in C. 	
 *
 *	Turning: (.c) and (.gbd)
 *	Exercise Description: [optional - include for your own benefit]
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Increment, Decrement, Reset, Wait, Release}state;
unsigned char counter;
unsigned char button_0, button_1;

void TickSM_Part2();
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
    /* Insert your solution below */
    state = Start;
    counter = 0x07;
    while (1) {
        button_0 = ~PINA & 0x01;
        button_1 = (~PINA & 0x02) > 0;
                TickSM_Part2();
    }
    return 1;
}

void TickSM_Part2(){
	switch(state){ 		//Transition Switch
		case Start:
			state = Wait;
			break;
        case Wait:
            if(button_0 && !button_1){
                state = Increment;
            }
            else if (!button_0 && button_1){
                state = Decrement;
            }
            else if(button_0 && button_1){
                state = Reset;
            }
            else{
                state = Wait;
            }
            break;
		case Increment:
            state = Release;
			break;
		case Decrement:
            state = Release;
			break;
		case Reset:
            state = Wait;
			break;
        case Release:
            if(!button_0 && !button_1)
                state = Wait;
		default:
			state = Start;
			break;
	
	}
	switch(state){		//State Switch Satement 
        case Start:
            counter = 0x07;
            PORTC = counter;
			break;
		case Increment:
            if(counter == 0x09){
                break;
            }
            else if(counter < 0x09){
                counter = counter + 0x01;
                PORTC = counter;
            }
			break;
		case Decrement:
            if(counter == 0x00){
                break;
            }
            else if(counter > 0x00){
                counter = counter - 0x01;
                PORTC = counter;
            }
            break;
		case Reset:
			counter = 0x00;
            PORTC = counter;
			break;
        case Wait:
            PORTC = counter;
            break;
        case Release:
            break;
		default:
			break;
	}
    //PORTC = counter;
}

