/*	Author: Pita
 *  Partner(s) Name: none
 *	Lab Section:
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description:
 * 
 *		PB0 and PB1 each connect to an LED, and PB0's LED is initially on.
 *		Pressing a button connected to PA0 turns off PB0's LED and turns on PB1's LED, 
 *			staying that way after button release. 
 *		Pressing the button again turns off PB1's LED and turns on PB0's LED.
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Init, PressA0, OffPressA0, OffRelease}state;
void TickSM_Part1();

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    while (1) {
	TickSM_Part1();
    }
    return 1;
}

void TickSM_Part1(){
	switch(state){ 		//Transition Switch
		case Start:
			state = Init;
			break;
		case Init:
			if((PINA & 0x01) == 1){
				state = Init;
			}
			else{
				state = PressA0;
			}
			break;
		case PressA0:
			if((PINA & 0x01) == 1){
				state = OffPressA0;
			}
			else{
				state = PressA0;
			}
			break;
		case OffPressA0:
			if((PINA & 0x01) == 1){
				state = OffPressA0;
			}
			else{
				state = OffRelease;
			}
			break;
		case OffRelease:
			if((PINA & 0x01) == 1){
				state = Init;
			}
			else{
				state = OffRelease;
			}
			break;
		default:
			state = Start;
			break;
	}
	switch(state){		//State Switch Satement 
		case Start:
			PORTB = 0x01;
			break;
		case Init:
			PORTB = 0x01;
			break;
		case PressA0:
			break;
		case OffPressA0:
			PORTB = 0x02;
			break;
		case OffRelease:
			break;
		default:
			PORTB = 0x01;
			break;
	}
}
