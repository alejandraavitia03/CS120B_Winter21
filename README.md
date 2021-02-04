# CS120B_Winter21
This repository is for my CS120B Embedded Systems course at UCR. Taken during winter 2021. 
  > Instructor: Dr. Philip Brisk 
  > Teacher Assistant: Amir Hosseinian (021)

# Labs
Below are the prompts for each lab
## Lab 2:
  - Exercise 1: Garage open at night-- A garage door sensor connects to PA0 (1 means door open), and a light sensor connects to PA1 (1 means light is sensed). Write a program that illuminates an LED connected to PB0 (1 means illuminate) if the garage door is open at night.
  - Exercise 2: Port A's pins 3 to 0, each connect to a parking space sensor, 1 meaning a car is parked in the space, of a four-space parking lot. Write a program that outputs in binary on port C the number of available spaces (Hint: declare a variable "unsigned char cntavail;" you can assign a number to a port as follows: PORTC = cntavail;).
  - Exercise 3: Extend the previous program to still write the available spaces number, but only to PC3..PC0, and to set PC7 to 1 if the lot is full. 
  - Exercise 4: (Challenge) An amusement park kid ride cart has three seats, with 8-bit weight sensors connected to ports A, B, and C (measuring from 0-255 kilograms). Set PD0 to 1 if the cart's total passenger weight exceeds the maximum of 140 kg. Also, the cart must be balanced: Set port PD1 to 1 if the difference between A and C exceeds 80 kg.  The remaining 6 bits on D should display an approximation of the total combined weight
  
## Lab 3:
  - Exercise 1: 
