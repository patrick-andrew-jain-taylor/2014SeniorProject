#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include "WiringPi/wiringPi/wiringPi.h"
#include "WiringPi/wiringPi/wiringPiSPI.h"

//wiringPi pins
const int CE0 = 10;
// const int CE1 = 11;
// const int MOSI = 12;
// const int MISO = 13;
// const int SCLK = 14;
const int RS = 7;
const int RESETB = 0;

int seps525_reg(int address, int data)
{
	char buffer[8];
	//write address
	digitalWrite(CE0, LOW);
	digitalWrite(RS, LOW);
	snprintf(buffer, sizeof(buffer), "%d", address);
	wiringPiSPIDataRW(0, (unsigned char *)buffer, 8);
	digitalWrite(RS, HIGH);
	digitalWrite(CE0, HIGH);
	//write data
	digitalWrite(CE0, LOW);
	snprintf(buffer, sizeof(buffer), "%d", data);
	wiringPiSPIDataRW(0, (unsigned char *)buffer, 8);
	digitalWrite(CE0, HIGH);
	return 0;
}

int seps525_datastart(void)
{
	char buffer[8];
	digitalWrite(CE0, LOW);
	digitalWrite(RS, LOW);
	snprintf(buffer, sizeof(buffer), "%d", 0x22);
	wiringPiSPIDataRW(0, (unsigned char *)buffer, 8);
	digitalWrite(RS, HIGH);
	return 0;
}

int seps525_data(int data)
{
	char buffer[16];
	snprintf(buffer, sizeof(buffer), "%d", data);
	wiringPiSPIDataRW(0,(unsigned char *)buffer, 16);
	return 0;
	
}

int seps525_dataend(void)
{
	digitalWrite(CE0, HIGH);
	return 0;
}

int seps525_setup(void)
{
	wiringPiSetup();
	wiringPiSPISetup(0, 16670000); //initialize SPI channel 0 at 16.67 MHz
	//set RS and CE
	pinMode(RS, OUTPUT);
	pinMode(CE0, OUTPUT);
	return 0;
}

int seps525_set_region(int x, int y, int xs, int ys)
{
	//draw region
	seps525_reg(0x17,x);
	seps525_reg(0x18,x+xs-1);
	seps525_reg(0x19,y);
	seps525_reg(0x1a,y+ys-1);
	//start position
	seps525_reg(0x20,x);
	seps525_reg(0x21,y);
	return 0;
}

int seps525_init(void)
{
	seps525_setup();
	//from SEPS525 datasheet
	digitalWrite(RESETB, LOW);
	delay(2); //VDDIO ON, VDDH ON
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
	seps525_dataend();
	seps525_reg(0x06, 0x01);
	return 0;
}

int socketServer(void) //receive from RPi1
{
  int listenfd = 0/*,connfd = 0*/;
  
  struct sockaddr_in serv_addr;
 
  char recvBuff[3];  

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  printf("socket retrieve success\n");
  
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(recvBuff, '0', sizeof(recvBuff));
      
  serv_addr.sin_family = AF_INET;    
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
  serv_addr.sin_port = htons(17350);    
 
  bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
  
  if(listen(listenfd, 10) == -1){
      printf("Failed to listen\n");
      return -1;
  }
  /*connfd = */accept(listenfd, (struct sockaddr*)NULL ,NULL); // accept awaiting request

  //interpret data from RPi1
  while(1)
  {
	  int n = 0;
	  if((n = read(listenfd, recvBuff, sizeof(recvBuff)-1)) > 0)
	  {
		  int tmp = atoi(recvBuff);
		  seps525_datastart();
		  seps525_data(tmp);
		  seps525_dataend();
	  }
  }
  return 0;
}

int socketClient(void) //send to RPi2
{
  int sockfd = 0/*,n = 0*/;
  char sendBuff[2];
  struct sockaddr_in serv_addr;
 
  memset(sendBuff, '0' ,sizeof(sendBuff));
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
      printf("\n Error : Could not create socket \n");
      return 1;
    }
 
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(17350);
  serv_addr.sin_addr.s_addr = inet_addr("192.168.0.102");
 
  if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
      printf("\n Error : Connect Failed \n");
      return 1;
    }
 

  //send screen adjustment information to RPi2
  
  return 0;
}


int RPi3Setup(void) //need to work with Jake to finalize
{
	wiringPiSetup();
	//set pin modes
	/*pinModeWPi(7, INPUT); //GPIO 4  -- Output O1
	pinModeWPi(0, INPUT); //GPIO 17 -- Output O2
	pinModeWPi(1, INPUT); //GPIO 18 -- Output O3
	pinModeWPi(2, INPUT); //GPIO 21 -- Output O4
	pinModeWPi(3, INPUT); //GPIO 22 -- Output O5
	pinModeWPi(4, INPUT); //GPIO 23 -- Output O6
	pinModeWPi(5, OUTPUT);  //GPIO 24 -- Input  S1
	pinModeWPi(6, OUTPUT);  //GPIO 25 -- Input  S0
	*/
	return 0;
}

int main(void)
{
	seps525_init();
	//pthread_t server;
	//pthread_t client;
	//pthread_create(&client, NULL, socketClient, NULL);
	//pthread_create(&server, NULL, socketServer, NULL);
	return 0;
}
