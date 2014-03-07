#include "wiringPiI2C.h"

void I2CADC{
	wiringPiI2CSetup(0x4C); //unsure what device ID is for now, will test
	//set PLL to 800
	wiringPiI2CWriteReg16(0x4C, 0x01,0x2800); //PLL to 640
	wiringPiI2CWriteReg8(0x4C, 0x03, 0x28) //VCO/CPMP
}
