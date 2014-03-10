#include "wiringPi.h"

/* This code provides basic functionality to and from the MUX on board 1 from 
 * RPI1. The GPIO pins will read 6 RGB values per cycle, with 4 cycles of 
 * selector bits for a full 24-bit RGB word.
 * */
int RPi1Setup(void)
{
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

}
uint8_t[] GPIOMux(void){ //reads 24-bit RGB from GPIO
	uint8_t[] RGB = new int[24];
	// "00"
	digitalWriteWPi(5, LOW);
	digitalWriteWPi(6, LOW);
	RGB[23] = (uint8_t)digitalReadWPi(7);
	RGB[22] = (uint8_t)digitalReadWPi(0);
	RGB[21] = (uint8_t)digitalReadWPi(1);
	RGB[20] = (uint8_t)digitalReadWPi(2);
	RGB[19] = (uint8_t)digitalReadWPi(3);
	RGB[18] = (uint8_t)digitalReadWPi(4);
	// "01"
	digitalWriteWPi(5, LOW);
	digitalWriteWPi(6, HIGH);
	RGB[17] = (uint8_t)digitalReadWPi(7);
	RGB[16] = (uint8_t)digitalReadWPi(0);
	RGB[15] = (uint8_t)digitalReadWPi(1);
	RGB[14] = (uint8_t)digitalReadWPi(2);
	RGB[13] = (uint8_t)digitalReadWPi(3);
	RGB[12] = (uint8_t)digitalReadWPi(4);
	// "10"
	digitalWriteWPi(5, HIGH);
	digitalWriteWPi(6, LOW);
	RGB[11] = (uint8_t)digitalReadWPi(7);
	RGB[10] = (uint8_t)digitalReadWPi(0);
	RGB[9] 	= (uint8_t)digitalReadWPi(1);
	RGB[8] 	= (uint8_t)digitalReadWPi(2);
	RGB[7]	= (uint8_t)digitalReadWPi(3);
	RGB[6] 	= (uint8_t)digitalReadWPi(4);
	// "11"
	digitalWriteWPi(5, HIGH);
	digitalWriteWPi(6, HIGH);
	RGB[5] = (uint8_t)digitalReadWPi(7);
	RGB[4] = (uint8_t)digitalReadWPi(0);
	RGB[3] = (uint8_t)digitalReadWPi(1);
	RGB[2] = (uint8_t)digitalReadWPi(2);
	RGB[1] = (uint8_t)digitalReadWPi(3);
	RGB[0] = (uint8_t)digitalReadWPi(4);
	return RGB;
	}
}

uint16_t displayData(uint8_t[] GPIOMUX){//converts from 24-bit RGB to 16-bit RGB
	uint8_t r = (GPIO[23] << 7) | (GPIO[22] << 6) | (GPIO[21] << 5) | (GPIO[20] << 4) | (GPIO[19] << 3) | (GPIO[18] << 2) | (GPIO[17] << 1) | GPIO[16];
	uint8_t g = (GPIO[15] << 7) | (GPIO[14] << 6) | (GPIO[13] << 5) | (GPIO[12] << 4) | (GPIO[11] << 3) | (GPIO[10] << 2) | (GPIO[9] << 1) | GPIO[8];
	uint8_t b = (GPIO[7] << 7) | (GPIO[6] << 6) | (GPIO[5] << 5) | (GPIO[4] << 4) | (GPIO[3] << 3) | (GPIO[2] << 2) | (GPIO[1] << 1) | GPIO[0];
	return (r << 11) | (g << 5) | b;
}
