/*	Author: Alejandra Avitia-Davila
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #12  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO: https://youtu.be/L_nvDoEz44E
 */
#include <avr/io.h>
#include <timer.h>
#include <scheduler.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

char pattern[5] = {0, 0x3C, 0x24, 0x3C, 0};
char pattern2[5] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF};
unsigned char button_A1, button_A0, button_A2, button_A3;
unsigned char change = 0;
unsigned char side = 1;

enum States{Wait, Inc, Dec,Right, Left, Release}state;
int Tick(int state){
	switch(state){
		case Wait:
			if(button_A0)
				state = Inc;
			else if(button_A1)
				state = Dec;
			else if(button_A2)
				state = Left;
			else if(button_A3)
				state = Right;
			else
				state = Wait;
			break;
		case Inc:
			if(pattern[0] > 0x00)
				side = 1;
			if(side == 1)
				side = 0;
			else{
				for(int i = 0; i < 4; ++i)
					pattern[i] = pattern[i+1];
				pattern[4] = 0x00;
			}
			state = Release;
			break;
		case Dec:
			if(pattern[4]  > 0)
				side = 1;
			if(side == 1)
				side = 0;
			else{
				for(int i =4; i > 0; --i)
					pattern[i] = pattern[i-1];
				pattern[0] = 0x00;
			}
			state = Release;
			break;
		case Left:
			for(int i = 0; i < 5; ++i){
				if(pattern[i] & 0x80)
					side = 1;
			}
			if(side == 1)
				side = 0;
			else{
				for(int i = 0; i < 5; ++i)
					pattern[i] = pattern[i] << 1;
			}
			state = Release;
			break;
		case Right:
			for(int i = 0; i < 5; ++i){
				if(pattern[i] & 0x01)
					side = 1;
			}
			if(side == 1)
				side = 0;
			else{
				for(int i = 0; i < 5; ++i)
					pattern[i] = (pattern[i] >> 1) & 0x7F;
			}
			state = Release;
			break;
		case Release:
			side = 0;
			if(!button_A0 && !button_A1 && !button_A2 && !button_A3)
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
			PORTC = pattern[change];
			PORTD = pattern2[change];
			++change;
			if(change > 4)
				change = 0;
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
    task2.period = 1;
    task2.elapsedTime = task2.period;
    task2.TickFct = &D_Tick;

    TimerSet(1);
    TimerOn();
    unsigned short i;
    while (1) {
	    button_A0 = ~PINA & 0x01;
	    button_A1 = ~PINA & 0x02;
	    button_A2 = ~PINA & 0x04;
	    button_A3 = ~PINA & 0x08;

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
