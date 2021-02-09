/*	Author: Alejandra Avitia-Davila (aavit004)
 *  Partner(s) Name: none
 *	Lab Section: 021
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	Create a synchSM to blink three LEDs connected to PB0, PB1, and PB2 in sequence, 1 second each. Implement that synchSM in C using the method defined in class. In addition to demoing your program, you will need to show that your code adheres entirely to the method with no variations.
    To clarify and match the video, the lights should turn on in the following sequence:
		PB0,PB1,PB2,PB0,PB1,PB2,PB0,PB1,PB2…
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO: https://youtu.be/b31RV40l7wY
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum States{Init, s0, s1, s2}state;

void TickSM1(){
	switch(state){
		case Init:
			state = s0;
			break;
		case s0:
			PORTB = 0x01;
			state = s1;
			break;
		case s1:
			PORTB = 0x02;
			state = s2;
			break;
		case s2:
			PORTB = 0x04;
			state = s0;
			break;
		default:
			PORTB = 0x00;
			state = Init;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(1000);
    TimerOn();
    state = Init;
    while(1){
	    //TickSM1();
            while(!TimerFlag);
            TimerFlag = 0;
	    TickSM1();
    }
    return 1;
}
