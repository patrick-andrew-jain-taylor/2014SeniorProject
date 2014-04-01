#!/usr/bin/python

import smbus

bus = smbus.SMBus(0) #call port I2C0

DEVICE_ADDRESS = 0X4C
DEVICE_PLL_DIV_MSB = 0x01
DEVICE_PLL_DIV_LSB = 0x02
DEVICE_VCO_CPMP = 0x03

#set PLL to 800
bus.write_byte_data(DEVICE_ADDRESS,DEVICE_PLL_DIV_MSB,0x03)
bus.write_byte_data(DEVICE_ADDRESS,DEVICE_PLL_DIV_LSB,0x20)
#set VCO/CPMP
bus.write_byte_data(DEVICE_ADDRESS,DEVICE_VCO_CPMP,0x28)  
