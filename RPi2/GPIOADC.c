#include "wiringPi.h"

/* This code contains the basic functionality between the GPIO pins of RPI2 and
 * the AD9983A ADC. This will include 
 * */
void GPIOADC{
	wiringPiSetup();
	//set modes
	pinMode(7, GPIO_CLOCK); // GPIO 4  -- HSYNC0
	pinMode(0, INPUT);	// GPIO 17 -- HSOUT
	pinMode(1, OUTPUT);	// GPIO 18 -- VSYNC0
	pinMode(2, INPUT);	// GPIO 21 -- VSOUT
	pinMode(3, OUTPUT);	// GPIO 22 -- A0
	pinMode(4, INPUT);	// GPIO 23 -- EXTCLK
	pinMode(5, OUTPUT);	// GPIO 24 -- CLAMP
	pinMode(6, OUTPUT);	// GPIO 25 -- COAST
	//set function
	gpioClockSetWPi(7, 31500); //set HSYNC to 31.5KHz
	//basic implementation of 60Hz clock -- NOT FINAL
	while(1){
		digitalWrite(1, HIGH); 
		delayMicroseconds(16667);
		digitalWrite(0, LOW);
		delayMicroSeconds(16667);
	}
	//no need for other functions yet -- will add additional code if the 
	//case	
}
