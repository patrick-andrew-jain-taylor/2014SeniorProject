#include "wiringPiI2C.h"

void I2CADC{
	wiringPiI2CSetup(##); //unsure what device ID is for now, will test
	//set PLL to 800
	wiringPiI2CWriteReg16(##, 0x01,800 << 4); 
}
