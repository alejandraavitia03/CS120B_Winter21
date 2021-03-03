/*	Author: Alejandra Avitia-Davila
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #10  Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	Passcode: 1,2,3,4,5. This sets PB0 to 1. 
 *	Press and Release # starting over
 *	Button on PB7 locks door so PB0 is 0
 *	CHECK FOR MORE THAT ONE BUTTON
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO LINK:https://youtu.be/WYiFqicPw_A 
 */
#include <avr/io.h>
#include <keypad.h>
#include <timer.h>
#include <scheduler.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Wait, Success, Failed, Open}statek;
unsigned char passcode[] = {'#', '1', '2', '3', '4', '5'};
unsigned char temp;
int Tick_Keypad(int state){
	static unsigned char i;
	unsigned char x = GetKeypadKey();
	switch(state){
		case Start:
			temp = 0;
			i = 0;
			state = Wait;
			break;
		case Wait:
			if(x == passcode[i])
				state = Success;
			if(x != '\0' && x != passcode[i])
				state = Failed;
			break;
		case Success:
			if(x != '\0' && x != passcode[i]){
				state = Failed;
			}
			else if(x == '\0'){
				i++;
                                state = Wait;
				if(i >= 6){
					state = Open;
					temp = 0x01;
				}
			}
			break;
		case Failed:
			if (x == '\0'){
				state = Wait;
				i = 0;
			}
			break;
		case Open:
			i = 0;
			state = Wait;
			break;
		default:
			state = Wait;
			temp = 0x00;
	}
	PORTB = temp;
	return state;
}

enum LockedStates{Startl, Waitl, Lock}statel;

int Tick_Lock(int state){
	switch(state){
		case Startl:
			state = Waitl;
			break;
		case Waitl:
			if((PINB & 0x80) == 0x80){
				state = Lock;
			}
			break;
		case Lock:
			state = Waitl;
	}
	switch(state){
		case Startl:
			break;
		case Waitl:
			if((~PINB & 0x80) == 0x80)
				temp = 0;
			break;
		case Lock:
			break;
		default:
			temp = 0;
	}
	PORTB =  temp;
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0x0F; PORTB = 0xF0;
    DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
    unsigned short i;
    TimerSet(1);
    TimerOn();
    static task task1, task2;
    task *tasks[]={&task1, &task2};
    const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    //task 1
    task1.state = Start;
    task1.period = 1;
    task1.elapsedTime = task1.period;
    task1.TickFct = &Tick_Keypad;

    //task 2
    task2.state = Startl;
    task2.period = 1;
    task2.elapsedTime = task2.period;
    task2.TickFct = &Tick_Lock;
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
