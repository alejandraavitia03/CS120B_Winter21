/*	Author: Alejandra Avitia-Davila
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #13  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
	*
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO: https://youtu.be/BVaDI1o7gT0
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void A2D_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}
unsigned char row = 0x1E;
unsigned char col = 0x80;
enum M_States{start, matrix} statem;
void M_Tick(){
	switch(statem){
		case start:
			statem = matrix;
			break;
		case matrix:
			PORTC = col;
			PORTD = row;
			break;
	}
}

enum LR_States{startlr, wait, left, right}statelr;
unsigned short input = 0x00, count = 0x00, set = 0x02;
void LR_Tick(){
	input = ADC;
	switch(statelr){
		case startlr:
			statelr = wait;
			break;
		case wait:
			if(input < 0x1F4){
				count++;
				statelr = left;
			}
			else if(input > 0x212){
				count++;
				statelr = right;
			}
			else
				statelr = wait;
			break;
		case left:
			if(input < 0x1F4){
				statelr = left;
				count++;
				if(count >= set){
					count = 0x00;
					if(col != 0x80){
						col = col << 1;
					}
					else{
						col = 0x01;
					}
				}
			}
			else{
				statelr = wait;
				count = 0x00;
			}
			break;
		case right:
			if(input > 0x212){
				statelr = right;
				count++;
				if(count >= set){
					if(col != 0x01){
						col = col >> 1;
					}else{
						col = 0x80;
					}
					count = 0x00;
				}
			}
			else{
				count = 0x00;
				statelr = wait;
			}
			break;
		}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	TimerSet(50);
	TimerOn();
	A2D_init();
	//unsigned short input = ADC;
	statem = start;
	statelr = startlr;
    while (1) {
	    LR_Tick();
	    M_Tick();
	    while(!TimerFlag);
	    TimerFlag = 0;

    }
    return 1;
}
