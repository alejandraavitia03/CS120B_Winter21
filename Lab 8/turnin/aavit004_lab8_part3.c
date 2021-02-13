/*	Author: Alejandra Avitia-Davila
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *		
 *	Using the ATmega1284’s PWM functionality, 
 *	design a system that uses three buttons to select one of three tones to be generated on the speaker. 
 *	When a button is pressed, the tone mapped to it is generated on the speaker. Criteria:
	Use the tones C4, D4, and E4 from the table in the introduction section.
	When a button is pressed and held, the tone mapped to it is generated on the speaker.
	When more than one button is pressed simultaneously, the speaker remains silent. 
	When no buttons are pressed, the speaker remains silent.

 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO Link:https://youtu.be/gq4UbFqiqSc
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include <timer.h>
// 0.954 hz is lowest frequency possible with this function 
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency){
	static double current_frequency; //Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uniterrupted
	if(frequency != current_frequency)
	{
		if(!frequency) // stops timer/counter 
		{ 
			TCCR3B &= 0x08;
		} 
		else // resumes/continues timer/counter 
		{ 
			TCCR3B |= 0x03; 
		} 

		//prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { 
			OCR3A = 0xFFFF; 
		}

		// prevents OCR3A from underflowings, using prescaler 64
		// 31250 is largest frequency that will not result in underflow
		else if(frequency > 31250) { 
			OCR3A = 0x0000; 
		}
		//set OCR3A based on desired frequency
		else { 
			OCR3A = (short)(8000000 / (128 * frequency)) - 1; 
		}
		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the curr frequency
	}
}

void PWM_on(){
	TCCR3A = (1 << COM3A0);
		// COM3A0: Toggle PB3 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
		// WHM32: When counter (TCNT3) matches OCR3A, reset counter
		// // CS31 & CS30: Set a prescalar of 64
	set_PWM(0);
}

void PWM_off(){
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}
//B,C,B,G,F,C,B,A,G,G,F
enum States{Start, Play, Off, Toggle, Wait}state;
double noteFreq[] = {261.63,293.66,329.63,349.23,392,440,493.88,523.25};
		    // C(0)  D(1)   E(2)  F(3)   G(4)A(5) B(6)   C(7)
unsigned short unravel[]={6,7,6,4,3,7,6,5,4,4,3}; //BCBGFCBAGGF
unsigned char counter;
unsigned char button;
unsigned char tempA;
unsigned char i;
void Tick_SM3(){
	switch(state){
		case Start:
			state = Wait;
			button = 0;
			counter = 0;
			i = 0;
			PWM_on();
			break;
		case Play:
			if(!tempA)
				button = 0;
			if(counter >= 24){
				set_PWM(0);
				state = Toggle;
			}
			else
				counter++;
			break;
		case Off:
			if(!tempA)
				state = Wait;
			else
				state = Start;
			break;
		case Toggle:
			i++;
			if(!tempA)
				button = 0;
			counter = 0;
			if(i >= 11 && button == 1){
				state = Off;
				i = 0;
			}
			else if(i >= 11){
				i = 0;
				state = Wait;
			}
			else{
				set_PWM(noteFreq[unravel[i]]);
				state = Play;
			}
			break;
		case Wait:
			if(tempA == 0x01){
				set_PWM(noteFreq[unravel[i]]);
				i++;
				counter++;
				state = Play;
				button = 1;
			}
			else{
				state = Wait;
			}
			break;
		default:
			state = Wait;
			set_PWM(0);
			break;
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    state = Start;
    TimerSet(25);
    TimerOn();
    while (1) {
	    tempA = ~PINA & 0x01;
	    while(!TimerFlag);
	    TimerFlag = 0;
	    Tick_SM3();
    }
    PWM_off();
    return 1;
}
