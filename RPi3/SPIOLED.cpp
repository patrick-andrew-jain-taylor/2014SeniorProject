//This library is an extension of Tomaz Solc's SEPS525_OLED_ARDUINO library, combined with the wiringPiSPI library.

#include "wiringPi.h"
#include "wiringPISPI.h"

//wiringPi pins
static const int CE0 = 10;
//static const int CE1 = 11;
//static const int MOSI = 12;
//static const int MISO = 13;
//static const int SCLK = 14;
static const int RS = 7;
static const int RESETB = 0;

static void seps525_reg(int address, int data)
{
	char buffer[8];
	//write address
	digitalWrite(CE0, LOW);
	digitalWrite(RS, LOW);
	wiringPiSPIDataRW(0, itoa(address, buffer, 8), 8);
	digitalWrite(RS, HIGH);
	digitalWrite(CS, HIGH);
	//write data
	digitalWrite(CS, LOW);
	wiringPiSPIDataRW(0, itoa(data, buffer, 8), 8);
	digitalWrite(CS, HIGH);
}

static inline void seps525_datastart(void)
{
	char buffer[8];
	digitalWrite(CE0, LOW);
	digitalWrite(RS, LOW);
	wiringPiSPIDataRW(0, itoa(0x22, buffer, 8), 8);
	digitalWrite(RS, HIGH);
}

static inline void seps525_data(int data)
{
	char buffer[16];
	wiringPiSPIDataRW(0, itoa(data, buffer, 16), 16);
	
}

static inline void seps525_dataend(void)
{
	digitalWrite(CE0, HIGH);
}

static void seps525_setup(void)
{
	wiringPiSPISetup(0, 16670000); //initialize SPI channel 0 at 16.67 MHz
	//set RS and CE
	pinModeWPi(RS, OUTPUT);
	pinModeWPi(CE0, OUTPUT);
}

static void seps525_set_region(int x, int y, int xs, int ys)
{
	//draw region
	seps525_reg(0x17,x);
	seps525_reg(0x18,x+xs-1);
	seps525_reg(0x19,y);
	seps525_reg(0x1a,y+ys-1);
	//start position
	seps525_reg(0x20,x);
	seps525_reg(0x21,y);
}

static void seps525_init(void)
{
	seps525_setup();

	//from SEPS525 datasheet
	digitalWrite(RESETB, LOW);
	delay(2); //VDD & VDDIO ON, VDDH ON
	//RESETB => 'H'
	digitalWrite(RESETB, HIGH);
	delay(1);
	seps525_reg(0x04, 0x01); //display off, analog reset
	delay(1);
	seps525_reg(0x04, 0x00); //normal mode
	delay(1);
	//following from SEPS525_OLED.cpp
	seps525_reg(0x06, 0x00); //display off
	seps525_reg(0x02, 0x01); //internal oscillator, external resistor
	seps525_reg(0x03, 0x30); // 90 hz frame rate, divider 0
	seps525_reg(0x28, 0x7f); //duty cycle 127
	seps525_reg(0x29, 0x00); //start on line 0
	seps525_reg(0x14, 0x31); //rgb interface
	seps525_reg(0x16, 0x66); //memory write mode
	seps525_reg(0x10, 0x45); //driving current r (uA)
	seps525_reg(0x11, 0x34); //driving current g (uA)
	seps525_reg(0x12, 0x33); //driving current b (uA)
	seps525_reg(0x08, 0x04); //precharge time r	
	seps525_reg(0x09, 0x05); //precharge time g
	seps525_reg(0x0a, 0x05); //precharge time b
	seps525_reg(0x0b, 0x9d); //precharge current r (uA)
	seps525_reg(0x0c, 0x8c); //precharge current g (uA)
	seps525_reg(0x0d, 0x57); //precharge current b (uA)
	seps525_reg(0x80, 0x00);
	seps525_reg(0x13, 0x00); //mode set
	seps525_set_region(0, 0, 160, 128);
	seps525_datastart();
	int n;
	for(n = 0; n < 160*128; n++){
		seps525_data(0xffff);
	}
	seps525_reg(0x06, 0x01);
}
