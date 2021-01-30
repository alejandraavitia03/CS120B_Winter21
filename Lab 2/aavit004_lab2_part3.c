/*	Author: Pita
 *  Partner(s) Name: Alejandra Avitia-Davila
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif	

int main(void) {
		
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	//unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
    unsigned char tmpA = 0x00; // Temporary variable to hold the value of A's we can do it all in one
	unsigned char cntavail = 0x00; //be the counter for spots avail
	while(1) {
		// 1) Read input
        tmpA = PINA & 0x0F;
	
		// 2) Permoft the computation
		//4 if unconnected loops for each space increment 
        if (tmpA == 0x01 || tmpA == 0x02 || tmpA == 0x04 || tmpA == 0x08) { // True if PA0 is 1
                    cntavail = 3;
        }
        else if(tmpA == 0x03 || tmpA == 0x06 || tmpA == 0x0C || tmpA == 0x05 || tmpA == 0x0A || tmpA == 0x09 ) {
            cntavail = 2;
        }
        else if(tmpA == 0x07 || tmpA == 0x0E || tmpA == 0x0D || tmpA == 0x0B ){
            cntavail = 1;
        }
        else if(tmpA == 0x0F){
            cntavail = 128;
        }
        else{
            cntavail = 4;
        }
        PORTC = cntavail;
	}
	return 0;
}
