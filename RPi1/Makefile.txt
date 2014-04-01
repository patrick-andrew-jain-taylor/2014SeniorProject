CC=gcc
CFLAGS= -c -Wall
LDFLAGS= -lpthread
SOURCES=RPi1.c WiringPi/wiringPi/wiringPi.c WiringPi/wiringPi/piHiPri.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=RPi1

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	
.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -rf *.o $(EXECUTABLE)