#include "wiringPi.h"

/* This code contains the basic functionality between the GPIO pins of RPI2 and
 * the AD9983A ADC. This will include 
 * */
void GPIOADC{
	wiringPiSetup();
	//set modes
	pinMode(7, OUTPUT);	// GPIO 4  -- HSYNC0
	pinMode(0, INPUT);	// GPIO 17 -- HSOUT
	pinMode(1, OUTPUT);	// GPIO 18 -- VSYNC0
	pinMode(2, INPUT);	// GPIO 21 -- VSOUT
	pinMode(3, OUTPUT);	// GPIO 22 -- A0
	pinMode(4, INPUT);	// GPIO 23 -- EXTCLK
	pinMode(5, OUTPUT);	// GPIO 24 -- CLAMP
	pinMode(6, OUTPUT);	// GPIO 25 -- COAST
	//set clocks
	digitalWrite(7, HIGH);
	digitalWrite(1, HIGH);
	//initalize counters
	int hcount = 0; //used to generate 60Hz*480 clock
	int vcount = 0; //used to generate 60Hz clock
	//basic implementation of 60Hz clock -- NOT FINAL
	while(1){//loop indefinitely -- not final implementation
		//Check valid data first
		if (hcount < 640){ //0-639 valid data
			if (vcount < 480){ //0-479 valid data
				//tell RPI1 to inc mux
			}
		}
		//HSYNC first
		//@660 -> HSYNC set low
		if (hcount == 660) digitalWrite(7, LOW);
		//@756 -> HSYNC set high
		else if (hcount == 756) digitalWrite(7, HIGH);
		else if (hcount == 800) {
			//VSYNC Second
			//@494 -> VSYNC set low
			if (vcount == 494) digitalWrite(1, LOW);
			//@495 -> VSYNC set high
			else if (vcount == 495) digitalWrite(1, HIGH);
			vcount = (vcount++) % 525; //increment VSYNC < 525
		}
		hcount = (hcount++ % 800); //increment HSYNC < 800
	}
	//no need for other functions yet -- will add additional code if the 
	//case	
}
