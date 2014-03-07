#include "wiringPi.h"

/* This code provides basic functionality to and from the MUX on board 1 from 
 * RPI1. The GPIO pins will read 6 RGB values per cycle, with 4 cycles of 
 * selector bits for a full 24-bit RGB word.
 * */
void GPIOMux{
	wiringPiSetup();
	//set pin modes
	pinModeWPi(7, INPUT); //GPIO 4  -- Output O1
	pinModeWPi(0, INPUT); //GPIO 17 -- Output O2
	pinModeWPi(1, INPUT); //GPIO 18 -- Output O3
	pinModeWPi(2, INPUT); //GPIO 21 -- Output O4
	pinModeWPi(3, INPUT); //GPIO 22 -- Output O5
	pinModeWPi(4, INPUT); //GPIO 23 -- Output O6
	pinModeWPi(5, OUTPUT);  //GPIO 24 -- Input  S1
	pinModeWPi(6, OUTPUT);  //GPIO 25 -- Input  S0
	//cycle through indefinitely -- this will change
	int[] RGB = new int[24];
	while(1){
		// "00"
		digitalWriteWPi(5, LOW);
		digitalWriteWPi(6, LOW);
		RGB[23] = digitalReadWPi(7);
		RGB[22] = digitalReadWPi(0);
		RGB[21] = digitalReadWPi(1);
		RGB[20] = digitalReadWPi(2);
		RGB[19] = digitalReadWPi(3);
		RGB[18] = digitalReadWPi(4);
		// "01"
		digitalWriteWPi(5, LOW);
		digitalWriteWPi(6, HIGH);
		RGB[17] = digitalReadWPi(7);
		RGB[16] = digitalReadWPi(0);
		RGB[15] = digitalReadWPi(1);
		RGB[14] = digitalReadWPi(2);
		RGB[13] = digitalReadWPi(3);
		RGB[12] = digitalReadWPi(4);
		// "10"
		digitalWriteWPi(5, HIGH);
		digitalWriteWPi(6, LOW);
		RGB[11] = digitalReadWPi(7);
		RGB[10] = digitalReadWPi(0);
		RGB[9] 	= digitalReadWPi(1);
		RGB[8] 	= digitalReadWPi(2);
		RGB[7]	= digitalReadWPi(3);
		RGB[6] 	= digitalReadWPi(4);
		// "11"
		digitalWriteWPi(5, HIGH);
		digitalWriteWPi(6, HIGH);
		RGB[5] = digitalReadWPi(7);
		RGB[4] = digitalReadWPi(0);
		RGB[3] = digitalReadWPi(1);
		RGB[2] = digitalReadWPi(2);
		RGB[1] = digitalReadWPi(3);
		RGB[0] = digitalReadWPi(4);
	}
}
