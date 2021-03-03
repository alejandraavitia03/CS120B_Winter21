/*	Author: Pita
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #10  Exercise #Prelab
 *	Exercise Description: [optional - include for your own benefit]
 *
 *		Modify keypad.h to be in an SM task. 
 *		Add an LED to PB7 that lights up if ANY button pressed
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO LINK: https://youtu.be/-8CqE_CtYXg
 */
#include <avr/io.h>
#include <keypad.h>
#include <timer.h>
#include <scheduler.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Wait, Pressed}statek;

int Tick_Keypad(int state){
	unsigned char temp;
	unsigned char x = GetKeypadKey();
	switch(state){
		case Start:
			temp = 0;
			state = Wait;
			break;
		case Wait:
			if(x != '\0'){
				temp = 0x80;
				state = Pressed;
			}
			break;
		case Pressed:
			if( x == '\0'){
				temp = 0;
				state = Wait;
			}
			break;
		default:
			state = Wait;
			temp = 0;
	}
	PORTB = temp;
	return state;
}


int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
    unsigned short i;
    TimerSet(5);
    TimerOn();
    static task task1;
    task *tasks[]={&task1};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //task 1
    task1.state = Start;
    task1.period = 5;
    task1.elapsedTime = task1.period;
    task1.TickFct = &Tick_Keypad;
    while (1) {
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
