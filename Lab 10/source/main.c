/*	Author: Alejandra Avitia-Davila
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #10  Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	Passcode: 1,2,3,4,5. This sets PB0 to 1. 
 *	Press and Release # starting over
 *	Button on PB7 locks door so PB0 is 0
 *	PA7 has doorbell
 *	PB6 speaker
 *	CHECK FOR MORE THAT ONE BUTTON
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO LINK: 
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
task task1, task2, task3, task4; //Global for the SM's
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
			break;
		default:
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

enum DoorbellStates{Startd, Waitd, Play}statesd;
unsigned char alert;
int Tick_Doorbell(int state){
	switch(state){
		case Startd:
			state = Waitd;
			break;
		case Waitd:
			if((PINA & 0x80) == 0x80){
				state = Play;
			}
			break;
		case Play:
			state = Waitd;
			break;
		default:
			state = Waitd;
	}
	switch(state){
		case Startd:
			break;
		case Waitd:
			alert = 0x01;
			break;
		case Play:
			break;
		default:
			alert = 0x00;
	}
	return state;
}

enum SpeakerStates{Starts, Waits, s1, s2, s3}stateS;
unsigned char temps;

int Tick_Speaker(int state){
	static unsigned short count;
	switch(state){
		case Starts:
			state = Waits;
			temps = 0x00;
			count = 0;
			break;
		case Waits:
			if(alert == 0x01){
				state = s1;
				count++;
				temps = 0x40;
				task4.period = 2;
			}
			break;
		case s1:
			if(count < 500){
				if((count % 2) == 1)
					temps = 0x00;
				else
					temps = 0x40;
				count++;
			}
			else if(count >= 500){
				state = s2;
				count = 1;
				temps = 0x40;
				task4.period = 4;
			}
		case s2:
			if(count < 250){
				if((count % 2) == 1)
					temps = 0x00;
				else 
					temps = 0x40;
				count++;
			}
			else if(count >= 250){
				state = s3;
				count = 1;
				temps = 0x40;
				task4.period = 3;
			}
			break;
		case s3:
			if(count < 333){
				if ((count % 2) == 1)
					temps = 0x00;
				else
					temps = 0x40;
				count++;
			}
			else if(count >= 250){
				state = Waits;
				count = 1;
				temps = 0x00;
				task4.period = 2;
				alert = 0;
			}
			break;
		default:
			task4.period = 2;
			temps = 0x00;
			count = 0;
			state = Waits;

	}
	PORTB = (temps | temp);
	return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x7F; PORTB = 0x80;
    DDRC = 0xF0; PORTC = 0x0F;
    /* Insert your solution below */
    alert = 0;
    unsigned short i;
    TimerSet(1);
    TimerOn();

    //static task task1, task2;
    task *tasks[]={&task1, &task2, &task3, &task4};
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

    //task 3
    task3.state = Startd;
    task3.period = 200;
    task3.elapsedTime = task4.period;
    task3.TickFct = &Tick_Doorbell;


    //task 4
    task4.state = Starts;
    task4.period = 1;
    task4.elapsedTime = task4.period;
    task4.TickFct = &Tick_Speaker;
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
