/*	Author: Alejandra Avitia-Davila
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #13  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
	*
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO: https://youtu.be/oxcQcaBk_YQ

 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void A2D_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

void Set_A2D_Pin(unsigned char pinNum){
	ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
	static unsigned char i = 0;
	for(i = 0; i < 15; i++){ asm("nop"); }
}
unsigned char row = 0x1E;
unsigned char col = 0x80;
enum D_States{start, matrix} state;
void D_Tick(){
	switch(state){
		case start:
			state = matrix;
			break;
		case matrix:
			PORTC = col;
			PORTD = row;
			break;
	}
}
enum LR_States{startlr, wait, left, right}statelr;
unsigned short input = 0x00;
unsigned char count = 0x00, set = 0x02;
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
					}
					else{
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
enum T_States{startt,calc}states;
void T_Tick(){
	input = ADC;
	switch(states){
		case startt:
			states = calc;
			break;
		case calc:
			if(input <= 35){
				set = 2;
			}
			else if(input <= 200){
				set = 5;
			}
			else if(input <= 400){
				set = 10;
			}
			else if(input <= 515){
				set = 20;
			}
			else if(input <= 525){
				set = 100;
			}
			else if(input <= 650){
				set = 20;
			}
			else if(input <= 850){
				set = 10;
			}
			else if(input <= 980){
				set = 5;
			}
			else{
				set = 2;
			}
			states = calc;
break;
	}
}
/*
enum UD_States{startu, wait2, up, down}stated;
unsigned char counter= 0x00, setter = 0x02;
void UD_Tick(){
	input = ADC;
	switch(stated){
		case startu:
			stated = wait2;
			break;
		case wait2:
			if(input < 510){
				counter++;
				stated = down;
			}
			else if(input > 520){
				counter++;
				stated = up;
			}
			else
				stated = wait2;
			break;
		case down:
			if(input < 510){
				stated = down;
				counter++;
				if(counter >= setter){
					counter = 0x00;
					if(row!=0xEF){
						row = (row << 1) | 0x01;
					}
				}
			}
			else{
				stated = wait2;
				counter = 0x00;
			}
			break;
		case up:
			if(input > 520){
				stated = up;
				counter++;
				if(counter >= setter){
					if(row != 0xFE){
						row = (row >> 1) | 0x80;
					}
					counter = 0x00;
				}
			}
			else{
				counter = 0x00;
				stated = wait2;
			}
			break;
	}
	Set_A2D_Pin(0x00);
}
*/
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	TimerSet(2);
	TimerOn();
	A2D_init();
	//unsigned short input = ADC;
	statem = start;
	statelr = startlr;
	states = startt;
    while (1) {
	    T_Tick();
	    LR_Tick();
	    M_Tick();
	    TimerSet(50);
	    while(!TimerFlag);
	    TimerFlag = 0;

    }
    return 1;
}
