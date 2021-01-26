/*	Author: Pita
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description:
 *	
 * 	Weight (9 bits) => PD7 to PD0 to PB0. 
 *	if Weight >= 70 
 *		airbad = 1(PB1) and PB2 =0;
 *	if 5 < Weight < 70
 * 		airbag is 0 and display sensor is 1 (PB2)
 *	if Weight < 5
 *		PB1 = 0 and PB2 = 0;
 *	 		 
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0x00; PORTB = 0xFF;
	DDRD = 0xFE; PORTD = 0x00;

	unsigned char tempB = 0x00, tempD = 0x00;
	unsigned char sig = 0x00;
	unsigned short weight = 0;
    /* Insert your solution below */
    while (1) {
	weight = (tempD << 1) | (tempB & 0x01); //Reset the sum everytime is loops;

	//If loops to check and display
	
	if(weight >= 0x0046)
		sig = 0x02;
	else if(weight > 0x005)
		sig  = 0x04;
	else
		sig = 0x00;
	PORTB = sig;
    }
    return 1;
}
