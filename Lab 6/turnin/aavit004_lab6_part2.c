/*	Author: Alejandra Avitia-Davila (aavit004)
 *  Partner(s) Name: none
 *	Lab Section: 021
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	Create a simple light game that requires pressing a button on PA0 while the middle of three LEDs on PB0, PB1, and PB2 is lit. The LEDs light for 300 ms each in sequence. When the button is pressed, the currently lit LED stays lit. Pressing the button again restarts the game. 

To clarify and match the video, the lights should turn on in the following sequence:

PB0,PB1,PB2,PB1,PB0,PB1,PB2,PB1,PB0…

 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO: https://youtu.be/OL8t0dH0RZE
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum States{Init, s0, s1, s2, Press, Release, Restart}state;
unsigned char button,tmp;
void TickSM2(){
	switch(state){
		case Init:
			state = s0;
			break;
		case s0:
			if(button){
				state = Press;
			}
			else{
				state = s1;
			}
			break;
		case s1:
			if(button){
				state = Press;
			}
			else{
				state = s2;
			}
			break;
		case s2:
			if(button){
				state = Press;
			}
			else{
				state = s0;
			}
			break;
		case Press:
			if(button){
				state = Press;
			}
			else{
				state = Release;
			}
			break;
		case Release:
			if(button){
				state = Restart;
			}
			else{
				state = Release;
			}
			break;
		case Restart:
			if(button){
				state = Restart;
			}
			else{
				state = s0;
			}
			break;
		default:
			state = Init;
			break;
	}
	switch(state){
		case Init:
			break;
		case s0:
			tmp = 0x01;
			break;
		case s1:
			tmp = 0x02;
			break;
		case s2:
			tmp = 0x04;
			break;
		case Press:
			break;
		case Release:
			break;
		case Restart:
			break;
	}
	PORTB = tmp;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    
    TimerSet(300);
    TimerOn();
    state = Init;
    while(1){
	    button = ~PINA & 0x01;
            while(!TimerFlag);
            TimerFlag = 0;
	    TickSM2();
    }
    return 1;
}
