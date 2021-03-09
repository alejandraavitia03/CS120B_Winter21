/*	Author: Alejandra Avitia-Davila
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #13  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
	*
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO: https://youtu.be/xRuvwu2F89M
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
			if(input < 0x203){
				count++;
				statelr = left;
			}
			else if(input > 0x20D){
				count++;
				statelr = right;
			}
			else
				statelr = wait;
			break;
		case left:
			if(input < 0x203){
				statelr = left;
				count++;
				if(count >= set){
					count = 0x00;
					if(col != 0x80){
						col = col << 1;
					}
				}
			}
			else{
				statelr = wait;
				count = 0x00;
			}
			break;
		case right:
			if(input > 0x20D){
				statelr = right;
				count++;
				if(count >= set){
					if(col != 0x01){
						col = col >> 1;
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
	Set_A2D_Pin(0x01);

}

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

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	TimerSet(1);
	TimerOn();
	A2D_init();
	state = start;
	statelr = startlr;
	stated = startu;
    while (1) {
	    LR_Tick();
	    UD_Tick();
	    D_Tick();
	    TimerSet(50);
	    while(!TimerFlag);
	    TimerFlag = 0;

    }
    return 1;
}
