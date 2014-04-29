This directory will contain all configuration files for the first Raspberry Pi in our system, serving as a direct input/output interface between the A/D converted composite signal and the third Raspberry Pi. This directory contains the following files:

GPIOMux.c -- an extension of the wiringPi library to use 6 GPIO pins as inputs (to be used to read in 6 bits of a 24-bit RGB triplet) and 2 GPIO pins as outputs(to be used with the MUX to select which set of 6 bits to read).
