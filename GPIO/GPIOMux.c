#include "wiringPi.h"

/* This code provides basic functionality to and from the MUX on board 1 from 
 * RPI1. The GPIO pins will read 6 RGB values per cycle, with 4 cycles of 
 * selector bits for a full 24-bit RGB word.
 * */
void GPIOMux{
	//set pin modes
	pinModeWPi(7, OUTPUT); //GPIO 4  -- Output O1
	pinModeWPi(0, OUTPUT); //GPIO 17 -- Output O2
	pinModeWPi(1, OUTPUT); //GPIO 18 -- Output O3
	pinModeWPi(2, OUTPUT); //GPIO 21 -- Output O4
	pinModeWPi(3, OUTPUT); //GPIO 22 -- Output O5
	pinModeWPi(4, OUTPUT); //GPIO 23 -- Output O6
	pinModeWPi(5, INPUT);  //GPIO 24 -- Input  S1
	pinModeWPi(6, INPUT);  //GPIO 25 -- Input  S0
	//cycle through indefinitely -- this will change
	while(1){
		// "00"
		digitalWriteWPi(5, LOW);
		digitalWriteWPi(6, LOW);
		digitalReadWPi(7);
		digitalReadWPi(0);
		digitalReadWPi(1);
		digitalReadWPi(2);
		digitalReadWPi(3);
		digitalReadWPi(4);
		// "01"
		digitalWriteWPi(5, LOW);
		digitalWriteWPi(6, HIGH);
		digitalReadWPi(7);
		digitalReadWPi(0);
		digitalReadWPi(1);
		digitalReadWPi(2);
		digitalReadWPi(3);
		digitalReadWPi(4);
		// "10"
		digitalWriteWPi(5, HIGH);
		digitalWriteWPi(6, LOW);
		digitalReadWPi(7);
		digitalReadWPi(0);
		digitalReadWPi(1);
		digitalReadWPi(2);
		digitalReadWPi(3);
		digitalReadWPi(4);
		// "11"
		digitalWriteWPi(5, HIGH);
		digitalWriteWPi(6, HIGH);
		digitalReadWPi(7);
		digitalReadWPi(0);
		digitalReadWPi(1);
		digitalReadWPi(2);
		digitalReadWPi(3);
		digitalReadWPi(4);
	}
}
