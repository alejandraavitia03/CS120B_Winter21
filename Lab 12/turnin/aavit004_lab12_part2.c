/*	Author: Alejandra Avitia-Davila
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #12  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO: https://youtu.be/PmrfgsKlXZ4
 */
#include <avr/io.h>
#include <timer.h>
#include <scheduler.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char pattern = 0x80;
unsigned char button_A1, button_A0;

enum States{Wait, Inc, Dec, Release}state;
int Tick(int state){
	switch(state){
		case Wait:
			if(button_A0)
				state = Inc;
			else if(button_A1)
				state = Dec;
			else
				state = Wait;
			break;
		case Inc:
			if(pattern == 0x01){}
			else
				pattern = (pattern >> 1);
			state = Release;
			break;
		case Dec:
			if(pattern == 0x80){}
			else
				pattern = (pattern << 1);
			state = Release;
			break;
		case Release:
			if(!button_A0 && !button_A1)
				state = Wait;
			else
				state = Release;
			break;
		default:
			state = Wait;
	}
	return state;
}

enum DisplayState{display}dstate;
int D_Tick(int dstate){
	switch(dstate){
		case display:
			PORTC = pattern;
			//PORTD = temp;
			dstate = display;
			break;
		default:
			dstate = display;
	}
	return dstate;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    DDRA = 0x00; PINA = 0xFF;
    const char Init = -1;
    /* Insert your solution below */
    static task task1, task2;
    task *tasks[] = {&task1, &task2};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    task1.state = Init;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &Tick;

    task2.state = Init;
    task2.period = 50;
    task2.elapsedTime = task2.period;
    task2.TickFct = &D_Tick;

    TimerSet(1);
    TimerOn();
    unsigned short i;
    while (1) {
	    button_A0 = ~PINA & 0x01;
	    button_A1 = ~PINA & 0x02;

	    for(i = 0; i < numTasks; i++){
		    if(tasks[i]->elapsedTime == tasks[i]->period){
			    tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			    tasks[i]->elapsedTime = 0;
		    }
	    tasks[i]->elapsedTime += 1;
    	    }
	    while(!TimerFlag);
	    TimerFlag = 0;
    }
    return 1;
}
