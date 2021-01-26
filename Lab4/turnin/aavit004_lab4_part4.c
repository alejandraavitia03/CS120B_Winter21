/*	Author: Pita
 *  Partner(s) Name: none
 *	Lab Section:
 *	Assignment: Lab #4  Exercise #3
 *	Exercise Description:
 *
 *	A household has a digital combination
 *		 deadbolt lock system on the doorway.
 *	 The system has buttons on a keypad.
 *	 	Button 'X' connects to PA0, 
 *		'Y' to PA1, and '#' to PA2.
 *		Pressing and releasing '#',then pressing 'Y', 
 *			should unlock the door by setting PB0 to 1. 
 *	Any other sequence fails to unlock. 
 *	Pressing a button from inside the house (PA7) locks the door (PB0=0). 
 *	For debugging purposes, give each state a number, 
 *		and always write the current state to PORTC 
 *		(consider using the enum state variable). 
 *	Also, be sure to check that only one button is pressed at a time.
 *	Extend the above door so that it can also be locked by entering 
 *		the earlier code. 
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States{Start, Init, K1Press, K2Press, Release, Locked}state;
void TickSM_Part3();
int main(void) {
    /* Insert DDR and PORT initializations */
        DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
        DDRC = 0xFF; PORTC = 0x00;
    /* Insert your solution below */
    while (1) {
        TickSM_Part3();
    }
    return 1;
}
void TickSM_Part3(){
	switch(state){          //Transition Switch
        case Start:
            state = Init;
            break;
        case Init:
            if((PINA & 0x07) == 0x04){
                state = K1Press;
            }
            else{
                state = Init;
            }
            break;
        case K1Press:
            if((PINA & 0x07) == 0x04){
                state = K1Press;
            }
            else if((PINA & 0x07) == 0x00){
                state = Release;
            }
            else if((PINA & 0x07) == 0x02){
                state = K2Press;
            }
            else {
                state = Init;
            }
            break;
        case K2Press:
            if(((PINA & 0x07) == 0x02) && ((PORTB & 0x01) == 0x01)){
                state = Locked;
            }
            else if((PINA & 0x07) == 0x02){
                state = Release;
            }
            else if((PINA & 0x07) == 0x00){
                state = K2Press;
            }
            else{
                state = Init;
            }
            break;
        case Release:
            if((PINA & 0x07) == 0x02){
                state = Release;
            }
            else{
                state = Init;
            }
            break;
        case Locked:
            if(((PINA & 0x07) == 0x02) || ((PINA & 0x87) == 0x80)){
                state = Locked;
            }
            else{
                state = Init;
            }
            break;
        default:
            state = Start;
            break;
    }
    switch(state){          //State Switch Satement
        case Start:
            PORTB = 0x00;
            break;
        case Init:
            break;
        case K1Press:
            break;
        case K2Press:
            break;
        case Release:
            PORTB =((~PORTB) & 0x01);
            break;
        case Locked:
            PORTB = 0x00;
            break;
        default:
            PORTB = 0x00;
            break;
    }
    PORTC = state;
}
