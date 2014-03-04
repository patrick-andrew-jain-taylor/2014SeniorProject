This folder will contain all files pertaining to I2C configurations.

The only I2C device in our board will be the AD9983A YPrPb to RGB ADC, which used 2-wire serial to manipulate the data output. The ADC currently takes in a series of grounds, 1.8V, 3.3V, and 5V inputs, as well as an external filter, a reference series capacitor, and a few non-connected inputs. RPI2, our Raspberry Pi responsible for data manipulation, connects to the ADC using 8 GPIOs as well as the 2-wire serial, and we assume the following givens:

Our display (with SEPS525 Driver) outputs 160x120, 60Hz. This is accomplished by inputting a composite signal from the XBox 360, which is converted to 640x480 resolution with the following signals:

an HSYNC of 60Hz * 480 = 28.8kHz

