This folder contains all files relevant to GPIO functionality on either RPi.
This includes:

	*GPIO_RPI_1	<==> 	MUX communication
	*SPI_RPI2	<==> 	SEPS525 communication
	*Shift Adjuster Code

GPIO coding will use functions from the wiringPi library (wiringpi.com), which
virtually maps pins to the GPIO header for easier read/write functionality.

This library also includes a command-line utility gpio to program and setup
the GPIO pins -- allowing read/write and shell-script control.
